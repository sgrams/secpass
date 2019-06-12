/* src/common/bridge.cc
 * Stanisław Grams <sgrams@fmdx.pl>
 *
 * secpass.org
 * © 2019
 */
#include "bridge.h"
#include "file.h"
#include "argon2.h"
#include "aes.h"
#include "../core/crypto.h"

#include <sys/stat.h>
#include <vector>
#include <algorithm>
#include <openssl/sha.h>
#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <sstream>

using namespace std;

sgx_enclave_id_t global_eid = 0;
bool authenticated = false;
vector<string> names;
key_128bit_t key;

bridge_status_t
br_enclave_init (void)
{
  if (SGX_SUCCESS != sgx_create_enclave("../lib/enclave.so", 1, NULL, NULL, &global_eid, NULL)) {
    return BRIDGE_ER_INIT;
  }
  return BRIDGE_OK;
}

bridge_status_t
br_enclave_close (void)
{
  if (SGX_SUCCESS != sgx_destroy_enclave (global_eid)) {
    return BRIDGE_ER_DESTROY;
  }
  return BRIDGE_OK;
}

bridge_status_t
br_derive_key_argon2 (const char *password, const char *filepath, uint8_t *salt, size_t salt_len, uint8_t *key, size_t key_len)
{
  bridge_status_t status = BRIDGE_OK;
  size_t  source_len;
  uint8_t *source       = NULL;
  uint8_t *file_hash    = NULL;

  if (filepath != NULL) {
    FILE *fp = fopen (filepath, "rb");
    if (!fp) {
      status = BRIDGE_ER_WR_PARAM;
      return status;
    }
    file_hash = (uint8_t *)malloc (KDF_KEYFILE_HASH_SIZE);
    size_t  buf_size = 32768;
    uint8_t buffer[buf_size];

    SHA256_CTX sha256;
    SHA256_Init (&sha256);

    size_t bytes_read = 0;
    while ((bytes_read = fread (buffer, 1, buf_size, fp)))
    {
      SHA256_Update (&sha256, buffer, bytes_read);
    }

    SHA256_Final (file_hash, &sha256);
    fclose (fp);
  }

  // derive with/without keyfile
  if (filepath != NULL) {
    source_len = strlen (password) + KDF_KEY_SIZE;
    source = (uint8_t *)malloc (source_len);
    memcpy (source, password, strlen (password));
    memcpy (source + strlen (password), file_hash, KDF_KEY_SIZE);
  } else {
    source_len = strlen (password);
    source = (uint8_t *)malloc (source_len);
    memcpy (source, password, strlen (password));
  }

  uint32_t t_cost = 2; // 1-pass computation
  uint32_t m_cost = (1<<16); // 64 MiB mem usage
  uint32_t parallelism = 1; // number of threads

  argon2i_hash_raw (
    t_cost, m_cost,
    parallelism,
    source,
    source_len,
    salt,
    salt_len,
    key,
    key_len
    );

  if (file_hash) {
    memset (file_hash, 0, KDF_KEYFILE_HASH_SIZE);
    free (file_hash);
  }

  if (source) {
    memset (source, 0, source_len);
    free (source);
  }

  return status;
}

bridge_status_t
br_secret_add (const string name, const string secret)
{
  uint64_t rv;
  uint8_t  sc;
  bridge_status_t status = BRIDGE_OK;
  br_secret_check (name, &sc);
  if (!sc) {
    return BRIDGE_ER_UNDEF;
  }

  const char *str = name.c_str ();
  char sec[MAX_SECRET_LEN] = {0};
  strncpy (sec, secret.c_str (), secret.size () + 1);

  if (SGX_SUCCESS != entry_add (global_eid, &rv, str, sec)) {
    return BRIDGE_ER_ADD;
  }
  names.push_back (name);

  return status;
}

bridge_status_t
br_secret_set (char *name, size_t name_len, char *secret, size_t secret_len)
{
  bridge_status_t status = BRIDGE_OK;

  return status;
}

bridge_status_t
br_secret_del (const string name)
{
  uint64_t rv;
  bridge_status_t status = BRIDGE_OK;

  const char *str = name.c_str ();
  if (SGX_SUCCESS != entry_del (global_eid, &rv, str)) {
    return BRIDGE_ER_DEL;
  }
  auto it = find (names.begin (), names.end (), name);
  if (it != names.end ()) {
    names.erase (it);
  }

  return status;
}

bridge_status_t
br_secret_check (const string name, uint8_t *rv) {
  uint64_t rs;
  const char *str = name.c_str ();

  if (SGX_SUCCESS != entry_check (global_eid, &rs, str, rv)) {
    return BRIDGE_ER_UNDEF;
  }
  return rs;
}

bridge_status_t
br_secret_fetch (const string name, char *secret)
{
  uint64_t rv;
  const char *str = name.c_str ();

  if (SGX_SUCCESS != fetch_decrypted_secret (global_eid, &rv, str, secret)) {
    return BRIDGE_ER_FETCH;
  }

  return rv;
}

bridge_status_t
br_fetch_names (vector<string> *rv)
{
  bridge_status_t status = BRIDGE_OK;
  *rv = names;
  return status;
}

// file-specific methods
bridge_status_t
br_file_check (const string filepath)
{
  bridge_status_t status = BRIDGE_OK;

  if (filepath.substr (filepath.find_last_of (".") + 1) != DEFAULT_FILE_EXTENSION) {
    status = BRIDGE_ER_WR_EXT;
    return status;
  }

  return br_file_exists (filepath);
}

bridge_status_t
br_file_exists (const string filepath)
{
  bridge_status_t status = BRIDGE_OK;
  ifstream file;
  struct stat buffer;

  if (stat (filepath.c_str (), &buffer) == 0) {
    status = BRIDGE_OK;
  } else {
    status = BRIDGE_ER_FILE_NONEX;
  }

  return status;
}

bridge_status_t
br_file_open (uint8_t *master_key, size_t master_key_len, const string filepath)
{
  uint64_t rv;

  uint8_t secret[MAX_SECRET_SIZ]  = {0};
  uint8_t iv[CRYPTO_FILE_IV_SIZE] = {0};
  size_t iv_len = CRYPTO_FILE_IV_SIZE;

  bridge_status_t status = BRIDGE_OK;
  ifstream in;

  // open file to read
  in.open (filepath, ios::binary);

  // get IV
  int iv_pos = SECPASS_MAGIC_SIZE + KDF_SALT_SIZE;
  in.seekg (iv_pos); // move to magic+salt -> iv
  in.read ((char *)iv, iv_len);

  // read data from file into buffer
  string enc_buf;
  int    len;
  int    ct_pos = iv_pos + iv_len;

  in.seekg (0, ios::end);
  len = in.tellg ();
  in.seekg (0, ios::beg);
  in.clear ();

  enc_buf.resize (len);
  in.read (&enc_buf[0], len);
  enc_buf.erase (0, ct_pos);

  // perform decryption
  string ptext;
  EVP_add_cipher (EVP_aes_256_cbc());

  ptext.resize (len);
  aes_256_cbc_decrypt (master_key, iv, enc_buf, ptext);

  // parse decrypted data
  // convert string to json data
  vector<uint8_t> bson_data;
  bson_data.resize (ptext.length ());
  copy (begin (ptext), end(ptext), begin(bson_data));
  json            json_data = json::from_bson (bson_data);

  string name;
  // TODO: read kdf, enc and version from file
  for (auto &el : json_data["en"].items ())
  {
    name = el.key ();
    names.push_back (name);
    auto &val = el.value ();

    val.get_to (secret);

    if (SGX_SUCCESS != load_to_sec_map (global_eid, &rv, name.c_str (), (char *)secret))
    {
      status = -1;
      return status;
    }
  }

  json_data.clear ();
  memset (secret, 0, MAX_SECRET_SIZ);
  memset (iv, 0, iv_len);

  ofstream lockfile (filepath + ".lockfile");
  lockfile.close ();
  return status;
}

bridge_status_t
br_file_save (const string filepath, uint8_t *salt, size_t salt_len)
{
  uint64_t rv;
  bridge_status_t status = BRIDGE_OK;
  ofstream out;
  json json_data;

  // create a new, unique and random IV
  uint8_t iv[CRYPTO_FILE_IV_SIZE] = {0};
  size_t iv_len = CRYPTO_FILE_IV_SIZE;

  if (SGX_SUCCESS != create_iv (global_eid, &rv, iv, iv_len)) {
    status = BRIDGE_ER_UNDEF;
    return status;
  }

  // open file to save
  out.open (filepath, ios::binary | ios::out);
  uint8_t magic[SECPASS_MAGIC_SIZE];
  for (int i = 0; i < SECPASS_MAGIC_SIZE; i++)
  {
    magic[i] = SECPASS_MAGIC;
  }

  // wrate magic, salt, iv
  out.write ((char *)&magic, SECPASS_MAGIC_SIZE);
  out.write ((char *)salt, salt_len);
  out.write ((char *)iv, iv_len);

  // set file json data
  json_data["version"] = SECPASS_FILE_VERSION;
  json_data["kdf"] = KDF_ARGON2;
  json_data["enc"] = ENC_AES128GCM;

  // fetch secrets from enclave
  uint8_t  secret[MAX_SECRET_SIZ] = {0};
  for (auto &el : names)
  {
    if (SGX_SUCCESS != fetch_encrypted_secret (global_eid, &rv, el.c_str (), secret)) {
      return BRIDGE_ER_WR_FILE;
    }
    json_data["en"][el] = secret;
  }

  // ---
  // AES-256-CBC encryption
  // TODO: enable other types of encryption...
  // ---

  // fetch key from enclave
  key_256bit_t tmp_key;
  if (SGX_SUCCESS != get_key (global_eid, &rv, tmp_key, CRYPTO_FILE_KEY_SIZE)) {
    status = BRIDGE_ER_FETCH;
    return status;
  }

  // encrypt json_data string
  vector<uint8_t> v_bson = json::to_bson (json_data);
  string ptext, ctext;
  ptext = string (v_bson.begin (), v_bson.end ());

  EVP_add_cipher (EVP_aes_256_cbc ());
  aes_256_cbc_encrypt (tmp_key, iv, ptext, ctext);

  // save output string to file
  out << ctext;

  // clear json_data string
  json_data.clear ();

  return status;
}

bridge_status_t
br_file_get_init (const string filepath, bool file_new, uint8_t *salt, size_t salt_len)
{
  uint64_t rv;
  bridge_status_t status = BRIDGE_OK;

  if (!salt || (salt_len <= 0)) {
    status = BRIDGE_ER_WR_PARAM;
    return status;
  }

  if (file_new) {
    if (SGX_SUCCESS != get_salt (global_eid, &rv, salt, salt_len)) {
      status = BRIDGE_ER_UNDEF;
    }
    status = BRIDGE_OK;
    return status;
  }

  ifstream file (filepath, ios::in | ios::binary);
  uint8_t magic[SECPASS_MAGIC_SIZE];
  for (int i = 0; i < SECPASS_MAGIC_SIZE; i++)
  {
    magic[i] = 0x00;
  }

  file.read ((char *)&magic, SECPASS_MAGIC_SIZE);
  file.read ((char *)salt, salt_len);

  file.close ();

  return status;
}

bridge_status_t
br_auth (uint8_t *master_key, size_t master_key_len)
{
  bridge_status_t status = BRIDGE_OK;
  uint64_t rv;
  // send master key to the enclave
  if (SGX_SUCCESS != auth (global_eid, &rv, master_key)) {
    status = BRIDGE_ER_INIT;
    return status;
  }
  return status;
}

bridge_status_t
br_file_valid (const string filepath)
{
  bridge_status_t status = BRIDGE_OK;
  struct stat     buffer;

  if (filepath.substr (filepath.find_last_of (".") + 1) != DEFAULT_FILE_EXTENSION) {
    status = BRIDGE_ER_WR_EXT;
    return status;
  }

  if (stat (filepath.c_str (), &buffer) != 0) {
    status = BRIDGE_ER_FILE_NONEX;
    return status;
  }

  ifstream file (filepath);
  uint8_t  magic[8] = {0x00};
  file.read ((char *)&magic, SECPASS_MAGIC_SIZE);

  if (magic[3] != SECPASS_MAGIC) {
    status = BRIDGE_ER_WR_FILE;
  }

  // check for lockfile
  status = br_file_exists (filepath + ".lockfile");
  if (status != 0) {
    status = BRIDGE_OK;
  } else {
    // lockfile exists, cannot open the database
    throw std::runtime_error ("br_file_valid: Cannot open file. It's already opened - check lockfile.");
  }
  return status;
}

void
aes_256_cbc_encrypt (
  uint8_t *key,
  uint8_t *iv,
  const string& ptext,
  string& ctext
  )
{
  EVP_CIPHER_CTX_ptr ctx (EVP_CIPHER_CTX_new(), ::EVP_CIPHER_CTX_free);
  int rc = EVP_EncryptInit_ex (ctx.get (), EVP_aes_256_cbc(), NULL, key, iv);
  if (rc != 1) {
    throw std::runtime_error ("aes_256_cbc_encrypt: EVP_EncryptInit_ex failed");
  }

  ctext.resize (ptext.size () + FILE_AES_BS);
  int out_len1 = (int)ctext.size();

    rc = EVP_EncryptUpdate(ctx.get (), (uint8_t*)&ctext[0], &out_len1, (const uint8_t*)&ptext[0], (int)ptext.size());
    if (rc != 1) {
      throw std::runtime_error ("aes_256_cbc_encrypt: EEVP_EncryptUpdate failed");
    }

    int out_len2 = (int) ctext.size () - out_len1;
    rc = EVP_EncryptFinal_ex (ctx.get (), (uint8_t *)&ctext[0]+out_len1, &out_len2);
    if (rc != 1) {
      throw std::runtime_error ("aes_256_cbc_encrypt: EEVP_EncryptFinal_ex failed");
    }

    ctext.resize (out_len1 + out_len2);
}

void
aes_256_cbc_decrypt (
  uint8_t *key,
  uint8_t *iv,
  const string &ctext,
  string& ptext
  )
{
  EVP_CIPHER_CTX_ptr ctx (EVP_CIPHER_CTX_new(), ::EVP_CIPHER_CTX_free);
  int rc = EVP_DecryptInit_ex (ctx.get (), EVP_aes_256_cbc(), NULL, key, iv);
  if (rc != 1) {
    throw std::runtime_error ("aes_256_cbc_decrypt: EVP_DecryptInit_ex failed");
  }

  ptext.resize (ctext.size () + FILE_AES_BS);
  int out_len1 = (int)ctext.size();

    rc = EVP_DecryptUpdate (ctx.get (), (uint8_t*)&ptext[0], &out_len1, (const uint8_t*)&ctext[0], (int)ctext.size());
    if (rc != 1) {
      throw std::runtime_error ("aes_256_cbc_decrypt: EEVP_DecryptUpdate failed");
    }

    int out_len2 = (int) ptext.size () - out_len1;
    rc = EVP_DecryptFinal_ex (ctx.get (), (uint8_t *)&ptext[0]+out_len1, &out_len2);
    if (rc != 1) {
      throw std::runtime_error ("aes_256_cbc_decrypt: EEVP_DecryptFinal_ex failed. Master password might be wrong.");
    }

    ptext.resize (out_len1 + out_len2);
}

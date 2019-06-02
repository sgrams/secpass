/* src/nctui/main.cc
 * Stanisław Grams <sgrams@fmdx.pl>
 *
 * secpass.org
 * © 2019
 */
#include <iostream>

#include <cstdio>
#include <cstdlib>
#include <unistd.h>

#include "../common/secpass.h"
#include "../core/core_u.h"

#include "sgx_eid.h"
#include "sgx_urts.h"

sgx_enclave_id_t global_eid  = 0;

// prototypes
void print_help ();

int initialize_enclave(void)
{
  sgx_status_t ret = SGX_ERROR_UNEXPECTED;

  ret = sgx_create_enclave("../lib/enclave.so", 1, NULL, NULL, &global_eid, NULL);
  if (ret == SGX_SUCCESS) {
    return 0;
  }
  ret = sgx_create_enclave("/usr/share/libsecpass/enclave.so", 1, NULL, NULL, &global_eid, NULL);
  if (ret != SGX_SUCCESS) {
    return -1;
  }
  return 0;
}

/*!
 * Main function
 * @param  int  argc
 * @param  char *argv[]
 * @retval int  status
 *
 * @author Stanisław Grams
 * @date   2019/05/03
 */
int
main (int argc, char *argv[])
{
  int x = 0;
  int status = 0;

  if (initialize_enclave () < 0) {
    std::cerr << "dupa!\n";
  }
  sgx_status_t rv = entry_test (global_eid, &x);
  std::cout << x << std::endl;
  if (rv != SGX_SUCCESS) {
    std::cout << "noob" << std::endl;
  }
  std::cout << "wtf!" << std::endl;

  return status;
}

/*!
 * Prints help to stdout
 * @param  void
 * @retval void
 *
 * @author Stanisław Grams
 * @date   2019/05/03
 */
  void
print_help ()
{
  std::cout <<
    "secpass-nc " << SECPASS_VERSION << " © " << SECPASS_AUTHORS << "\n" <<
    "Usage: secpass-nc /path/to/dbfile [OPTIONS] /path/to/file\n"     <<
    "Options:\n" <<
    "  -e\tExport secdb to kdbx file (requires path)\n"   <<
    "  -i\tImport secdb from kdbx file (requires path)\n" <<
    "  -t\tLock interface after given timeout in s\n";

  return;
}

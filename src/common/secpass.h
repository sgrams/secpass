/* src/common/secpass.h
 * Stanisław Grams <sjg@fmdx.pl>
 *
 * secpass.org
 * 2019
 */
#ifndef _SECPASS_COMMON_H
#define _SECPASS_COMMON_H

#define SECPASS_VERSION  "0.1-git"
#define SECPASS_AUTHORS  "Stanisław Grams <sjg@fmdx.pl>"
#define SECPASS_HOMEPAGE "https://secpass.org/"

#define MAX_NAME_LEN   1024
#define MAX_SECRET_LEN 2048
#define MAX_SECRET_SIZ 4096

#define DEFAULT_IDLE_TIME 60 // time in seconds
#define DEFAULT_FILE_EXTENSION "secdb"

// iv_t: 96 bit struct containing iv_t
typedef unsigned char iv_t[12];

// iv_struct_t: wrapper for iv_t
typedef struct iv {
  uint8_t iv[12];
} iv_struct_t;

// key_128_bit_t: 128 bit variable containing the master key
typedef uint8_t key_128bit_t[16];

// key_128_bit_t: 128 bit variable containing the master key
typedef uint8_t key_256bit_t[32];

#endif // _SECPASS_COMMON_H

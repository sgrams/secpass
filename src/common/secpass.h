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
#define SECPASS_MAGIC 0x73
#define SECPASS_MAGIC_SIZE 8

#define MAX_NAME_LEN   1024
#define MAX_SECRET_LEN 2048
#define MAX_SECRET_SIZ 4096

#define DEFAULT_IDLE_TIME 60 // time in seconds
#define DEFAULT_FILE_EXTENSION "secdb"
#define FILE_AES_BS   16

#define CLIPBOARD_TIME 15

// key_128_bit_t: 128 bit variable containing the master key
typedef uint8_t key_128bit_t[16];

// key_128_bit_t: 128 bit variable containing the master key
typedef uint8_t key_256bit_t[32];

#endif // _SECPASS_COMMON_H

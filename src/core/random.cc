/* src/core/random.cc
 * Stanisław Grams <sjg@fmdx.pl>
 *
 * secpass.org
 * © 2019
 */
#include <sgx.h>
#include <sgx_trts.h>
#include <sgx_cpuid.h>
#include <sgx_tcrypto.h>

#include "crypto.hh"
#include "random.hh"

static int _drng_init = DRNG_NOT_READY;

int32_t
_get_rng_support (int32_t *cpuinfo)
{
  int32_t ret = DRNG_UNAVAILABLE;

  #ifdef COMPILER_HAS_RDSEED_SUPPORT
	__cpuidex(info, 7, 0);
	if (((uint32_t) cpuinfo[1]) & (1<<18) ) ret |= DRNG_AVAILABLE;
#endif

  return status;
}

Random::Random ()
{
  int32_t cpuinfo[4];
  sgx_status_t status;

  status = sgx_cpuindex (cpuinfo, 1, 0);
  if (SGX_SUCCESS == status) {
    _drng_init = _get_rng_support (cpuinfo);
  }
}

Random::~Random ()
{
}

bool
Random::get_rng_support ()
{
  return ((_drng_init) & DRNG_AVAILABLE == DRNG_AVAILABLE);
}

core_status_t
get_u32_rand (uint32_t *rand)
{
  core_status_t status = CORE_OK;
  uint32_t retries = DRNG_DEF_RETRIES;

  while (retries > 0)
  {
    if ((sgx_read_rand (uint8_t *)rand, 4) != SGX_SUCCESS) {
      status = CORE_ER_DRNG;
    }
    retries--;
  }

  return status;
}

core_status_t
get_u64_rand (uint64_t *rand)
{
  core_status_t status = CORE_OK;
  uint32_t retries = DRNG_DEF_RETRIES;

  while (retries > 0)
  {
    if ((sgx_read_rand (uint8_t *)rand, 8) != SGX_SUCCESS) {
      status = CORE_ER_DRNG;
    }
    retries--;
  }

  return status;
}

core_status_t
get_u32_seed (uint32_t *seed)
{
  core_status_t status = CORE_OK;
  uint32_t retries = DRNG_DEF_RETRIES;

  if (this->get_rng_support ()) {
    return get_u32_rdrand_seed ();
  }

  while (retries > 0)
  {
    if (_rdseed32_)
    retries--;
  }
}

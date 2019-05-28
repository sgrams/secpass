/* src/core/time.cc
 * Stanisław Grams <sgrams@fmdx.pl>
 *
 * OCALLs to time library
 * secpass.org
 * © 2019
 */
#include <sgx.h>
#include <core.h>
#include <ctime>

/*!
 * Gets epoch to given uint64_t pointer
 * @param  uint64_t *time
 * @retval core_status_t status
 *
 * @author Stanislaw Grams
 * @date 2019/05/28
 */
core_status_t
ocall_get_epoch (uint64_t *time)
{
  core_status_t status = CORE_OK;

  if (NULL == time) {
    status = CORE_ER_WR_PARAM;
    *time  = 0;
  }
  time = std::time (nullptr);
  if (0 == time) {
    return CORE_ER_UNDEF;
  }

  return status;
}

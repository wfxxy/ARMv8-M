/*
 * Copyright (c) 2017-2018, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __SST_TEST_SERVICE_VENEERS_H__
#define __SST_TEST_SERVICE_VENEERS_H__

#include <stdint.h>
#include "tfm_sst_defs.h"

#ifdef __cplusplus
extern "C" {
#endif

#define __cmse_secure_gateway \
__attribute__((cmse_nonsecure_entry, noinline, section("SFN")))

/**
 * \brief Sets-up the SST test service so that it is ready for test functions to
 *        be called.
 *
 * \return Returns error code as specified in \ref tfm_sst_err_t
 */
enum tfm_sst_err_t sst_test_service_veneer_setup(void)
__cmse_secure_gateway;

/**
 * \brief Performs a dummy encryption on the supplied buffer, using the key
 *        stored in the asset with the given UUID.
 *
 * \param[in]     app_id    Application ID
 * \param[in]     key_uuid  UUID of asset containing key
 * \param[in,out] buf       Plaintext buffer
 * \param[in]     buf_size  Size of buf
 *
 * \return Returns error code as specified in \ref tfm_sst_err_t
 */
enum tfm_sst_err_t sst_test_service_veneer_dummy_encrypt(uint32_t app_id,
                                                         uint16_t key_uuid,
                                                         uint8_t *buf,
                                                         uint32_t buf_size)
__cmse_secure_gateway;

/**
 * \brief Performs a dummy decryption on the supplied buffer, using the key
 *        stored in the asset with the given UUID.
 *
 * \param[in]     app_id    Application ID
 * \param[in]     key_uuid  UUID of asset containing key
 * \param[in,out] buf       Ciphertext buffer
 * \param[in]     buf_size  Size of buf
 *
 * \return Returns error code as specified in \ref tfm_sst_err_t
 */
enum tfm_sst_err_t sst_test_service_veneer_dummy_decrypt(uint32_t app_id,
                                                         uint16_t key_uuid,
                                                         uint8_t *buf,
                                                         uint32_t buf_size)
__cmse_secure_gateway;

/**
 * \brief Cleans the secure storage used by the SST test service.
 *
 * \return Returns error code as specified in \ref tfm_sst_err_t
 */
enum tfm_sst_err_t sst_test_service_veneer_clean(void)
__cmse_secure_gateway;

#ifdef __cplusplus
}
#endif

#endif /* __SST_TEST_SERVICE_VENEERS_H__ */

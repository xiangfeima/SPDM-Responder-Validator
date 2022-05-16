/**
    Copyright Notice:
    Copyright 2021 DMTF. All rights reserved.
    License: BSD 3-Clause License. For full text see link: https://github.com/DMTF/spdm-emu/blob/main/LICENSE.md
**/

#include "spdm_responder_test.h"

bool spdm_test_case_capabilities_setup_version (void *test_context,
    spdm_version_number_t spdm_version)
{
    spdm_test_context_t *spdm_test_context;
    void *spdm_context;
    libspdm_return_t status;
    libspdm_data_parameter_t parameter;

    spdm_test_context = test_context;
    spdm_context = spdm_test_context->spdm_context;

    libspdm_zero_mem(&parameter, sizeof(parameter));
    parameter.location = LIBSPDM_DATA_LOCATION_LOCAL;
    libspdm_set_data(spdm_context, LIBSPDM_DATA_SPDM_VERSION, &parameter,
                     &spdm_version, sizeof(spdm_version));

    status = libspdm_get_version (spdm_context, NULL, NULL);
    if (LIBSPDM_STATUS_IS_ERROR(status)) {
        return false;
    }

    return true;
}

bool spdm_test_case_capabilities_setup_version_all (void *test_context)
{
    spdm_test_context_t *spdm_test_context;
    void *spdm_context;
    libspdm_return_t status;
    uint8_t version_number_entry_count;
    spdm_version_number_t version_number_entry[LIBSPDM_MAX_VERSION_COUNT];

    spdm_test_context = test_context;
    spdm_context = spdm_test_context->spdm_context;

    version_number_entry_count = LIBSPDM_MAX_VERSION_COUNT;
    status = libspdm_get_version (spdm_context, &version_number_entry_count, version_number_entry);
    if (LIBSPDM_STATUS_IS_ERROR(status)) {
        return false;
    }

    /* save responder versions to scratch buffer */
    libspdm_copy_mem (spdm_test_context->test_scratch_buffer,
                      sizeof(spdm_test_context->test_scratch_buffer),
                      &version_number_entry_count,
                      sizeof(version_number_entry_count));
    libspdm_copy_mem (spdm_test_context->test_scratch_buffer + sizeof(version_number_entry_count),
                      sizeof(spdm_test_context->test_scratch_buffer) - sizeof(version_number_entry_count),
                      version_number_entry,
                      sizeof(spdm_version_number_t) * version_number_entry_count);
    spdm_test_context->test_scratch_buffer_size = sizeof(version_number_entry_count) +
        sizeof(spdm_version_number_t) * version_number_entry_count;

    return true;
}

bool spdm_test_case_capabilities_setup_vca (void *test_context)
{
    spdm_test_context_t *spdm_test_context;
    void *spdm_context;
    libspdm_return_t status;
    libspdm_data_parameter_t parameter;
    uint8_t data8;
    uint16_t data16;
    uint32_t data32;
    size_t data_size;

    spdm_test_context = test_context;
    spdm_context = spdm_test_context->spdm_context;

    libspdm_zero_mem(&parameter, sizeof(parameter));
    parameter.location = LIBSPDM_DATA_LOCATION_LOCAL;

    data8 = 0;
    libspdm_set_data(spdm_context, LIBSPDM_DATA_CAPABILITY_CT_EXPONENT,
                     &parameter, &data8, sizeof(data8));
    data32 = SPDM_GET_CAPABILITIES_REQUEST_FLAGS_CERT_CAP |
             SPDM_GET_CAPABILITIES_REQUEST_FLAGS_CHAL_CAP |
             SPDM_GET_CAPABILITIES_REQUEST_FLAGS_ENCRYPT_CAP |
             SPDM_GET_CAPABILITIES_REQUEST_FLAGS_MAC_CAP |
             SPDM_GET_CAPABILITIES_REQUEST_FLAGS_MUT_AUTH_CAP |
             SPDM_GET_CAPABILITIES_REQUEST_FLAGS_KEY_EX_CAP |
             SPDM_GET_CAPABILITIES_REQUEST_FLAGS_PSK_CAP_REQUESTER |
             SPDM_GET_CAPABILITIES_REQUEST_FLAGS_ENCAP_CAP |
             SPDM_GET_CAPABILITIES_REQUEST_FLAGS_HBEAT_CAP |
             SPDM_GET_CAPABILITIES_REQUEST_FLAGS_KEY_UPD_CAP |
             SPDM_GET_CAPABILITIES_REQUEST_FLAGS_HANDSHAKE_IN_THE_CLEAR_CAP;
    libspdm_set_data(spdm_context, LIBSPDM_DATA_CAPABILITY_FLAGS, &parameter,
                     &data32, sizeof(data32));

    data8 = SPDM_MEASUREMENT_BLOCK_HEADER_SPECIFICATION_DMTF;
    libspdm_set_data(spdm_context, LIBSPDM_DATA_MEASUREMENT_SPEC, &parameter,
                     &data8, sizeof(data8));
    data32 = SPDM_ALGORITHMS_BASE_ASYM_ALGO_TPM_ALG_ECDSA_ECC_NIST_P521 |
             SPDM_ALGORITHMS_BASE_ASYM_ALGO_TPM_ALG_ECDSA_ECC_NIST_P384 |
             SPDM_ALGORITHMS_BASE_ASYM_ALGO_TPM_ALG_ECDSA_ECC_NIST_P256 |
             SPDM_ALGORITHMS_BASE_ASYM_ALGO_TPM_ALG_RSAPSS_4096 |
             SPDM_ALGORITHMS_BASE_ASYM_ALGO_TPM_ALG_RSASSA_4096 |
             SPDM_ALGORITHMS_BASE_ASYM_ALGO_TPM_ALG_RSAPSS_3072 |
             SPDM_ALGORITHMS_BASE_ASYM_ALGO_TPM_ALG_RSASSA_3072 |
             SPDM_ALGORITHMS_BASE_ASYM_ALGO_TPM_ALG_RSAPSS_2048 |
             SPDM_ALGORITHMS_BASE_ASYM_ALGO_TPM_ALG_RSASSA_2048 |
             SPDM_ALGORITHMS_BASE_ASYM_ALGO_EDDSA_ED448 |
             SPDM_ALGORITHMS_BASE_ASYM_ALGO_EDDSA_ED25519 |
             SPDM_ALGORITHMS_BASE_ASYM_ALGO_TPM_ALG_SM2_ECC_SM2_P256;
    libspdm_set_data(spdm_context, LIBSPDM_DATA_BASE_ASYM_ALGO, &parameter,
                     &data32, sizeof(data32));
    data32 = SPDM_ALGORITHMS_BASE_HASH_ALGO_TPM_ALG_SHA3_512 |
             SPDM_ALGORITHMS_BASE_HASH_ALGO_TPM_ALG_SHA3_384 |
             SPDM_ALGORITHMS_BASE_HASH_ALGO_TPM_ALG_SHA3_256 |
             SPDM_ALGORITHMS_BASE_HASH_ALGO_TPM_ALG_SHA_512 |
             SPDM_ALGORITHMS_BASE_HASH_ALGO_TPM_ALG_SHA_384 |
             SPDM_ALGORITHMS_BASE_HASH_ALGO_TPM_ALG_SHA_256;
    libspdm_set_data(spdm_context, LIBSPDM_DATA_BASE_HASH_ALGO, &parameter,
                     &data32, sizeof(data32));
    data16 = SPDM_ALGORITHMS_DHE_NAMED_GROUP_SECP_521_R1 |
             SPDM_ALGORITHMS_DHE_NAMED_GROUP_SECP_384_R1 |
             SPDM_ALGORITHMS_DHE_NAMED_GROUP_SECP_256_R1 |
             SPDM_ALGORITHMS_DHE_NAMED_GROUP_FFDHE_4096 |
             SPDM_ALGORITHMS_DHE_NAMED_GROUP_FFDHE_3072 |
             SPDM_ALGORITHMS_DHE_NAMED_GROUP_FFDHE_2048 |
             SPDM_ALGORITHMS_DHE_NAMED_GROUP_SM2_P256;
    libspdm_set_data(spdm_context, LIBSPDM_DATA_DHE_NAME_GROUP, &parameter,
                     &data16, sizeof(data16));
    data16 = SPDM_ALGORITHMS_AEAD_CIPHER_SUITE_CHACHA20_POLY1305 |
             SPDM_ALGORITHMS_AEAD_CIPHER_SUITE_AES_256_GCM |
             SPDM_ALGORITHMS_AEAD_CIPHER_SUITE_AES_128_GCM |
             SPDM_ALGORITHMS_AEAD_CIPHER_SUITE_AEAD_SM4_GCM;
    libspdm_set_data(spdm_context, LIBSPDM_DATA_AEAD_CIPHER_SUITE, &parameter,
                     &data16, sizeof(data16));
    data16 = SPDM_ALGORITHMS_BASE_ASYM_ALGO_TPM_ALG_ECDSA_ECC_NIST_P521 |
             SPDM_ALGORITHMS_BASE_ASYM_ALGO_TPM_ALG_ECDSA_ECC_NIST_P384 |
             SPDM_ALGORITHMS_BASE_ASYM_ALGO_TPM_ALG_ECDSA_ECC_NIST_P256 |
             SPDM_ALGORITHMS_BASE_ASYM_ALGO_TPM_ALG_RSAPSS_4096 |
             SPDM_ALGORITHMS_BASE_ASYM_ALGO_TPM_ALG_RSASSA_4096 |
             SPDM_ALGORITHMS_BASE_ASYM_ALGO_TPM_ALG_RSAPSS_3072 |
             SPDM_ALGORITHMS_BASE_ASYM_ALGO_TPM_ALG_RSASSA_3072 |
             SPDM_ALGORITHMS_BASE_ASYM_ALGO_TPM_ALG_RSAPSS_2048 |
             SPDM_ALGORITHMS_BASE_ASYM_ALGO_TPM_ALG_RSASSA_2048 |
             SPDM_ALGORITHMS_BASE_ASYM_ALGO_EDDSA_ED448 |
             SPDM_ALGORITHMS_BASE_ASYM_ALGO_EDDSA_ED25519 |
             SPDM_ALGORITHMS_BASE_ASYM_ALGO_TPM_ALG_SM2_ECC_SM2_P256;
    libspdm_set_data(spdm_context, LIBSPDM_DATA_REQ_BASE_ASYM_ALG, &parameter,
                     &data16, sizeof(data16));
    data16 = SPDM_ALGORITHMS_KEY_SCHEDULE_HMAC_HASH;
    libspdm_set_data(spdm_context, LIBSPDM_DATA_KEY_SCHEDULE, &parameter, &data16,
                     sizeof(data16));
    data8 = SPDM_ALGORITHMS_OPAQUE_DATA_FORMAT_1;
    libspdm_set_data(spdm_context, LIBSPDM_DATA_OTHER_PARAMS_SUPPORT, &parameter,
                     &data8, sizeof(data8));

    status = libspdm_init_connection (spdm_context, false);
    if (LIBSPDM_STATUS_IS_ERROR(status)) {
        return false;
    }

    libspdm_zero_mem(&parameter, sizeof(parameter));
    parameter.location = LIBSPDM_DATA_LOCATION_CONNECTION;

    data_size = sizeof(data32);
    libspdm_get_data(spdm_context, LIBSPDM_DATA_CONNECTION_STATE, &parameter,
                     &data32, &data_size);
    LIBSPDM_ASSERT(data32 == LIBSPDM_CONNECTION_STATE_NEGOTIATED);

    data_size = sizeof(data32);
    libspdm_get_data(spdm_context, LIBSPDM_DATA_CAPABILITY_FLAGS, &parameter,
                     &data32, &data_size);

    /* save responder flags to scratch buffer */
    libspdm_copy_mem (spdm_test_context->test_scratch_buffer,
                      sizeof(spdm_test_context->test_scratch_buffer),
                      &data32,
                      sizeof(data32));
    spdm_test_context->test_scratch_buffer_size = sizeof(data32);

    return true;
}

bool spdm_test_case_capabilities_setup_version_10 (void *test_context)
{
    return spdm_test_case_capabilities_setup_version (test_context,
        SPDM_MESSAGE_VERSION_10 << SPDM_VERSION_NUMBER_SHIFT_BIT);
}

bool spdm_test_case_capabilities_setup_version_11 (void *test_context)
{
    return spdm_test_case_capabilities_setup_version (test_context,
        SPDM_MESSAGE_VERSION_11 << SPDM_VERSION_NUMBER_SHIFT_BIT);
}

bool spdm_test_case_capabilities_setup_version_12 (void *test_context)
{
    return spdm_test_case_capabilities_setup_version (test_context,
        SPDM_MESSAGE_VERSION_12 << SPDM_VERSION_NUMBER_SHIFT_BIT);
}

void spdm_test_case_capabilities_success_10 (void *test_context)
{
    spdm_test_context_t *spdm_test_context;
    void *spdm_context;
    libspdm_return_t status;
    spdm_get_capabilities_request_t spdm_request;
    size_t spdm_request_size;
    spdm_capabilities_response_t *spdm_response;
    uint8_t message[LIBSPDM_MAX_MESSAGE_BUFFER_SIZE];
    size_t spdm_response_size;
    common_test_result_t test_result;
    uint32_t flags;

    spdm_test_context = test_context;
    spdm_context = spdm_test_context->spdm_context;

    libspdm_zero_mem(&spdm_request, sizeof(spdm_request));
    spdm_request.header.spdm_version = SPDM_MESSAGE_VERSION_10;
    spdm_request_size = sizeof(spdm_request.header);
    spdm_request.header.request_response_code = SPDM_GET_CAPABILITIES;
    spdm_request.header.param1 = 0;
    spdm_request.header.param2 = 0;

    spdm_response = (void *)message;
    spdm_response_size = sizeof(message);
    libspdm_zero_mem(message, sizeof(message));
    status = libspdm_send_receive_data(spdm_context, NULL, false,
                                       &spdm_request, spdm_request_size,
                                       spdm_response, &spdm_response_size);
    if (LIBSPDM_STATUS_IS_ERROR(status)) {
        common_test_record_test_assertion (
            SPDM_RESPONDER_TEST_GROUP_CAPABILITIES, SPDM_RESPONDER_TEST_CASE_CAPABILITIES_SUCCESS_10, COMMON_TEST_ID_END,
            COMMON_TEST_RESULT_NOT_TESTED, "send/receive failure");
        return ;
    }

    if (spdm_response_size >= sizeof(spdm_capabilities_response_t) -
            sizeof(spdm_response->data_transfer_size) -
            sizeof(spdm_response->max_spdm_msg_size)) {
        test_result = COMMON_TEST_RESULT_PASS;
    } else {
        test_result = COMMON_TEST_RESULT_FAIL;
    }
    common_test_record_test_assertion (
        SPDM_RESPONDER_TEST_GROUP_CAPABILITIES, SPDM_RESPONDER_TEST_CASE_CAPABILITIES_SUCCESS_10, 1,
        test_result, "response size - %d", spdm_response_size);
    if (test_result == COMMON_TEST_RESULT_FAIL) {
        return ;
    }

    if (spdm_response->header.request_response_code == SPDM_CAPABILITIES) {
        test_result = COMMON_TEST_RESULT_PASS;
    } else {
        test_result = COMMON_TEST_RESULT_FAIL;
    }
    common_test_record_test_assertion (
        SPDM_RESPONDER_TEST_GROUP_CAPABILITIES, SPDM_RESPONDER_TEST_CASE_CAPABILITIES_SUCCESS_10, 2,
        test_result, "response code - 0x%02x", spdm_response->header.request_response_code);
    if (test_result == COMMON_TEST_RESULT_FAIL) {
        return ;
    }

    if (spdm_response->header.spdm_version == SPDM_MESSAGE_VERSION_10) {
        test_result = COMMON_TEST_RESULT_PASS;
    } else {
        test_result = COMMON_TEST_RESULT_FAIL;
    }
    common_test_record_test_assertion (
        SPDM_RESPONDER_TEST_GROUP_CAPABILITIES, SPDM_RESPONDER_TEST_CASE_CAPABILITIES_SUCCESS_10, 3,
        test_result, "response version - 0x%02x", spdm_response->header.spdm_version);
    if (test_result == COMMON_TEST_RESULT_FAIL) {
        return ;
    }

    flags = spdm_response->flags;
    if ((flags & SPDM_GET_CAPABILITIES_RESPONSE_FLAGS_MEAS_CAP) != SPDM_GET_CAPABILITIES_RESPONSE_FLAGS_MEAS_CAP) {
        test_result = COMMON_TEST_RESULT_PASS;
    } else {
        test_result = COMMON_TEST_RESULT_FAIL;
    }
    common_test_record_test_assertion (
        SPDM_RESPONDER_TEST_GROUP_CAPABILITIES, SPDM_RESPONDER_TEST_CASE_CAPABILITIES_SUCCESS_10, 4,
        test_result, "response flags - 0x%08x", spdm_response->flags);
}

void spdm_test_case_capabilities_version_mismatch (void *test_context)
{
    spdm_test_context_t *spdm_test_context;
    void *spdm_context;
    libspdm_return_t status;
    spdm_get_capabilities_request_t spdm_request;
    size_t spdm_request_size;
    spdm_error_response_t *spdm_response;
    uint8_t message[LIBSPDM_MAX_MESSAGE_BUFFER_SIZE];
    size_t spdm_response_size;
    common_test_result_t test_result;
    uint8_t mismatched_version[] = {
        SPDM_MESSAGE_VERSION_10 - 1,
        SPDM_MESSAGE_VERSION_12 + 1,
    };
    size_t index;
    uint8_t version_number_entry_count;
    spdm_version_number_t version_number_entry[LIBSPDM_MAX_VERSION_COUNT];

    spdm_test_context = test_context;
    spdm_context = spdm_test_context->spdm_context;

    libspdm_copy_mem (&version_number_entry_count,
                      sizeof(version_number_entry_count),
                      spdm_test_context->test_scratch_buffer,
                      sizeof(version_number_entry_count));
    libspdm_copy_mem (version_number_entry,
                      sizeof(version_number_entry),
                      spdm_test_context->test_scratch_buffer + sizeof(version_number_entry_count),
                      sizeof(spdm_version_number_t) * version_number_entry_count);

    mismatched_version[0] = (uint8_t)(version_number_entry[version_number_entry_count - 1] - 1);
    mismatched_version[1] = (uint8_t)(version_number_entry[0] + 1);

    for (index = 0; index < LIBSPDM_ARRAY_SIZE(mismatched_version); index++) {
        common_test_record_test_message ("test mismatched_version - 0x%02x\n", mismatched_version[index]);
        libspdm_zero_mem(&spdm_request, sizeof(spdm_request));
        spdm_request.header.spdm_version = mismatched_version[index];
        spdm_request_size = sizeof(spdm_request.header);
        spdm_request.header.request_response_code = SPDM_GET_CAPABILITIES;
        spdm_request.header.param1 = 0;
        spdm_request.header.param2 = 0;

        spdm_response = (void *)message;
        spdm_response_size = sizeof(message);
        libspdm_zero_mem(message, sizeof(message));
        status = libspdm_send_receive_data(spdm_context, NULL, false,
                                          &spdm_request, spdm_request_size,
                                          spdm_response, &spdm_response_size);
        if (LIBSPDM_STATUS_IS_ERROR(status)) {
            common_test_record_test_assertion (
                SPDM_RESPONDER_TEST_GROUP_CAPABILITIES, SPDM_RESPONDER_TEST_CASE_CAPABILITIES_VERSION_MISMATCH, COMMON_TEST_ID_END,
                COMMON_TEST_RESULT_NOT_TESTED, "send/receive failure");
            continue ;
        }

        if (spdm_response_size >= sizeof(spdm_error_response_t)) {
            test_result = COMMON_TEST_RESULT_PASS;
        } else {
            test_result = COMMON_TEST_RESULT_FAIL;
        }
        common_test_record_test_assertion (
            SPDM_RESPONDER_TEST_GROUP_CAPABILITIES, SPDM_RESPONDER_TEST_CASE_CAPABILITIES_VERSION_MISMATCH, 1,
            test_result, "response size - %d", spdm_response_size);
        if (test_result == COMMON_TEST_RESULT_FAIL) {
            continue ;
        }

        if (spdm_response->header.request_response_code == SPDM_ERROR) {
            test_result = COMMON_TEST_RESULT_PASS;
        } else {
            test_result = COMMON_TEST_RESULT_FAIL;
        }
        common_test_record_test_assertion (
            SPDM_RESPONDER_TEST_GROUP_CAPABILITIES, SPDM_RESPONDER_TEST_CASE_CAPABILITIES_VERSION_MISMATCH, 2,
            test_result, "response code - 0x%02x", spdm_response->header.request_response_code);
        if (test_result == COMMON_TEST_RESULT_FAIL) {
            continue ;
        }

        if (spdm_response->header.spdm_version == SPDM_MESSAGE_VERSION_10) {
            test_result = COMMON_TEST_RESULT_PASS;
        } else {
            test_result = COMMON_TEST_RESULT_FAIL;
        }
        common_test_record_test_assertion (
            SPDM_RESPONDER_TEST_GROUP_CAPABILITIES, SPDM_RESPONDER_TEST_CASE_CAPABILITIES_VERSION_MISMATCH, 3,
            test_result, "response version - 0x%02x", spdm_response->header.spdm_version);
        if (test_result == COMMON_TEST_RESULT_FAIL) {
            continue ;
        }

        if (spdm_response->header.param1 == SPDM_ERROR_CODE_VERSION_MISMATCH) {
            test_result = COMMON_TEST_RESULT_PASS;
        } else {
            test_result = COMMON_TEST_RESULT_FAIL;
        }
        common_test_record_test_assertion (
            SPDM_RESPONDER_TEST_GROUP_CAPABILITIES, SPDM_RESPONDER_TEST_CASE_CAPABILITIES_VERSION_MISMATCH, 4,
            test_result, "response param1 - 0x%02x", spdm_response->header.param1);

        if (spdm_response->header.param2 == 0) {
            test_result = COMMON_TEST_RESULT_PASS;
        } else {
            test_result = COMMON_TEST_RESULT_FAIL;
        }
        common_test_record_test_assertion (
            SPDM_RESPONDER_TEST_GROUP_CAPABILITIES, SPDM_RESPONDER_TEST_CASE_CAPABILITIES_VERSION_MISMATCH, 5,
            test_result, "response param2 - 0x%02x", spdm_response->header.param2);
    }
}

void spdm_test_case_capabilities_success_11 (void *test_context)
{
    spdm_test_context_t *spdm_test_context;
    void *spdm_context;
    libspdm_return_t status;
    spdm_get_capabilities_request_t spdm_request;
    size_t spdm_request_size;
    spdm_capabilities_response_t *spdm_response;
    uint8_t message[LIBSPDM_MAX_MESSAGE_BUFFER_SIZE];
    size_t spdm_response_size;
    common_test_result_t test_result;
    uint32_t flags;

    spdm_test_context = test_context;
    spdm_context = spdm_test_context->spdm_context;

    libspdm_zero_mem(&spdm_request, sizeof(spdm_request));
    spdm_request.header.spdm_version = SPDM_MESSAGE_VERSION_11;
    spdm_request_size = offsetof(spdm_get_capabilities_request_t, data_transfer_size);
    spdm_request.header.request_response_code = SPDM_GET_CAPABILITIES;
    spdm_request.header.param1 = 0;
    spdm_request.header.param2 = 0;
    spdm_request.reserved = 0;
    spdm_request.ct_exponent = 0;
    spdm_request.reserved2 = 0;
    spdm_request.flags = SPDM_GET_CAPABILITIES_REQUEST_FLAGS_CERT_CAP |
                         SPDM_GET_CAPABILITIES_REQUEST_FLAGS_CHAL_CAP |
                         SPDM_GET_CAPABILITIES_REQUEST_FLAGS_ENCRYPT_CAP |
                         SPDM_GET_CAPABILITIES_REQUEST_FLAGS_MAC_CAP |
                         SPDM_GET_CAPABILITIES_REQUEST_FLAGS_MUT_AUTH_CAP |
                         SPDM_GET_CAPABILITIES_REQUEST_FLAGS_KEY_EX_CAP |
                         SPDM_GET_CAPABILITIES_REQUEST_FLAGS_PSK_CAP_REQUESTER |
                         SPDM_GET_CAPABILITIES_REQUEST_FLAGS_ENCAP_CAP |
                         SPDM_GET_CAPABILITIES_REQUEST_FLAGS_HBEAT_CAP |
                         SPDM_GET_CAPABILITIES_REQUEST_FLAGS_KEY_UPD_CAP;

    spdm_response = (void *)message;
    spdm_response_size = sizeof(message);
    libspdm_zero_mem(message, sizeof(message));
    status = libspdm_send_receive_data(spdm_context, NULL, false,
                                       &spdm_request, spdm_request_size,
                                       spdm_response, &spdm_response_size);
    if (LIBSPDM_STATUS_IS_ERROR(status)) {
        common_test_record_test_assertion (
            SPDM_RESPONDER_TEST_GROUP_CAPABILITIES, SPDM_RESPONDER_TEST_CASE_CAPABILITIES_SUCCESS_11, COMMON_TEST_ID_END,
            COMMON_TEST_RESULT_NOT_TESTED, "send/receive failure");
        return ;
    }

    if (spdm_response_size >= sizeof(spdm_capabilities_response_t) -
            sizeof(spdm_response->data_transfer_size) -
            sizeof(spdm_response->max_spdm_msg_size)) {
        test_result = COMMON_TEST_RESULT_PASS;
    } else {
        test_result = COMMON_TEST_RESULT_FAIL;
    }
    common_test_record_test_assertion (
        SPDM_RESPONDER_TEST_GROUP_CAPABILITIES, SPDM_RESPONDER_TEST_CASE_CAPABILITIES_SUCCESS_11, 1,
        test_result, "response size - %d", spdm_response_size);
    if (test_result == COMMON_TEST_RESULT_FAIL) {
        return ;
    }

    if (spdm_response->header.request_response_code == SPDM_CAPABILITIES) {
        test_result = COMMON_TEST_RESULT_PASS;
    } else {
        test_result = COMMON_TEST_RESULT_FAIL;
    }
    common_test_record_test_assertion (
        SPDM_RESPONDER_TEST_GROUP_CAPABILITIES, SPDM_RESPONDER_TEST_CASE_CAPABILITIES_SUCCESS_11, 2,
        test_result, "response code - 0x%02x", spdm_response->header.request_response_code);
    if (test_result == COMMON_TEST_RESULT_FAIL) {
        return ;
    }

    if (spdm_response->header.spdm_version == SPDM_MESSAGE_VERSION_11) {
        test_result = COMMON_TEST_RESULT_PASS;
    } else {
        test_result = COMMON_TEST_RESULT_FAIL;
    }
    common_test_record_test_assertion (
        SPDM_RESPONDER_TEST_GROUP_CAPABILITIES, SPDM_RESPONDER_TEST_CASE_CAPABILITIES_SUCCESS_11, 3,
        test_result, "response version - 0x%02x", spdm_response->header.spdm_version);
    if (test_result == COMMON_TEST_RESULT_FAIL) {
        return ;
    }

    flags = spdm_response->flags;
    if ((flags & SPDM_GET_CAPABILITIES_RESPONSE_FLAGS_MEAS_CAP) != SPDM_GET_CAPABILITIES_RESPONSE_FLAGS_MEAS_CAP) {
        test_result = COMMON_TEST_RESULT_PASS;
    } else {
        test_result = COMMON_TEST_RESULT_FAIL;
    }
    common_test_record_test_assertion (
        SPDM_RESPONDER_TEST_GROUP_CAPABILITIES, SPDM_RESPONDER_TEST_CASE_CAPABILITIES_SUCCESS_11, 4,
        test_result, "response flags - 0x%08x", spdm_response->flags);

    if ((flags & SPDM_GET_CAPABILITIES_RESPONSE_FLAGS_ENCRYPT_CAP) != 0) {
        if (((flags & SPDM_GET_CAPABILITIES_RESPONSE_FLAGS_KEY_EX_CAP) != 0) ||
            ((flags & SPDM_GET_CAPABILITIES_RESPONSE_FLAGS_PSK_CAP) != 0)) {
            test_result = COMMON_TEST_RESULT_PASS;
        } else {
            test_result = COMMON_TEST_RESULT_FAIL;
        }
        common_test_record_test_assertion (
            SPDM_RESPONDER_TEST_GROUP_CAPABILITIES, SPDM_RESPONDER_TEST_CASE_CAPABILITIES_SUCCESS_11, 5,
            test_result, "response flags - 0x%08x", spdm_response->flags);
    }

    if ((flags & SPDM_GET_CAPABILITIES_RESPONSE_FLAGS_MAC_CAP) != 0) {
        if (((flags & SPDM_GET_CAPABILITIES_RESPONSE_FLAGS_KEY_EX_CAP) != 0) ||
            ((flags & SPDM_GET_CAPABILITIES_RESPONSE_FLAGS_PSK_CAP) != 0)) {
            test_result = COMMON_TEST_RESULT_PASS;
        } else {
            test_result = COMMON_TEST_RESULT_FAIL;
        }
        common_test_record_test_assertion (
            SPDM_RESPONDER_TEST_GROUP_CAPABILITIES, SPDM_RESPONDER_TEST_CASE_CAPABILITIES_SUCCESS_11, 6,
            test_result, "response flags - 0x%08x", spdm_response->flags);
    }

    if ((flags & SPDM_GET_CAPABILITIES_RESPONSE_FLAGS_KEY_EX_CAP) != 0) {
        if (((flags & SPDM_GET_CAPABILITIES_RESPONSE_FLAGS_ENCRYPT_CAP) != 0) ||
            ((flags & SPDM_GET_CAPABILITIES_RESPONSE_FLAGS_MAC_CAP) != 0)) {
            test_result = COMMON_TEST_RESULT_PASS;
        } else {
            test_result = COMMON_TEST_RESULT_FAIL;
        }
        common_test_record_test_assertion (
            SPDM_RESPONDER_TEST_GROUP_CAPABILITIES, SPDM_RESPONDER_TEST_CASE_CAPABILITIES_SUCCESS_11, 7,
            test_result, "response flags - 0x%08x", spdm_response->flags);
    }

    if ((flags & SPDM_GET_CAPABILITIES_RESPONSE_FLAGS_PSK_CAP) != SPDM_GET_CAPABILITIES_RESPONSE_FLAGS_PSK_CAP) {
        test_result = COMMON_TEST_RESULT_PASS;
    } else {
        test_result = COMMON_TEST_RESULT_FAIL;
    }
    common_test_record_test_assertion (
        SPDM_RESPONDER_TEST_GROUP_CAPABILITIES, SPDM_RESPONDER_TEST_CASE_CAPABILITIES_SUCCESS_11, 8,
        test_result, "response flags - 0x%08x", spdm_response->flags);

    if ((flags & SPDM_GET_CAPABILITIES_RESPONSE_FLAGS_PSK_CAP) != 0) {
        if (((flags & SPDM_GET_CAPABILITIES_RESPONSE_FLAGS_ENCRYPT_CAP) != 0) ||
            ((flags & SPDM_GET_CAPABILITIES_RESPONSE_FLAGS_MAC_CAP) != 0)) {
            test_result = COMMON_TEST_RESULT_PASS;
        } else {
            test_result = COMMON_TEST_RESULT_FAIL;
        }
        common_test_record_test_assertion (
            SPDM_RESPONDER_TEST_GROUP_CAPABILITIES, SPDM_RESPONDER_TEST_CASE_CAPABILITIES_SUCCESS_11, 9,
            test_result, "response flags - 0x%08x", spdm_response->flags);
    }
    
    if ((flags & SPDM_GET_CAPABILITIES_RESPONSE_FLAGS_MUT_AUTH_CAP) != 0) {
        if ((flags & SPDM_GET_CAPABILITIES_RESPONSE_FLAGS_ENCAP_CAP) != 0) {
            test_result = COMMON_TEST_RESULT_PASS;
        } else {
            test_result = COMMON_TEST_RESULT_FAIL;
        }
        common_test_record_test_assertion (
            SPDM_RESPONDER_TEST_GROUP_CAPABILITIES, SPDM_RESPONDER_TEST_CASE_CAPABILITIES_SUCCESS_11, 10,
            test_result, "response flags - 0x%08x", spdm_response->flags);
    }

    if ((flags & SPDM_GET_CAPABILITIES_RESPONSE_FLAGS_HANDSHAKE_IN_THE_CLEAR_CAP) != 0) {
        if ((flags & SPDM_GET_CAPABILITIES_RESPONSE_FLAGS_KEY_EX_CAP) != 0) {
            test_result = COMMON_TEST_RESULT_PASS;
        } else {
            test_result = COMMON_TEST_RESULT_FAIL;
        }
        common_test_record_test_assertion (
            SPDM_RESPONDER_TEST_GROUP_CAPABILITIES, SPDM_RESPONDER_TEST_CASE_CAPABILITIES_SUCCESS_11, 11,
            test_result, "response flags - 0x%08x", spdm_response->flags);
    }

    if ((flags & SPDM_GET_CAPABILITIES_RESPONSE_FLAGS_PUB_KEY_ID_CAP) != 0) {
        if ((flags & SPDM_GET_CAPABILITIES_RESPONSE_FLAGS_CERT_CAP) == 0) {
            test_result = COMMON_TEST_RESULT_PASS;
        } else {
            test_result = COMMON_TEST_RESULT_FAIL;
        }
        common_test_record_test_assertion (
            SPDM_RESPONDER_TEST_GROUP_CAPABILITIES, SPDM_RESPONDER_TEST_CASE_CAPABILITIES_SUCCESS_11, 12,
            test_result, "response flags - 0x%08x", spdm_response->flags);
    }
}

void spdm_test_case_capabilities_invalid_request (void *test_context)
{
    spdm_test_context_t *spdm_test_context;
    void *spdm_context;
    libspdm_return_t status;
    spdm_get_capabilities_request_t spdm_request;
    size_t spdm_request_size;
    spdm_error_response_t *spdm_response;
    uint8_t message[LIBSPDM_MAX_MESSAGE_BUFFER_SIZE];
    size_t spdm_response_size;
    common_test_result_t test_result;
    uint8_t version;
    size_t index;
    uint8_t version_number_entry_count;
    spdm_version_number_t version_number_entry[LIBSPDM_MAX_VERSION_COUNT];
    uint32_t support_version_bitmask;
    uint32_t invalid_flags_v11[] = {
        SPDM_GET_CAPABILITIES_REQUEST_FLAGS_CERT_CAP |
            SPDM_GET_CAPABILITIES_REQUEST_FLAGS_CHAL_CAP |
            /* SPDM_GET_CAPABILITIES_REQUEST_FLAGS_ENCRYPT_CAP | */
            /* SPDM_GET_CAPABILITIES_REQUEST_FLAGS_MAC_CAP | */
            SPDM_GET_CAPABILITIES_REQUEST_FLAGS_MUT_AUTH_CAP |
            SPDM_GET_CAPABILITIES_REQUEST_FLAGS_KEY_EX_CAP |
            SPDM_GET_CAPABILITIES_REQUEST_FLAGS_PSK_CAP_REQUESTER |
            SPDM_GET_CAPABILITIES_REQUEST_FLAGS_ENCAP_CAP |
            SPDM_GET_CAPABILITIES_REQUEST_FLAGS_HBEAT_CAP |
            SPDM_GET_CAPABILITIES_REQUEST_FLAGS_KEY_UPD_CAP,
        SPDM_GET_CAPABILITIES_REQUEST_FLAGS_CERT_CAP |
            SPDM_GET_CAPABILITIES_REQUEST_FLAGS_CHAL_CAP |
            SPDM_GET_CAPABILITIES_REQUEST_FLAGS_ENCRYPT_CAP |
            SPDM_GET_CAPABILITIES_REQUEST_FLAGS_MAC_CAP |
            SPDM_GET_CAPABILITIES_REQUEST_FLAGS_MUT_AUTH_CAP |
            /* SPDM_GET_CAPABILITIES_REQUEST_FLAGS_KEY_EX_CAP | */
            /* SPDM_GET_CAPABILITIES_REQUEST_FLAGS_PSK_CAP_REQUESTER | */
            SPDM_GET_CAPABILITIES_REQUEST_FLAGS_ENCAP_CAP |
            SPDM_GET_CAPABILITIES_REQUEST_FLAGS_HBEAT_CAP |
            SPDM_GET_CAPABILITIES_REQUEST_FLAGS_KEY_UPD_CAP,
        SPDM_GET_CAPABILITIES_REQUEST_FLAGS_CERT_CAP |
            SPDM_GET_CAPABILITIES_REQUEST_FLAGS_CHAL_CAP |
            SPDM_GET_CAPABILITIES_REQUEST_FLAGS_ENCRYPT_CAP |
            SPDM_GET_CAPABILITIES_REQUEST_FLAGS_MAC_CAP |
            SPDM_GET_CAPABILITIES_REQUEST_FLAGS_MUT_AUTH_CAP |
            SPDM_GET_CAPABILITIES_REQUEST_FLAGS_KEY_EX_CAP |
            SPDM_GET_CAPABILITIES_REQUEST_FLAGS_PSK_CAP_REQUESTER |
            /* SPDM_GET_CAPABILITIES_REQUEST_FLAGS_ENCAP_CAP | */
            SPDM_GET_CAPABILITIES_REQUEST_FLAGS_HBEAT_CAP |
            SPDM_GET_CAPABILITIES_REQUEST_FLAGS_KEY_UPD_CAP,
    };
    uint32_t invalid_transport_size_v12[] = {
        SPDM_MIN_DATA_TRRANSFER_SIZE_VERSION_12 - 1,
        LIBSPDM_MAX_SPDM_MSG_SIZE + 1,
    };

    spdm_test_context = test_context;
    spdm_context = spdm_test_context->spdm_context;

    libspdm_copy_mem (&version_number_entry_count,
                      sizeof(version_number_entry_count),
                      spdm_test_context->test_scratch_buffer,
                      sizeof(version_number_entry_count));
    libspdm_copy_mem (version_number_entry,
                      sizeof(version_number_entry),
                      spdm_test_context->test_scratch_buffer + sizeof(version_number_entry_count),
                      sizeof(spdm_version_number_t) * version_number_entry_count);

    support_version_bitmask = 0;
    for (index = 0; index < version_number_entry_count; index++) {
        version = version_number_entry[index] >> SPDM_VERSION_NUMBER_SHIFT_BIT;
        if (version == SPDM_MESSAGE_VERSION_10) {
            support_version_bitmask |= SPDM_TEST_VERSION_MASK_V10;
        } else if (version == SPDM_MESSAGE_VERSION_11) {
            support_version_bitmask |= SPDM_TEST_VERSION_MASK_V11;
        } else if (version == SPDM_MESSAGE_VERSION_12) {
            support_version_bitmask |= SPDM_TEST_VERSION_MASK_V12;
        }
    }

    if ((support_version_bitmask & (SPDM_TEST_VERSION_MASK_V11 | SPDM_TEST_VERSION_MASK_V12)) != 0) {
        for (index = 0; index < LIBSPDM_ARRAY_SIZE(invalid_flags_v11); index++) {
            common_test_record_test_message ("test v11 flags - 0x%08x\n", invalid_flags_v11[index]);

            if ((support_version_bitmask & SPDM_TEST_VERSION_MASK_V12) != 0) {
                version = SPDM_MESSAGE_VERSION_12;
                spdm_request_size = sizeof(spdm_request);
            } else {
                version = SPDM_MESSAGE_VERSION_11;
                spdm_request_size = offsetof(spdm_get_capabilities_request_t, data_transfer_size);
            }

            libspdm_zero_mem(&spdm_request, sizeof(spdm_request));
            spdm_request.header.spdm_version = version;
            spdm_request.header.request_response_code = SPDM_GET_CAPABILITIES;
            spdm_request.header.param1 = 0;
            spdm_request.header.param2 = 0;
            spdm_request.reserved = 0;
            spdm_request.ct_exponent = 0;
            spdm_request.reserved2 = 0;
            spdm_request.flags = invalid_flags_v11[index];
            spdm_request.data_transfer_size = LIBSPDM_DATA_TRANSFER_SIZE;
            spdm_request.max_spdm_msg_size = LIBSPDM_MAX_SPDM_MSG_SIZE;

            spdm_response = (void *)message;
            spdm_response_size = sizeof(message);
            libspdm_zero_mem(message, sizeof(message));
            status = libspdm_send_receive_data(spdm_context, NULL, false,
                                            &spdm_request, spdm_request_size,
                                            spdm_response, &spdm_response_size);
            if (LIBSPDM_STATUS_IS_ERROR(status)) {
                common_test_record_test_assertion (
                    SPDM_RESPONDER_TEST_GROUP_CAPABILITIES, SPDM_RESPONDER_TEST_CASE_CAPABILITIES_INVALID_REQUEST, COMMON_TEST_ID_END,
                    COMMON_TEST_RESULT_NOT_TESTED, "send/receive failure");
                continue ;
            }

            if (spdm_response_size >= sizeof(spdm_error_response_t)) {
                test_result = COMMON_TEST_RESULT_PASS;
            } else {
                test_result = COMMON_TEST_RESULT_FAIL;
            }
            common_test_record_test_assertion (
                SPDM_RESPONDER_TEST_GROUP_CAPABILITIES, SPDM_RESPONDER_TEST_CASE_CAPABILITIES_INVALID_REQUEST, 1,
                test_result, "response size - %d", spdm_response_size);
            if (test_result == COMMON_TEST_RESULT_FAIL) {
                continue ;
            }

            if (spdm_response->header.request_response_code == SPDM_ERROR) {
                test_result = COMMON_TEST_RESULT_PASS;
            } else {
                test_result = COMMON_TEST_RESULT_FAIL;
            }
            common_test_record_test_assertion (
                SPDM_RESPONDER_TEST_GROUP_CAPABILITIES, SPDM_RESPONDER_TEST_CASE_CAPABILITIES_INVALID_REQUEST, 2,
                test_result, "response code - 0x%02x", spdm_response->header.request_response_code);
            if (test_result == COMMON_TEST_RESULT_FAIL) {
                continue ;
            }

            if (spdm_response->header.spdm_version == version) {
                test_result = COMMON_TEST_RESULT_PASS;
            } else {
                test_result = COMMON_TEST_RESULT_FAIL;
            }
            common_test_record_test_assertion (
                SPDM_RESPONDER_TEST_GROUP_CAPABILITIES, SPDM_RESPONDER_TEST_CASE_CAPABILITIES_INVALID_REQUEST, 3,
                test_result, "response version - 0x%02x", spdm_response->header.spdm_version);
            if (test_result == COMMON_TEST_RESULT_FAIL) {
                continue ;
            }

            if (spdm_response->header.param1 == SPDM_ERROR_CODE_INVALID_REQUEST) {
                test_result = COMMON_TEST_RESULT_PASS;
            } else {
                test_result = COMMON_TEST_RESULT_FAIL;
            }
            common_test_record_test_assertion (
                SPDM_RESPONDER_TEST_GROUP_CAPABILITIES, SPDM_RESPONDER_TEST_CASE_CAPABILITIES_INVALID_REQUEST, 4,
                test_result, "response param1 - 0x%02x", spdm_response->header.param1);

            if (spdm_response->header.param2 == 0) {
                test_result = COMMON_TEST_RESULT_PASS;
            } else {
                test_result = COMMON_TEST_RESULT_FAIL;
            }
            common_test_record_test_assertion (
                SPDM_RESPONDER_TEST_GROUP_CAPABILITIES, SPDM_RESPONDER_TEST_CASE_CAPABILITIES_INVALID_REQUEST, 5,
                test_result, "response param2 - 0x%02x", spdm_response->header.param2);
        }
    }

    if ((support_version_bitmask & SPDM_TEST_VERSION_MASK_V12) != 0) {
        for (index = 0; index < LIBSPDM_ARRAY_SIZE(invalid_transport_size_v12); index++) {
            common_test_record_test_message ("test v12 transfer_size - 0x%08x\n", invalid_transport_size_v12[index]);

            version = SPDM_MESSAGE_VERSION_12;

            libspdm_zero_mem(&spdm_request, sizeof(spdm_request));
            spdm_request.header.spdm_version = version;
            spdm_request.header.request_response_code = SPDM_GET_CAPABILITIES;
            spdm_request.header.param1 = 0;
            spdm_request.header.param2 = 0;
            spdm_request.reserved = 0;
            spdm_request.ct_exponent = 0;
            spdm_request.reserved2 = 0;
            spdm_request.flags = SPDM_GET_CAPABILITIES_REQUEST_FLAGS_CERT_CAP |
                                 SPDM_GET_CAPABILITIES_REQUEST_FLAGS_CHAL_CAP |
                                 SPDM_GET_CAPABILITIES_REQUEST_FLAGS_ENCRYPT_CAP |
                                 SPDM_GET_CAPABILITIES_REQUEST_FLAGS_MAC_CAP |
                                 SPDM_GET_CAPABILITIES_REQUEST_FLAGS_MUT_AUTH_CAP |
                                 SPDM_GET_CAPABILITIES_REQUEST_FLAGS_KEY_EX_CAP |
                                 SPDM_GET_CAPABILITIES_REQUEST_FLAGS_PSK_CAP_REQUESTER |
                                 SPDM_GET_CAPABILITIES_REQUEST_FLAGS_ENCAP_CAP |
                                 SPDM_GET_CAPABILITIES_REQUEST_FLAGS_HBEAT_CAP |
                                 SPDM_GET_CAPABILITIES_REQUEST_FLAGS_KEY_UPD_CAP;
            spdm_request.data_transfer_size = invalid_transport_size_v12[index];
            spdm_request.max_spdm_msg_size = LIBSPDM_MAX_SPDM_MSG_SIZE;

            spdm_request_size = sizeof(spdm_request);

            spdm_response = (void *)message;
            spdm_response_size = sizeof(message);
            libspdm_zero_mem(message, sizeof(message));
            status = libspdm_send_receive_data(spdm_context, NULL, false,
                                            &spdm_request, spdm_request_size,
                                            spdm_response, &spdm_response_size);
            if (LIBSPDM_STATUS_IS_ERROR(status)) {
                common_test_record_test_assertion (
                    SPDM_RESPONDER_TEST_GROUP_CAPABILITIES, SPDM_RESPONDER_TEST_CASE_CAPABILITIES_INVALID_REQUEST, COMMON_TEST_ID_END,
                    COMMON_TEST_RESULT_NOT_TESTED, "send/receive failure");
                continue ;
            }

            if (spdm_response_size >= sizeof(spdm_error_response_t)) {
                test_result = COMMON_TEST_RESULT_PASS;
            } else {
                test_result = COMMON_TEST_RESULT_FAIL;
            }
            common_test_record_test_assertion (
                SPDM_RESPONDER_TEST_GROUP_CAPABILITIES, SPDM_RESPONDER_TEST_CASE_CAPABILITIES_INVALID_REQUEST, 1,
                test_result, "response size - %d", spdm_response_size);
            if (test_result == COMMON_TEST_RESULT_FAIL) {
                continue ;
            }

            if (spdm_response->header.request_response_code == SPDM_ERROR) {
                test_result = COMMON_TEST_RESULT_PASS;
            } else {
                test_result = COMMON_TEST_RESULT_FAIL;
            }
            common_test_record_test_assertion (
                SPDM_RESPONDER_TEST_GROUP_CAPABILITIES, SPDM_RESPONDER_TEST_CASE_CAPABILITIES_INVALID_REQUEST, 2,
                test_result, "response code - 0x%02x", spdm_response->header.request_response_code);
            if (test_result == COMMON_TEST_RESULT_FAIL) {
                continue ;
            }

            if (spdm_response->header.spdm_version == version) {
                test_result = COMMON_TEST_RESULT_PASS;
            } else {
                test_result = COMMON_TEST_RESULT_FAIL;
            }
            common_test_record_test_assertion (
                SPDM_RESPONDER_TEST_GROUP_CAPABILITIES, SPDM_RESPONDER_TEST_CASE_CAPABILITIES_INVALID_REQUEST, 3,
                test_result, "response version - 0x%02x", spdm_response->header.spdm_version);
            if (test_result == COMMON_TEST_RESULT_FAIL) {
                continue ;
            }

            if (spdm_response->header.param1 == SPDM_ERROR_CODE_INVALID_REQUEST) {
                test_result = COMMON_TEST_RESULT_PASS;
            } else {
                test_result = COMMON_TEST_RESULT_FAIL;
            }
            common_test_record_test_assertion (
                SPDM_RESPONDER_TEST_GROUP_CAPABILITIES, SPDM_RESPONDER_TEST_CASE_CAPABILITIES_INVALID_REQUEST, 4,
                test_result, "response param1 - 0x%02x", spdm_response->header.param1);

            if (spdm_response->header.param2 == 0) {
                test_result = COMMON_TEST_RESULT_PASS;
            } else {
                test_result = COMMON_TEST_RESULT_FAIL;
            }
            common_test_record_test_assertion (
                SPDM_RESPONDER_TEST_GROUP_CAPABILITIES, SPDM_RESPONDER_TEST_CASE_CAPABILITIES_INVALID_REQUEST, 5,
                test_result, "response param2 - 0x%02x", spdm_response->header.param2);
        }
    }
}

void spdm_test_case_capabilities_success_12 (void *test_context)
{
    spdm_test_context_t *spdm_test_context;
    void *spdm_context;
    libspdm_return_t status;
    spdm_get_capabilities_request_t spdm_request;
    size_t spdm_request_size;
    spdm_capabilities_response_t *spdm_response;
    uint8_t message[LIBSPDM_MAX_MESSAGE_BUFFER_SIZE];
    size_t spdm_response_size;
    common_test_result_t test_result;
    uint32_t flags;

    spdm_test_context = test_context;
    spdm_context = spdm_test_context->spdm_context;

    libspdm_zero_mem(&spdm_request, sizeof(spdm_request));
    spdm_request.header.spdm_version = SPDM_MESSAGE_VERSION_12;
    spdm_request_size = sizeof(spdm_request);
    spdm_request.header.request_response_code = SPDM_GET_CAPABILITIES;
    spdm_request.header.param1 = 0;
    spdm_request.header.param2 = 0;
    spdm_request.reserved = 0;
    spdm_request.ct_exponent = 0;
    spdm_request.reserved2 = 0;
    spdm_request.flags = SPDM_GET_CAPABILITIES_REQUEST_FLAGS_CERT_CAP |
                         SPDM_GET_CAPABILITIES_REQUEST_FLAGS_CHAL_CAP |
                         SPDM_GET_CAPABILITIES_REQUEST_FLAGS_ENCRYPT_CAP |
                         SPDM_GET_CAPABILITIES_REQUEST_FLAGS_MAC_CAP |
                         SPDM_GET_CAPABILITIES_REQUEST_FLAGS_MUT_AUTH_CAP |
                         SPDM_GET_CAPABILITIES_REQUEST_FLAGS_KEY_EX_CAP |
                         SPDM_GET_CAPABILITIES_REQUEST_FLAGS_PSK_CAP_REQUESTER |
                         SPDM_GET_CAPABILITIES_REQUEST_FLAGS_ENCAP_CAP |
                         SPDM_GET_CAPABILITIES_REQUEST_FLAGS_HBEAT_CAP |
                         SPDM_GET_CAPABILITIES_REQUEST_FLAGS_KEY_UPD_CAP;
    spdm_request.data_transfer_size = LIBSPDM_DATA_TRANSFER_SIZE;
    spdm_request.max_spdm_msg_size = LIBSPDM_MAX_SPDM_MSG_SIZE;

    spdm_response = (void *)message;
    spdm_response_size = sizeof(message);
    libspdm_zero_mem(message, sizeof(message));
    status = libspdm_send_receive_data(spdm_context, NULL, false,
                                       &spdm_request, spdm_request_size,
                                       spdm_response, &spdm_response_size);
    if (LIBSPDM_STATUS_IS_ERROR(status)) {
        common_test_record_test_assertion (
            SPDM_RESPONDER_TEST_GROUP_CAPABILITIES, SPDM_RESPONDER_TEST_CASE_CAPABILITIES_SUCCESS_12, COMMON_TEST_ID_END,
            COMMON_TEST_RESULT_NOT_TESTED, "send/receive failure");
        return ;
    }

    if (spdm_response_size >= sizeof(spdm_capabilities_response_t)) {
        test_result = COMMON_TEST_RESULT_PASS;
    } else {
        test_result = COMMON_TEST_RESULT_FAIL;
    }
    common_test_record_test_assertion (
        SPDM_RESPONDER_TEST_GROUP_CAPABILITIES, SPDM_RESPONDER_TEST_CASE_CAPABILITIES_SUCCESS_12, 1,
        test_result, "response size - %d", spdm_response_size);
    if (test_result == COMMON_TEST_RESULT_FAIL) {
        return ;
    }

    if (spdm_response->header.request_response_code == SPDM_CAPABILITIES) {
        test_result = COMMON_TEST_RESULT_PASS;
    } else {
        test_result = COMMON_TEST_RESULT_FAIL;
    }
    common_test_record_test_assertion (
        SPDM_RESPONDER_TEST_GROUP_CAPABILITIES, SPDM_RESPONDER_TEST_CASE_CAPABILITIES_SUCCESS_12, 2,
        test_result, "response code - 0x%02x", spdm_response->header.request_response_code);
    if (test_result == COMMON_TEST_RESULT_FAIL) {
        return ;
    }

    if (spdm_response->header.spdm_version == SPDM_MESSAGE_VERSION_12) {
        test_result = COMMON_TEST_RESULT_PASS;
    } else {
        test_result = COMMON_TEST_RESULT_FAIL;
    }
    common_test_record_test_assertion (
        SPDM_RESPONDER_TEST_GROUP_CAPABILITIES, SPDM_RESPONDER_TEST_CASE_CAPABILITIES_SUCCESS_12, 3,
        test_result, "response version - 0x%02x", spdm_response->header.spdm_version);
    if (test_result == COMMON_TEST_RESULT_FAIL) {
        return ;
    }

    flags = spdm_response->flags;
    if ((flags & SPDM_GET_CAPABILITIES_RESPONSE_FLAGS_MEAS_CAP) != SPDM_GET_CAPABILITIES_RESPONSE_FLAGS_MEAS_CAP) {
        test_result = COMMON_TEST_RESULT_PASS;
    } else {
        test_result = COMMON_TEST_RESULT_FAIL;
    }
    common_test_record_test_assertion (
        SPDM_RESPONDER_TEST_GROUP_CAPABILITIES, SPDM_RESPONDER_TEST_CASE_CAPABILITIES_SUCCESS_12, 4,
        test_result, "response flags - 0x%08x", spdm_response->flags);

    if ((flags & SPDM_GET_CAPABILITIES_RESPONSE_FLAGS_ENCRYPT_CAP) != 0) {
        if (((flags & SPDM_GET_CAPABILITIES_RESPONSE_FLAGS_KEY_EX_CAP) != 0) ||
            ((flags & SPDM_GET_CAPABILITIES_RESPONSE_FLAGS_PSK_CAP) != 0)) {
            test_result = COMMON_TEST_RESULT_PASS;
        } else {
            test_result = COMMON_TEST_RESULT_FAIL;
        }
        common_test_record_test_assertion (
            SPDM_RESPONDER_TEST_GROUP_CAPABILITIES, SPDM_RESPONDER_TEST_CASE_CAPABILITIES_SUCCESS_12, 5,
            test_result, "response flags - 0x%08x", spdm_response->flags);
    }

    if ((flags & SPDM_GET_CAPABILITIES_RESPONSE_FLAGS_MAC_CAP) != 0) {
        if (((flags & SPDM_GET_CAPABILITIES_RESPONSE_FLAGS_KEY_EX_CAP) != 0) ||
            ((flags & SPDM_GET_CAPABILITIES_RESPONSE_FLAGS_PSK_CAP) != 0)) {
            test_result = COMMON_TEST_RESULT_PASS;
        } else {
            test_result = COMMON_TEST_RESULT_FAIL;
        }
        common_test_record_test_assertion (
            SPDM_RESPONDER_TEST_GROUP_CAPABILITIES, SPDM_RESPONDER_TEST_CASE_CAPABILITIES_SUCCESS_12, 6,
            test_result, "response flags - 0x%08x", spdm_response->flags);
    }

    if ((flags & SPDM_GET_CAPABILITIES_RESPONSE_FLAGS_KEY_EX_CAP) != 0) {
        if (((flags & SPDM_GET_CAPABILITIES_RESPONSE_FLAGS_ENCRYPT_CAP) != 0) ||
            ((flags & SPDM_GET_CAPABILITIES_RESPONSE_FLAGS_MAC_CAP) != 0)) {
            test_result = COMMON_TEST_RESULT_PASS;
        } else {
            test_result = COMMON_TEST_RESULT_FAIL;
        }
        common_test_record_test_assertion (
            SPDM_RESPONDER_TEST_GROUP_CAPABILITIES, SPDM_RESPONDER_TEST_CASE_CAPABILITIES_SUCCESS_12, 7,
            test_result, "response flags - 0x%08x", spdm_response->flags);
    }

    if ((flags & SPDM_GET_CAPABILITIES_RESPONSE_FLAGS_PSK_CAP) != SPDM_GET_CAPABILITIES_RESPONSE_FLAGS_PSK_CAP) {
        test_result = COMMON_TEST_RESULT_PASS;
    } else {
        test_result = COMMON_TEST_RESULT_FAIL;
    }
    common_test_record_test_assertion (
        SPDM_RESPONDER_TEST_GROUP_CAPABILITIES, SPDM_RESPONDER_TEST_CASE_CAPABILITIES_SUCCESS_12, 8,
        test_result, "response flags - 0x%08x", spdm_response->flags);

    if ((flags & SPDM_GET_CAPABILITIES_RESPONSE_FLAGS_PSK_CAP) != 0) {
        if (((flags & SPDM_GET_CAPABILITIES_RESPONSE_FLAGS_ENCRYPT_CAP) != 0) ||
            ((flags & SPDM_GET_CAPABILITIES_RESPONSE_FLAGS_MAC_CAP) != 0)) {
            test_result = COMMON_TEST_RESULT_PASS;
        } else {
            test_result = COMMON_TEST_RESULT_FAIL;
        }
        common_test_record_test_assertion (
            SPDM_RESPONDER_TEST_GROUP_CAPABILITIES, SPDM_RESPONDER_TEST_CASE_CAPABILITIES_SUCCESS_12, 9,
            test_result, "response flags - 0x%08x", spdm_response->flags);
    }
    
    if ((flags & SPDM_GET_CAPABILITIES_RESPONSE_FLAGS_MUT_AUTH_CAP) != 0) {
        if ((flags & SPDM_GET_CAPABILITIES_RESPONSE_FLAGS_ENCAP_CAP) != 0) {
            test_result = COMMON_TEST_RESULT_PASS;
        } else {
            test_result = COMMON_TEST_RESULT_FAIL;
        }
        common_test_record_test_assertion (
            SPDM_RESPONDER_TEST_GROUP_CAPABILITIES, SPDM_RESPONDER_TEST_CASE_CAPABILITIES_SUCCESS_12, 10,
            test_result, "response flags - 0x%08x", spdm_response->flags);
    }

    if ((flags & SPDM_GET_CAPABILITIES_RESPONSE_FLAGS_HANDSHAKE_IN_THE_CLEAR_CAP) != 0) {
        if ((flags & SPDM_GET_CAPABILITIES_RESPONSE_FLAGS_KEY_EX_CAP) != 0) {
            test_result = COMMON_TEST_RESULT_PASS;
        } else {
            test_result = COMMON_TEST_RESULT_FAIL;
        }
        common_test_record_test_assertion (
            SPDM_RESPONDER_TEST_GROUP_CAPABILITIES, SPDM_RESPONDER_TEST_CASE_CAPABILITIES_SUCCESS_12, 11,
            test_result, "response flags - 0x%08x", spdm_response->flags);
    }

    if ((flags & SPDM_GET_CAPABILITIES_RESPONSE_FLAGS_PUB_KEY_ID_CAP) != 0) {
        if ((flags & SPDM_GET_CAPABILITIES_RESPONSE_FLAGS_CERT_CAP) == 0) {
            test_result = COMMON_TEST_RESULT_PASS;
        } else {
            test_result = COMMON_TEST_RESULT_FAIL;
        }
        common_test_record_test_assertion (
            SPDM_RESPONDER_TEST_GROUP_CAPABILITIES, SPDM_RESPONDER_TEST_CASE_CAPABILITIES_SUCCESS_12, 12,
            test_result, "response flags - 0x%08x", spdm_response->flags);
    }

    if (spdm_response->data_transfer_size >= SPDM_MIN_DATA_TRRANSFER_SIZE_VERSION_12) {
        test_result = COMMON_TEST_RESULT_PASS;
    } else {
        test_result = COMMON_TEST_RESULT_FAIL;
    }
    common_test_record_test_assertion (
        SPDM_RESPONDER_TEST_GROUP_CAPABILITIES, SPDM_RESPONDER_TEST_CASE_CAPABILITIES_SUCCESS_12, 13,
        test_result, "response data_transfer_size - 0x%08x", spdm_response->data_transfer_size);

    if (spdm_response->max_spdm_msg_size >= spdm_response->data_transfer_size) {
        test_result = COMMON_TEST_RESULT_PASS;
    } else {
        test_result = COMMON_TEST_RESULT_FAIL;
    }
    common_test_record_test_assertion (
        SPDM_RESPONDER_TEST_GROUP_CAPABILITIES, SPDM_RESPONDER_TEST_CASE_CAPABILITIES_SUCCESS_12, 14,
        test_result, "response max_spdm_msg_size - 0x%08x, data_transfer_size - 0x%08x", spdm_response->max_spdm_msg_size, spdm_response->data_transfer_size);
}

void spdm_test_case_capabilities_unexpected_non_identical (void *test_context)
{
    spdm_test_context_t *spdm_test_context;
    void *spdm_context;
    libspdm_return_t status;
    spdm_get_capabilities_request_t spdm_request;
    spdm_get_capabilities_request_t spdm_request_new;
    size_t spdm_request_size;
    spdm_capabilities_response_t *spdm_response;
    uint8_t message[LIBSPDM_MAX_MESSAGE_BUFFER_SIZE];
    size_t spdm_response_size;
    common_test_result_t test_result;
    uint8_t version;
    size_t index;
    uint8_t version_number_entry_count;
    spdm_version_number_t version_number_entry[LIBSPDM_MAX_VERSION_COUNT];
    uint32_t support_version_bitmask;

    spdm_test_context = test_context;
    spdm_context = spdm_test_context->spdm_context;

    libspdm_copy_mem (&version_number_entry_count,
                      sizeof(version_number_entry_count),
                      spdm_test_context->test_scratch_buffer,
                      sizeof(version_number_entry_count));
    libspdm_copy_mem (version_number_entry,
                      sizeof(version_number_entry),
                      spdm_test_context->test_scratch_buffer + sizeof(version_number_entry_count),
                      sizeof(spdm_version_number_t) * version_number_entry_count);

    support_version_bitmask = 0;
    for (index = 0; index < version_number_entry_count; index++) {
        version = version_number_entry[index] >> SPDM_VERSION_NUMBER_SHIFT_BIT;
        if (version == SPDM_MESSAGE_VERSION_10) {
            support_version_bitmask |= SPDM_TEST_VERSION_MASK_V10;
        } else if (version == SPDM_MESSAGE_VERSION_11) {
            support_version_bitmask |= SPDM_TEST_VERSION_MASK_V11;
        } else if (version == SPDM_MESSAGE_VERSION_12) {
            support_version_bitmask |= SPDM_TEST_VERSION_MASK_V12;
        }
    }

    if ((support_version_bitmask & SPDM_TEST_VERSION_MASK_V12) != 0) {
        version = SPDM_MESSAGE_VERSION_12;
        spdm_request_size = sizeof(spdm_request);
    } else if ((support_version_bitmask & SPDM_TEST_VERSION_MASK_V11) != 0) {
        version = SPDM_MESSAGE_VERSION_11;
        spdm_request_size = offsetof(spdm_get_capabilities_request_t, data_transfer_size);
    } else {
        version = SPDM_MESSAGE_VERSION_10;
        spdm_request_size = sizeof(spdm_request.header);
    }

    libspdm_zero_mem(&spdm_request, sizeof(spdm_request));
    spdm_request.header.spdm_version = version;
    spdm_request_size = sizeof(spdm_request);
    spdm_request.header.request_response_code = SPDM_GET_CAPABILITIES;
    spdm_request.header.param1 = 0;
    spdm_request.header.param2 = 0;
    spdm_request.reserved = 0;
    spdm_request.ct_exponent = 0;
    spdm_request.reserved2 = 0;
    spdm_request.flags = SPDM_GET_CAPABILITIES_REQUEST_FLAGS_CERT_CAP |
                         SPDM_GET_CAPABILITIES_REQUEST_FLAGS_CHAL_CAP |
                         SPDM_GET_CAPABILITIES_REQUEST_FLAGS_ENCRYPT_CAP |
                         SPDM_GET_CAPABILITIES_REQUEST_FLAGS_MAC_CAP |
                         SPDM_GET_CAPABILITIES_REQUEST_FLAGS_MUT_AUTH_CAP |
                         SPDM_GET_CAPABILITIES_REQUEST_FLAGS_KEY_EX_CAP |
                         SPDM_GET_CAPABILITIES_REQUEST_FLAGS_PSK_CAP_REQUESTER |
                         SPDM_GET_CAPABILITIES_REQUEST_FLAGS_ENCAP_CAP |
                         SPDM_GET_CAPABILITIES_REQUEST_FLAGS_HBEAT_CAP |
                         SPDM_GET_CAPABILITIES_REQUEST_FLAGS_KEY_UPD_CAP;
    spdm_request.data_transfer_size = LIBSPDM_DATA_TRANSFER_SIZE;
    spdm_request.max_spdm_msg_size = LIBSPDM_MAX_SPDM_MSG_SIZE;

    spdm_response = (void *)message;
    spdm_response_size = sizeof(message);
    libspdm_zero_mem(message, sizeof(message));
    status = libspdm_send_receive_data(spdm_context, NULL, false,
                                       &spdm_request, spdm_request_size,
                                       spdm_response, &spdm_response_size);
    if (LIBSPDM_STATUS_IS_ERROR(status)) {
        common_test_record_test_assertion (
            SPDM_RESPONDER_TEST_GROUP_CAPABILITIES, SPDM_RESPONDER_TEST_CASE_CAPABILITIES_UNEXPECTED_REQUEST_NON_IDENTICAL, COMMON_TEST_ID_END,
            COMMON_TEST_RESULT_NOT_TESTED, "First send/receive failure");
        return ;
    }
    if (spdm_response->header.request_response_code != SPDM_CAPABILITIES) {
        common_test_record_test_assertion (
            SPDM_RESPONDER_TEST_GROUP_CAPABILITIES, SPDM_RESPONDER_TEST_CASE_CAPABILITIES_UNEXPECTED_REQUEST_NON_IDENTICAL, COMMON_TEST_ID_END,
            COMMON_TEST_RESULT_NOT_TESTED, "First GET_CAPABILITIES failure");
        return ;
    }

    if ((support_version_bitmask & (SPDM_TEST_VERSION_MASK_V10 | SPDM_TEST_VERSION_MASK_V11 | SPDM_TEST_VERSION_MASK_V12)) != 0) {
        for (index = 0; index < 1; index++) {
            libspdm_copy_mem (&spdm_request_new, sizeof(spdm_request_new), &spdm_request, sizeof(spdm_request));
            spdm_request_new.header.param2 = 1;

            spdm_response = (void *)message;
            spdm_response_size = sizeof(message);
            libspdm_zero_mem(message, sizeof(message));
            status = libspdm_send_receive_data(spdm_context, NULL, false,
                                            &spdm_request, spdm_request_size,
                                            spdm_response, &spdm_response_size);
            if (LIBSPDM_STATUS_IS_ERROR(status)) {
                common_test_record_test_assertion (
                    SPDM_RESPONDER_TEST_GROUP_CAPABILITIES, SPDM_RESPONDER_TEST_CASE_CAPABILITIES_UNEXPECTED_REQUEST_NON_IDENTICAL, COMMON_TEST_ID_END,
                    COMMON_TEST_RESULT_NOT_TESTED, "Second send/receive failure");
                return ;
            }

            if (spdm_response_size >= sizeof(spdm_error_response_t)) {
                test_result = COMMON_TEST_RESULT_PASS;
            } else {
                test_result = COMMON_TEST_RESULT_FAIL;
            }
            common_test_record_test_assertion (
                SPDM_RESPONDER_TEST_GROUP_CAPABILITIES, SPDM_RESPONDER_TEST_CASE_CAPABILITIES_UNEXPECTED_REQUEST_NON_IDENTICAL, 1,
                test_result, "response size - %d", spdm_response_size);
            if (test_result == COMMON_TEST_RESULT_FAIL) {
                continue ;
            }

            if (spdm_response->header.request_response_code == SPDM_ERROR) {
                test_result = COMMON_TEST_RESULT_PASS;
            } else {
                test_result = COMMON_TEST_RESULT_FAIL;
            }
            common_test_record_test_assertion (
                SPDM_RESPONDER_TEST_GROUP_CAPABILITIES, SPDM_RESPONDER_TEST_CASE_CAPABILITIES_UNEXPECTED_REQUEST_NON_IDENTICAL, 2,
                test_result, "response code - 0x%02x", spdm_response->header.request_response_code);
            if (test_result == COMMON_TEST_RESULT_FAIL) {
                continue ;
            }

            if (spdm_response->header.spdm_version == version) {
                test_result = COMMON_TEST_RESULT_PASS;
            } else {
                test_result = COMMON_TEST_RESULT_FAIL;
            }
            common_test_record_test_assertion (
                SPDM_RESPONDER_TEST_GROUP_CAPABILITIES, SPDM_RESPONDER_TEST_CASE_CAPABILITIES_UNEXPECTED_REQUEST_NON_IDENTICAL, 3,
                test_result, "response version - 0x%02x", spdm_response->header.spdm_version);
            if (test_result == COMMON_TEST_RESULT_FAIL) {
                continue ;
            }

            if (spdm_response->header.param1 == SPDM_ERROR_CODE_UNEXPECTED_REQUEST) {
                test_result = COMMON_TEST_RESULT_PASS;
            } else {
                test_result = COMMON_TEST_RESULT_FAIL;
            }
            common_test_record_test_assertion (
                SPDM_RESPONDER_TEST_GROUP_CAPABILITIES, SPDM_RESPONDER_TEST_CASE_CAPABILITIES_UNEXPECTED_REQUEST_NON_IDENTICAL, 4,
                test_result, "response param1 - 0x%02x", spdm_response->header.param1);

            if (spdm_response->header.param2 == 0) {
                test_result = COMMON_TEST_RESULT_PASS;
            } else {
                test_result = COMMON_TEST_RESULT_FAIL;
            }
            common_test_record_test_assertion (
                SPDM_RESPONDER_TEST_GROUP_CAPABILITIES, SPDM_RESPONDER_TEST_CASE_CAPABILITIES_UNEXPECTED_REQUEST_NON_IDENTICAL, 5,
                test_result, "response param2 - 0x%02x", spdm_response->header.param2);
        }
    }

    if ((support_version_bitmask & (SPDM_TEST_VERSION_MASK_V11 | SPDM_TEST_VERSION_MASK_V12)) != 0) {
        for (index = 0; index < 1; index++) {
            libspdm_copy_mem (&spdm_request_new, sizeof(spdm_request_new), &spdm_request, sizeof(spdm_request));
            spdm_request_new.ct_exponent += 1;
            spdm_request_new.flags &= ~SPDM_GET_CAPABILITIES_REQUEST_FLAGS_HBEAT_CAP;

            spdm_response = (void *)message;
            spdm_response_size = sizeof(message);
            libspdm_zero_mem(message, sizeof(message));
            status = libspdm_send_receive_data(spdm_context, NULL, false,
                                            &spdm_request, spdm_request_size,
                                            spdm_response, &spdm_response_size);
            if (LIBSPDM_STATUS_IS_ERROR(status)) {
                common_test_record_test_assertion (
                    SPDM_RESPONDER_TEST_GROUP_CAPABILITIES, SPDM_RESPONDER_TEST_CASE_CAPABILITIES_UNEXPECTED_REQUEST_NON_IDENTICAL, COMMON_TEST_ID_END,
                    COMMON_TEST_RESULT_NOT_TESTED, "Second send/receive failure");
                return ;
            }

            if (spdm_response_size >= sizeof(spdm_error_response_t)) {
                test_result = COMMON_TEST_RESULT_PASS;
            } else {
                test_result = COMMON_TEST_RESULT_FAIL;
            }
            common_test_record_test_assertion (
                SPDM_RESPONDER_TEST_GROUP_CAPABILITIES, SPDM_RESPONDER_TEST_CASE_CAPABILITIES_UNEXPECTED_REQUEST_NON_IDENTICAL, 1,
                test_result, "response size - %d", spdm_response_size);
            if (test_result == COMMON_TEST_RESULT_FAIL) {
                continue ;
            }

            if (spdm_response->header.request_response_code == SPDM_ERROR) {
                test_result = COMMON_TEST_RESULT_PASS;
            } else {
                test_result = COMMON_TEST_RESULT_FAIL;
            }
            common_test_record_test_assertion (
                SPDM_RESPONDER_TEST_GROUP_CAPABILITIES, SPDM_RESPONDER_TEST_CASE_CAPABILITIES_UNEXPECTED_REQUEST_NON_IDENTICAL, 2,
                test_result, "response code - 0x%02x", spdm_response->header.request_response_code);
            if (test_result == COMMON_TEST_RESULT_FAIL) {
                continue ;
            }

            if (spdm_response->header.spdm_version == version) {
                test_result = COMMON_TEST_RESULT_PASS;
            } else {
                test_result = COMMON_TEST_RESULT_FAIL;
            }
            common_test_record_test_assertion (
                SPDM_RESPONDER_TEST_GROUP_CAPABILITIES, SPDM_RESPONDER_TEST_CASE_CAPABILITIES_UNEXPECTED_REQUEST_NON_IDENTICAL, 3,
                test_result, "response version - 0x%02x", spdm_response->header.spdm_version);
            if (test_result == COMMON_TEST_RESULT_FAIL) {
                continue ;
            }

            if (spdm_response->header.param1 == SPDM_ERROR_CODE_UNEXPECTED_REQUEST) {
                test_result = COMMON_TEST_RESULT_PASS;
            } else {
                test_result = COMMON_TEST_RESULT_FAIL;
            }
            common_test_record_test_assertion (
                SPDM_RESPONDER_TEST_GROUP_CAPABILITIES, SPDM_RESPONDER_TEST_CASE_CAPABILITIES_UNEXPECTED_REQUEST_NON_IDENTICAL, 4,
                test_result, "response param1 - 0x%02x", spdm_response->header.param1);

            if (spdm_response->header.param2 == 0) {
                test_result = COMMON_TEST_RESULT_PASS;
            } else {
                test_result = COMMON_TEST_RESULT_FAIL;
            }
            common_test_record_test_assertion (
                SPDM_RESPONDER_TEST_GROUP_CAPABILITIES, SPDM_RESPONDER_TEST_CASE_CAPABILITIES_UNEXPECTED_REQUEST_NON_IDENTICAL, 5,
                test_result, "response param2 - 0x%02x", spdm_response->header.param2);
        }
    }
    
    if ((support_version_bitmask & SPDM_TEST_VERSION_MASK_V12) != 0) {
        for (index = 0; index < 1; index++) {
            libspdm_copy_mem (&spdm_request_new, sizeof(spdm_request_new), &spdm_request, sizeof(spdm_request));
            spdm_request_new.data_transfer_size += 1;
            spdm_request_new.max_spdm_msg_size += 1;

            spdm_response = (void *)message;
            spdm_response_size = sizeof(message);
            libspdm_zero_mem(message, sizeof(message));
            status = libspdm_send_receive_data(spdm_context, NULL, false,
                                            &spdm_request, spdm_request_size,
                                            spdm_response, &spdm_response_size);
            if (LIBSPDM_STATUS_IS_ERROR(status)) {
                common_test_record_test_assertion (
                    SPDM_RESPONDER_TEST_GROUP_CAPABILITIES, SPDM_RESPONDER_TEST_CASE_CAPABILITIES_UNEXPECTED_REQUEST_NON_IDENTICAL, COMMON_TEST_ID_END,
                    COMMON_TEST_RESULT_NOT_TESTED, "Second send/receive failure");
                return ;
            }

            if (spdm_response_size >= sizeof(spdm_error_response_t)) {
                test_result = COMMON_TEST_RESULT_PASS;
            } else {
                test_result = COMMON_TEST_RESULT_FAIL;
            }
            common_test_record_test_assertion (
                SPDM_RESPONDER_TEST_GROUP_CAPABILITIES, SPDM_RESPONDER_TEST_CASE_CAPABILITIES_UNEXPECTED_REQUEST_NON_IDENTICAL, 1,
                test_result, "response size - %d", spdm_response_size);
            if (test_result == COMMON_TEST_RESULT_FAIL) {
                continue ;
            }

            if (spdm_response->header.request_response_code == SPDM_ERROR) {
                test_result = COMMON_TEST_RESULT_PASS;
            } else {
                test_result = COMMON_TEST_RESULT_FAIL;
            }
            common_test_record_test_assertion (
                SPDM_RESPONDER_TEST_GROUP_CAPABILITIES, SPDM_RESPONDER_TEST_CASE_CAPABILITIES_UNEXPECTED_REQUEST_NON_IDENTICAL, 2,
                test_result, "response code - 0x%02x", spdm_response->header.request_response_code);
            if (test_result == COMMON_TEST_RESULT_FAIL) {
                continue ;
            }

            if (spdm_response->header.spdm_version == version) {
                test_result = COMMON_TEST_RESULT_PASS;
            } else {
                test_result = COMMON_TEST_RESULT_FAIL;
            }
            common_test_record_test_assertion (
                SPDM_RESPONDER_TEST_GROUP_CAPABILITIES, SPDM_RESPONDER_TEST_CASE_CAPABILITIES_UNEXPECTED_REQUEST_NON_IDENTICAL, 3,
                test_result, "response version - 0x%02x", spdm_response->header.spdm_version);
            if (test_result == COMMON_TEST_RESULT_FAIL) {
                continue ;
            }

            if (spdm_response->header.param1 == SPDM_ERROR_CODE_UNEXPECTED_REQUEST) {
                test_result = COMMON_TEST_RESULT_PASS;
            } else {
                test_result = COMMON_TEST_RESULT_FAIL;
            }
            common_test_record_test_assertion (
                SPDM_RESPONDER_TEST_GROUP_CAPABILITIES, SPDM_RESPONDER_TEST_CASE_CAPABILITIES_UNEXPECTED_REQUEST_NON_IDENTICAL, 4,
                test_result, "response param1 - 0x%02x", spdm_response->header.param1);

            if (spdm_response->header.param2 == 0) {
                test_result = COMMON_TEST_RESULT_PASS;
            } else {
                test_result = COMMON_TEST_RESULT_FAIL;
            }
            common_test_record_test_assertion (
                SPDM_RESPONDER_TEST_GROUP_CAPABILITIES, SPDM_RESPONDER_TEST_CASE_CAPABILITIES_UNEXPECTED_REQUEST_NON_IDENTICAL, 5,
                test_result, "response param2 - 0x%02x", spdm_response->header.param2);
        }
    }
}

common_test_case_t m_spdm_test_group_capabilities[] = {
    {SPDM_RESPONDER_TEST_CASE_CAPABILITIES_SUCCESS_10, "spdm_test_case_capabilities_success_10", spdm_test_case_capabilities_success_10, spdm_test_case_capabilities_setup_version_10},
    {SPDM_RESPONDER_TEST_CASE_CAPABILITIES_VERSION_MISMATCH, "spdm_test_case_capabilities_version_mismatch", spdm_test_case_capabilities_version_mismatch, spdm_test_case_capabilities_setup_version_all},
    /* {SPDM_RESPONDER_TEST_CASE_CAPABILITIES_UNSUPPORTED_REQUEST, "spdm_test_case_capabilities_unsupported_request", spdm_test_case_capabilities_unsupported_request, spdm_test_case_capabilities_setup_version_all}, */
    {SPDM_RESPONDER_TEST_CASE_CAPABILITIES_SUCCESS_11, "spdm_test_case_capabilities_success_11", spdm_test_case_capabilities_success_11, spdm_test_case_capabilities_setup_version_11},
    {SPDM_RESPONDER_TEST_CASE_CAPABILITIES_INVALID_REQUEST, "spdm_test_case_capabilities_invalid_request", spdm_test_case_capabilities_invalid_request, spdm_test_case_capabilities_setup_version_all},
    {SPDM_RESPONDER_TEST_CASE_CAPABILITIES_SUCCESS_12, "spdm_test_case_capabilities_success_12", spdm_test_case_capabilities_success_12, spdm_test_case_capabilities_setup_version_12},
    {SPDM_RESPONDER_TEST_CASE_CAPABILITIES_UNEXPECTED_REQUEST_NON_IDENTICAL, "spdm_test_case_capabilities_unexpected_non_identical", spdm_test_case_capabilities_unexpected_non_identical, spdm_test_case_capabilities_setup_version_all},
    {COMMON_TEST_ID_END, NULL, NULL},
};

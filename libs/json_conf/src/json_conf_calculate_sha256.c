/**
 ******************************************************************************
 * @file           :  json_conf_calculate_sha256
 * @brief          :
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */

#include "json_conf_calculate_sha256.h"
#include <openssl/evp.h>
#include <openssl/sha.h>

#define BUFFER_SIZE 8192

bool json_conf_calculate_sha256(FILE *file, unsigned char *hash) {
    EVP_MD_CTX *mdctx;
    const EVP_MD *md = EVP_sha256();
    mdctx = EVP_MD_CTX_new();
    EVP_DigestInit_ex(mdctx, md, NULL);

    unsigned char buffer[BUFFER_SIZE];
    size_t bytes_read;
    while ((bytes_read = fread(buffer, 1, sizeof(buffer), file)) > 0) {
        EVP_DigestUpdate(mdctx, buffer, bytes_read);
    }

    unsigned int hash_len;
    EVP_DigestFinal_ex(mdctx, hash, &hash_len);
    EVP_MD_CTX_free(mdctx);

    printf("SHA256 hash calculated from JSON file: ");
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        printf("%02x", hash[i]);
    }

    printf("\n");

    return true;
}


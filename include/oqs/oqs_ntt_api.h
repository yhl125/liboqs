/**
 * @file oqs_ntt_api.h
 * @brief Custom API for exposing NTT (Number Theoretic Transform) functions
 *        from liboqs signature schemes
 *
 * This header provides a unified interface to access NTT functions used internally
 * by various post-quantum signature schemes in liboqs. NTT is a key computational
 * primitive used for efficient polynomial multiplication in lattice-based cryptography.
 *
 * Supported Signature Schemes:
 * - ML-DSA (FIPS 204 - Module Lattice Digital Signature Algorithm)
 *   - ML-DSA-44, ML-DSA-65, ML-DSA-87
 * - Falcon (NIST Round 3 finalist)
 *   - Falcon-512, Falcon-1024, Falcon-padded-512, Falcon-padded-1024
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef OQS_NTT_API_H
#define OQS_NTT_API_H

#include <stdint.h>

#if defined(__cplusplus)
extern "C" {
#endif

/* ============================================================================
 * ML-DSA (Dilithium) NTT Functions
 * ============================================================================
 * ML-DSA uses NTT over polynomials in Z_q[X]/(X^256 + 1) with q = 8380417
 * - Polynomial degree: N = 256
 * - Coefficient type: int32_t
 * - All three security levels (44, 65, 87) share the same NTT parameters
 */

/**
 * @brief ML-DSA-44 Reference Implementation - Forward NTT
 *
 * Performs in-place forward NTT transformation.
 * No modular reduction after additions/subtractions.
 * Output is in bitreversed order.
 *
 * @param a Pointer to array of 256 int32_t coefficients (input/output)
 */
void OQS_SIG_ml_dsa_44_ref_ntt(int32_t a[256]);

/**
 * @brief ML-DSA-44 Reference Implementation - Inverse NTT with Montgomery
 *
 * Performs in-place inverse NTT and multiplies by Montgomery factor 2^32.
 * Input coefficients must be smaller than Q in absolute value.
 *
 * @param a Pointer to array of 256 int32_t coefficients (input/output)
 */
void OQS_SIG_ml_dsa_44_ref_invntt_tomont(int32_t a[256]);

/**
 * @brief ML-DSA-44 Reference Implementation - Inverse NTT
 *
 * Performs in-place inverse NTT and applies Montgomery reduction to return
 * coefficients in normal form (not Montgomery form).
 *
 * @param a Pointer to array of 256 int32_t coefficients (input/output)
 */
void OQS_SIG_ml_dsa_44_ref_invntt(int32_t a[256]);

/**
 * @brief ML-DSA-65 Reference Implementation - Forward NTT
 * @param a Pointer to array of 256 int32_t coefficients (input/output)
 */
void OQS_SIG_ml_dsa_65_ref_ntt(int32_t a[256]);

/**
 * @brief ML-DSA-65 Reference Implementation - Inverse NTT with Montgomery
 * @param a Pointer to array of 256 int32_t coefficients (input/output)
 */
void OQS_SIG_ml_dsa_65_ref_invntt_tomont(int32_t a[256]);

/**
 * @brief ML-DSA-65 Reference Implementation - Inverse NTT
 * @param a Pointer to array of 256 int32_t coefficients (input/output)
 */
void OQS_SIG_ml_dsa_65_ref_invntt(int32_t a[256]);

/**
 * @brief ML-DSA-87 Reference Implementation - Forward NTT
 * @param a Pointer to array of 256 int32_t coefficients (input/output)
 */
void OQS_SIG_ml_dsa_87_ref_ntt(int32_t a[256]);

/**
 * @brief ML-DSA-87 Reference Implementation - Inverse NTT with Montgomery
 * @param a Pointer to array of 256 int32_t coefficients (input/output)
 */
void OQS_SIG_ml_dsa_87_ref_invntt_tomont(int32_t a[256]);

/**
 * @brief ML-DSA-87 Reference Implementation - Inverse NTT
 * @param a Pointer to array of 256 int32_t coefficients (input/output)
 */
void OQS_SIG_ml_dsa_87_ref_invntt(int32_t a[256]);

/* ============================================================================
 * Falcon NTT Functions (Clean/Portable Implementation)
 * ============================================================================
 * Falcon clean implementation uses simpler integer NTT with modular arithmetic
 * Works on all platforms without SIMD optimizations
 * - Modulus: q = 12289
 * - logn parameter: 9 for Falcon-512 (n=512), 10 for Falcon-1024 (n=1024)
 * - Coefficient type: uint16_t in range [0, q-1]
 *
 * Note: All Falcon variants (512, 1024, padded-512, padded-1024) use the same
 * NTT algorithm, so only two functions are needed for all variants.
 */

/**
 * @brief Falcon Clean Implementation - Forward NTT
 *
 * Performs in-place forward NTT using modular arithmetic over Z_12289.
 * This is a portable C implementation without SIMD optimizations.
 * Works for all Falcon variants (512, 1024, padded-512, padded-1024).
 *
 * @param a Pointer to array of uint16_t coefficients (input/output)
 * @param logn Base-2 logarithm of polynomial degree (9 for n=512, 10 for n=1024)
 */
void FALCON_CLEAN_mq_NTT(uint16_t *a, unsigned logn);

/**
 * @brief Falcon Clean Implementation - Inverse NTT
 *
 * Performs in-place inverse NTT with final division by n.
 * Works for all Falcon variants (512, 1024, padded-512, padded-1024).
 *
 * @param a Pointer to array of uint16_t coefficients (input/output)
 * @param logn Base-2 logarithm of polynomial degree (9 for n=512, 10 for n=1024)
 */
void FALCON_CLEAN_mq_iNTT(uint16_t *a, unsigned logn);

#if defined(__cplusplus)
}
#endif

#endif /* OQS_NTT_API_H */

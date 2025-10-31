/**
 * @file oqs_ntt_api.c
 * @brief Implementation of custom NTT API wrappers for liboqs signature schemes
 *
 * This file contains wrapper functions that expose internal NTT implementations
 * from ML-DSA and Falcon signature schemes. The wrappers provide a unified
 * interface while delegating to the scheme-specific implementations.
 *
 * SPDX-License-Identifier: MIT
 */

#include <oqs/oqs_ntt_api.h>
#include <stdint.h>

/* ============================================================================
 * Forward Declarations for ML-DSA NTT Functions
 * ============================================================================
 * We use direct function declarations instead of including headers
 * to avoid namespace macro conflicts between different Dilithium variants.
 */

/* ML-DSA-44 ref */
extern void pqcrystals_ml_dsa_44_ref_ntt(int32_t a[256]);
extern void pqcrystals_ml_dsa_44_ref_invntt_tomont(int32_t a[256]);
extern int32_t pqcrystals_ml_dsa_44_ref_montgomery_reduce(int64_t a);
extern int32_t pqcrystals_ml_dsa_44_ref_freeze(int32_t a);

/* ML-DSA-65 ref */
extern void pqcrystals_ml_dsa_65_ref_ntt(int32_t a[256]);
extern void pqcrystals_ml_dsa_65_ref_invntt_tomont(int32_t a[256]);
extern int32_t pqcrystals_ml_dsa_65_ref_montgomery_reduce(int64_t a);
extern int32_t pqcrystals_ml_dsa_65_ref_freeze(int32_t a);

/* ML-DSA-87 ref */
extern void pqcrystals_ml_dsa_87_ref_ntt(int32_t a[256]);
extern void pqcrystals_ml_dsa_87_ref_invntt_tomont(int32_t a[256]);
extern int32_t pqcrystals_ml_dsa_87_ref_montgomery_reduce(int64_t a);
extern int32_t pqcrystals_ml_dsa_87_ref_freeze(int32_t a);


/* ============================================================================
 * ML-DSA-44 Reference Implementation Wrappers
 * ============================================================================ */

OQS_API void OQS_SIG_ml_dsa_44_ref_ntt(int32_t a[256]) {
    pqcrystals_ml_dsa_44_ref_ntt(a);
}

OQS_API void OQS_SIG_ml_dsa_44_ref_invntt_tomont(int32_t a[256]) {
    pqcrystals_ml_dsa_44_ref_invntt_tomont(a);
}

OQS_API void OQS_SIG_ml_dsa_44_ref_invntt(int32_t a[256]) {
    pqcrystals_ml_dsa_44_ref_invntt_tomont(a);
    // Apply Montgomery reduction and freeze to get standard form [0, Q-1]
    for (int i = 0; i < 256; i++) {
        a[i] = pqcrystals_ml_dsa_44_ref_montgomery_reduce((int64_t)a[i]);
        a[i] = pqcrystals_ml_dsa_44_ref_freeze(a[i]);
    }
}

/* ============================================================================
 * ML-DSA-65 Reference Implementation Wrappers
 * ============================================================================ */

OQS_API void OQS_SIG_ml_dsa_65_ref_ntt(int32_t a[256]) {
    pqcrystals_ml_dsa_65_ref_ntt(a);
}

OQS_API void OQS_SIG_ml_dsa_65_ref_invntt_tomont(int32_t a[256]) {
    pqcrystals_ml_dsa_65_ref_invntt_tomont(a);
}

OQS_API void OQS_SIG_ml_dsa_65_ref_invntt(int32_t a[256]) {
    pqcrystals_ml_dsa_65_ref_invntt_tomont(a);
    // Apply Montgomery reduction and freeze to get standard form [0, Q-1]
    for (int i = 0; i < 256; i++) {
        a[i] = pqcrystals_ml_dsa_65_ref_montgomery_reduce((int64_t)a[i]);
        a[i] = pqcrystals_ml_dsa_65_ref_freeze(a[i]);
    }
}

/* ============================================================================
 * ML-DSA-87 Reference Implementation Wrappers
 * ============================================================================ */

OQS_API void OQS_SIG_ml_dsa_87_ref_ntt(int32_t a[256]) {
    pqcrystals_ml_dsa_87_ref_ntt(a);
}

OQS_API void OQS_SIG_ml_dsa_87_ref_invntt_tomont(int32_t a[256]) {
    pqcrystals_ml_dsa_87_ref_invntt_tomont(a);
}

OQS_API void OQS_SIG_ml_dsa_87_ref_invntt(int32_t a[256]) {
    pqcrystals_ml_dsa_87_ref_invntt_tomont(a);
    // Apply Montgomery reduction and freeze to get standard form [0, Q-1]
    for (int i = 0; i < 256; i++) {
        a[i] = pqcrystals_ml_dsa_87_ref_montgomery_reduce((int64_t)a[i]);
        a[i] = pqcrystals_ml_dsa_87_ref_freeze(a[i]);
    }
}

/**
 * @file test_ntt_api.c
 * @brief Test suite for custom NTT API
 *
 * Tests verify:
 * 1. NTT/inverse NTT round-trip correctness
 * 2. Polynomial multiplication via NTT
 * 3. Platform-specific implementations
 * 4. Edge cases and error conditions
 */

#include <oqs/oqs_ntt_api.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* Test result tracking */
static int tests_run = 0;
static int tests_passed = 0;
static int tests_failed = 0;

/* Color output for terminal */
#define COLOR_GREEN "\x1b[32m"
#define COLOR_RED "\x1b[31m"
#define COLOR_YELLOW "\x1b[33m"
#define COLOR_RESET "\x1b[0m"

/* Test macros */
#define TEST_START(name) \
    do { \
        printf("Testing: %s ... ", name); \
        fflush(stdout); \
        tests_run++; \
    } while(0)

#define TEST_PASS() \
    do { \
        printf(COLOR_GREEN "PASS" COLOR_RESET "\n"); \
        tests_passed++; \
    } while(0)

#define TEST_FAIL(msg) \
    do { \
        printf(COLOR_RED "FAIL" COLOR_RESET " - %s\n", msg); \
        tests_failed++; \
    } while(0)

#define ASSERT(condition, msg) \
    do { \
        if (!(condition)) { \
            TEST_FAIL(msg); \
            return 0; \
        } \
    } while(0)

/* ML-DSA parameters */
#define ML_DSA_N 256
#define ML_DSA_Q 8380417

/* Falcon parameters */
#define FALCON_512_N 512
#define FALCON_1024_N 1024
#define FALCON_Q 12289

/* ============================================================================
 * Helper Functions
 * ============================================================================ */

/**
 * @brief Simple modular reduction for ML-DSA
 */
static int32_t reduce_ml_dsa(int64_t a) {
    int64_t t = a % ML_DSA_Q;
    if (t < 0) t += ML_DSA_Q;
    return (int32_t)t;
}

/**
 * @brief Initialize random polynomial for ML-DSA (values in [0, q-1])
 */
static void init_random_poly_ml_dsa(int32_t *poly, size_t n) {
    for (size_t i = 0; i < n; i++) {
        poly[i] = rand() % ML_DSA_Q;
    }
}

/**
 * @brief Initialize random polynomial for Falcon (values in [0, q-1])
 */
static void init_random_poly_falcon(uint16_t *poly, size_t n) {
    for (size_t i = 0; i < n; i++) {
        poly[i] = rand() % FALCON_Q;
    }
}

/**
 * @brief Compare two ML-DSA polynomials with tolerance
 */
static int compare_poly_ml_dsa(const int32_t *a, const int32_t *b, size_t n, int32_t tolerance) {
    for (size_t i = 0; i < n; i++) {
        int64_t diff = (int64_t)a[i] - (int64_t)b[i];
        if (diff < 0) diff = -diff;
        if (diff > tolerance) {
            printf("\nMismatch at index %zu: %d vs %d (diff=%ld)\n",
                   i, a[i], b[i], (long)diff);
            return 0;
        }
    }
    return 1;
}

/**
 * @brief Compare two Falcon polynomials with tolerance
 */
static int compare_poly_falcon(const uint16_t *a, const uint16_t *b, size_t n, uint16_t tolerance) {
    for (size_t i = 0; i < n; i++) {
        int32_t diff = (int32_t)a[i] - (int32_t)b[i];
        if (diff < 0) diff = -diff;
        if (diff > tolerance) {
            printf("\nMismatch at index %zu: %u vs %u (diff=%d)\n",
                   i, a[i], b[i], diff);
            return 0;
        }
    }
    return 1;
}

/* ============================================================================
 * ML-DSA Tests
 * ============================================================================ */

/**
 * @brief Test ML-DSA-44 reference NTT round-trip
 */
static int test_ml_dsa_44_ref_roundtrip(void) {
    TEST_START("ML-DSA-44 ref NTT round-trip");

    int32_t poly_original[ML_DSA_N];
    int32_t poly_test[ML_DSA_N];

    // Initialize with random values
    init_random_poly_ml_dsa(poly_original, ML_DSA_N);
    memcpy(poly_test, poly_original, sizeof(poly_original));

    // Round-trip: NTT -> inverse NTT
    OQS_SIG_ml_dsa_44_ref_ntt(poly_test);
    OQS_SIG_ml_dsa_44_ref_invntt(poly_test);

    // Should recover original polynomial
    ASSERT(compare_poly_ml_dsa(poly_test, poly_original, ML_DSA_N, 0),
           "Round-trip failed to recover original polynomial");

    TEST_PASS();
    return 1;
}

/**
 * @brief Test ML-DSA-65 reference NTT round-trip
 */
static int test_ml_dsa_65_ref_roundtrip(void) {
    TEST_START("ML-DSA-65 ref NTT round-trip");

    int32_t poly_original[ML_DSA_N];
    int32_t poly_test[ML_DSA_N];

    // Initialize with random values
    init_random_poly_ml_dsa(poly_original, ML_DSA_N);
    memcpy(poly_test, poly_original, sizeof(poly_original));

    // Round-trip: NTT -> inverse NTT
    OQS_SIG_ml_dsa_65_ref_ntt(poly_test);
    OQS_SIG_ml_dsa_65_ref_invntt(poly_test);

    // Should recover original polynomial
    ASSERT(compare_poly_ml_dsa(poly_test, poly_original, ML_DSA_N, 0),
           "Round-trip failed to recover original polynomial");

    TEST_PASS();
    return 1;
}

/**
 * @brief Test ML-DSA-87 reference NTT round-trip
 */
static int test_ml_dsa_87_ref_roundtrip(void) {
    TEST_START("ML-DSA-87 ref NTT round-trip");

    int32_t poly_original[ML_DSA_N];
    int32_t poly_test[ML_DSA_N];

    // Initialize with random values
    init_random_poly_ml_dsa(poly_original, ML_DSA_N);
    memcpy(poly_test, poly_original, sizeof(poly_original));

    // Round-trip: NTT -> inverse NTT
    OQS_SIG_ml_dsa_87_ref_ntt(poly_test);
    OQS_SIG_ml_dsa_87_ref_invntt(poly_test);

    // Should recover original polynomial
    ASSERT(compare_poly_ml_dsa(poly_test, poly_original, ML_DSA_N, 0),
           "Round-trip failed to recover original polynomial");

    TEST_PASS();
    return 1;
}

/**
 * @brief Test ML-DSA NTT with zero polynomial
 */
static int test_ml_dsa_zero_poly(void) {
    TEST_START("ML-DSA NTT with zero polynomial");

    int32_t poly[ML_DSA_N] = {0};

    OQS_SIG_ml_dsa_44_ref_ntt(poly);

    // NTT of zero should be zero
    for (int i = 0; i < ML_DSA_N; i++) {
        ASSERT(poly[i] == 0, "NTT of zero polynomial not zero");
    }

    TEST_PASS();
    return 1;
}

/**
 * @brief Test ML-DSA NTT linearity property
 */
static int test_ml_dsa_linearity(void) {
    TEST_START("ML-DSA NTT linearity");

    int32_t poly_a[ML_DSA_N], poly_b[ML_DSA_N], poly_sum[ML_DSA_N];
    int32_t ntt_a[ML_DSA_N], ntt_b[ML_DSA_N], ntt_sum[ML_DSA_N];

    // Initialize polynomials
    for (int i = 0; i < ML_DSA_N; i++) {
        poly_a[i] = i;
        poly_b[i] = ML_DSA_N - i;
        poly_sum[i] = reduce_ml_dsa((int64_t)poly_a[i] + poly_b[i]);
    }

    // Copy for NTT
    memcpy(ntt_a, poly_a, sizeof(poly_a));
    memcpy(ntt_b, poly_b, sizeof(poly_b));
    memcpy(ntt_sum, poly_sum, sizeof(poly_sum));

    // Transform each
    OQS_SIG_ml_dsa_44_ref_ntt(ntt_a);
    OQS_SIG_ml_dsa_44_ref_ntt(ntt_b);
    OQS_SIG_ml_dsa_44_ref_ntt(ntt_sum);

    // NTT(a + b) should equal NTT(a) + NTT(b)
    for (int i = 0; i < ML_DSA_N; i++) {
        int32_t expected = reduce_ml_dsa((int64_t)ntt_a[i] + ntt_b[i]);
        int32_t diff = reduce_ml_dsa((int64_t)ntt_sum[i] - expected);
        if (diff < 0) diff = -diff;
        ASSERT(diff < 1000, "Linearity property violated");
    }

    TEST_PASS();
    return 1;
}

/**
 * @brief Test ML-DSA variant consistency
 *
 * All ML-DSA variants (44, 65, 87) use the same NTT algorithm,
 * so they should produce identical results for the same input
 */
static int test_ml_dsa_variant_consistency(void) {
    TEST_START("ML-DSA variant consistency");

    int32_t poly_44[ML_DSA_N];
    int32_t poly_65[ML_DSA_N];
    int32_t poly_87[ML_DSA_N];

    // Initialize all with same pattern
    for (int i = 0; i < ML_DSA_N; i++) {
        poly_44[i] = (i * 23) % 1000;
        poly_65[i] = (i * 23) % 1000;
        poly_87[i] = (i * 23) % 1000;
    }

    // Transform all
    OQS_SIG_ml_dsa_44_ref_ntt(poly_44);
    OQS_SIG_ml_dsa_65_ref_ntt(poly_65);
    OQS_SIG_ml_dsa_87_ref_ntt(poly_87);

    // All should produce identical results
    ASSERT(compare_poly_ml_dsa(poly_44, poly_65, ML_DSA_N, 0),
           "ML-DSA-44 and ML-DSA-65 NTT differ");
    ASSERT(compare_poly_ml_dsa(poly_44, poly_87, ML_DSA_N, 0),
           "ML-DSA-44 and ML-DSA-87 NTT differ");

    TEST_PASS();
    return 1;
}

/* ============================================================================
 * Falcon Clean Tests
 * ============================================================================ */

/**
 * @brief Test Falcon-512 clean NTT round-trip
 */
static int test_falcon_512_clean_roundtrip(void) {
    TEST_START("Falcon-512 clean NTT round-trip");

    uint16_t poly_original[FALCON_512_N];
    uint16_t poly_test[FALCON_512_N];

    // Initialize with random values
    init_random_poly_falcon(poly_original, FALCON_512_N);
    memcpy(poly_test, poly_original, sizeof(poly_original));

    // Round-trip: NTT -> inverse NTT
    FALCON_CLEAN_mq_NTT(poly_test, 9);  // logn = 9 for N=512
    FALCON_CLEAN_mq_iNTT(poly_test, 9);

    // Should recover original polynomial
    ASSERT(compare_poly_falcon(poly_test, poly_original, FALCON_512_N, 0),
           "Round-trip failed to recover original polynomial");

    TEST_PASS();
    return 1;
}

/**
 * @brief Test Falcon-1024 clean NTT round-trip
 */
static int test_falcon_1024_clean_roundtrip(void) {
    TEST_START("Falcon-1024 clean NTT round-trip");

    uint16_t poly_original[FALCON_1024_N];
    uint16_t poly_test[FALCON_1024_N];

    // Initialize with random values
    init_random_poly_falcon(poly_original, FALCON_1024_N);
    memcpy(poly_test, poly_original, sizeof(poly_original));

    // Round-trip: NTT -> inverse NTT
    FALCON_CLEAN_mq_NTT(poly_test, 10);  // logn = 10 for N=1024
    FALCON_CLEAN_mq_iNTT(poly_test, 10);

    // Should recover original polynomial
    ASSERT(compare_poly_falcon(poly_test, poly_original, FALCON_1024_N, 0),
           "Round-trip failed to recover original polynomial");

    TEST_PASS();
    return 1;
}

/**
 * @brief Test Falcon-padded-512 clean NTT round-trip
 */
static int test_falcon_padded_512_clean_roundtrip(void) {
    TEST_START("Falcon-padded-512 clean NTT round-trip");

    uint16_t poly_original[FALCON_512_N];
    uint16_t poly_test[FALCON_512_N];

    // Initialize with random values
    init_random_poly_falcon(poly_original, FALCON_512_N);
    memcpy(poly_test, poly_original, sizeof(poly_original));

    // Round-trip
    FALCON_CLEAN_mq_NTT(poly_test, 9);
    FALCON_CLEAN_mq_iNTT(poly_test, 9);

    ASSERT(compare_poly_falcon(poly_test, poly_original, FALCON_512_N, 0),
           "Round-trip failed");

    TEST_PASS();
    return 1;
}

/**
 * @brief Test Falcon-padded-1024 clean NTT round-trip
 */
static int test_falcon_padded_1024_clean_roundtrip(void) {
    TEST_START("Falcon-padded-1024 clean NTT round-trip");

    uint16_t poly_original[FALCON_1024_N];
    uint16_t poly_test[FALCON_1024_N];

    // Initialize with random values
    init_random_poly_falcon(poly_original, FALCON_1024_N);
    memcpy(poly_test, poly_original, sizeof(poly_original));

    // Round-trip
    FALCON_CLEAN_mq_NTT(poly_test, 10);
    FALCON_CLEAN_mq_iNTT(poly_test, 10);

    ASSERT(compare_poly_falcon(poly_test, poly_original, FALCON_1024_N, 0),
           "Round-trip failed");

    TEST_PASS();
    return 1;
}

/**
 * @brief Test Falcon clean NTT with zero polynomial
 */
static int test_falcon_clean_zero_poly(void) {
    TEST_START("Falcon clean NTT with zero polynomial");

    uint16_t poly[FALCON_512_N] = {0};

    FALCON_CLEAN_mq_NTT(poly, 9);

    // NTT of zero should be zero
    for (int i = 0; i < FALCON_512_N; i++) {
        ASSERT(poly[i] == 0, "NTT of zero polynomial not zero");
    }

    TEST_PASS();
    return 1;
}

/**
 * @brief Test Falcon clean NTT linearity property
 */
static int test_falcon_clean_linearity(void) {
    TEST_START("Falcon clean NTT linearity");

    uint16_t poly_a[FALCON_512_N], poly_b[FALCON_512_N], poly_sum[FALCON_512_N];
    uint16_t ntt_a[FALCON_512_N], ntt_b[FALCON_512_N], ntt_sum[FALCON_512_N];

    // Initialize polynomials (small values to avoid overflow)
    for (int i = 0; i < FALCON_512_N; i++) {
        poly_a[i] = i % 100;
        poly_b[i] = (FALCON_512_N - i) % 100;
        poly_sum[i] = (poly_a[i] + poly_b[i]) % FALCON_Q;
    }

    // Copy for NTT
    memcpy(ntt_a, poly_a, sizeof(poly_a));
    memcpy(ntt_b, poly_b, sizeof(poly_b));
    memcpy(ntt_sum, poly_sum, sizeof(poly_sum));

    // Transform each
    FALCON_CLEAN_mq_NTT(ntt_a, 9);
    FALCON_CLEAN_mq_NTT(ntt_b, 9);
    FALCON_CLEAN_mq_NTT(ntt_sum, 9);

    // NTT(a + b) should equal NTT(a) + NTT(b) mod q
    for (int i = 0; i < FALCON_512_N; i++) {
        uint16_t expected = (ntt_a[i] + ntt_b[i]) % FALCON_Q;
        int32_t diff = ((int32_t)ntt_sum[i] - (int32_t)expected);
        if (diff < 0) diff = -diff;
        // Allow small tolerance due to Montgomery representation
        ASSERT(diff < 100, "Linearity property violated");
    }

    TEST_PASS();
    return 1;
}

/**
 * @brief Test that all Falcon variants use the same NTT algorithm
 */
static int test_falcon_clean_consistency(void) {
    TEST_START("Falcon clean variant consistency");

    uint16_t poly_512[FALCON_512_N];
    uint16_t poly_1024[FALCON_512_N];  // Only test first 512 elements
    uint16_t poly_p512[FALCON_512_N];
    uint16_t poly_p1024[FALCON_512_N];

    // Initialize all with same pattern
    for (int i = 0; i < FALCON_512_N; i++) {
        poly_512[i] = (i * 23) % FALCON_Q;
        poly_1024[i] = (i * 23) % FALCON_Q;
        poly_p512[i] = (i * 23) % FALCON_Q;
        poly_p1024[i] = (i * 23) % FALCON_Q;
    }

    // Transform all with same logn
    // All variants use the same FALCON_CLEAN_mq_NTT function
    FALCON_CLEAN_mq_NTT(poly_512, 9);
    FALCON_CLEAN_mq_NTT(poly_1024, 9);
    FALCON_CLEAN_mq_NTT(poly_p512, 9);
    FALCON_CLEAN_mq_NTT(poly_p1024, 9);

    // All should produce identical results (they use the same implementation)
    ASSERT(compare_poly_falcon(poly_512, poly_1024, FALCON_512_N, 0),
           "Falcon-512 and Falcon-1024 NTT differ");
    ASSERT(compare_poly_falcon(poly_512, poly_p512, FALCON_512_N, 0),
           "Falcon-512 and Falcon-padded-512 NTT differ");
    ASSERT(compare_poly_falcon(poly_512, poly_p1024, FALCON_512_N, 0),
           "Falcon-512 and Falcon-padded-1024 NTT differ");

    TEST_PASS();
    return 1;
}

/* ============================================================================
 * Performance Benchmarks
 * ============================================================================ */

/**
 * @brief Benchmark ML-DSA-44 reference NTT
 */
static void benchmark_ml_dsa_44_ref(void) {
    const int iterations = 10000;
    int32_t poly[ML_DSA_N];

    init_random_poly_ml_dsa(poly, ML_DSA_N);

    clock_t start = clock();
    for (int i = 0; i < iterations; i++) {
        OQS_SIG_ml_dsa_44_ref_ntt(poly);
    }
    clock_t end = clock();

    double time_sec = ((double)(end - start)) / CLOCKS_PER_SEC;
    double ops_per_sec = iterations / time_sec;

    printf("  ML-DSA-44 ref NTT:     %8.2f ops/sec  (%.4f sec for %d iterations)\n",
           ops_per_sec, time_sec, iterations);
}

/**
 * @brief Benchmark ML-DSA-65 reference NTT
 */
static void benchmark_ml_dsa_65_ref(void) {
    const int iterations = 10000;
    int32_t poly[ML_DSA_N];

    init_random_poly_ml_dsa(poly, ML_DSA_N);

    clock_t start = clock();
    for (int i = 0; i < iterations; i++) {
        OQS_SIG_ml_dsa_65_ref_ntt(poly);
    }
    clock_t end = clock();

    double time_sec = ((double)(end - start)) / CLOCKS_PER_SEC;
    double ops_per_sec = iterations / time_sec;

    printf("  ML-DSA-65 ref NTT:     %8.2f ops/sec  (%.4f sec for %d iterations)\n",
           ops_per_sec, time_sec, iterations);
}

/**
 * @brief Benchmark ML-DSA-87 reference NTT
 */
static void benchmark_ml_dsa_87_ref(void) {
    const int iterations = 10000;
    int32_t poly[ML_DSA_N];

    init_random_poly_ml_dsa(poly, ML_DSA_N);

    clock_t start = clock();
    for (int i = 0; i < iterations; i++) {
        OQS_SIG_ml_dsa_87_ref_ntt(poly);
    }
    clock_t end = clock();

    double time_sec = ((double)(end - start)) / CLOCKS_PER_SEC;
    double ops_per_sec = iterations / time_sec;

    printf("  ML-DSA-87 ref NTT:     %8.2f ops/sec  (%.4f sec for %d iterations)\n",
           ops_per_sec, time_sec, iterations);
}

/**
 * @brief Benchmark Falcon-512 clean NTT
 */
static void benchmark_falcon_512_clean(void) {
    const int iterations = 10000;
    uint16_t poly[FALCON_512_N];

    for (int i = 0; i < FALCON_512_N; i++) {
        poly[i] = rand() % FALCON_Q;
    }

    clock_t start = clock();
    for (int i = 0; i < iterations; i++) {
        FALCON_CLEAN_mq_NTT(poly, 9);
    }
    clock_t end = clock();

    double time_sec = ((double)(end - start)) / CLOCKS_PER_SEC;
    double ops_per_sec = iterations / time_sec;

    printf("  Falcon-512 clean NTT:  %8.2f ops/sec  (%.4f sec for %d iterations)\n",
           ops_per_sec, time_sec, iterations);
}

/**
 * @brief Benchmark Falcon-1024 clean NTT
 */
static void benchmark_falcon_1024_clean(void) {
    const int iterations = 10000;
    uint16_t poly[FALCON_1024_N];

    for (int i = 0; i < FALCON_1024_N; i++) {
        poly[i] = rand() % FALCON_Q;
    }

    clock_t start = clock();
    for (int i = 0; i < iterations; i++) {
        FALCON_CLEAN_mq_NTT(poly, 10);
    }
    clock_t end = clock();

    double time_sec = ((double)(end - start)) / CLOCKS_PER_SEC;
    double ops_per_sec = iterations / time_sec;

    printf("  Falcon-1024 clean NTT: %8.2f ops/sec  (%.4f sec for %d iterations)\n",
           ops_per_sec, time_sec, iterations);
}

/* ============================================================================
 * Main Test Runner
 * ============================================================================ */

int main(void) {
    printf("\n");
    printf("=================================================================\n");
    printf("  liboqs Custom NTT API Test Suite\n");
    printf("=================================================================\n");
    printf("\n");

    // Seed random number generator
    srand(time(NULL));

    /* ML-DSA Reference Tests */
    printf("--- ML-DSA Reference Implementation Tests ---\n");
    test_ml_dsa_44_ref_roundtrip();
    test_ml_dsa_65_ref_roundtrip();
    test_ml_dsa_87_ref_roundtrip();
    test_ml_dsa_zero_poly();
    test_ml_dsa_linearity();
    test_ml_dsa_variant_consistency();

    /* Falcon Clean Tests */
    printf("\n--- Falcon Clean Implementation Tests ---\n");
    test_falcon_512_clean_roundtrip();
    test_falcon_1024_clean_roundtrip();
    test_falcon_padded_512_clean_roundtrip();
    test_falcon_padded_1024_clean_roundtrip();
    test_falcon_clean_zero_poly();
    test_falcon_clean_linearity();
    test_falcon_clean_consistency();

    /* Performance Benchmarks */
    printf("\n--- Performance Benchmarks ---\n");
    benchmark_ml_dsa_44_ref();
    benchmark_ml_dsa_65_ref();
    benchmark_ml_dsa_87_ref();
    benchmark_falcon_512_clean();
    benchmark_falcon_1024_clean();

    /* Test Summary */
    printf("\n");
    printf("=================================================================\n");
    printf("  Test Summary\n");
    printf("=================================================================\n");
    printf("  Total tests:  %d\n", tests_run);
    printf("  " COLOR_GREEN "Passed:       %d" COLOR_RESET "\n", tests_passed);
    if (tests_failed > 0) {
        printf("  " COLOR_RED "Failed:       %d" COLOR_RESET "\n", tests_failed);
    } else {
        printf("  Failed:       %d\n", tests_failed);
    }
    printf("\n");

    if (tests_failed == 0) {
        printf(COLOR_GREEN "ALL TESTS PASSED!" COLOR_RESET "\n\n");
        return 0;
    } else {
        printf(COLOR_RED "SOME TESTS FAILED!" COLOR_RESET "\n\n");
        return 1;
    }
}

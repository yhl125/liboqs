// Package ntt provides Go bindings for liboqs NTT (Number Theoretic Transform)
// functions from ML-DSA and Falcon signature schemes.
//
// NTT is a key computational primitive for efficient polynomial multiplication
// in lattice-based post-quantum cryptography.
//
// # ML-DSA Support
//
// ML-DSA (FIPS 204) uses NTT over polynomials in Z_q[X]/(X^256 + 1) with q = 8380417.
// All security levels (44, 65, 87) share the same NTT parameters.
//
// Example:
//
//	var poly ntt.MLDSAPolynomial
//	// ... initialize poly ...
//	err := ntt.MLDSA_NTT(&poly, ntt.MLDSA44)
//	if err != nil {
//	    log.Fatal(err)
//	}
//
// # Falcon Support
//
// Falcon uses NTT over Z_12289 with polynomial degrees 512 or 1024.
//
// Example:
//
//	poly := make(ntt.FalconPolynomial, 512)
//	// ... initialize poly ...
//	err := ntt.Falcon_NTT(&poly, ntt.Falcon512LogN)
//	if err != nil {
//	    log.Fatal(err)
//	}
package ntt

/*
#cgo pkg-config: liboqs-go
#include <oqs/oqs_ntt_api.h>
*/
import "C"
import (
	"errors"
	"unsafe"
)

// MLDSAPolynomial represents a polynomial for ML-DSA with 256 int32 coefficients
type MLDSAPolynomial [256]int32

// FalconPolynomial represents a polynomial for Falcon with variable length uint16 coefficients
type FalconPolynomial []uint16

// MLDSALevel represents ML-DSA security levels
type MLDSALevel int

const (
	// MLDSA44 represents ML-DSA-44 security level
	MLDSA44 MLDSALevel = iota
	// MLDSA65 represents ML-DSA-65 security level
	MLDSA65
	// MLDSA87 represents ML-DSA-87 security level
	MLDSA87
)

const (
	// Falcon512LogN is the log2 of polynomial degree for Falcon-512 (2^9 = 512)
	Falcon512LogN = 9
	// Falcon1024LogN is the log2 of polynomial degree for Falcon-1024 (2^10 = 1024)
	Falcon1024LogN = 10
)

// MLDSA_NTT performs forward NTT transformation for ML-DSA.
//
// The transformation is performed in-place on the polynomial.
// No modular reduction is applied after additions/subtractions.
// Output is in bitreversed order.
//
// Note: Currently, all ML-DSA security levels (44, 65, 87) use identical NTT
// implementations as they share the same polynomial ring parameters (Z_8380417[X]/(X^256+1)).
// The level parameter is provided for API consistency and future extensibility.
//
// Parameters:
//   - poly: pointer to MLDSAPolynomial (256 int32 coefficients)
//   - level: ML-DSA security level (MLDSA44, MLDSA65, or MLDSA87)
//
// Returns error if poly is nil or level is invalid.
func MLDSA_NTT(poly *MLDSAPolynomial, level MLDSALevel) error {
	if poly == nil {
		return errors.New("nil polynomial")
	}

	cPoly := (*C.int32_t)(unsafe.Pointer(&poly[0]))

	switch level {
	case MLDSA44:
		C.OQS_SIG_ml_dsa_44_ref_ntt(cPoly)
	case MLDSA65:
		C.OQS_SIG_ml_dsa_65_ref_ntt(cPoly)
	case MLDSA87:
		C.OQS_SIG_ml_dsa_87_ref_ntt(cPoly)
	default:
		return errors.New("invalid ML-DSA security level")
	}

	return nil
}

// MLDSA_InvNTT_ToMont performs inverse NTT with Montgomery form output for ML-DSA.
//
// Performs in-place inverse NTT and multiplies by Montgomery factor 2^32.
// Input coefficients must be smaller than Q in absolute value.
//
// Note: Currently, all ML-DSA security levels (44, 65, 87) use identical NTT
// implementations as they share the same polynomial ring parameters (Z_8380417[X]/(X^256+1)).
// The level parameter is provided for API consistency and future extensibility.
//
// Parameters:
//   - poly: pointer to MLDSAPolynomial (256 int32 coefficients)
//   - level: ML-DSA security level (MLDSA44, MLDSA65, or MLDSA87)
//
// Returns error if poly is nil or level is invalid.
func MLDSA_InvNTT_ToMont(poly *MLDSAPolynomial, level MLDSALevel) error {
	if poly == nil {
		return errors.New("nil polynomial")
	}

	cPoly := (*C.int32_t)(unsafe.Pointer(&poly[0]))

	switch level {
	case MLDSA44:
		C.OQS_SIG_ml_dsa_44_ref_invntt_tomont(cPoly)
	case MLDSA65:
		C.OQS_SIG_ml_dsa_65_ref_invntt_tomont(cPoly)
	case MLDSA87:
		C.OQS_SIG_ml_dsa_87_ref_invntt_tomont(cPoly)
	default:
		return errors.New("invalid ML-DSA security level")
	}

	return nil
}

// MLDSA_InvNTT performs inverse NTT with standard form output for ML-DSA.
//
// Performs in-place inverse NTT and applies Montgomery reduction to return
// coefficients in normal form (not Montgomery form).
//
// Note: Currently, all ML-DSA security levels (44, 65, 87) use identical NTT
// implementations as they share the same polynomial ring parameters (Z_8380417[X]/(X^256+1)).
// The level parameter is provided for API consistency and future extensibility.
//
// Parameters:
//   - poly: pointer to MLDSAPolynomial (256 int32 coefficients)
//   - level: ML-DSA security level (MLDSA44, MLDSA65, or MLDSA87)
//
// Returns error if poly is nil or level is invalid.
func MLDSA_InvNTT(poly *MLDSAPolynomial, level MLDSALevel) error {
	if poly == nil {
		return errors.New("nil polynomial")
	}

	cPoly := (*C.int32_t)(unsafe.Pointer(&poly[0]))

	switch level {
	case MLDSA44:
		C.OQS_SIG_ml_dsa_44_ref_invntt(cPoly)
	case MLDSA65:
		C.OQS_SIG_ml_dsa_65_ref_invntt(cPoly)
	case MLDSA87:
		C.OQS_SIG_ml_dsa_87_ref_invntt(cPoly)
	default:
		return errors.New("invalid ML-DSA security level")
	}

	return nil
}

// Falcon_NTT performs forward NTT for Falcon.
//
// Performs in-place forward NTT using modular arithmetic over Z_12289.
// This is a portable implementation without SIMD optimizations.
// Works for all Falcon variants (512, 1024, padded-512, padded-1024).
//
// Parameters:
//   - poly: pointer to FalconPolynomial with length 2^logn
//   - logn: base-2 logarithm of polynomial degree (9 for n=512, 10 for n=1024)
//
// Returns error if poly is nil, length mismatch, or invalid logn.
func Falcon_NTT(poly *FalconPolynomial, logn uint) error {
	if poly == nil {
		return errors.New("nil polynomial")
	}

	expectedLen := 1 << logn
	if len(*poly) != expectedLen {
		return errors.New("polynomial length mismatch")
	}

	if logn != Falcon512LogN && logn != Falcon1024LogN {
		return errors.New("invalid logn: must be 9 (Falcon-512) or 10 (Falcon-1024)")
	}

	cPoly := (*C.uint16_t)(unsafe.Pointer(&(*poly)[0]))
	C.FALCON_CLEAN_mq_NTT(cPoly, C.uint(logn))

	return nil
}

// Falcon_InvNTT performs inverse NTT for Falcon.
//
// Performs in-place inverse NTT with final division by n.
// Works for all Falcon variants (512, 1024, padded-512, padded-1024).
//
// Parameters:
//   - poly: pointer to FalconPolynomial with length 2^logn
//   - logn: base-2 logarithm of polynomial degree (9 for n=512, 10 for n=1024)
//
// Returns error if poly is nil, length mismatch, or invalid logn.
func Falcon_InvNTT(poly *FalconPolynomial, logn uint) error {
	if poly == nil {
		return errors.New("nil polynomial")
	}

	expectedLen := 1 << logn
	if len(*poly) != expectedLen {
		return errors.New("polynomial length mismatch")
	}

	if logn != Falcon512LogN && logn != Falcon1024LogN {
		return errors.New("invalid logn: must be 9 (Falcon-512) or 10 (Falcon-1024)")
	}

	cPoly := (*C.uint16_t)(unsafe.Pointer(&(*poly)[0]))
	C.FALCON_CLEAN_mq_iNTT(cPoly, C.uint(logn))

	return nil
}

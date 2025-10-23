package ntt_test

import (
	"math/rand"
	"testing"

	"github.com/yhl125/liboqs/bindings/go/ntt"
)

// TestMLDSA_NTT_Roundtrip verifies NTT/InvNTT roundtrip correctness for all ML-DSA security levels
func TestMLDSA_NTT_Roundtrip(t *testing.T) {
	levels := []ntt.MLDSALevel{
		ntt.MLDSA44,
		ntt.MLDSA65,
		ntt.MLDSA87,
	}

	for _, level := range levels {
		t.Run(levelName(level), func(t *testing.T) {
			// Generate random polynomial
			original := generateRandomMLDSAPoly()
			working := original

			// Forward NTT
			err := ntt.MLDSA_NTT(&working, level)
			if err != nil {
				t.Fatalf("NTT failed: %v", err)
			}

			// Inverse NTT
			err = ntt.MLDSA_InvNTT(&working, level)
			if err != nil {
				t.Fatalf("InvNTT failed: %v", err)
			}

			// Verify roundtrip
			if !polyEqual(original, working) {
				t.Errorf("Roundtrip failed: polynomials differ")
				// Show first few mismatches for debugging
				count := 0
				for i := range original {
					if original[i] != working[i] && count < 5 {
						t.Logf("Mismatch at index %d: original=%d, result=%d", i, original[i], working[i])
						count++
					}
				}
			}
		})
	}
}

// TestMLDSA_Montgomery verifies Montgomery form operations
func TestMLDSA_Montgomery(t *testing.T) {
	levels := []ntt.MLDSALevel{ntt.MLDSA44, ntt.MLDSA65, ntt.MLDSA87}

	for _, level := range levels {
		t.Run(levelName(level), func(t *testing.T) {
			poly := generateRandomMLDSAPoly()

			// NTT → InvNTT_ToMont should differ from NTT → InvNTT
			poly1 := poly
			poly2 := poly

			ntt.MLDSA_NTT(&poly1, level)
			ntt.MLDSA_InvNTT_ToMont(&poly1, level)

			ntt.MLDSA_NTT(&poly2, level)
			ntt.MLDSA_InvNTT(&poly2, level)

			if polyEqual(poly1, poly2) {
				t.Errorf("Montgomery and standard forms should differ")
			}
		})
	}
}

// TestMLDSA_ErrorHandling validates error conditions
func TestMLDSA_ErrorHandling(t *testing.T) {
	t.Run("NilPointer", func(t *testing.T) {
		err := ntt.MLDSA_NTT(nil, ntt.MLDSA44)
		if err == nil {
			t.Error("Expected error for nil polynomial")
		}
	})

	t.Run("InvalidSecurityLevel", func(t *testing.T) {
		poly := ntt.MLDSAPolynomial{}
		err := ntt.MLDSA_NTT(&poly, 999)
		if err == nil {
			t.Error("Expected error for invalid security level")
		}
	})

	t.Run("InvNTT_ToMont_NilPointer", func(t *testing.T) {
		err := ntt.MLDSA_InvNTT_ToMont(nil, ntt.MLDSA44)
		if err == nil {
			t.Error("Expected error for nil polynomial")
		}
	})

	t.Run("InvNTT_NilPointer", func(t *testing.T) {
		err := ntt.MLDSA_InvNTT(nil, ntt.MLDSA44)
		if err == nil {
			t.Error("Expected error for nil polynomial")
		}
	})
}

// TestFalcon_NTT_Roundtrip verifies Falcon NTT correctness
func TestFalcon_NTT_Roundtrip(t *testing.T) {
	testCases := []struct {
		name string
		logn uint
	}{
		{"Falcon-512", ntt.Falcon512LogN},
		{"Falcon-1024", ntt.Falcon1024LogN},
	}

	for _, tc := range testCases {
		t.Run(tc.name, func(t *testing.T) {
			n := 1 << tc.logn
			original := generateRandomFalconPoly(n)
			working := make(ntt.FalconPolynomial, n)
			copy(working, original)

			// Forward NTT
			err := ntt.Falcon_NTT(&working, tc.logn)
			if err != nil {
				t.Fatalf("NTT failed: %v", err)
			}

			// Inverse NTT
			err = ntt.Falcon_InvNTT(&working, tc.logn)
			if err != nil {
				t.Fatalf("InvNTT failed: %v", err)
			}

			// Verify roundtrip
			if !falconPolyEqual(original, working) {
				t.Errorf("Roundtrip failed")
				// Show first few mismatches for debugging
				count := 0
				for i := range original {
					if original[i] != working[i] && count < 5 {
						t.Logf("Mismatch at index %d: original=%d, result=%d", i, original[i], working[i])
						count++
					}
				}
			}
		})
	}
}

// TestFalcon_Validation tests input validation
func TestFalcon_Validation(t *testing.T) {
	t.Run("WrongLength", func(t *testing.T) {
		// Create poly with wrong length for Falcon-512
		poly := make(ntt.FalconPolynomial, 256)
		err := ntt.Falcon_NTT(&poly, ntt.Falcon512LogN)
		if err == nil {
			t.Error("Expected error for length mismatch")
		}
	})

	t.Run("InvalidLogN", func(t *testing.T) {
		poly := make(ntt.FalconPolynomial, 512)
		err := ntt.Falcon_NTT(&poly, 8)
		if err == nil {
			t.Error("Expected error for invalid logn")
		}
	})

	t.Run("NilPointer", func(t *testing.T) {
		err := ntt.Falcon_NTT(nil, ntt.Falcon512LogN)
		if err == nil {
			t.Error("Expected error for nil polynomial")
		}
	})

	t.Run("InvNTT_WrongLength", func(t *testing.T) {
		poly := make(ntt.FalconPolynomial, 256)
		err := ntt.Falcon_InvNTT(&poly, ntt.Falcon512LogN)
		if err == nil {
			t.Error("Expected error for length mismatch")
		}
	})

	t.Run("InvNTT_NilPointer", func(t *testing.T) {
		err := ntt.Falcon_InvNTT(nil, ntt.Falcon512LogN)
		if err == nil {
			t.Error("Expected error for nil polynomial")
		}
	})
}

// Helper functions

// generateRandomMLDSAPoly creates random polynomial for testing
func generateRandomMLDSAPoly() ntt.MLDSAPolynomial {
	var poly ntt.MLDSAPolynomial
	const Q = 8380417

	for i := range poly {
		poly[i] = rand.Int31n(Q)
	}
	return poly
}

// generateRandomFalconPoly creates random Falcon polynomial
func generateRandomFalconPoly(n int) ntt.FalconPolynomial {
	poly := make(ntt.FalconPolynomial, n)
	const Q = 12289

	for i := range poly {
		poly[i] = uint16(rand.Intn(Q))
	}
	return poly
}

// polyEqual compares ML-DSA polynomials
func polyEqual(a, b ntt.MLDSAPolynomial) bool {
	for i := range a {
		if a[i] != b[i] {
			return false
		}
	}
	return true
}

// falconPolyEqual compares Falcon polynomials
func falconPolyEqual(a, b ntt.FalconPolynomial) bool {
	if len(a) != len(b) {
		return false
	}
	for i := range a {
		if a[i] != b[i] {
			return false
		}
	}
	return true
}

// levelName returns string name for ML-DSA security level
func levelName(level ntt.MLDSALevel) string {
	switch level {
	case ntt.MLDSA44:
		return "ML-DSA-44"
	case ntt.MLDSA65:
		return "ML-DSA-65"
	case ntt.MLDSA87:
		return "ML-DSA-87"
	default:
		return "Unknown"
	}
}

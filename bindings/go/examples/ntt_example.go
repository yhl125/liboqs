package main

import (
	"fmt"
	"liboqs/bindings/go/ntt"
	"log"
)

func main() {
	fmt.Println("========================================")
	fmt.Println("liboqs NTT API Go Binding Example")
	fmt.Println("========================================")
	fmt.Println()

	// ML-DSA Example
	runMLDSAExample()

	fmt.Println()

	// Falcon Example
	runFalconExample()
}

func runMLDSAExample() {
	fmt.Println("--- ML-DSA NTT Example ---")
	fmt.Println()

	// Initialize polynomial with test values
	var mldsaPoly ntt.MLDSAPolynomial
	for i := range mldsaPoly {
		mldsaPoly[i] = int32(i)
	}

	fmt.Println("ML-DSA-44 Round-trip Test:")
	fmt.Printf("  Original poly[0]   = %d\n", mldsaPoly[0])
	fmt.Printf("  Original poly[100] = %d\n", mldsaPoly[100])
	fmt.Printf("  Original poly[255] = %d\n", mldsaPoly[255])

	// Forward NTT
	err := ntt.MLDSA_NTT(&mldsaPoly, ntt.MLDSA44)
	if err != nil {
		log.Fatalf("ML-DSA NTT failed: %v", err)
	}
	fmt.Printf("  After NTT poly[0]  = %d\n", mldsaPoly[0])
	fmt.Printf("  After NTT poly[100]= %d\n", mldsaPoly[100])
	fmt.Printf("  After NTT poly[255]= %d\n", mldsaPoly[255])

	// Inverse NTT
	err = ntt.MLDSA_InvNTT(&mldsaPoly, ntt.MLDSA44)
	if err != nil {
		log.Fatalf("ML-DSA InvNTT failed: %v", err)
	}
	fmt.Printf("  After InvNTT[0]    = %d\n", mldsaPoly[0])
	fmt.Printf("  After InvNTT[100]  = %d\n", mldsaPoly[100])
	fmt.Printf("  After InvNTT[255]  = %d\n", mldsaPoly[255])

	// Verify roundtrip
	roundtripOK := true
	for i := range mldsaPoly {
		if mldsaPoly[i] != int32(i) {
			roundtripOK = false
			break
		}
	}

	if roundtripOK {
		fmt.Println("  ✓ Roundtrip successful!")
	} else {
		fmt.Println("  ✗ Roundtrip failed!")
	}

	// Test Montgomery form
	fmt.Println()
	fmt.Println("ML-DSA Montgomery Form Test:")

	var poly1 ntt.MLDSAPolynomial
	var poly2 ntt.MLDSAPolynomial
	for i := range poly1 {
		poly1[i] = int32(i + 1)
		poly2[i] = int32(i + 1)
	}

	// Standard form
	ntt.MLDSA_NTT(&poly1, ntt.MLDSA44)
	ntt.MLDSA_InvNTT(&poly1, ntt.MLDSA44)

	// Montgomery form
	ntt.MLDSA_NTT(&poly2, ntt.MLDSA44)
	ntt.MLDSA_InvNTT_ToMont(&poly2, ntt.MLDSA44)

	fmt.Printf("  Standard form[0]   = %d\n", poly1[0])
	fmt.Printf("  Montgomery form[0] = %d\n", poly2[0])

	if poly1[0] == poly2[0] {
		fmt.Println("  ✗ Forms should differ!")
	} else {
		fmt.Println("  ✓ Forms differ as expected!")
	}
}

func runFalconExample() {
	fmt.Println("--- Falcon NTT Example ---")
	fmt.Println()

	// Falcon-512 example
	fmt.Println("Falcon-512 Round-trip Test:")
	falcon512Poly := make(ntt.FalconPolynomial, 512)
	for i := range falcon512Poly {
		falcon512Poly[i] = uint16(i % 12289)
	}

	fmt.Printf("  Original poly[0]   = %d\n", falcon512Poly[0])
	fmt.Printf("  Original poly[256] = %d\n", falcon512Poly[256])
	fmt.Printf("  Original poly[511] = %d\n", falcon512Poly[511])

	// Forward NTT
	err := ntt.Falcon_NTT(&falcon512Poly, ntt.Falcon512LogN)
	if err != nil {
		log.Fatalf("Falcon NTT failed: %v", err)
	}
	fmt.Printf("  After NTT poly[0]  = %d\n", falcon512Poly[0])
	fmt.Printf("  After NTT poly[256]= %d\n", falcon512Poly[256])
	fmt.Printf("  After NTT poly[511]= %d\n", falcon512Poly[511])

	// Inverse NTT
	err = ntt.Falcon_InvNTT(&falcon512Poly, ntt.Falcon512LogN)
	if err != nil {
		log.Fatalf("Falcon InvNTT failed: %v", err)
	}
	fmt.Printf("  After InvNTT[0]    = %d\n", falcon512Poly[0])
	fmt.Printf("  After InvNTT[256]  = %d\n", falcon512Poly[256])
	fmt.Printf("  After InvNTT[511]  = %d\n", falcon512Poly[511])

	// Verify roundtrip
	roundtripOK := true
	for i := range falcon512Poly {
		if falcon512Poly[i] != uint16(i%12289) {
			roundtripOK = false
			break
		}
	}

	if roundtripOK {
		fmt.Println("  ✓ Roundtrip successful!")
	} else {
		fmt.Println("  ✗ Roundtrip failed!")
	}

	// Falcon-1024 example
	fmt.Println()
	fmt.Println("Falcon-1024 Round-trip Test:")
	falcon1024Poly := make(ntt.FalconPolynomial, 1024)
	for i := range falcon1024Poly {
		falcon1024Poly[i] = uint16((i * 7) % 12289)
	}

	fmt.Printf("  Original poly[0]    = %d\n", falcon1024Poly[0])
	fmt.Printf("  Original poly[512]  = %d\n", falcon1024Poly[512])
	fmt.Printf("  Original poly[1023] = %d\n", falcon1024Poly[1023])

	err = ntt.Falcon_NTT(&falcon1024Poly, ntt.Falcon1024LogN)
	if err != nil {
		log.Fatalf("Falcon-1024 NTT failed: %v", err)
	}

	err = ntt.Falcon_InvNTT(&falcon1024Poly, ntt.Falcon1024LogN)
	if err != nil {
		log.Fatalf("Falcon-1024 InvNTT failed: %v", err)
	}

	fmt.Printf("  After InvNTT[0]     = %d\n", falcon1024Poly[0])
	fmt.Printf("  After InvNTT[512]   = %d\n", falcon1024Poly[512])
	fmt.Printf("  After InvNTT[1023]  = %d\n", falcon1024Poly[1023])

	roundtripOK = true
	for i := range falcon1024Poly {
		if falcon1024Poly[i] != uint16((i*7)%12289) {
			roundtripOK = false
			break
		}
	}

	if roundtripOK {
		fmt.Println("  ✓ Roundtrip successful!")
	} else {
		fmt.Println("  ✗ Roundtrip failed!")
	}
}

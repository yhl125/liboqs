# liboqs NTT API - Go Bindings

Go bindings for the liboqs NTT (Number Theoretic Transform) API, providing access to high-performance NTT implementations from ML-DSA and Falcon post-quantum signature schemes.

## Overview

This package exposes NTT functions used internally by liboqs signature schemes for efficient polynomial multiplication in lattice-based cryptography.

**Supported Algorithms:**
- **ML-DSA (FIPS 204)**: ML-DSA-44, ML-DSA-65, ML-DSA-87
- **Falcon**: Falcon-512, Falcon-1024

## Installation

### Prerequisites

1. **Build liboqs with NTT API support:**
```bash
cd /path/to/liboqs
mkdir build && cd build
cmake ..
ninja  # or make
```

2. **Set up pkg-config:**
```bash
export PKG_CONFIG_PATH=/path/to/liboqs/.config:$PKG_CONFIG_PATH
export DYLD_LIBRARY_PATH=/path/to/liboqs/build/lib:$DYLD_LIBRARY_PATH  # macOS
# or
export LD_LIBRARY_PATH=/path/to/liboqs/build/lib:$LD_LIBRARY_PATH      # Linux
```

3. **Clean Go cache:**
```bash
go clean -cache
```

## Usage

### ML-DSA Example

```go
package main

import (
    "fmt"
    "liboqs/oqs/ntt"
    "log"
)

func main() {
    // Create a polynomial
    var poly ntt.MLDSAPolynomial
    for i := range poly {
        poly[i] = int32(i)
    }

    // Forward NTT
    err := ntt.MLDSA_NTT(&poly, ntt.MLDSA44)
    if err != nil {
        log.Fatal(err)
    }

    // Inverse NTT
    err = ntt.MLDSA_InvNTT(&poly, ntt.MLDSA44)
    if err != nil {
        log.Fatal(err)
    }

    fmt.Println("Roundtrip successful!")
}
```

### Falcon Example

```go
package main

import (
    "fmt"
    "liboqs/oqs/ntt"
    "log"
)

func main() {
    // Create Falcon-512 polynomial
    poly := make(ntt.FalconPolynomial, 512)
    for i := range poly {
        poly[i] = uint16(i % 12289)
    }

    // Forward NTT
    err := ntt.Falcon_NTT(&poly, ntt.Falcon512LogN)
    if err != nil {
        log.Fatal(err)
    }

    // Inverse NTT
    err = ntt.Falcon_InvNTT(&poly, ntt.Falcon512LogN)
    if err != nil {
        log.Fatal(err)
    }

    fmt.Println("Roundtrip successful!")
}
```

## API Reference

### Types

#### `MLDSAPolynomial`
Fixed-size array of 256 int32 coefficients for ML-DSA polynomials.

#### `FalconPolynomial`
Variable-length slice of uint16 coefficients for Falcon polynomials (512 or 1024 elements).

#### `MLDSALevel`
Security level constants:
- `MLDSA44`: ML-DSA-44
- `MLDSA65`: ML-DSA-65
- `MLDSA87`: ML-DSA-87

### ML-DSA Functions

**Note**: Currently, all ML-DSA security levels (44, 65, 87) use identical NTT implementations as they share the same polynomial ring parameters (Z_8380417[X]/(X^256+1)). The level parameter is provided for API consistency and future extensibility.

#### `MLDSA_NTT(poly *MLDSAPolynomial, level MLDSALevel) error`
Performs in-place forward NTT transformation.

#### `MLDSA_InvNTT_ToMont(poly *MLDSAPolynomial, level MLDSALevel) error`
Performs in-place inverse NTT with Montgomery form output.

#### `MLDSA_InvNTT(poly *MLDSAPolynomial, level MLDSALevel) error`
Performs in-place inverse NTT with standard form output.

### Falcon Functions

#### `Falcon_NTT(poly *FalconPolynomial, logn uint) error`
Performs in-place forward NTT for Falcon.
- `logn`: 9 for Falcon-512, 10 for Falcon-1024

#### `Falcon_InvNTT(poly *FalconPolynomial, logn uint) error`
Performs in-place inverse NTT for Falcon.
- `logn`: 9 for Falcon-512, 10 for Falcon-1024

## Testing

Run the test suite:

```bash
cd oqstests
export PKG_CONFIG_PATH=/path/to/liboqs/.config:$PKG_CONFIG_PATH
export DYLD_LIBRARY_PATH=/path/to/liboqs/build/lib:$DYLD_LIBRARY_PATH
go test -v
```

Run specific tests:
```bash
go test -v -run TestMLDSA   # ML-DSA tests only
go test -v -run TestFalcon  # Falcon tests only
```

## Examples

Run the example program:

```bash
cd examples
export PKG_CONFIG_PATH=/path/to/liboqs/.config:$PKG_CONFIG_PATH
export DYLD_LIBRARY_PATH=/path/to/liboqs/build/lib:$DYLD_LIBRARY_PATH
go run ntt_example.go
```

## Technical Details

### ML-DSA NTT Parameters
- **Modulus**: q = 8380417
- **Polynomial Ring**: Z_q[X]/(X^256 + 1)
- **Degree**: N = 256
- **Coefficient Type**: int32

### Falcon NTT Parameters
- **Modulus**: q = 12289
- **Polynomial Degrees**: 512 (Falcon-512), 1024 (Falcon-1024)
- **Coefficient Type**: uint16
- **Range**: [0, q-1]

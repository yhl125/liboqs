# liboqs Go Bindings - NTT API

Go bindings for liboqs NTT (Number Theoretic Transform) functions from ML-DSA and Falcon post-quantum signature schemes.

## Quick Start

### Build Everything

```bash
cd bindings/go
make all
```

This will:
1. Build liboqs C library (if not already built)
2. Build Go bindings
3. Run tests

### Run Tests

```bash
make test
```

### Run Example

```bash
make example
```

## Manual Usage

### 1. Build liboqs

```bash
# From liboqs root
mkdir build && cd build
cmake ..
ninja
```

### 2. Set Environment Variables

```bash
# From bindings/go directory
export PKG_CONFIG_PATH=$(pwd)/.config:$PKG_CONFIG_PATH

# macOS
export DYLD_LIBRARY_PATH=../../build/lib:$DYLD_LIBRARY_PATH

# Linux
export LD_LIBRARY_PATH=../../build/lib:$LD_LIBRARY_PATH
```

### 3. Build Go Bindings

```bash
go clean -cache
cd ntt && go build
```

### 4. Run Tests

```bash
cd ntt_test
go test -v
```

### 5. Run Example

```bash
cd examples
go run ntt_example.go
```

## Project Structure

```
bindings/go/
├── .config/
│   └── liboqs-go.pc          # pkg-config configuration
├── ntt/
│   ├── ntt.go                 # NTT API bindings
│   └── README.md              # NTT API documentation
├── ntt_test/
│   └── ntt_test.go            # Test suite
├── examples/
│   └── ntt_example.go         # Usage example
├── go.mod                     # Go module definition
├── Makefile                   # Build system
└── README.md                  # This file
```

## Usage in Other Projects

### Method 1: GitHub

After pushing to GitHub with a tag:

```bash
go get github.com/yhl125/liboqs/bindings/go/ntt@v0.1.3
```

Then in your code:

```go
import "github.com/yhl125/liboqs/bindings/go/ntt"
```

### Method 2: Local Development

In your project's `go.mod`:

```go
module my-project

replace github.com/yhl125/liboqs/bindings/go => /path/to/liboqs/bindings/go

require github.com/yhl125/liboqs/bindings/go v0.0.0
```

**Important:** Users must have liboqs C library installed on their system!

## API Overview

### ML-DSA (FIPS 204)

```go
import "github.com/yhl125/liboqs/bindings/go/ntt"

var poly ntt.MLDSAPolynomial
ntt.MLDSA_NTT(&poly, ntt.MLDSA44)
ntt.MLDSA_InvNTT(&poly, ntt.MLDSA44)
```

### Falcon

```go
import "github.com/yhl125/liboqs/bindings/go/ntt"

poly := make(ntt.FalconPolynomial, 512)
ntt.Falcon_NTT(&poly, ntt.Falcon512LogN)
ntt.Falcon_InvNTT(&poly, ntt.Falcon512LogN)
```

## Requirements

- liboqs (built from source or installed)
- Go 1.23 or later
- pkg-config
- CMake & Ninja (for building liboqs)

## Makefile Targets

```bash
make all        # Build everything
make liboqs     # Build C library only
make go         # Build Go bindings only
make test       # Run tests
make example    # Run example
make clean      # Clean Go artifacts
make clean-all  # Clean everything including liboqs build
make rebuild    # Clean and rebuild
```

## Troubleshooting

### IDE shows red underlines on C functions

This is normal with CGO. The code will compile and run correctly. To reduce warnings:

1. Set environment variables in your shell
2. Run `go clean -cache`
3. Restart your IDE's language server

### "Package liboqs-go was not found"

Make sure `PKG_CONFIG_PATH` is set correctly:

```bash
export PKG_CONFIG_PATH=/path/to/liboqs/bindings/go/.config:$PKG_CONFIG_PATH
```

### "dyld: Library not loaded: liboqs"

Set the library path:

```bash
# macOS
export DYLD_LIBRARY_PATH=/path/to/liboqs/build/lib:$DYLD_LIBRARY_PATH

# Linux
export LD_LIBRARY_PATH=/path/to/liboqs/build/lib:$LD_LIBRARY_PATH
```

## License

MIT License - See liboqs LICENSE.txt

## Documentation

For detailed API documentation, see [ntt/README.md](ntt/README.md)

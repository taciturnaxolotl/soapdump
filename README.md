# SoapDump

A high-performance PayPal SOAP log parser written in C++.

![gif of the program in action](https://github.com/taciturnaxolotl/soapdump/blob/main/docs/vhs.gif)

## Usage

```bash
# Get all transactions
soapdump payments.log

# Get only successful transactions
soapdump payments.log | grep Success

# Count transactions by state
soapdump payments.log | cut -d'|' -f8 | sort | uniq -c | sort -nr

# Find largest transaction
soapdump payments.log | sort -t'|' -k2 -nr | head -1

# Get transactions over $500
soapdump payments.log | awk -F'|' '$2 > 500'

# Summary stats
soapdump -s payments.log
```

## Building

### Using Nix

```bash
# Build the project
nix build

# Run the parser
nix run . -- payments.log

# Development shell
nix develop
```

### Using CMake

```bash
# Build the project
cmake -B build -S .
cmake --build build --config Release

# Run the parser
./build/soapdump payments.log
```

### Installation

```bash
nix profile install github:taciturnaxolotl/soapdump
```

or in the flake:

```nix
{
  inputs.soapdump.url = "github:taciturnaxolotl/soapdump";

  outputs = { self, nixpkgs, soapdump, ... }: {
    # Access the package as:
    # soapdump.packages.${system}.default
  };
}
```

Or run without installing:

```bash
nix run github:taciturnaxolotl/soapdump -- payments.log
```

## Output Format

Pipe-separated values with the following fields:

```
TRANS_NUM|AMOUNT|CURRENCY|FIRSTNAME|LASTNAME|STREET|CITY|STATE|ZIP|CCTYPE|CCLAST4|EXPMONTH|EXPYEAR|CVV|TRANSID|STATUS|CORRID|PROC_AMOUNT
```

<p align="center">
	<i><code>&copy 2025-present <a href="https://github.com/taciturnaxolotl">Kieran Klukas</a></code></i>
</p>

<p align="center">
	<a href="https://github.com/taciturnaxolotl/soapdump/blob/main/LICENSE.md"><img src="https://img.shields.io/static/v1.svg?style=for-the-badge&label=License&message=MIT&logoColor=d9e0ee&colorA=363a4f&colorB=b7bdf8"/></a>
</p>

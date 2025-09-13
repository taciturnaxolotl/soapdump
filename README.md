# SoapDump

A high-performance PayPal SOAP log parser written in C++.

![a media offline screen from davinci resolve](https://hc-cdn.hel1.your-objectstorage.com/s/v3/8744661d883d695adc6b14a17a52ac8970d7c2dd_image.png)

## Features

- Fast parsing of PayPal SOAP transaction logs
- Structured output for easy analysis
- Summary statistics mode
- Pipe-friendly output format
- Nix flake for reproducible builds

## Usage

```bash
# Get all transactions
./transaction-parser payments.log

# Get only successful transactions
./transaction-parser payments.log | grep Success

# Count transactions by state
./transaction-parser payments.log | cut -d'|' -f8 | sort | uniq -c | sort -nr

# Find largest transaction
./transaction-parser payments.log | sort -t'|' -k2 -nr | head -1

# Get transactions over $500
./transaction-parser payments.log | awk -F'|' '$2 > 500'

# Summary stats
./transaction-parser -s payments.log
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
./build/transaction-parser payments.log
```

## Output Format

Tab-separated values with the following fields:

```
TRANS_NUM|AMOUNT|CURRENCY|FIRSTNAME|LASTNAME|STREET|CITY|STATE|ZIP|CCTYPE|CCLAST4|EXPMONTH|EXPYEAR|CVV|TRANSID|STATUS|CORRID|PROC_AMOUNT
```

<p align="center">
	<i><code>&copy 2025-present <a href="https://github.com/taciturnaxolotl">Kieran Klukas</a></code></i>
</p>

<p align="center">
	<a href="https://github.com/taciturnaxolotl/soapdump/blob/main/LICENSE.md"><img src="https://img.shields.io/static/v1.svg?style=for-the-badge&label=License&message=MIT&logoColor=d9e0ee&colorA=363a4f&colorB=b7bdf8"/></a>
</p>
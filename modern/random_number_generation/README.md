# Evaluation of Operating-System Random Number Generators

## Objective
Generate 1 MiB of random data using Linux /dev/urandom, OpenSSL rand, and the Windows .NET cryptographic random-number API. Analyze their statistical properties and discuss cryptographic suitability.

## Directory Structure
- scripts/generate_linux.sh: Linux random data generation.
- scripts/generate_openssl.sh: OpenSSL random data generation.
- scripts/generate_windows.ps1: Windows cryptographic random data generation.
- scripts/verify_data.sh: Validates generated file sizes.
- notebooks/analysis.ipynb: Statistical analysis notebook.
- results/statistical_summary.csv: Measured statistical results.
- data/: Generated binary files, excluded from Git.

## Linux /dev/urandom
Linux provides /dev/urandom as an interface to the kernel cryptographically secure pseudorandom number generator (CSPRNG). It is suitable for generating cryptographic random bytes on modern Linux systems.

## OpenSSL rand
OpenSSL rand uses OpenSSL's cryptographic random-number infrastructure and a deterministic random bit generator (DRBG). Its security depends on correct initialization, entropy sources, configuration, and implementation.

## Windows CNG / .NET
The Windows script uses System.Security.Cryptography.RandomNumberGenerator. On Windows, this API uses the operating system's cryptographic random-number facilities and is intended for cryptographic applications.

## Data Generation
Each generator creates exactly 1,048,576 bytes (1 MiB).

Run the Linux generator:
bash scripts/generate_linux.sh

Run the OpenSSL generator:
bash scripts/generate_openssl.sh

Run the Windows generator in Windows PowerShell:
powershell.exe -ExecutionPolicy Bypass -File .\scripts\generate_windows.ps1

Validate all files:
bash scripts/verify_data.sh

## Statistical Analysis
The notebook analyzes byte-frequency distributions, chi-square goodness-of-fit, bit balance, repeated adjacent bytes, repeated aligned 16-byte blocks, longest identical-bit runs, lag-1 serial correlation, and an approximate runs test.

## Interpretation
A uniform byte distribution has an expected count of 4,096 for each of the 256 possible byte values in a 1 MiB sample. Actual counts fluctuate naturally.

A low p-value may indicate a discrepancy from a tested statistical model, but it does not automatically prove a security flaw. A high p-value does not prove cryptographic security either. Statistical tests cannot establish that future output is unpredictable.

OpenSSL may obtain entropy from the operating system, so these generators should not be assumed to be independent entropy sources.

## Conclusion
This experiment compares three cryptographic random-byte generation interfaces using equal-sized samples and statistical diagnostics. Results can help identify anomalies, but cryptographic security depends on the generator design, implementation, entropy sources, and operating environment—not statistical tests alone.

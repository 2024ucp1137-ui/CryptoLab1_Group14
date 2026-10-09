# AES-CBC Padding Oracle Attack

Performers ----
    GAJESH  --  2024UCP1189
    RANVEER SINGH -- 2024UCP1137

## Overview
This project demonstrates how a Padding Oracle Attack can recover plaintext from AES-CBC ciphertext without knowing the encryption key, using a simulated oracle that reveals whether PKCS#7 padding is valid.

## Features
- AES-CBC encryption and PKCS#7 padding.
- Boolean padding oracle.
- Plaintext recovery without accessing the AES key in the attack function.
- Oracle query counting.
- Unit tests for padding and plaintext recovery.

## Requirements
- Python 3
- PyCryptodome

Install the dependency:

```bash
python3 -m pip install pycryptodome
```

## Run the Program

```bash
python3 padding_oracle.py
```

To try a different message:

```bash
python3 padding_oracle.py 
```

## Run Tests

```bash
python3 test_oracle.py
```

The tests check padding validation, plaintext recovery, oracle query counting, and malformed ciphertext handling.

## How It Works
AES-CBC uses XOR with the previous ciphertext block during decryption. By modifying that block and observing the oracle's padding-validity responses, an attacker can recover plaintext bytes from right to left without knowing the AES key.

## Security Recommendations
- Prefer authenticated encryption such as AES-GCM.
- Authenticate ciphertext before decryption if CBC must be used.
- Avoid distinguishable padding errors and timing leaks.

## Conclusion
This lab demonstrates why exposing padding validity can compromise AES-CBC confidentiality and why authentication is essential for secure encryption.


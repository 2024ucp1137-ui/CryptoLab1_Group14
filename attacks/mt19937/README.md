# Assignment 10: MT19937 Stream Cipher and Predictable Token Attack

## Team Members

| Name | Student ID |
|---|---|
| Gajesh Bishnoi | 2024UCP1189 |
| Ranveer Singh  | 2024UCP1137 |

## Objective

Study the weaknesses of using a statistical pseudorandom number generator for cryptographic purposes. This experiment uses the MT19937 implementation provided by the C++ standard library to build a simple stream cipher, recover a weak 16-bit seed using known plaintext, and demonstrate how timestamp-based reset tokens can be predicted.

## Directory Structure

```text
attacks/
└── mt19937_predictable_token/
    └── src/
        └── main.cpp
```

The program generates its own plaintext, ciphertext, and simulated reset token at runtime. Separate test-case files are therefore not required.

## MT19937 Stream Cipher

The first part uses the built-in `std::mt19937` generator from the C++ `<random>` library.

For each input byte, the program generates one 32-bit MT19937 output and extracts its lowest 8 bits using:

```cpp
generator() & 0xFF
```

The extracted byte is XORed with the corresponding input byte. The same function performs encryption and decryption because XOR is reversible.

### Known-Plaintext Seed Recovery

The program creates a plaintext consisting of a random printable prefix of 5–20 characters followed by 14 consecutive `A` characters. It encrypts this message using a randomly selected 16-bit seed.

The recovery procedure tests every possible seed from 0 through 65,535. For each candidate, it decrypts the ciphertext and searches for the known 14-character sequence without assuming its position.

After finding a candidate, the program decrypts the complete ciphertext again and checks that the recovered plaintext matches the original message.

## Predictable Password Reset Tokens

The second part simulates an application that generates a 16-byte reset token using the current Unix timestamp as the MT19937 seed.

The program records the timestamp and displays the generated token in hexadecimal.

To simulate prediction, it searches candidate timestamps from 300 seconds before the recorded time to 300 seconds after it. For each candidate timestamp, it generates a 16-byte token and compares it with the observed token.

When a match is found, the program regenerates the token using the recovered timestamp and verifies that it is identical to the original token.

## Implementation Details

- Language: C++
- PRNG: Built-in `std::mt19937`
- Byte extraction: Lowest 8 bits of each 32-bit output (`generator() & 0xFF`)
- Stream-cipher seed range: 0–65,535
- Known plaintext: 14 consecutive `A` characters
- Reset-token size: 16 bytes
- Timestamp search interval: ±300 seconds
- Total timestamp candidates: 601

The same MT19937 library and byte-extraction procedure are used throughout the C++ implementation. The generator is reinitialized with the relevant seed for each encryption, decryption, or token-generation operation.

## Execution

Compile the program using a C++17-compatible compiler:

```bash
g++ -std=c++17 src/main.cpp -o mt19937_assignment
```

Run on Linux:

```bash
./mt19937_assignment
```

Run on Windows PowerShell:

```powershell
.\mt19937_assignment.exe
```

## Verification

The program displays the generated plaintext, ciphertext, original seed, and decryption result.

For the stream-cipher experiment, successful execution should show:

- Encryption and decryption produce the original plaintext.
- The seed search examines all 65,536 possible values.
- The recovered seed matches the seed used for encryption.
- The complete recovered plaintext matches the original plaintext and contains the known 14-character sequence.

For the reset-token experiment, successful execution should show:

- The original Unix timestamp and 16-byte token in hexadecimal.
- The search interval covering 601 timestamps.
- The recovered timestamp matching the generation timestamp.
- The regenerated token matching the observed token.

The actual seed, plaintext, ciphertext, timestamp, and token vary between executions.

## Security Observations

MT19937 is deterministic: the same seed and sequence of generator calls produce the same outputs. Its statistical quality does not make it suitable for cryptographic applications.

A small seed space, such as 16 bits, can be exhaustively searched. Known plaintext provides a way to recognize a correct candidate decryption.

Timestamp-based seeding is also weak when an attacker can estimate when a token was generated. A narrow time interval can reduce the candidate seed space substantially.

Statistical randomness and cryptographic unpredictability are different properties. Security-sensitive tokens should be generated using a cryptographically secure random-number generator provided by the operating system or a trusted cryptographic library.

## Conclusion

This experiment demonstrates two ways that predictable pseudorandom generation can undermine a cryptographic application: brute-force recovery of a small seed using known plaintext, and prediction of tokens generated from timestamps.

The results show why MT19937, despite being useful for simulations and statistical applications, should not be used to generate encryption keystreams, password reset tokens, session identifiers, or other security-sensitive values.

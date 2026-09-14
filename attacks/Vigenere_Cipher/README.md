# Assignment 6 - Vigenere Cipher Cryptanalysis

## Objective

Cryptanalysis of the Vigenere Cipher using Kasiski Examination,
Index of Coincidence, and Frequency Analysis.

Group Number: 14

Since Group 14 is an even-numbered group, Ciphertext 2 was used.

## Methodology

1. Preprocess the ciphertext by removing spaces and special characters.
2. Find repeated patterns in the ciphertext.
3. Calculate distances between repeated patterns.
4. Find factors of the distances.
5. Use Kasiski Examination to estimate the key length.
6. Calculate the Index of Coincidence.
7. Divide the ciphertext into groups according to the estimated key length.
8. Perform frequency analysis on each group.
9. Determine the Caesar shift for each group.
10. Combine the shifts to recover the Vigenere key.
11. Decrypt the ciphertext using the recovered key.
12. Re-encrypt the recovered plaintext.
13. Compare the re-encrypted ciphertext with the original ciphertext.

## Functions Implemented

- clean_ciphertext()
- find_repeated_patterns()
- calculate_distances()
- find_factors()
- kasiski_analysis()
- calculate_ic()
- split_into_groups()
- frequency_analysis()
- find_shift()
- find_key()
- vigenere_decrypt()
- vigenere_encrypt()
- verify()

## Results

Kasiski suggested key length: 3

Index of Coincidence analysis suggested key length: 12

Recovered key:

UNITEDSTATES

The recovered plaintext was successfully decrypted.

## Verification

The recovered plaintext was re-encrypted using the recovered key.

Result:

Re-encryption matches original ciphertext: SUCCESS

## Conclusion

The Vigenere ciphertext was successfully cryptanalysed using
Kasiski Examination, Index of Coincidence, and frequency analysis.
The recovered key was UNITEDSTATES, and verification confirmed
that re-encryption of the recovered plaintext produces the
original ciphertext.
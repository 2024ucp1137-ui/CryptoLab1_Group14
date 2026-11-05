# Cryptographic Prime Generation Using Miller-Rabin

## Objective
Generate a probable prime integer of exactly 512 bits.

## Implementation
The program uses C++17 and Boost.Multiprecision for large-integer arithmetic.

## Algorithm
1. Generate a random odd 512-bit candidate.
2. Decompose n-1 as 2^s * d, where d is odd.
3. Apply the Miller-Rabin primality test using 20 bases.
4. Reject candidates that fail and repeat until a candidate passes.
5. Record the prime, candidate count, and execution time.

## Independent Verification
The Python verification script checks the bit length, oddness, all 20 Miller-Rabin rounds, and primality using SymPy's isprime() function.

## Results
Experimental measurements are saved in outputs/experiment_results.txt.
The generated prime is saved in outputs/prime_candidate.txt.
Verification results are saved in outputs/verification_results.txt.

## Conclusion
The experiment demonstrates 512-bit probable-prime generation and independent primality verification. Miller-Rabin is a probabilistic test, so passing finite rounds alone is not a mathematical proof of primality.

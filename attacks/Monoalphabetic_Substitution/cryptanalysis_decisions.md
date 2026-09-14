# Cryptanalysis Decision Table

| Step | Observation | Possible Substitution | Substitution Tested | Result | Decision |
|------|-------------|-----------------------|----------------------|--------|----------|
| 1 | Most frequent ciphertext symbol was T | T -> E | T -> E | Partial plaintext became more meaningful | Accepted |
| 2 | Three-letter and repeated word patterns suggested T as plaintext T | Z -> T | Z -> T | More recognizable word fragments appeared | Accepted |
| 3 | Pattern analysis suggested H | I -> H | I -> H | Partial plaintext improved | Accepted |
| 4 | Frequency and word-pattern analysis used to continue mapping | Remaining letters | Candidate mappings tested | Plaintext became readable | Accepted |
| 5 | Complete substitution mapping obtained | All remaining letters | Full mapping | Meaningful plaintext recovered | Accepted |
| 6 | Re-encryption performed | Recovered key | Plaintext encrypted again | Generated ciphertext matched original | Verified |
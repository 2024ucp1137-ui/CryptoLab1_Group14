# CryptoLabX

## Cryptography Laboratory Assignments 2 & 3

### Project Overview

CryptoLabX is a modular cryptography toolkit developed as part of the Cryptography Laboratory course. The project is being developed incrementally through multiple assignments.

Assignment 2 focuses on building the foundation for classical cryptography operations and extending the toolkit structure.

Assignment 3 focuses on application security, vulnerability identification, and Static Application Security Testing (SAST) using Semgrep.

---

## Team Members

| Name | Roll Number |
|------|-------------|
| Ranveer Singh | 2024UCP1137 |
| Gajesh Bishnoi | 2024UCP1189 |

---

# Assignment 2

## Overview

Assignment 2 extends the CryptoLabX toolkit with basic cryptographic functionality and related analysis.

The assignment focuses on implementing and testing classical cryptography concepts within the existing project structure.

## Features Implemented

- Classical cryptography functionality added to CryptoLabX.
- Encryption and decryption operations implemented as required.
- Cryptographic operations integrated into the existing toolkit structure.
- Test cases and sample inputs used to verify the implementations.

---

# Assignment 3

## Overview

Assignment 3 focuses on secure application development and Static Application Security Testing (SAST).

A Python-based Library Management System was developed using SQLite. The application was intentionally analyzed for security vulnerabilities using Semgrep.

## Library Management System

The application provides the following functionalities:

- User registration
- User login
- Book search
- Book listing
- Book issue
- Book return
- Fine calculation
- Book file viewing

## Security Vulnerabilities

The application demonstrates the following vulnerabilities:

### 1. SQL Injection

The book search functionality initially uses direct string concatenation to construct an SQL query. This allows user input to become part of the SQL statement.

### 2. Missing Authentication

The fine calculation functionality can be accessed without requiring the user to log in.

### 3. Directory Traversal

The book file viewing functionality accepts a user-controlled file path without sufficient path validation.

## SAST Analysis

Semgrep is used as the Static Application Security Testing tool.

The source code is scanned using Semgrep to identify security vulnerabilities and generate a structured JSON report.

Example command:

```bash
semgrep scan --config auto secure_application/src/library.py
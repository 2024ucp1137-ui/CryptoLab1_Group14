#!/usr/bin/env bash
set -euo pipefail

DATA_DIR="$(dirname "$0")/../data"
EXPECTED_SIZE=1048576
FAILED=0

for FILE in urandom.bin openssl.bin cng.bin; do
    PATH_TO_FILE="$DATA_DIR/$FILE"

    if [[ ! -f "$PATH_TO_FILE" ]]; then
        echo "MISSING: $FILE"
        FAILED=1
        continue
    fi

    ACTUAL_SIZE=$(wc -c < "$PATH_TO_FILE")

    if [[ "$ACTUAL_SIZE" -eq "$EXPECTED_SIZE" ]]; then
        echo "PASS: $FILE ($ACTUAL_SIZE bytes)"
    else
        echo "FAIL: $FILE ($ACTUAL_SIZE bytes; expected $EXPECTED_SIZE)"
        FAILED=1
    fi
done

if [[ "$FAILED" -ne 0 ]]; then
    exit 1
fi

echo "All random data files passed validation."

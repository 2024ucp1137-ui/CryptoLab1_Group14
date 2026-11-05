#!/bin/bash

head -c 1048576 /dev/urandom > urandom.bin
openssl rand -out openssl.bin 1048576

ls -l *.bin

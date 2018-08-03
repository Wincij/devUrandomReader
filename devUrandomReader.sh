#!/bin/bash
# MAREK PAJACZKOWSKI
# SYSTEMY OPERACYJNE 2017/2018
# K6X4S1
echo BashStarts
gcc -o devUrandomReader devUrandomReader.c
./devUrandomReader
rm ./FIFO
echo [TERMINATED]

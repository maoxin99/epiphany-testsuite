#!/bin/bash

set -e

ESDK=${EPIPHANY_HOME}
ELIBS=${ESDK}/tools/host/lib
EINCS=${ESDK}/tools/host/include
ELDF=${ESDK}/bsps/current/fast.ldf
ELDF=${ESDK}/bsps/current/internal.ldf

# Build HOST side application
gcc src/main.c -o Debug/main.elf -I ${EINCS} -L ${ELIBS} -le-hal

# Build DEVICE side program
e-gcc -T ${ELDF} src/emain.c -o Debug/emain.elf -le-lib
# Build DEVICE origin program
e-gcc -T ${ELDF} src/emainorigin.c -o Debug/emainorigin.elf -le-lib




# Convert ebinary to SREC file
e-objcopy --srec-forceS3 --output-target srec Debug/emain.elf Debug/emain.srec
e-objcopy --srec-forceS3 --output-target srec Debug/emainorigin.elf Debug/emainorigin.srec



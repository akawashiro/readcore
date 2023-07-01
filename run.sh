#! /bin/bash -ux

sudo bash -c 'echo core.%t > /proc/sys/kernel/core_pattern'
echo 0 | sudo tee /proc/sys/kernel/randomize_va_space

ulimit -c unlimited
./build/make_core

echo 2 | sudo tee /proc/sys/kernel/randomize_va_space

#!/bin/bash
make -C /usr/src/linux-headers-`uname -r` SUBDIRS=/home/dk/code/userspace-krb modules

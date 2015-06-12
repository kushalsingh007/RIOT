/*
 * Copyright (C) 2015  Kushal Singh <kushal.spiderman.singh@gmail.com
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @ingroup     examples
 * @{
 *
 * @file
 * @brief       A basic example for linking in RIOT using custom header file
 *
 * @author      Kushal Singh <kushal.spiderman.singh@gmail.com>
 *
 * @}
 */

#include "test.h"

int main(void)
{
    void (*ptr)();
    ptr=&test_bin;
    ptr();
    return 0;
}

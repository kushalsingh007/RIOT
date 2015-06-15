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

#include <stdio.h>
#include "test.h"

typedef int (*f_ptr)(void);
f_ptr ptr = (f_ptr) &test_bin[0];

void my_test(void) __attribute__((used));
void my_test(void) {
    printf("test\n");
}

int main(void)
{
    printf("Jumping to 0x%08x...\n", (unsigned)*ptr);
    int res = ptr();
    printf("res=%i\n", res);
    return 0;
}

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
 * @brief       A basic example for linking in RIOT
 *
 * @author      Kushal Singh <kushal.spiderman.singh@gmail.com>
 *
 * @}
 */

volatile unsigned int a=3735928559;
volatile unsigned int x=1;
volatile unsigned int y=3735928559;

void my_test(void);
int lifo_empty(int *);

int test(void) {
    int my_array[10];

    while(x!=1);
    my_test();
    lifo_empty(my_array);

    return 1;
}

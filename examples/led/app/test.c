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
 * @brief       A basic led blink example involving linking in RIOT
 *
 * @author      Kushal Singh <kushal.spiderman.singh@gmail.com>
 *
 * @}
 */

#include "board.h"

int test(void) {

    LED_RED_ON;
    /*Switch the LED on and wait*/
    volatile int x;

    for(x=0;x<100000;x++);

    LED_RED_OFF;
    /*Switch the LED off and return*/

    return 1;
}

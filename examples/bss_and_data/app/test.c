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

#include <stdio.h>

volatile unsigned int a=64;
volatile unsigned int x;
//volatile unsigned int k=0;
volatile unsigned int y=3199196845;
//volatile unsigned int alpha_beta=3735928559;
int my_dance(void);

int my_dance(void)
{return 0;}

int test(void)
{

   // x=1;
   //y=1;
   //a=3199196845;
  // x=3735928559;    
    my_dance();
   // puts("test");

   return 1;
}

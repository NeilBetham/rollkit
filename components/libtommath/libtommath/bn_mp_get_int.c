#include "tommath_private.h"
#ifdef BN_MP_GET_INT_C
/* LibTomMath, multiple-precision integer library -- Tom St Denis */
/* SPDX-License-Identifier: Unlicense */

/* get the lower 32-bits of an mp_int */
unsigned long mp_get_int(const mp_int *a)
{
   /* force result to 32-bits always so it is consistent on non 32-bit platforms */
   return mp_get_long(a) & 0xFFFFFFFFUL;
}
#endif

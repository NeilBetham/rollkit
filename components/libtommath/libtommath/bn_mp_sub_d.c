#include "tommath_private.h"
#ifdef BN_MP_SUB_D_C
/* LibTomMath, multiple-precision integer library -- Tom St Denis */
/* SPDX-License-Identifier: Unlicense */

/* single digit subtraction */
mp_err mp_sub_d(const mp_int *a, mp_digit b, mp_int *c)
{
   mp_digit *tmpa, *tmpc, mu;
   mp_err    res;
   int       ix, oldused;

   /* grow c as required */
   if (c->alloc < (a->used + 1)) {
      if ((res = mp_grow(c, a->used + 1)) != MP_OKAY) {
         return res;
      }
   }

   /* if a is negative just do an unsigned
    * addition [with fudged signs]
    */
   if (a->sign == MP_NEG) {
      mp_int a_ = *a;
      a_.sign = MP_ZPOS;
      res     = mp_add_d(&a_, b, c);
      c->sign = MP_NEG;

      /* clamp */
      mp_clamp(c);

      return res;
   }

   /* setup regs */
   oldused = c->used;
   tmpa    = a->dp;
   tmpc    = c->dp;

   /* if a <= b simply fix the single digit */
   if (((a->used == 1) && (a->dp[0] <= b)) || (a->used == 0)) {
      if (a->used == 1) {
         *tmpc++ = b - *tmpa;
      } else {
         *tmpc++ = b;
      }
      ix      = 1;

      /* negative/1digit */
      c->sign = MP_NEG;
      c->used = 1;
   } else {
      /* positive/size */
      c->sign = MP_ZPOS;
      c->used = a->used;

      /* subtract first digit */
      *tmpc    = *tmpa++ - b;
      mu       = *tmpc >> (MP_SIZEOF_BITS(mp_digit) - 1u);
      *tmpc++ &= MP_MASK;

      /* handle rest of the digits */
      for (ix = 1; ix < a->used; ix++) {
         *tmpc    = *tmpa++ - mu;
         mu       = *tmpc >> (MP_SIZEOF_BITS(mp_digit) - 1u);
         *tmpc++ &= MP_MASK;
      }
   }

   /* zero excess digits */
   MP_ZERO_DIGITS(tmpc, oldused - ix);

   mp_clamp(c);
   return MP_OKAY;
}

#endif

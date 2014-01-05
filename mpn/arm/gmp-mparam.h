/* gmp-mparam.h -- Compiler/machine parameter header file.

Copyright 1991, 1993, 1994, 1999, 2000, 2001, 2002, 2003 Free Software
Foundation, Inc.

This file is part of the GNU MP Library.

The GNU MP Library is free software; you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation; either version 2.1 of the License, or (at your
option) any later version.

The GNU MP Library is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
License for more details.

You should have received a copy of the GNU Lesser General Public License
along with the GNU MP Library; see the file COPYING.LIB.  If not, write to
the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
MA 02110-1301, USA. */

#define BITS_PER_MP_LIMB 32
#define BYTES_PER_MP_LIMB 4

/* ARMv7 */

/* Generated by tuneup.c, 2012-10-25, gcc 4.6 */

#define MUL_KARATSUBA_THRESHOLD          22
#define MUL_TOOM3_THRESHOLD             121
#define MUL_TOOM4_THRESHOLD             180
#define MUL_TOOM8H_THRESHOLD            274

#define SQR_BASECASE_THRESHOLD            7
#define SQR_KARATSUBA_THRESHOLD          44
#define SQR_TOOM3_THRESHOLD             145
#define SQR_TOOM4_THRESHOLD             375
#define SQR_TOOM8_THRESHOLD             375

#define POWM_THRESHOLD                  190

#define HGCD_THRESHOLD                   61
#define GCD_DC_THRESHOLD               1737
#define GCDEXT_DC_THRESHOLD            1240
#define JACOBI_BASE_METHOD                1

#define DIVREM_1_NORM_THRESHOLD           0  /* preinv always */
#define DIVREM_1_UNNORM_THRESHOLD         0  /* always */
#define MOD_1_NORM_THRESHOLD              0  /* always */
#define MOD_1_UNNORM_THRESHOLD            0  /* always */
#define USE_PREINV_DIVREM_1               1  /* preinv always */
#define USE_PREINV_MOD_1                  1  /* preinv always */
#define DIVREM_2_THRESHOLD                0  /* preinv always */
#define DIVEXACT_1_THRESHOLD              0  /* always */
#define MODEXACT_1_ODD_THRESHOLD          0  /* always */
#define MOD_1_1_THRESHOLD                 5
#define MOD_1_2_THRESHOLD                10
#define MOD_1_3_THRESHOLD                34
#define DIVREM_HENSEL_QR_1_THRESHOLD     11
#define RSH_DIVREM_HENSEL_QR_1_THRESHOLD    996
#define DIVREM_EUCLID_HENSEL_THRESHOLD      8

#define ROOTREM_THRESHOLD                 6

#define GET_STR_DC_THRESHOLD             10
#define GET_STR_PRECOMPUTE_THRESHOLD     19
#define SET_STR_DC_THRESHOLD           4183
#define SET_STR_PRECOMPUTE_THRESHOLD  30759

#define MUL_FFT_FULL_THRESHOLD         3776

#define SQR_FFT_FULL_THRESHOLD         6912

#define MULLOW_BASECASE_THRESHOLD         0  /* always */
#define MULLOW_DC_THRESHOLD              53
#define MULLOW_MUL_THRESHOLD           4437

#define MULHIGH_BASECASE_THRESHOLD        0  /* always */
#define MULHIGH_DC_THRESHOLD             54
#define MULHIGH_MUL_THRESHOLD          9970

#define MULMOD_2EXPM1_THRESHOLD          26

#define FAC_UI_THRESHOLD               1203
#define DC_DIV_QR_THRESHOLD             358
#define DC_DIVAPPR_Q_N_THRESHOLD        229
#define INV_DIV_QR_THRESHOLD           1187
#define INV_DIVAPPR_Q_N_THRESHOLD       229
#define DC_DIV_Q_THRESHOLD              263
#define INV_DIV_Q_THRESHOLD            4318
#define DC_DIVAPPR_Q_THRESHOLD          229
#define INV_DIVAPPR_Q_THRESHOLD       10153
#define DC_BDIV_QR_THRESHOLD            358
#define DC_BDIV_Q_THRESHOLD             112

/* fft_tuning -- autogenerated by tune-fft */

#define FFT_TAB \
   { { 3, 2 }, { 3, 2 }, { 2, 1 }, { 1, 0 }, { 1, 0 } }

#define MULMOD_TAB \
   { 4, 3, 3, 3, 3, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1 }

#define FFT_N_NUM 15

#define FFT_MULMOD_2EXPP1_CUTOFF 128


/* Tuneup completed successfully, took 1044 seconds */

/* Generated by tuneup.c, 2014-03-21, gcc 4.4 */

#define MUL_KARATSUBA_THRESHOLD          21
#define MUL_TOOM3_THRESHOLD             133
#define MUL_TOOM4_THRESHOLD             232
#define MUL_TOOM8H_THRESHOLD            414

#define SQR_BASECASE_THRESHOLD            0  /* always (native) */
#define SQR_KARATSUBA_THRESHOLD          30
#define SQR_TOOM3_THRESHOLD             113
#define SQR_TOOM4_THRESHOLD             532
#define SQR_TOOM8_THRESHOLD             532

#define POWM_THRESHOLD                  984

#define DIVREM_1_NORM_THRESHOLD       MP_SIZE_T_MAX  /* never */
#define DIVREM_1_UNNORM_THRESHOLD     MP_SIZE_T_MAX  /* never */
#define MOD_1_NORM_THRESHOLD              0  /* always */
#define MOD_1_UNNORM_THRESHOLD            0  /* always */
#define USE_PREINV_DIVREM_1               1  /* native */
#define USE_PREINV_MOD_1                  1
#define DIVEXACT_1_THRESHOLD              0  /* always */
#define MODEXACT_1_ODD_THRESHOLD          0  /* always (native) */
#define MOD_1_1_THRESHOLD                 5
#define MOD_1_2_THRESHOLD                 9
#define MOD_1_3_THRESHOLD                25
#define DIVREM_HENSEL_QR_1_THRESHOLD     11
#define RSH_DIVREM_HENSEL_QR_1_THRESHOLD      6
#define DIVREM_EUCLID_HENSEL_THRESHOLD    134

#define MUL_FFT_FULL_THRESHOLD         6080

#define SQR_FFT_FULL_THRESHOLD         3904

#define MULLOW_BASECASE_THRESHOLD         9
#define MULLOW_DC_THRESHOLD              30
#define MULLOW_MUL_THRESHOLD           7941

#define MULHIGH_BASECASE_THRESHOLD       10
#define MULHIGH_DC_THRESHOLD             32
#define MULHIGH_MUL_THRESHOLD          3717

#define MULMOD_2EXPM1_THRESHOLD          18

#define SB_DIVAPPR_Q_SMALL_THRESHOLD      0  /* always */
#define SB_DIV_QR_SMALL_THRESHOLD        25
#define DC_DIV_QR_THRESHOLD              56
#define INV_DIV_QR_THRESHOLD           2444
#define INV_DIVAPPR_Q_N_THRESHOLD        56
#define DC_DIV_Q_THRESHOLD               60
#define INV_DIV_Q_THRESHOLD            1970
#define DC_DIVAPPR_Q_THRESHOLD           39
#define INV_DIVAPPR_Q_THRESHOLD       18575
#define DC_BDIV_QR_THRESHOLD             58
#define DC_BDIV_Q_THRESHOLD              33

#define ROOTREM_THRESHOLD                 6

#define MATRIX22_STRASSEN_THRESHOLD      25
#define HGCD_THRESHOLD                  115
#define HGCD_APPR_THRESHOLD              50
#define HGCD_REDUCE_THRESHOLD          6852
#define GCD_DC_THRESHOLD                566
#define GCDEXT_DC_THRESHOLD             375
#define JACOBI_BASE_METHOD                2

#define GET_STR_DC_THRESHOLD             14
#define GET_STR_PRECOMPUTE_THRESHOLD     24
#define SET_STR_DC_THRESHOLD            177
#define SET_STR_PRECOMPUTE_THRESHOLD   1414

#define FAC_DSC_THRESHOLD               866
#define FAC_ODD_THRESHOLD                26

/* fft_tuning -- autogenerated by tune-fft */

#define FFT_TAB \
   { { 4, 4 }, { 4, 3 }, { 3, 2 }, { 2, 2 }, { 2, 0 } }

#define MULMOD_TAB \
   { 4, 4, 4, 4, 4, 3, 3, 3, 3, 3, 2, 3, 2, 2, 2, 2, 2, 2, 1, 1, 1 }

#define FFT_N_NUM 21

#define FFT_MULMOD_2EXPP1_CUTOFF 256


/* Tuneup completed successfully, took 164 seconds */


/* Generated by tuneup.c, 2012-10-23, gcc 4.6 */

#define MUL_KARATSUBA_THRESHOLD          14
#define MUL_TOOM3_THRESHOLD             114
#define MUL_TOOM4_THRESHOLD             154
#define MUL_TOOM8H_THRESHOLD            262

#define SQR_BASECASE_THRESHOLD            0  /* always (native) */
#define SQR_KARATSUBA_THRESHOLD          30 
#define SQR_TOOM3_THRESHOLD             179
#define SQR_TOOM4_THRESHOLD             280
#define SQR_TOOM8_THRESHOLD             286

#define POWM_THRESHOLD                   44

#define HGCD_THRESHOLD                   50
#define GCD_DC_THRESHOLD                507
#define GCDEXT_DC_THRESHOLD             692
#define JACOBI_BASE_METHOD                1

#define DIVREM_1_NORM_THRESHOLD       MP_SIZE_T_MAX  /* never */
#define DIVREM_1_UNNORM_THRESHOLD     MP_SIZE_T_MAX  /* never */
#define MOD_1_NORM_THRESHOLD              0  /* always */
#define MOD_1_UNNORM_THRESHOLD            0  /* always */
#define USE_PREINV_DIVREM_1               1  /* native */
#define USE_PREINV_MOD_1                  1
#define DIVEXACT_1_THRESHOLD              0  /* always */
#define MODEXACT_1_ODD_THRESHOLD          0  /* always (native) */
#define MOD_1_1_THRESHOLD                 6
#define MOD_1_2_THRESHOLD                 8
#define MOD_1_3_THRESHOLD                13
#define DIVREM_HENSEL_QR_1_THRESHOLD      7
#define RSH_DIVREM_HENSEL_QR_1_THRESHOLD      6
#define DIVREM_EUCLID_HENSEL_THRESHOLD      8

#define ROOTREM_THRESHOLD                 6

#define GET_STR_DC_THRESHOLD             17
#define GET_STR_PRECOMPUTE_THRESHOLD     28
#define SET_STR_DC_THRESHOLD           5456
#define SET_STR_PRECOMPUTE_THRESHOLD  40166

#define MUL_FFT_FULL_THRESHOLD         2880

#define SQR_FFT_FULL_THRESHOLD         4032

#define MULLOW_BASECASE_THRESHOLD         4
#define MULLOW_DC_THRESHOLD              13
#define MULLOW_MUL_THRESHOLD           3176

#define MULHIGH_BASECASE_THRESHOLD        8
#define MULHIGH_DC_THRESHOLD             12
#define MULHIGH_MUL_THRESHOLD          3207

#define MULMOD_2EXPM1_THRESHOLD          14

#define FAC_UI_THRESHOLD               1402
#define DC_DIV_QR_THRESHOLD             100
#define DC_DIVAPPR_Q_N_THRESHOLD         78
#define INV_DIV_QR_THRESHOLD            465
#define INV_DIVAPPR_Q_N_THRESHOLD        78
#define DC_DIV_Q_THRESHOLD              148
#define INV_DIV_Q_THRESHOLD            2350
#define DC_DIVAPPR_Q_THRESHOLD           97
#define INV_DIVAPPR_Q_THRESHOLD        3777
#define DC_BDIV_QR_THRESHOLD            100
#define DC_BDIV_Q_THRESHOLD              45

/* fft_tuning -- autogenerated by tune-fft */

#define FFT_TAB \
   { { 4, 3 }, { 3, 3 }, { 3, 2 }, { 2, 1 }, { 1, 0 } }

#define MULMOD_TAB \
   { 4, 4, 4, 4, 4, 3, 3, 3, 3, 3, 2, 3, 3, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1 }

#define FFT_N_NUM 23

#define FFT_MULMOD_2EXPP1_CUTOFF 128


/* Tuneup crashed MULMOD_TAB taken from k8 values */

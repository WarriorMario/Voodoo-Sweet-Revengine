#pragma once
#include <immintrin.h>
// Int32
#define AVX_INT32_FROM1(x) _mm256_set1_epi32(x)
#define AVX_INT32_FROM8(x,y,z,w,a,b,c,d) _mm256_set_epi32(d,c,b,a,w,z,y,x)
#define AVX_INT32_FROM_FLOAT(x) _mm256_cvttps_epi32(x)

#define AVX_INT32_ADD(x,y) _mm256_add_epi32(x, y)
#define AVX_INT32_SUB(x,y) _mm256_sub_epi32(x, y)
#define AVX_INT32_MUL(x,y) _mm256_mullo_epi32(x, y)

#define AVX_INT32_GT(x,y) _mm256_cmpgt_epi32
#define AVX_INT32_EQUAL(x,y) _mm256_cmpeq_epi32

#define AVX_INT32_AND(x,y) _mm256_and_si256(x,y)
#define AVX_INT32_AND_NOT(x,y) _mm256_andnot_si256(x,y)
#define AVX_INT32_OR(x,y) _mm256_or_si256(x,y)
#define AVX_INT32_XOR(x,y) _mm256_xor_si256(x,y)

#define AVX_INT32_SHIFT_RIGHT(x,y) _mm256_srli_epi32(x,y)
#define AVX_INT32_SHIFT_LEFT(x,y) _mm256_slli_epi32(x,y)

// Float
#define AVX_FLOAT_FROM1(x) _mm256_set1_ps(x)
#define AVX_FLOAT_FROM8(x) (x,y,z,w,a,b,c,d) _mm256_set_ps(d,c,b,a,w,z,y,x)

#define AVX_FLOAT_ADD(x,y) _mm256_add_ps(x,y)
#define AVX_FLOAT_SUB(x,y) _mm256_sub_ps(x,y)
#define AVX_FLOAT_MUL(x,y) _mm256_mul_ps(x,y)
#define AVX_FLOAT_DIV(x,y) _mm256_div_ps(x,y)
#define AVX_FLOAT_RECIP(x) _mm256_rcp_ps(x)

#define AVX_FLOAT_GT(x,y) _mm256_cmp_ps(x,y,_CMP_GT_OS)
#define AVX_FLOAT_GTE(x,y) _mm256_cmp_ps(x,y,_CMP_GE_OS)
#define AVX_FLOAT_LT(x,y) _mm256_cmp_ps(x,y,_CMP_LT_OS)
#define AVX_FLOAT_LTE(x,y) _mm256_cmp_ps(x,y,_CMP_LE_OS)

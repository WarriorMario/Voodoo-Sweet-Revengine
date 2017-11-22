#pragma once
#include "AVX.h"
// Eventually we want to be able to switch here between avx and sse
#define AVX 1
#if AVX
#define SIMD_INT32_FROM1(x) AVX_INT32_FROM1
//... avx
#else
#define SIMD_INT32_FROM1(x) //SSE_INT32_FROM1
//... sse
#endif
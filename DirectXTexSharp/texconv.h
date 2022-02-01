//
// Created by moritz.baron on 28/01/2022.
//
#pragma once

#ifdef _MSC_VER
#include <SDKDDKVer.h>
#endif

#define _CRT_SECURE_NO_WARNINGS 1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdint.h>
#include <limits.h>

#if defined(_MSC_VER)
#include <Windows.h>
#include <intrin.h>
#include <dxgiformat.h>

#undef max
#undef min
#else
#include <stddef.h>
	#ifdef __GNUC__
		// GNU C supports forcing inline, just via a different syntax.
        // disable this on MinGW compiler
		#define __forceinline inline __attribute__((always_inline))
	#else
		// Fall back to the standardized inline keyword for unknown dialects
		#define __forceinline inline
	#endif //__GNUC__

	#define _byteswap_ushort(x) __builtin_bswap16((uint16)(x))
	#define _byteswap_ulong(x) __builtin_bswap32((uint32)(x))
	#define _byteswap_uint64(x) __builtin_bswap64((uint64)(x))
	#define _BitScanForward(dst, x) (*(dst) = __builtin_ctz(x))
	#define _BitScanReverse(dst, x) (*(dst) = (__builtin_clz(x) ^ 31))

    // disable this on MinGW compiler
	static inline uint32_t _rotl(uint32_t x, int n) {
	  return (((x) << (n)) | ((x) >> (32-(n))));
	}
#endif

// Windows has this enabled implicitly (and uses different headers), GNU C compilers generally require passing an additional flag, so check for that first.
#ifdef __AVX__
#include <xmmintrin.h>
#endif

#ifdef __GNUC__
#define UNUSED __attribute__((unused))
#elif defined(__cplusplus) && __cplusplus >= 201703L //C++17 attributes
#define UNUSED [[maybe_unused]]
#else //unknown or incompatible method of indicating unused.
#define UNUSED
#endif


#if defined(_MSC_VER)
//  Microsoft
#define EXPORT extern "C" __declspec(dllexport) //
#define IMPORT __declspec(dllimport)
#elif defined(__GNUC__)
//  GCC
    #define EXPORT __attribute__((visibility("default")))
    #define IMPORT
#else
    //  do nothing and hope for the best?
    #define EXPORT
    #define IMPORT
    #pragma warning Unknown dynamic link import/export semantics.
#endif

#define IS_CLR (_MANAGED == 1) || (_M_CEE == 1)

typedef uint8_t byte;
typedef uint8_t uint8;
typedef uint32_t uint32;
typedef uint64_t uint64;
typedef int64_t int64;
typedef int32_t int32;
typedef uint16_t uint16;
typedef int16_t int16;
typedef unsigned int uint;

#include "DirectXTexEnums.h"
#include "../DirectXTex/DirectXTex/DirectXTex.h"

struct Blob
{
    void*   m_buffer;
    size_t  m_size;
};

EXPORT size_t ConvertFromDds(
        byte* bytePtr,
        int len,
        Blob& blob,
        DirectXTexSharp::ESaveFileTypes filetype,
        bool vflip,
        bool hflip);

EXPORT size_t ConvertToDds(
        byte* inBuff,
        int inBuff_len,
        Blob& blob,
        DirectXTexSharp::ESaveFileTypes filetype,
        DXGI_FORMAT format,
        bool vflip,
        bool hflip);

EXPORT int ConvertAndSaveDdsImage(
        byte* bytePtr,
        int len,
        const wchar_t* szFile,
        DirectXTexSharp::ESaveFileTypes filetype,
        bool vflip,
        bool hflip);

namespace DirectXTexSharp::Format
{
    EXPORT size_t ComputeRowPitch(DXGI_FORMAT format, long width, long height);
    EXPORT size_t ComputeSlicePitch(DXGI_FORMAT format, long width, long height);
    EXPORT size_t BitsPerPixel(DXGI_FORMAT format);
}

namespace DirectXTexSharp::Metadata
{
    EXPORT DirectX::TexMetadata GetMetadataFromDDSFile(
            const wchar_t* szFile,
            DirectXTexSharp::DDSFLAGS flags);
    EXPORT DirectX::TexMetadata GetMetadataFromTGAFile(
            const wchar_t* szFile,
            DirectXTexSharp::TGA_FLAGS flags);
    EXPORT DirectX::TexMetadata GetMetadataFromDDSMemory(
            byte* pSource,
            int size,
            DirectXTexSharp::DDSFLAGS flags);
}
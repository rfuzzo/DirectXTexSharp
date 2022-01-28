#pragma once

#define IS_CLR (_MANAGED == 1) || (_M_CEE == 1)

namespace DirectXTexSharp {

#if IS_CLR
    public
#endif
    enum class DDSFLAGS : unsigned long
    {
        DDS_FLAGS_NONE = 0x0,

        DDS_FLAGS_LEGACY_DWORD = 0x1,
        // Assume pitch is DWORD aligned instead of BYTE aligned (used by some legacy DDS files)

        DDS_FLAGS_NO_LEGACY_EXPANSION = 0x2,
        // Do not implicitly convert legacy formats that result in larger pixel sizes (24 bpp, 3:3:2, A8L8, A4L4, P8, A8P8)

        DDS_FLAGS_NO_R10B10G10A2_FIXUP = 0x4,
        // Do not use work-around for long-standing D3DX DDS file format issue which reversed the 10:10:10:2 color order masks

        DDS_FLAGS_FORCE_RGB = 0x8,
        // Convert DXGI 1.1 BGR formats to DXGI_FORMAT_R8G8B8A8_UNORM to avoid use of optional WDDM 1.1 formats

        DDS_FLAGS_NO_16BPP = 0x10,
        // Conversions avoid use of 565, 5551, and 4444 formats and instead expand to 8888 to avoid use of optional WDDM 1.2 formats

        DDS_FLAGS_EXPAND_LUMINANCE = 0x20,
        // When loading legacy luminance formats expand replicating the color channels rather than leaving them packed (L8, L16, A8L8)

        DDS_FLAGS_BAD_DXTN_TAILS = 0x40,
        // Some older DXTn DDS files incorrectly handle mipchain tails for blocks smaller than 4x4

        DDS_FLAGS_FORCE_DX10_EXT = 0x10000,
        // Always use the 'DX10' header extension for DDS writer (i.e. don't try to write DX9 compatible DDS files)

        DDS_FLAGS_FORCE_DX10_EXT_MISC2 = 0x20000,
        // DDS_FLAGS_FORCE_DX10_EXT including miscFlags2 information (result may not be compatible with D3DX10 or D3DX11)

        DDS_FLAGS_FORCE_DX9_LEGACY = 0x40000,
        // Force use of legacy header for DDS writer (will fail if unable to write as such)

        DDS_FLAGS_ALLOW_LARGE_FILES = 0x1000000,
        // Enables the loader to read large dimension .dds files (i.e. greater than known hardware requirements)
    };

#if IS_CLR
    public
#endif
    enum class TGA_FLAGS : unsigned long
    {
        TGA_FLAGS_NONE = 0x0,

        TGA_FLAGS_BGR = 0x1,
        // 24bpp files are returned as BGRX; 32bpp files are returned as BGRA

        TGA_FLAGS_ALLOW_ALL_ZERO_ALPHA = 0x2,
        // If the loaded image has an all zero alpha channel, normally we assume it should be opaque. This flag leaves it alone.

        TGA_FLAGS_IGNORE_SRGB = 0x10,
        // Ignores sRGB TGA 2.0 metadata if present in the file

        TGA_FLAGS_FORCE_SRGB = 0x20,
        // Writes sRGB metadata into the file reguardless of format (TGA 2.0 only)

        TGA_FLAGS_FORCE_LINEAR = 0x40,
        // Writes linear gamma metadata into the file reguardless of format (TGA 2.0 only)

        TGA_FLAGS_DEFAULT_SRGB = 0x80,
        // If no colorspace is specified in TGA 2.0 metadata, assume sRGB
    };

#if IS_CLR
    public
#endif
    enum class WIC_FLAGS : unsigned long
    {
        WIC_FLAGS_NONE = 0x0,

        WIC_FLAGS_FORCE_RGB = 0x1,
        // Loads DXGI 1.1 BGR formats as DXGI_FORMAT_R8G8B8A8_UNORM to avoid use of optional WDDM 1.1 formats

        WIC_FLAGS_NO_X2_BIAS = 0x2,
        // Loads DXGI 1.1 X2 10:10:10:2 format as DXGI_FORMAT_R10G10B10A2_UNORM

        WIC_FLAGS_NO_16BPP = 0x4,
        // Loads 565, 5551, and 4444 formats as 8888 to avoid use of optional WDDM 1.2 formats

        WIC_FLAGS_ALLOW_MONO = 0x8,
        // Loads 1-bit monochrome (black & white) as R1_UNORM rather than 8-bit grayscale

        WIC_FLAGS_ALL_FRAMES = 0x10,
        // Loads all images in a multi-frame file, converting/resizing to match the first frame as needed, defaults to 0th frame otherwise

        WIC_FLAGS_IGNORE_SRGB = 0x20,
        // Ignores sRGB metadata if present in the file

        WIC_FLAGS_FORCE_SRGB = 0x40,
        // Writes sRGB metadata into the file reguardless of format

        WIC_FLAGS_FORCE_LINEAR = 0x80,
        // Writes linear gamma metadata into the file reguardless of format

        WIC_FLAGS_DEFAULT_SRGB = 0x100,
        // If no colorspace is specified, assume sRGB

        WIC_FLAGS_DITHER = 0x10000,
        // Use ordered 4x4 dithering for any required conversions

        WIC_FLAGS_DITHER_DIFFUSION = 0x20000,
        // Use error-diffusion dithering for any required conversions

        WIC_FLAGS_FILTER_POINT = 0x100000,
        WIC_FLAGS_FILTER_LINEAR = 0x200000,
        WIC_FLAGS_FILTER_CUBIC = 0x300000,
        WIC_FLAGS_FILTER_FANT = 0x400000, // Combination of Linear and Box filter
            // Filtering mode to use for any required image resizing (only needed when loading arrays of differently sized images; defaults to Fant)
    };

#if IS_CLR
    public
#endif
    enum class TEX_DIMENSION
        // Subset here matches D3D10_RESOURCE_DIMENSION and D3D11_RESOURCE_DIMENSION
    {
        TEX_DIMENSION_TEXTURE1D = 2,
        TEX_DIMENSION_TEXTURE2D = 3,
        TEX_DIMENSION_TEXTURE3D = 4,
    };

#if IS_CLR
    public
#endif
    enum class TEX_FILTER_FLAGS : unsigned long
    {
        TEX_FILTER_DEFAULT = 0,

        TEX_FILTER_WRAP_U = 0x1,
        TEX_FILTER_WRAP_V = 0x2,
        TEX_FILTER_WRAP_W = 0x4,
        TEX_FILTER_WRAP = (TEX_FILTER_WRAP_U | TEX_FILTER_WRAP_V | TEX_FILTER_WRAP_W),
        TEX_FILTER_MIRROR_U = 0x10,
        TEX_FILTER_MIRROR_V = 0x20,
        TEX_FILTER_MIRROR_W = 0x40,
        TEX_FILTER_MIRROR = (TEX_FILTER_MIRROR_U | TEX_FILTER_MIRROR_V | TEX_FILTER_MIRROR_W),
        // Wrap vs. Mirror vs. Clamp filtering options

        TEX_FILTER_SEPARATE_ALPHA = 0x100,
        // Resize color and alpha channel independently

        TEX_FILTER_FLOAT_X2BIAS = 0x200,
        // Enable *2 - 1 conversion cases for unorm<->float and positive-only float formats

        TEX_FILTER_RGB_COPY_RED = 0x1000,
        TEX_FILTER_RGB_COPY_GREEN = 0x2000,
        TEX_FILTER_RGB_COPY_BLUE = 0x4000,
        // When converting RGB to R, defaults to using grayscale. These flags indicate copying a specific channel instead
        // When converting RGB to RG, defaults to copying RED | GREEN. These flags control which channels are selected instead.

        TEX_FILTER_DITHER = 0x10000,
        // Use ordered 4x4 dithering for any required conversions
        TEX_FILTER_DITHER_DIFFUSION = 0x20000,
        // Use error-diffusion dithering for any required conversions

        TEX_FILTER_POINT = 0x100000,
        TEX_FILTER_LINEAR = 0x200000,
        TEX_FILTER_CUBIC = 0x300000,
        TEX_FILTER_BOX = 0x400000,
        TEX_FILTER_FANT = 0x400000, // Equiv to Box filtering for mipmap generation
        TEX_FILTER_TRIANGLE = 0x500000,
        // Filtering mode to use for any required image resizing

        TEX_FILTER_SRGB_IN = 0x1000000,
        TEX_FILTER_SRGB_OUT = 0x2000000,
        TEX_FILTER_SRGB = (TEX_FILTER_SRGB_IN | TEX_FILTER_SRGB_OUT),
        // sRGB <-> RGB for use in conversion operations
        // if the input format type is IsSRGB(), then SRGB_IN is on by default
        // if the output format type is IsSRGB(), then SRGB_OUT is on by default

        TEX_FILTER_FORCE_NON_WIC = 0x10000000,
        // Forces use of the non-WIC path when both are an option

        TEX_FILTER_FORCE_WIC = 0x20000000,
        // Forces use of the WIC path even when logic would have picked a non-WIC path when both are an option
    };

    //const SValue<uint32_t> g_pSaveFileTypes[] =   // valid formats to write to
    //{
    //    { L"BMP",   WIC_CODEC_BMP  },
    //    { L"JPG",   WIC_CODEC_JPEG },
    //    { L"JPEG",  WIC_CODEC_JPEG },
    //    { L"PNG",   WIC_CODEC_PNG  },
    //    { L"DDS",   CODEC_DDS      },
    //    { L"TGA",   CODEC_TGA      },
    //    { L"HDR",   CODEC_HDR      },
    //    { L"TIF",   WIC_CODEC_TIFF },
    //    { L"TIFF",  WIC_CODEC_TIFF },
    //    { L"WDP",   WIC_CODEC_WMP  },
    //    { L"HDP",   CODEC_HDP      },
    //    { L"JXR",   CODEC_JXR      },
    //    { L"PPM",   CODEC_PPM      },
    //    { L"PFM",   CODEC_PFM      },
    //#ifdef USE_OPENEXR
    //    { L"EXR",   CODEC_EXR      },
    //#endif
    //    { nullptr,  CODEC_DDS      }
    //};

#if IS_CLR
    public
#endif
    enum class ESaveFileTypes
    {
        BMP,
        JPEG,
        PNG,
        TGA,
        //HDR, //broken for some reason, disabling for now
        TIFF,
        /*WIC_CODEC_WMP,
        CODEC_HDP,
        CODEC_JXR,
        CODEC_PPM,
        CODEC_PFMm*/
    };

}

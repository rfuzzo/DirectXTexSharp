#pragma once

namespace DirectXTexSharp {

    public enum class TEX_FILTER_FLAGS : unsigned long
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
}
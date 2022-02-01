#if (_MANAGED == 1) || (_M_CEE == 1)
#include "DirectXTexSharpLib.h"
#include <msclr/marshal.h>
#include <msclr/marshal_cppstd.h>
#else
#include <comdef.h>  // Declares _com_error

#include "texconv.h"
#include "../DirectXTex/DirectXTex/DirectXTex.h"
#endif


#include <wincodec.h>
#include <memory>
//#include <wrl/client.h> //not supported in /clr

using namespace DirectXTexSharp;

namespace
{
    inline static bool ispow2(size_t x)
    {
        return ((x != 0) && !(x & (x - 1)));
    }

    const wchar_t* GetErrorDesc(HRESULT hr)
    {
        static wchar_t desc[1024] = {};

        LPWSTR errorText = nullptr;

        DWORD result = FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_ALLOCATE_BUFFER,
            nullptr, static_cast<DWORD>(hr),
            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), reinterpret_cast<LPWSTR>(&errorText), 0, nullptr);

        *desc = 0;

        if (result > 0 && errorText)
        {
            swprintf_s(desc, L": %ls", errorText);

            size_t len = wcslen(desc);
            if (len >= 2)
            {
                desc[len - 2] = 0;
                desc[len - 1] = 0;
            }

            if (errorText)
                LocalFree(errorText);
        }

        return desc;
    }

    void GetWicPropsJpg(IPropertyBag2* props)
    {
        bool wicLossless = true;
        float wicQuality = -1.f;

        if (wicLossless || wicQuality >= 0.f)
        {
            PROPBAG2 options = {};
            VARIANT varValues = {};
            options.pstrName = const_cast<wchar_t*>(L"ImageQuality");
            varValues.vt = VT_R4;
            varValues.fltVal = (wicLossless) ? 1.f : wicQuality;
            (void)props->Write(1, &options, &varValues);
        }
    }

    void GetWicPropsTiff(IPropertyBag2* props)
    {
        bool wicLossless = true;
        float wicQuality = -1.f;

        PROPBAG2 options = {};
        VARIANT varValues = {};
        if (wicLossless)
        {
            options.pstrName = const_cast<wchar_t*>(L"TiffCompressionMethod");
            varValues.vt = VT_UI1;
            varValues.bVal = WICTiffCompressionNone;
        }
        else if (wicQuality >= 0.f)
        {
            options.pstrName = const_cast<wchar_t*>(L"CompressionQuality");
            varValues.vt = VT_R4;
            varValues.fltVal = wicQuality;
        }
        (void)props->Write(1, &options, &varValues);
    }
}


inline void throw_or_clr(HRESULT hr)
{
    if (FAILED(hr))
    {
#if (_MANAGED == 1) || (_M_CEE == 1)
        System::Runtime::InteropServices::Marshal::ThrowExceptionForHR(hr);
#else
        throw _com_error(hr);
#endif
    }
}

std::unique_ptr<DirectX::ScratchImage> ConvertFromDdsMemory(
        byte* bytePtr,
        int len,
        DirectXTexSharp::ESaveFileTypes filetype,
        bool vflip,
        bool hflip)
{
    // defaults
    DXGI_FORMAT format = DXGI_FORMAT_UNKNOWN;
    DirectX::TEX_FILTER_FLAGS dwFilter = DirectX::TEX_FILTER_DEFAULT;
    DirectX::TEX_FILTER_FLAGS dwSRGB = DirectX::TEX_FILTER_DEFAULT;
    DirectX::TEX_FILTER_FLAGS dwConvert = DirectX::TEX_FILTER_DEFAULT;
    DirectX::TEX_COMPRESS_FLAGS dwCompress = DirectX::TEX_COMPRESS_DEFAULT;
    DirectX::TEX_FILTER_FLAGS dwFilterOpts = DirectX::TEX_FILTER_DEFAULT;
    float alphaThreshold = DirectX::TEX_THRESHOLD_DEFAULT;
    bool non4bc = false;

    HRESULT hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);

    // load image from memory
    DirectX::DDS_FLAGS ddsFlags = DirectX::DDS_FLAGS_ALLOW_LARGE_FILES;
    DirectX::TexMetadata info;
    std::unique_ptr<DirectX::ScratchImage> image(new (std::nothrow) DirectX::ScratchImage);

    hr = DirectX::LoadFromDDSMemory(bytePtr, len, ddsFlags, &info, *image);
    if (FAILED(hr))
    {
        wprintf(L" FAILED (%08X%ls)\n", static_cast<unsigned int>(hr), L"hr");
        throw_or_clr(hr);
    }

    DXGI_FORMAT tformat = (format == DXGI_FORMAT_UNKNOWN) ? info.format : format;

    // decompress if needed
    if (DirectX::IsCompressed(info.format))
    {
        // Direct3D can only create BC resources with multiple-of-4 top levels
        if ((info.width % 4) != 0 || (info.height % 4) != 0)
        {
            if (DirectX::IsCompressed(tformat))
            {
                non4bc = true;
            }
        }

        auto img = image->GetImage(0, 0, 0);
        assert(img);
        size_t nimg = image->GetImageCount();

        std::unique_ptr<DirectX::ScratchImage> timage(new (std::nothrow) DirectX::ScratchImage);
        if (!timage)
        {
            wprintf(L"\nERROR: Memory allocation failed\n");
            return nullptr;
        }

        auto hr = DirectX::Decompress(img, nimg, info, DXGI_FORMAT_UNKNOWN /* picks good default */, *timage);
        if (FAILED(hr))
        {
            wprintf(L" FAILED [decompress] (%08X%ls)\n", static_cast<unsigned int>(hr), L"hr");
            throw_or_clr(hr);
        }

        auto& tinfo = timage->GetMetadata();

        info.format = tinfo.format;

        assert(info.width == tinfo.width);
        assert(info.height == tinfo.height);
        assert(info.depth == tinfo.depth);
        assert(info.arraySize == tinfo.arraySize);
        assert(info.mipLevels == tinfo.mipLevels);
        assert(info.miscFlags == tinfo.miscFlags);
        assert(info.dimension == tinfo.dimension);

        image.swap(timage);

    }

    // Flip/Rotate
    if (vflip || hflip)
    {
        std::unique_ptr<DirectX::ScratchImage> timage(new (std::nothrow) DirectX::ScratchImage);
        if (!timage)
        {
            wprintf(L"\nERROR: Memory allocation failed\n");
            return nullptr;
        }

        DirectX::TEX_FR_FLAGS dwFlags = DirectX::TEX_FR_ROTATE0;

        if (hflip)
            dwFlags |= DirectX::TEX_FR_FLIP_HORIZONTAL;

        if (vflip)
            dwFlags |= DirectX::TEX_FR_FLIP_VERTICAL;

        assert(dwFlags != 0);

        auto hr = FlipRotate(image->GetImages(), image->GetImageCount(), image->GetMetadata(), dwFlags, *timage);
        if (FAILED(hr))
        {
            wprintf(L" FAILED [fliprotate] (%08X%ls)\n", static_cast<unsigned int>(hr), L"hr");
            throw_or_clr(hr);
        }

        auto& tinfo = timage->GetMetadata();

        info.width = tinfo.width;
        info.height = tinfo.height;

        assert(info.depth == tinfo.depth);
        assert(info.arraySize == tinfo.arraySize);
        assert(info.mipLevels == tinfo.mipLevels);
        assert(info.miscFlags == tinfo.miscFlags);
        assert(info.format == tinfo.format);
        assert(info.dimension == tinfo.dimension);

        image.swap(timage);


        //cimage.reset();
    }

    // Resize
    // Swizzle
    // Color rotation
    // Tonemap

    // Convert NormalMaps


    // Convert
    if (filetype == DirectXTexSharp::ESaveFileTypes::TGA ||
        filetype == DirectXTexSharp::ESaveFileTypes::PNG ||
        filetype == DirectXTexSharp::ESaveFileTypes::JPEG ||
        filetype == DirectXTexSharp::ESaveFileTypes::BMP ||
        filetype == DirectXTexSharp::ESaveFileTypes::TIFF)
    {
        tformat = DXGI_FORMAT_R8G8B8A8_UNORM;
    }

    if (info.format != tformat && !DirectX::IsCompressed(tformat))
    {
        std::unique_ptr<DirectX::ScratchImage> timage(new (std::nothrow) DirectX::ScratchImage);
        if (!timage)
        {
            wprintf(L"\nERROR: Memory allocation failed\n");
            return nullptr;
        }

        auto hr = DirectX::Convert(image->GetImages(), image->GetImageCount(), image->GetMetadata(), tformat,
                                   dwFilter | dwFilterOpts | dwSRGB | dwConvert, alphaThreshold, *timage);
        if (FAILED(hr))
        {
            wprintf(L" FAILED [convert] (%08X%ls)\n", static_cast<unsigned int>(hr), L"hr");
            throw_or_clr(hr);
        }

        auto& tinfo = timage->GetMetadata();

        assert(tinfo.format == tformat);
        info.format = tinfo.format;

        assert(info.width == tinfo.width);
        assert(info.height == tinfo.height);
        assert(info.depth == tinfo.depth);
        assert(info.arraySize == tinfo.arraySize);
        assert(info.mipLevels == tinfo.mipLevels);
        assert(info.miscFlags == tinfo.miscFlags);
        assert(info.dimension == tinfo.dimension);

        image.swap(timage);


        //cimage.reset();
    }

    // ColorKey/ChromaKey /o
    // Invert Y Channel /o
    // Reconstruct Z Channel /o
    // Determine whether preserve alpha coverage is required /o
    // Generate mips

    // Preserve mipmap alpha coverage /o
    // Premultiplied alpha /o
    // Compress
    {
        //cimage.reset();
    }

    // Set alpha mode
    {
        if (DirectX::HasAlpha(info.format) && info.format != DXGI_FORMAT_A8_UNORM)
        {
            if (image->IsAlphaAllOpaque())
            {
                info.SetAlphaMode(DirectX::TEX_ALPHA_MODE_OPAQUE);
            }
            else if (info.IsPMAlpha())
            {
                // Aleady set TEX_ALPHA_MODE_PREMULTIPLIED
            }
                /*else if (dwOptions & (uint64_t(1) << OPT_SEPALPHA))
                {
                    info.SetAlphaMode(TEX_ALPHA_MODE_CUSTOM);
                }*/
            else if (info.GetAlphaMode() == DirectX::TEX_ALPHA_MODE_UNKNOWN)
            {
                info.SetAlphaMode(DirectX::TEX_ALPHA_MODE_STRAIGHT);
            }
        }
        else
        {
            info.SetAlphaMode(DirectX::TEX_ALPHA_MODE_UNKNOWN);
        }
    }

    return image;
}

DirectX::Blob ConvertToDdsMemory(
        byte* bytePtr,
        size_t len,
        DirectXTexSharp::ESaveFileTypes filetype,
        DXGI_FORMAT format,
        bool vflip,
        bool hflip)
{
    // defaults
    size_t mipLevels = 0; // since we're converting FROM non-dds images
    bool non4bc = false;
    DirectX::TEX_FILTER_FLAGS dwFilter = DirectX::TEX_FILTER_DEFAULT;
    DirectX::TEX_FILTER_FLAGS dwSRGB = DirectX::TEX_FILTER_DEFAULT;
    DirectX::TEX_FILTER_FLAGS dwConvert = DirectX::TEX_FILTER_DEFAULT;
    DirectX::TEX_COMPRESS_FLAGS dwCompress = DirectX::TEX_COMPRESS_DEFAULT; //TEX_COMPRESS_PARALLEL, TEX_COMPRESS_DEFAULT
    DirectX::TEX_FILTER_FLAGS dwFilterOpts = DirectX::TEX_FILTER_DEFAULT;
    float alphaThreshold = DirectX::TEX_THRESHOLD_DEFAULT;
    bool preserveAlphaCoverage = false;
    float alphaWeight = 1.f;

    HRESULT hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
    DirectX::Blob blob;

    // load image from other formats
    DirectX::TexMetadata info;
    std::unique_ptr<DirectX::ScratchImage> image(new (std::nothrow) DirectX::ScratchImage);

    switch (filetype)
    {
        case DirectXTexSharp::ESaveFileTypes::TGA:
        {
            hr = DirectX::LoadFromTGAMemory(bytePtr, len, &info, *image);
            if (FAILED(hr))
            {
                wprintf(L" FAILED (%08X%ls)\n", static_cast<unsigned int>(hr), GetErrorDesc(hr));
                throw_or_clr(hr);
            }
            break;
        }
        case DirectXTexSharp::ESaveFileTypes::BMP:
        {

            hr = DirectX::LoadFromWICMemory(bytePtr, len, DirectX::WIC_FLAGS_NONE, &info, *image);
            if (FAILED(hr))
            {
                wprintf(L" FAILED (%08X%ls)\n", static_cast<unsigned int>(hr), GetErrorDesc(hr));
                throw_or_clr(hr);
                //hr = DirectX::LoadFromExtendedBMPMemory(bmpData.get(), bmpSize, metadata, image);
            }


            break;
        }
        default:
        {
            // WIC shares the same filter values for mode and dither
            static_assert(static_cast<int>(DirectX::WIC_FLAGS_DITHER) == static_cast<int>(DirectX::TEX_FILTER_DITHER), "WIC_FLAGS_* & TEX_FILTER_* should match");
            static_assert(static_cast<int>(DirectX::WIC_FLAGS_DITHER_DIFFUSION) == static_cast<int>(DirectX::TEX_FILTER_DITHER_DIFFUSION), "WIC_FLAGS_* & TEX_FILTER_* should match");
            static_assert(static_cast<int>(DirectX::WIC_FLAGS_FILTER_POINT) == static_cast<int>(DirectX::TEX_FILTER_POINT), "WIC_FLAGS_* & TEX_FILTER_* should match");
            static_assert(static_cast<int>(DirectX::WIC_FLAGS_FILTER_LINEAR) == static_cast<int>(DirectX::TEX_FILTER_LINEAR), "WIC_FLAGS_* & TEX_FILTER_* should match");
            static_assert(static_cast<int>(DirectX::WIC_FLAGS_FILTER_CUBIC) == static_cast<int>(DirectX::TEX_FILTER_CUBIC), "WIC_FLAGS_* & TEX_FILTER_* should match");
            static_assert(static_cast<int>(DirectX::WIC_FLAGS_FILTER_FANT) == static_cast<int>(DirectX::TEX_FILTER_FANT), "WIC_FLAGS_* & TEX_FILTER_* should match");

            DirectX::WIC_FLAGS wicFlags = DirectX::WIC_FLAGS_NONE | dwFilter;
            hr = DirectX::LoadFromWICMemory(bytePtr, len, wicFlags, &info, *image);
            if (FAILED(hr))
            {
                wprintf(L" FAILED (%08X%ls)\n", static_cast<unsigned int>(hr), GetErrorDesc(hr));
                throw_or_clr(hr);
            }
        }
    }

    size_t tMips = mipLevels;

    // --- Planar ------------------------------------------------------------------

    DXGI_FORMAT tformat = (format == DXGI_FORMAT_UNKNOWN) ? info.format : format;

    // --- Decompress --------------------------------------------------------------
    std::unique_ptr<DirectX::ScratchImage> cimage;
    if (DirectX::IsCompressed(info.format))
    {
        // Direct3D can only create BC resources with multiple-of-4 top levels
        if ((info.width % 4) != 0 || (info.height % 4) != 0)
        {
            if (DirectX::IsCompressed(tformat))
            {
                non4bc = true;
            }
        }

        auto img = image->GetImage(0, 0, 0);
        assert(img);
        size_t nimg = image->GetImageCount();

        std::unique_ptr<DirectX::ScratchImage> timage(new (std::nothrow) DirectX::ScratchImage);
        if (!timage)
        {
            wprintf(L"\nERROR: Memory allocation failed\n");
            throw_or_clr(hr);
        }

        hr = Decompress(img, nimg, info, DXGI_FORMAT_UNKNOWN /* picks good default */, *timage);
        if (FAILED(hr))
        {
            wprintf(L" FAILED [decompress] (%08X%ls)\n", static_cast<unsigned int>(hr), GetErrorDesc(hr));
            throw_or_clr(hr);
        }

        auto& tinfo = timage->GetMetadata();

        info.format = tinfo.format;

        assert(info.width == tinfo.width);
        assert(info.height == tinfo.height);
        assert(info.depth == tinfo.depth);
        assert(info.arraySize == tinfo.arraySize);
        assert(info.mipLevels == tinfo.mipLevels);
        assert(info.miscFlags == tinfo.miscFlags);
        assert(info.dimension == tinfo.dimension);

        // Keep the original compressed image in case we can reuse it
        cimage.reset(image.release());
        image.reset(timage.release());
    }

    // --- Undo Premultiplied Alpha (if requested) ---------------------------------
    // --- Flip/Rotate -------------------------------------------------------------
    if (hflip || vflip)
    {
        std::unique_ptr<DirectX::ScratchImage> timage(new (std::nothrow) DirectX::ScratchImage);
        if (!timage)
        {
            wprintf(L"\nERROR: Memory allocation failed\n");
            return blob/*nullptr*/;
        }

        DirectX::TEX_FR_FLAGS dwFlags = DirectX::TEX_FR_ROTATE0;

        if (hflip)
            dwFlags |= DirectX::TEX_FR_FLIP_HORIZONTAL;

        if (vflip)
            dwFlags |= DirectX::TEX_FR_FLIP_VERTICAL;

        assert(dwFlags != 0);

        hr = FlipRotate(image->GetImages(), image->GetImageCount(), image->GetMetadata(), dwFlags, *timage);
        if (FAILED(hr))
        {
            wprintf(L" FAILED [fliprotate] (%08X%ls)\n", static_cast<unsigned int>(hr), GetErrorDesc(hr));
            throw_or_clr(hr);
        }

        auto& tinfo = timage->GetMetadata();

        info.width = tinfo.width;
        info.height = tinfo.height;

        assert(info.depth == tinfo.depth);
        assert(info.arraySize == tinfo.arraySize);
        assert(info.mipLevels == tinfo.mipLevels);
        assert(info.miscFlags == tinfo.miscFlags);
        assert(info.format == tinfo.format);
        assert(info.dimension == tinfo.dimension);

        image.swap(timage);
        cimage.reset();
    }
    // --- Resize ------------------------------------------------------------------
    {

    }
    // --- Swizzle (if requested) --------------------------------------------------
    // --- Color rotation (if requested) -------------------------------------------
    // --- Tonemap (if requested) --------------------------------------------------
    // --- Convert -----------------------------------------------------------------
    {
        if (info.format != tformat && !DirectX::IsCompressed(tformat))
        {
            std::unique_ptr<DirectX::ScratchImage> timage(new (std::nothrow) DirectX::ScratchImage);
            if (!timage)
            {
                wprintf(L"\nERROR: Memory allocation failed\n");
                return blob/*nullptr*/;
            }

            hr = DirectX::Convert(image->GetImages(), image->GetImageCount(), image->GetMetadata(), tformat,
                                  dwFilter | dwFilterOpts | dwSRGB | dwConvert, alphaThreshold, *timage);
            if (FAILED(hr))
            {
                wprintf(L" FAILED [convert] (%08X%ls)\n", static_cast<unsigned int>(hr), GetErrorDesc(hr));
                throw_or_clr(hr);
            }

            auto& tinfo = timage->GetMetadata();

            assert(tinfo.format == tformat);
            info.format = tinfo.format;

            assert(info.width == tinfo.width);
            assert(info.height == tinfo.height);
            assert(info.depth == tinfo.depth);
            assert(info.arraySize == tinfo.arraySize);
            assert(info.mipLevels == tinfo.mipLevels);
            assert(info.miscFlags == tinfo.miscFlags);
            assert(info.dimension == tinfo.dimension);

            image.swap(timage);
            cimage.reset();
        }
    }
    // --- ColorKey/ChromaKey ------------------------------------------------------
    // --- Invert Y Channel --------------------------------------------------------
    // --- Reconstruct Z Channel ---------------------------------------------------
    // --- Determine whether preserve alpha coverage is required (if requested) ----
    // --- Generate mips -----------------------------------------------------------
    {
        DirectX::TEX_FILTER_FLAGS dwFilter3D = dwFilter;
        if (!ispow2(info.width) || !ispow2(info.height) || !ispow2(info.depth))
        {
            /*if (!tMips || info.mipLevels != 1)
            {
                nonpow2warn = true;
            }*/

            if (info.dimension == DirectX::TEX_DIMENSION_TEXTURE3D)
            {
                // Must force triangle filter for non-power-of-2 volume textures to get correct results
                dwFilter3D = DirectX::TEX_FILTER_TRIANGLE;
            }
        }

        if ((!tMips || info.mipLevels != tMips || preserveAlphaCoverage) && (info.mipLevels != 1))
        {
            // Mips generation only works on a single base image, so strip off existing mip levels
            // Also required for preserve alpha coverage so that existing mips are regenerated

            std::unique_ptr<DirectX::ScratchImage> timage(new (std::nothrow) DirectX::ScratchImage);
            if (!timage)
            {
                wprintf(L"\nERROR: Memory allocation failed\n");
                return blob/*nullptr*/;
            }

            DirectX::TexMetadata mdata = info;
            mdata.mipLevels = 1;
            hr = timage->Initialize(mdata);
            if (FAILED(hr))
            {
                wprintf(L" FAILED [copy to single level] (%08X%ls)\n", static_cast<unsigned int>(hr), GetErrorDesc(hr));
                throw_or_clr(hr);
            }

            if (info.dimension == DirectX::TEX_DIMENSION_TEXTURE3D)
            {
                for (size_t d = 0; d < info.depth; ++d)
                {
                    hr = CopyRectangle(*image->GetImage(0, 0, d), DirectX::Rect(0, 0, info.width, info.height),
                                       *timage->GetImage(0, 0, d), DirectX::TEX_FILTER_DEFAULT, 0, 0);
                    if (FAILED(hr))
                    {
                        wprintf(L" FAILED [copy to single level] (%08X%ls)\n", static_cast<unsigned int>(hr), GetErrorDesc(hr));
                        throw_or_clr(hr);
                    }
                }
            }
            else
            {
                for (size_t i = 0; i < info.arraySize; ++i)
                {
                    hr = CopyRectangle(*image->GetImage(0, i, 0), DirectX::Rect(0, 0, info.width, info.height),
                                       *timage->GetImage(0, i, 0), DirectX::TEX_FILTER_DEFAULT, 0, 0);
                    if (FAILED(hr))
                    {
                        wprintf(L" FAILED [copy to single level] (%08X%ls)\n", static_cast<unsigned int>(hr), GetErrorDesc(hr));
                        throw_or_clr(hr);
                    }
                }
            }

            image.swap(timage);
            info.mipLevels = 1;

            if (cimage && (tMips == 1))
            {
                // Special case for trimming mips off compressed images and keeping the original compressed highest level mip
                mdata = cimage->GetMetadata();
                mdata.mipLevels = 1;
                hr = timage->Initialize(mdata);
                if (FAILED(hr))
                {
                    wprintf(L" FAILED [copy compressed to single level] (%08X%ls)\n", static_cast<unsigned int>(hr), GetErrorDesc(hr));
                    throw_or_clr(hr);
                }

                if (mdata.dimension == DirectX::TEX_DIMENSION_TEXTURE3D)
                {
                    for (size_t d = 0; d < mdata.depth; ++d)
                    {
                        auto simg = cimage->GetImage(0, 0, d);
                        auto dimg = timage->GetImage(0, 0, d);

                        memcpy_s(dimg->pixels, dimg->slicePitch, simg->pixels, simg->slicePitch);
                    }
                }
                else
                {
                    for (size_t i = 0; i < mdata.arraySize; ++i)
                    {
                        auto simg = cimage->GetImage(0, i, 0);
                        auto dimg = timage->GetImage(0, i, 0);

                        memcpy_s(dimg->pixels, dimg->slicePitch, simg->pixels, simg->slicePitch);
                    }
                }

                cimage.swap(timage);
            }
            else
            {
                cimage.reset();
            }
        }

        if ((!tMips || info.mipLevels != tMips) && (info.width > 1 || info.height > 1 || info.depth > 1))
        {
            std::unique_ptr<DirectX::ScratchImage> timage(new (std::nothrow) DirectX::ScratchImage);
            if (!timage)
            {
                wprintf(L"\nERROR: Memory allocation failed\n");
                return blob/*nullptr*/;
            }

            if (info.dimension == DirectX::TEX_DIMENSION_TEXTURE3D)
            {
                hr = DirectX::GenerateMipMaps3D(image->GetImages(), image->GetImageCount(), image->GetMetadata(), dwFilter3D | dwFilterOpts, tMips, *timage);
            }
            else
            {
                hr = DirectX::GenerateMipMaps(image->GetImages(), image->GetImageCount(), image->GetMetadata(), dwFilter | dwFilterOpts, tMips, *timage);
            }
            if (FAILED(hr))
            {
                wprintf(L" FAILED [mipmaps] (%08X%ls)\n", static_cast<unsigned int>(hr), GetErrorDesc(hr));
                throw_or_clr(hr);
            }

            auto& tinfo = timage->GetMetadata();
            info.mipLevels = tinfo.mipLevels;

            assert(info.width == tinfo.width);
            assert(info.height == tinfo.height);
            assert(info.depth == tinfo.depth);
            assert(info.arraySize == tinfo.arraySize);
            assert(info.miscFlags == tinfo.miscFlags);
            assert(info.format == tinfo.format);
            assert(info.dimension == tinfo.dimension);

            image.swap(timage);
            cimage.reset();
        }
    }
    // --- Preserve mipmap alpha coverage (if requested) ---------------------------
    // --- Premultiplied alpha (if requested) --------------------------------------
    // --- Compress ----------------------------------------------------------------
    {
        if (DirectX::IsCompressed(tformat))
        {
            /*Microsoft::WRL::ComPtr<ID3D11Device> pDevice;
            int adapter = -1;*/



            if (cimage && (cimage->GetMetadata().format == tformat))
            {
                // We never changed the image and it was already compressed in our desired format, use original data
                image.reset(cimage.release());

                auto& tinfo = image->GetMetadata();

                if ((tinfo.width % 4) != 0 || (tinfo.height % 4) != 0)
                {
                    non4bc = true;
                }

                info.format = tinfo.format;
                assert(info.width == tinfo.width);
                assert(info.height == tinfo.height);
                assert(info.depth == tinfo.depth);
                assert(info.arraySize == tinfo.arraySize);
                assert(info.mipLevels == tinfo.mipLevels);
                assert(info.miscFlags == tinfo.miscFlags);
                assert(info.dimension == tinfo.dimension);
            }
            else
            {
                cimage.reset();

                auto img = image->GetImage(0, 0, 0);
                assert(img);
                size_t nimg = image->GetImageCount();

                std::unique_ptr<DirectX::ScratchImage> timage(new (std::nothrow) DirectX::ScratchImage);
                if (!timage)
                {
                    wprintf(L"\nERROR: Memory allocation failed\n");
                    return blob/*nullptr*/;
                }

                bool bc6hbc7 = false;
                switch (tformat)
                {
                    case DXGI_FORMAT_BC6H_TYPELESS:
                    case DXGI_FORMAT_BC6H_UF16:
                    case DXGI_FORMAT_BC6H_SF16:
                    case DXGI_FORMAT_BC7_TYPELESS:
                    case DXGI_FORMAT_BC7_UNORM:
                    case DXGI_FORMAT_BC7_UNORM_SRGB:
                        bc6hbc7 = true;

                        {
                            static bool s_tryonce = false;

                            if (!s_tryonce)
                            {
                                s_tryonce = true;

                                //if (true) //use GPU encoding by default
                                //{
                                //    if (!CreateDevice(adapter, pDevice.GetAddressOf()))
                                //        wprintf(L"\nWARNING: DirectCompute is not available, using BC6H / BC7 CPU codec\n");
                                //}
                                //else
                                {
                                    wprintf(L"\nWARNING: using BC6H / BC7 CPU codec\n");
                                }
                            }
                        }
                        break;

                    default:
                        break;
                }

                DirectX::TEX_COMPRESS_FLAGS cflags = dwCompress;
#ifdef _OPENMP
                //if (!(dwOptions & (uint64_t(1) << OPT_FORCE_SINGLEPROC)))
                {
                    cflags |= DirectX::TEX_COMPRESS_PARALLEL;
                }
#endif

                if ((img->width % 4) != 0 || (img->height % 4) != 0)
                {
                    non4bc = true;
                }

                /* if (bc6hbc7 && pDevice)
                 {
                     hr = Compress(pDevice.Get(), img, nimg, info, tformat, dwCompress | dwSRGB, alphaWeight, *timage);
                 }
                 else*/
                {
                    hr = Compress(img, nimg, info, tformat, cflags | dwSRGB, alphaThreshold, *timage);
                }
                if (FAILED(hr))
                {
                    wprintf(L" FAILED [compress] (%08X%ls)\n", static_cast<unsigned int>(hr), GetErrorDesc(hr));
                    throw_or_clr(hr);
                }

                auto& tinfo = timage->GetMetadata();

                info.format = tinfo.format;
                assert(info.width == tinfo.width);
                assert(info.height == tinfo.height);
                assert(info.depth == tinfo.depth);
                assert(info.arraySize == tinfo.arraySize);
                assert(info.mipLevels == tinfo.mipLevels);
                assert(info.miscFlags == tinfo.miscFlags);
                assert(info.dimension == tinfo.dimension);

                image.swap(timage);
            }
        }
        else
        {
            cimage.reset();
        }
    }
    // --- Set alpha mode ----------------------------------------------------------
    {
        if (DirectX::HasAlpha(info.format)
            && info.format != DXGI_FORMAT_A8_UNORM)
        {
            if (image->IsAlphaAllOpaque())
            {
                info.SetAlphaMode(DirectX::TEX_ALPHA_MODE_OPAQUE);
            }
            else if (info.IsPMAlpha())
            {
                // Aleady set TEX_ALPHA_MODE_PREMULTIPLIED
            }
                /* else if (dwOptions & (uint64_t(1) << OPT_SEPALPHA))
                 {
                     info.SetAlphaMode(DirectX::TEX_ALPHA_MODE_CUSTOM);
                 }*/
            else if (info.GetAlphaMode() == DirectX::TEX_ALPHA_MODE_UNKNOWN)
            {
                info.SetAlphaMode(DirectX::TEX_ALPHA_MODE_STRAIGHT);
            }
        }
        else
        {
            info.SetAlphaMode(DirectX::TEX_ALPHA_MODE_UNKNOWN);
        }
    }


    // --- Save result -------------------------------------------------------------
    auto img = image->GetImage(0, 0, 0);
    assert(img);
    size_t nimg = image->GetImageCount();



    DirectX::DDS_FLAGS ddsFlags = DirectX::DDS_FLAGS_NONE;
    // use dx10
    ddsFlags |= DirectX::DDS_FLAGS_FORCE_DX10_EXT | DirectX::DDS_FLAGS_FORCE_DX10_EXT_MISC2;

    hr = DirectX::SaveToDDSMemory(img, nimg, info, ddsFlags, blob);

    if (FAILED(hr))
    {
        wprintf(L"Failed to initialize COM (%08X%ls)\n", static_cast<unsigned int>(hr), L"hr");
        throw_or_clr(hr);
    }

    return blob;


}



//HRESULT __cdecl SaveToTGAMemory(_In_ const Image & image,
//	_In_ TGA_FLAGS flags,
//	_Out_ Blob & blob, _In_opt_ const TexMetadata * metadata = nullptr) noexcept;
int ConvertAndSaveDdsImage(
    byte* bytePtr,
    int len,
#if (_MANAGED == 1) || (_M_CEE == 1)
    System::String^ szFile,
#else
    const wchar_t* szFile,
#endif
	DirectXTexSharp::ESaveFileTypes filetype, 
    bool vflip, 
    bool hflip) {

#if (_MANAGED == 1) || (_M_CEE == 1)
    msclr::interop::marshal_context context;
#endif

    HRESULT hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
        
    auto image = ConvertFromDdsMemory(bytePtr, len, filetype, vflip, hflip);
    auto img = image->GetImage(0, 0, 0);
    assert(img);

    switch (filetype)
    {
    case DirectXTexSharp::ESaveFileTypes::TGA:
    {
        hr = DirectX::SaveToTGAFile(
            img[0], 
            DirectX::TGA_FLAGS_NONE,
#if (_MANAGED == 1) || (_M_CEE == 1)
            context.marshal_as<const wchar_t*>(szFile),
#else
            szFile,
#endif
            /*(dwOptions & (uint64_t(1) << OPT_TGA20)) ? &info :*/ nullptr);
        break;
    }
    // broken for some reason
    /*case DirectXTexSharp::ESaveFileTypes::HDR:
    {
        hr = DirectX::SaveToHDRFile(img[0], context.marshal_as<const wchar_t*>(szFile));
        break;
    }*/
    case DirectXTexSharp::ESaveFileTypes::JPEG:
    {
        DirectX::WICCodecs codec = DirectX::WICCodecs::WIC_CODEC_JPEG;
        size_t nimages = 1;

        hr = DirectX::SaveToWICFile(
            img, 
            nimages, 
            DirectX::WIC_FLAGS_NONE, 
            DirectX::GetWICCodec(codec),
#if (_MANAGED == 1) || (_M_CEE == 1)
            context.marshal_as<const wchar_t*>(szFile),
#else
            szFile,
#endif
            nullptr, 
            GetWicPropsJpg);
        break;
    }
    case DirectXTexSharp::ESaveFileTypes::TIFF:
    {
        DirectX::WICCodecs codec = DirectX::WICCodecs::WIC_CODEC_TIFF;
        size_t nimages = 1;

        hr = DirectX::SaveToWICFile(
            img,
            nimages,
            DirectX::WIC_FLAGS_NONE,
            DirectX::GetWICCodec(codec),
#if (_MANAGED == 1) || (_M_CEE == 1)
            context.marshal_as<const wchar_t*>(szFile),
#else
            szFile,
#endif
            nullptr,
            GetWicPropsTiff);
        break;
    }
    case DirectXTexSharp::ESaveFileTypes::PNG:
    {
        DirectX::WICCodecs codec = DirectX::WICCodecs::WIC_CODEC_PNG;
        size_t nimages = 1;

        hr = DirectX::SaveToWICFile(
            img,
            nimages,
            DirectX::WIC_FLAGS_NONE,
            DirectX::GetWICCodec(codec),
#if (_MANAGED == 1) || (_M_CEE == 1)
            context.marshal_as<const wchar_t*>(szFile),
#else
            szFile,
#endif
            nullptr,
            nullptr);
        break;
    }
    case DirectXTexSharp::ESaveFileTypes::BMP:
    {
        DirectX::WICCodecs codec = DirectX::WICCodecs::WIC_CODEC_BMP;
        size_t nimages = 1;

        hr = DirectX::SaveToWICFile(
            img,
            nimages,
            DirectX::WIC_FLAGS_NONE,
            DirectX::GetWICCodec(codec),
#if (_MANAGED == 1) || (_M_CEE == 1)
            context.marshal_as<const wchar_t*>(szFile),
#else
            szFile,
#endif
            nullptr,
            nullptr);
        break;
    }

    }

    if (FAILED(hr))
    {
        wprintf(L"Failed to initialize COM (%08X%ls)\n", static_cast<unsigned int>(hr), L"hr");
        return 1;
    }

    return 0;
}

EXPORT size_t ConvertFromDds(
        byte* bytePtr,
        int len,
        Blob& blob,
        DirectXTexSharp::ESaveFileTypes filetype,
        bool vflip,
        bool hflip)
{
    HRESULT hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);

    auto image = ConvertFromDdsMemory(bytePtr, len, filetype, vflip, hflip);
    auto img = image->GetImage(0, 0, 0);
    assert(img);

    DirectX::Blob dblob;

    switch (filetype)
    {
    case DirectXTexSharp::ESaveFileTypes::TGA:
    {
            
        hr = DirectX::SaveToTGAMemory(
                img[0],
                DirectX::TGA_FLAGS_NONE,
                dblob,
            /*(dwOptions & (uint64_t(1) << OPT_TGA20)) ? &info :*/ nullptr);
        break;
    }
    // broken for some reason
    /*case DirectXTexSharp::ESaveFileTypes::HDR:
    {
        hr = DirectX::SaveToHDRFile(img[0], context.marshal_as<const wchar_t*>(szFile));
        break;
    }*/
    case DirectXTexSharp::ESaveFileTypes::JPEG:
    {
        DirectX::WICCodecs codec = DirectX::WICCodecs::WIC_CODEC_JPEG;
        size_t nimages = 1;

        hr = DirectX::SaveToWICMemory(
                img,
                nimages,
                DirectX::WIC_FLAGS_NONE,
                DirectX::GetWICCodec(codec),
                dblob,
                nullptr,
                GetWicPropsJpg);
        break;
    }
    case DirectXTexSharp::ESaveFileTypes::TIFF:
    {
        DirectX::WICCodecs codec = DirectX::WICCodecs::WIC_CODEC_TIFF;
        size_t nimages = 1;

        hr = DirectX::SaveToWICMemory(
                img,
                nimages,
                DirectX::WIC_FLAGS_NONE,
                DirectX::GetWICCodec(codec),
                dblob,
                nullptr,
                GetWicPropsTiff);
        break;
    }
    case DirectXTexSharp::ESaveFileTypes::PNG:
    {
        DirectX::WICCodecs codec = DirectX::WICCodecs::WIC_CODEC_PNG;
        size_t nimages = 1;

        hr = DirectX::SaveToWICMemory(
                img,
                nimages,
                DirectX::WIC_FLAGS_NONE,
                DirectX::GetWICCodec(codec),
                dblob,
                nullptr,
                nullptr);
        break;
    }
    case DirectXTexSharp::ESaveFileTypes::BMP:
    {
        DirectX::WICCodecs codec = DirectX::WICCodecs::WIC_CODEC_BMP;
        size_t nimages = 1;

        hr = DirectX::SaveToWICMemory(
                img,
                nimages,
                DirectX::WIC_FLAGS_NONE,
                DirectX::GetWICCodec(codec),
                dblob,
                nullptr,
                nullptr);
        break;
    }

    }

    if (FAILED(hr))
    {
        wprintf(L"Failed to initialize COM (%08X%ls)\n", static_cast<unsigned int>(hr), L"hr");
        throw_or_clr(hr);
    }

    auto len_buffer = dblob.GetBufferSize();

    blob.m_buffer = new byte[len_buffer];
    blob.m_size = len_buffer;
    memcpy(blob.m_buffer, dblob.GetBufferPointer(), len_buffer);

    dblob.Release();
    return len_buffer;
}

EXPORT size_t ConvertToDds(
        byte* inBuff,
        int inBuff_len,
        Blob& blob,
        DirectXTexSharp::ESaveFileTypes filetype,
        DXGI_FORMAT format,
        bool vflip,
        bool hflip)
{
    HRESULT hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);

    auto dblob = ConvertToDdsMemory(inBuff, inBuff_len, filetype, static_cast<__dxgiformat_h__::DXGI_FORMAT> (format), vflip, hflip);

    auto len_buffer = dblob.GetBufferSize();

    blob.m_buffer = new byte[len_buffer];
    blob.m_size = len_buffer;
    memcpy(blob.m_buffer, dblob.GetBufferPointer(), len_buffer);

    dblob.Release();
    return len_buffer;
}
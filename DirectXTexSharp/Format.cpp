#include "DirectXTexSharpLib.h"

#include "DXGI_FORMAT.h"



using namespace DirectXTexSharp;

//---------------------------------------------------------------------------------
// DXGI Format Utilities

//HRESULT __cdecl ComputePitch(
//    _In_ DXGI_FORMAT fmt, _In_ size_t width, _In_ size_t height,
//    _Out_ size_t& rowPitch, _Out_ size_t& slicePitch, _In_ CP_FLAGS flags = CP_FLAGS_NONE) noexcept;
long DirectXTexSharp::Format::ComputeRowPitch(DirectXTexSharp::DXGI_FORMAT_WRAPPED format, const long width, const long height)
{
    size_t row_pitch;
    size_t slice_pitch;
    const auto result = DirectX::ComputePitch(static_cast<__dxgiformat_h__::DXGI_FORMAT> (format), width, height, row_pitch, slice_pitch);

    Marshal::ThrowExceptionForHR(result);

    return row_pitch;
}

long DirectXTexSharp::Format::ComputeSlicePitch(DirectXTexSharp::DXGI_FORMAT_WRAPPED format, const long width, const long height)
{
    size_t row_pitch;
    size_t slice_pitch;
    const auto result = DirectX::ComputePitch(static_cast<__dxgiformat_h__::DXGI_FORMAT> (format), width, height, row_pitch, slice_pitch);

    Marshal::ThrowExceptionForHR(result);

    return slice_pitch;
}

//size_t __cdecl BitsPerPixel(_In_ DXGI_FORMAT fmt) noexcept;
long DirectXTexSharp::Format::BitsPerPixel(DirectXTexSharp::DXGI_FORMAT_WRAPPED format)
{
    return DirectX::BitsPerPixel(static_cast<__dxgiformat_h__::DXGI_FORMAT> (format));
}




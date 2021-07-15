#include "DirectXTexSharpLib.h"

#include <msclr/marshal.h>
#include <msclr/marshal_cppstd.h>

#include "DirectXTexEnums.h"
#include "DXGI_FORMAT.h"
#include "Image.h"
#include "ScratchImage.h"

using namespace DirectXTexSharp;


//---------------------------------------------------------------------------------
// Texture conversion, resizing, mipmap generation, and block compression

//HRESULT __cdecl Convert(
//	_In_ const Image& srcImage, _In_ DXGI_FORMAT format, _In_ TEX_FILTER_FLAGS filter, _In_ float threshold,
//	_Out_ ScratchImage& image) noexcept;
DirectXTexSharp::ScratchImage^ DirectXTexSharp::Conversion::Convert(
	DirectXTexSharp::Image^ srcImage,
	DirectXTexSharp::DXGI_FORMAT_WRAPPED format,
	DirectXTexSharp::TEX_FILTER_FLAGS filter,
	const float threshold/*,
	DirectXTexSharp::ScratchImage^ image*/) {

	DirectX::ScratchImage image;
	auto result = DirectX::Convert(
		*srcImage->get_instance(),
		static_cast<__dxgiformat_h__::DXGI_FORMAT> (format),
		static_cast<DirectX::TEX_FILTER_FLAGS> (filter),
		threshold,
		image);

	Marshal::ThrowExceptionForHR(result);

	return gcnew DirectXTexSharp::ScratchImage(image);
}

//HRESULT __cdecl Decompress(_In_ const Image& cImage, _In_ DXGI_FORMAT format, _Out_ ScratchImage& image) noexcept;
DirectXTexSharp::ScratchImage^ DirectXTexSharp::Conversion::Decompress(
	Image^ cImage,
	DirectXTexSharp::DXGI_FORMAT_WRAPPED format/*,
	DirectXTexSharp::ScratchImage^ image*/) {

	DirectX::ScratchImage image;
	auto result = DirectX::Decompress(
		*cImage->get_instance(),
		static_cast<__dxgiformat_h__::DXGI_FORMAT> (format),
		image	);

	Marshal::ThrowExceptionForHR(result);

	return gcnew DirectXTexSharp::ScratchImage(image);
};






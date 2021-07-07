#include "DirectXTex.h"

#include "DirectXTexSharpLib.h"

#include <msclr/marshal.h>
#include <msclr/marshal_cppstd.h>

#include "Image.h"
#include "ScratchImage.h"

using namespace DirectXTexSharp;

//HRESULT __cdecl GetMetadataFromDDSFile(
//	_In_z_ const wchar_t* szFile,
//	_In_ DDS_FLAGS flags,
//	_Out_ TexMetadata& metadata) noexcept;
long DirectXTexSharp::DirectXTexSharpLib::GetMetadataFromDDSFile(
	System::String^ szFile, 
	DirectXTexSharp::DDSFLAGS flags,
	DirectXTexSharp::TexMetadata^ metadata) {
	
	auto context = gcnew msclr::interop::marshal_context();
	const auto result = context->marshal_as<const wchar_t*>(szFile);
	
	return DirectX::GetMetadataFromDDSFile(
		result, 
		static_cast<DirectX::DDS_FLAGS> (flags), 
		*metadata->GetInstance());
}

//HRESULT __cdecl GetMetadataFromDDSMemory(
//	_In_reads_bytes_(size) const void* pSource, _In_ size_t size,
//	_In_ DDS_FLAGS flags,
//	_Out_ TexMetadata& metadata) noexcept;
long DirectXTexSharp::DirectXTexSharpLib::GetMetadataFromDDSMemory(
	IntPtr^ pSource,
	const int size,
	DirectXTexSharp::DDSFLAGS flags,
	DirectXTexSharp::TexMetadata^ metadata) {
	return DirectX::GetMetadataFromDDSMemory(
		static_cast<void*>(*pSource),
		size,
		static_cast<DirectX::DDS_FLAGS> (flags),
		*metadata->GetInstance());
}

//HRESULT __cdecl Convert(
//	_In_ const Image& srcImage, _In_ DXGI_FORMAT format, _In_ TEX_FILTER_FLAGS filter, _In_ float threshold,
//	_Out_ ScratchImage& image) noexcept;
long DirectXTexSharp::DirectXTexSharpLib::Convert(
	DirectXTexSharp::Image^ srcImage,
	DirectXTexSharp::DXGI_FORMAT_WRAPPED format,
	DirectXTexSharp::TEX_FILTER_FLAGS filter,
	const float threshold,
	DirectXTexSharp::ScratchImage^ image) {
	return DirectX::Convert(
		*srcImage->GetInstance(),
		static_cast<__dxgiformat_h__::DXGI_FORMAT> (format),
		static_cast<DirectX::TEX_FILTER_FLAGS> (filter),
		threshold,
		*image->GetInstance());

}

/*HRESULT __cdecl LoadFromDDSMemory(
_In_reads_bytes_(size) const void* pSource, _In_ size_t size,
_In_ DDS_FLAGS flags,
_Out_opt_ TexMetadata * metadata, _Out_ ScratchImage & image) noexcept;*/
long DirectXTexSharp::DirectXTexSharpLib::LoadFromDDSMemory(
	IntPtr ^ pSource,
	const int size,
	DirectXTexSharp::DDSFLAGS flags,
	DirectXTexSharp::TexMetadata^ metadata,
	DirectXTexSharp::ScratchImage^ image) {
	return DirectX::LoadFromDDSMemory(
		static_cast<void*>(*pSource),
		size,
		static_cast<DirectX::DDS_FLAGS> (flags),
		metadata->GetInstance(),
		*image->GetInstance());
}

//HRESULT __cdecl SaveToTGAMemory(_In_ const Image & image,
//	_In_ TGA_FLAGS flags,
//	_Out_ Blob & blob, _In_opt_ const TexMetadata * metadata = nullptr) noexcept;
long DirectXTexSharp::DirectXTexSharpLib::SaveToTGAFile(
	Image^ image,
	System::String^ szFile,
	DirectXTexSharp::TexMetadata^ metadata) {

	auto context = gcnew msclr::interop::marshal_context();
	const auto result = context->marshal_as<const wchar_t*>(szFile);
	
	return DirectX::SaveToTGAFile(
		*image->GetInstance(),
		result,
		metadata->GetInstance());
}


//HRESULT __cdecl Decompress(_In_ const Image& cImage, _In_ DXGI_FORMAT format, _Out_ ScratchImage& image) noexcept;
long DirectXTexSharp::DirectXTexSharpLib::Decompress(
	Image^ cImage,
	DirectXTexSharp::DXGI_FORMAT_WRAPPED format,
	DirectXTexSharp::ScratchImage^ image) {
	return DirectX::Decompress(
		*cImage->GetInstance(),
		static_cast<__dxgiformat_h__::DXGI_FORMAT> (format),
		*image->GetInstance()
		);
};
	





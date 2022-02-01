#include "DirectXTexSharpLib.h"

#include <msclr/marshal.h>
#include <msclr/marshal_cppstd.h>



using namespace DirectXTexSharp;

//---------------------------------------------------------------------------------
// Texture metadata

//HRESULT __cdecl GetMetadataFromDDSFile(
//	_In_z_ const wchar_t* szFile,
//	_In_ DDS_FLAGS flags,
//	_Out_ TexMetadata& metadata) noexcept;
DirectXTexSharp::TexMetadata^ DirectXTexSharp::Metadata::GetMetadataFromDDSFile(
	System::String^ szFile,
	DirectXTexSharp::DDSFLAGS flags) {
	
	msclr::interop::marshal_context context;

	DirectX::TexMetadata metadata;
	const auto result = DirectX::GetMetadataFromDDSFile(
		context.marshal_as<const wchar_t*>(szFile),
		static_cast<DirectX::DDS_FLAGS> (flags),
		metadata);

	System::Runtime::InteropServices::Marshal::ThrowExceptionForHR(result);

	return gcnew DirectXTexSharp::TexMetadata(metadata);
}

//HRESULT __cdecl GetMetadataFromDDSMemory(
//	_In_reads_bytes_(size) const void* pSource, _In_ size_t size,
//	_In_ DDS_FLAGS flags,
//	_Out_ TexMetadata& metadata) noexcept;
DirectXTexSharp::TexMetadata^ DirectXTexSharp::Metadata::GetMetadataFromDDSMemory(
	byte* pSource,
	const int size,
	DirectXTexSharp::DDSFLAGS flags) {

	DirectX::TexMetadata metadata;
	const auto result = DirectX::GetMetadataFromDDSMemory(
        pSource,
		size,
		static_cast<DirectX::DDS_FLAGS> (flags),
		metadata);

	System::Runtime::InteropServices::Marshal::ThrowExceptionForHR(result);

	return gcnew DirectXTexSharp::TexMetadata(metadata);
}

//HRESULT __cdecl GetMetadataFromTGAFile(
//    _In_z_ const wchar_t* szFile,
//    _In_ TGA_FLAGS flags,
//    _Out_ TexMetadata& metadata) noexcept;
DirectXTexSharp::TexMetadata^ DirectXTexSharp::Metadata::GetMetadataFromTGAFile(
	System::String^ szFile,
	DirectXTexSharp::TGA_FLAGS flags) {
	
	msclr::interop::marshal_context context;

	DirectX::TexMetadata metadata;
	const auto result = DirectX::GetMetadataFromTGAFile(
		context.marshal_as<const wchar_t*>(szFile),
		static_cast<DirectX::TGA_FLAGS> (flags),
		metadata);

	System::Runtime::InteropServices::Marshal::ThrowExceptionForHR(result);

	return gcnew DirectXTexSharp::TexMetadata(metadata);
}

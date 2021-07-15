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

	auto context = gcnew msclr::interop::marshal_context();
	const auto wchar = context->marshal_as<const wchar_t*>(szFile);

	DirectX::TexMetadata metadata;
	const auto result = DirectX::GetMetadataFromDDSFile(
		wchar,
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
	System::IntPtr^ pSource,
	const int size,
	DirectXTexSharp::DDSFLAGS flags) {

	DirectX::TexMetadata metadata;
	const auto result = DirectX::GetMetadataFromDDSMemory(
		static_cast<void*>(*pSource),
		size,
		static_cast<DirectX::DDS_FLAGS> (flags),
		metadata);

	System::Runtime::InteropServices::Marshal::ThrowExceptionForHR(result);

	return gcnew DirectXTexSharp::TexMetadata(metadata);
	
	
}
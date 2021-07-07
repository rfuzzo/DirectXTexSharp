#include "DirectXTex.h"

#include "DirectXTexSharpLib.h"

#include <msclr/marshal.h>
#include <msclr/marshal_cppstd.h>

#include "DirectXTexEnums.h"
#include "Image.h"
#include "ScratchImage.h"

using namespace DirectXTexSharp;

//---------------------------------------------------------------------------------
// Texture metadata

//HRESULT __cdecl GetMetadataFromDDSFile(
//	_In_z_ const wchar_t* szFile,
//	_In_ DDS_FLAGS flags,
//	_Out_ TexMetadata& metadata) noexcept;
long DirectXTexSharp::Metadata::GetMetadataFromDDSFile(
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
long DirectXTexSharp::Metadata::GetMetadataFromDDSMemory(
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
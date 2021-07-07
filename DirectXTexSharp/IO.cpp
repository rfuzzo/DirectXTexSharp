#include "DirectXTex.h"

#include "DirectXTexSharpLib.h"

#include <msclr/marshal.h>
#include <msclr/marshal_cppstd.h>

#include "DirectXTexEnums.h"
#include "Image.h"
#include "ScratchImage.h"

using namespace DirectXTexSharp;

//---------------------------------------------------------------------------------
// Image I/O

/*HRESULT __cdecl LoadFromDDSMemory(
_In_reads_bytes_(size) const void* pSource, _In_ size_t size,
_In_ DDS_FLAGS flags,
_Out_opt_ TexMetadata * metadata, _Out_ ScratchImage & image) noexcept;*/
long DirectXTexSharp::IO::LoadFromDDSMemory(
	IntPtr^ pSource,
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
long DirectXTexSharp::IO::SaveToTGAFile(
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

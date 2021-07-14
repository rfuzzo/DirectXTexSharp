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
DirectXTexSharp::ScratchImage^ DirectXTexSharp::IO::LoadFromDDSMemory(
	IntPtr^ pSource,
	const int size,
	DirectXTexSharp::DDSFLAGS flags,
	DirectXTexSharp::TexMetadata^ metadata) {

	DirectX::ScratchImage image;
	auto native = &metadata->ToNative();
	auto result = DirectX::LoadFromDDSMemory(
		static_cast<void*>(*pSource),
		size,
		static_cast<DirectX::DDS_FLAGS> (flags),
		native,
		image);

	Marshal::ThrowExceptionForHR(result);

	return gcnew DirectXTexSharp::ScratchImage(/*image*/);
}


//HRESULT __cdecl SaveToTGAMemory(_In_ const Image & image,
//	_In_ TGA_FLAGS flags,
//	_Out_ Blob & blob, _In_opt_ const TexMetadata * metadata = nullptr) noexcept;
long DirectXTexSharp::IO::SaveToTGAFile(
	Image^ image,
	System::String^ szFile,
	DirectXTexSharp::TexMetadata^ metadata) {

	auto context = gcnew msclr::interop::marshal_context();
	const auto wchar = context->marshal_as<const wchar_t*>(szFile);

	auto native = &metadata->ToNative();
	auto result = DirectX::SaveToTGAFile(
		*image->GetInstance(),
		wchar,
		native);

	Marshal::ThrowExceptionForHR(result);

	return 0;
}

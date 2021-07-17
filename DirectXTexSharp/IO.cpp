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
	byte* pSource,
	const int size,
	DirectXTexSharp::DDSFLAGS flags,
	DirectXTexSharp::TexMetadata^ metadata) {

	DirectX::ScratchImage image;

	const auto final_metadata = metadata != nullptr ? metadata->get_instance() : nullptr;

	auto result = DirectX::LoadFromDDSMemory(
		pSource,
		size,
		static_cast<DirectX::DDS_FLAGS> (flags),
		final_metadata,
		image);

	System::Runtime::InteropServices::Marshal::ThrowExceptionForHR(result);

	return gcnew DirectXTexSharp::ScratchImage(image);
}

//HRESULT __cdecl SaveToTGAMemory(_In_ const Image & image,
//	_In_ TGA_FLAGS flags,
//	_Out_ Blob & blob, _In_opt_ const TexMetadata * metadata = nullptr) noexcept;
void DirectXTexSharp::IO::SaveToTGAFile(
	Image^ srcImage,
	System::String^ szFile,
	DirectXTexSharp::TexMetadata^ metadata) {

	msclr::interop::marshal_context context;

	const auto final_metadata = metadata != nullptr ? metadata->get_instance() : nullptr;
	
	const auto result = DirectX::SaveToTGAFile(
		*srcImage->get_instance(),
		context.marshal_as<const wchar_t*>(szFile),
		final_metadata);

	System::Runtime::InteropServices::Marshal::ThrowExceptionForHR(result);
}

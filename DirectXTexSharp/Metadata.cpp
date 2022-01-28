#if (_MANAGED == 1) || (_M_CEE == 1)
#include "DirectXTexSharpLib.h"
#include <msclr/marshal.h>
#include <msclr/marshal_cppstd.h>
#include "DXGI_FORMAT.h"
#else
#include <comdef.h>  // Declares _com_error
#include "texconv.h"
#include "../DirectXTex/DirectXTex/DirectXTex.h"
#endif

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

using namespace DirectXTexSharp;

//---------------------------------------------------------------------------------
// Texture metadata

//HRESULT __cdecl GetMetadataFromDDSFile(
//	_In_z_ const wchar_t* szFile,
//	_In_ DDS_FLAGS flags,
//	_Out_ TexMetadata& metadata) noexcept;
DirectX::TexMetadata DirectXTexSharp::Metadata::GetMetadataFromDDSFile(
#if (_MANAGED == 1) || (_M_CEE == 1)
        System::String^ szFile,
#else
        const wchar_t* szFile,
#endif
	DirectXTexSharp::DDSFLAGS flags) {

#if (_MANAGED == 1) || (_M_CEE == 1)
    msclr::interop::marshal_context context;
#endif

	DirectX::TexMetadata metadata;
	const auto result = DirectX::GetMetadataFromDDSFile(
#if (_MANAGED == 1) || (_M_CEE == 1)
            context.marshal_as<const wchar_t*>(szFile),
#else
            szFile,
#endif
		static_cast<DirectX::DDS_FLAGS> (flags),
		metadata);

    throw_or_clr(result);

	return metadata;
}

//HRESULT __cdecl GetMetadataFromDDSMemory(
//	_In_reads_bytes_(size) const void* pSource, _In_ size_t size,
//	_In_ DDS_FLAGS flags,
//	_Out_ TexMetadata& metadata) noexcept;
DirectX::TexMetadata DirectXTexSharp::Metadata::GetMetadataFromDDSMemory(
	byte* pSource,
	const int size,
	DirectXTexSharp::DDSFLAGS flags) {

	DirectX::TexMetadata metadata;
	const auto result = DirectX::GetMetadataFromDDSMemory(
        pSource,
		size,
		static_cast<DirectX::DDS_FLAGS> (flags),
		metadata);

    throw_or_clr(result);

	return metadata;
}

//HRESULT __cdecl GetMetadataFromTGAFile(
//    _In_z_ const wchar_t* szFile,
//    _In_ TGA_FLAGS flags,
//    _Out_ TexMetadata& metadata) noexcept;
DirectX::TexMetadata DirectXTexSharp::Metadata::GetMetadataFromTGAFile(
#if (_MANAGED == 1) || (_M_CEE == 1)
        System::String^ szFile,
#else
        const wchar_t* szFile,
#endif
	DirectXTexSharp::TGA_FLAGS flags) {

#if (_MANAGED == 1) || (_M_CEE == 1)
    msclr::interop::marshal_context context;
#endif

	DirectX::TexMetadata metadata;
	const auto result = DirectX::GetMetadataFromTGAFile(
#if (_MANAGED == 1) || (_M_CEE == 1)
            context.marshal_as<const wchar_t*>(szFile),
#else
            szFile,
#endif
		static_cast<DirectX::TGA_FLAGS> (flags),
		metadata);

    throw_or_clr(result);

	return metadata;
}

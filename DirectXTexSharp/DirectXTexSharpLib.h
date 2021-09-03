#pragma once

#include "DXGI_FORMAT.h"
#include "Image.h"
#include "ScratchImage.h"
#include "TexMetadata.h"
#include <memory>

using namespace System;
using namespace System::Runtime::InteropServices;

namespace DirectXTexSharp {

	public ref class Texconv sealed
	{
	public:
		static int ConvertAndSaveDdsImage(
			byte* bytePtr,
			int len,
			System::String^ szFile,
			DirectXTexSharp::ESaveFileTypes filetype,
			bool vflip, 
			bool hflip);

		static array<System::Byte>^ ConvertFromDdsArray(
			byte* bytePtr,
			int len,
			DirectXTexSharp::ESaveFileTypes filetype,
			bool vflip,
			bool hflip);

		static array<System::Byte>^ ConvertToDdsArray(
			byte* bytePtr,
			int len,
			DirectXTexSharp::ESaveFileTypes filetype,
			DirectXTexSharp::DXGI_FORMAT_WRAPPED format,
			bool vflip,
			bool hflip);

	private:
		static std::unique_ptr<DirectX::ScratchImage>  ConvertFromDdsMemory(
			byte* bytePtr,
			int len,
			DirectXTexSharp::ESaveFileTypes filetype,
			bool vflip,
			bool hflip);

		static DirectX::Blob  ConvertToDdsMemory(
			byte* bytePtr,
			int len,
			DirectXTexSharp::ESaveFileTypes filetype,
			DXGI_FORMAT format,
			bool vflip,
			bool hflip);

	};

    public ref class Format sealed
    {
    public:
        /*HRESULT __cdecl ComputePitch(
            _In_ DXGI_FORMAT fmt, _In_ size_t width, _In_ size_t height,
            _Out_ size_t& rowPitch, _Out_ size_t& slicePitch, _In_ CP_FLAGS flags = CP_FLAGS_NONE) noexcept;*/
        /*static void ComputePitch(
            DirectXTexSharp::DXGI_FORMAT_WRAPPED format,
            const long width,
            const long height,
            [System::Runtime::InteropServices::OutAttribute] long% rowPitch,
            [System::Runtime::InteropServices::OutAttribute] long% slicePitch
        );*/

        static long ComputeRowPitch(
            DirectXTexSharp::DXGI_FORMAT_WRAPPED format,
            const long width,
            const long height);

        static long ComputeSlicePitch(
            DirectXTexSharp::DXGI_FORMAT_WRAPPED format,
            const long width,
            const long height);

        static long BitsPerPixel(DirectXTexSharp::DXGI_FORMAT_WRAPPED format);
    };

	public ref class Metadata sealed
	{
	public:
		static DirectXTexSharp::TexMetadata^ GetMetadataFromDDSFile(
			System::String^ szFile,
			DirectXTexSharp::DDSFLAGS flags);

        static DirectXTexSharp::TexMetadata^ GetMetadataFromTGAFile(
            System::String^ szFile,
            DirectXTexSharp::TGA_FLAGS flags);

		static DirectXTexSharp::TexMetadata^ GetMetadataFromDDSMemory(
            byte* pSource,
			int size,
			DirectXTexSharp::DDSFLAGS flags);
	};

	public ref class IO sealed
	{
	public:
		static DirectXTexSharp::ScratchImage^ LoadFromDDSMemory(
			byte* pSource,
			const int size,
			DirectXTexSharp::DDSFLAGS flags,
			DirectXTexSharp::TexMetadata^ metadata);

		
		static void SaveToTGAFile(
			DirectXTexSharp::Image^ srcImage,
			System::String^ szFile,
			DirectXTexSharp::TexMetadata^ metadata);

	};
	
	public ref class Conversion
	{
	public:
	
        static DirectXTexSharp::ScratchImage^ Convert(
            DirectXTexSharp::Image^ srcImage,
			DirectXTexSharp::DXGI_FORMAT_WRAPPED format,
            DirectXTexSharp::TEX_FILTER_FLAGS filter,
            float threshold);

     
		static DirectXTexSharp::ScratchImage^ Decompress(
			Image^ cImage,
			DirectXTexSharp::DXGI_FORMAT_WRAPPED format);
	
	};

    
	
}

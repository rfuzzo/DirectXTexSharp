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

		static array<System::Byte>^ ConvertDdsImageToArray(
			byte* bytePtr,
			int len,
			DirectXTexSharp::ESaveFileTypes filetype,
			bool vflip,
			bool hflip);

	private:
		static void DirectXTexSharp::Texconv::GetWicPropsJpg(IPropertyBag2* props);
		static void DirectXTexSharp::Texconv::GetWicPropsTiff(IPropertyBag2* props);

		static std::unique_ptr<DirectX::ScratchImage>  ConvertDdsMemory(
			byte* bytePtr,
			int len,
			DirectXTexSharp::ESaveFileTypes filetype,
			bool vflip,
			bool hflip);

	};


	public ref class Metadata sealed
	{
	public:
		static DirectXTexSharp::TexMetadata^ GetMetadataFromDDSFile(
			System::String^ szFile,
			DirectXTexSharp::DDSFLAGS flags);

		static DirectXTexSharp::TexMetadata^ GetMetadataFromDDSMemory(
			IntPtr^ pSource,
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

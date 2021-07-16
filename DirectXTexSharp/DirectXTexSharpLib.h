#pragma once

#include "DXGI_FORMAT.h"
#include "Image.h"
#include "ScratchImage.h"
#include "TexMetadata.h"

using namespace System;
using namespace System::Runtime::InteropServices;

namespace DirectXTexSharp {

	public ref class Texcconv sealed
	{
	public:
		static void ConvertDdsImage(
			DirectXTexSharp::ScratchImage^ srcImage,
			System::String^ szFile,
			DirectXTexSharp::ESaveFileTypes filetype,
			bool vflip, 
			bool hflip);

		static void DirectXTexSharp::Texcconv::GetWicPropsJpg(IPropertyBag2* props);
		static void DirectXTexSharp::Texcconv::GetWicPropsTiff(IPropertyBag2* props);

	};


	public ref class Metadata sealed
	{
	public:
		static DirectXTexSharp::TexMetadata^ GetMetadataFromDDSFile(
			System::String^ szFile,
			DirectXTexSharp::DDSFLAGS flags/*,
			[Out] DirectXTexSharp::TexMetadata^ metadata*/);

		static DirectXTexSharp::TexMetadata^ GetMetadataFromDDSMemory(
			IntPtr^ pSource,
			int size,
			DirectXTexSharp::DDSFLAGS flags/*,
			DirectXTexSharp::TexMetadata^ metadata*/);
	};

	public ref class IO sealed
	{
	public:
		static DirectXTexSharp::ScratchImage^ LoadFromDDSMemory(
			IntPtr^ pSource,
			const int size,
			DirectXTexSharp::DDSFLAGS flags,
			DirectXTexSharp::TexMetadata^ metadata/*,
			DirectXTexSharp::ScratchImage^ image*/);

		static DirectXTexSharp::ScratchImage^ LoadFromDDSMemory(
			byte* pSource,
			const int size,
			DirectXTexSharp::DDSFLAGS flags,
			DirectXTexSharp::TexMetadata^ metadata/*,
			DirectXTexSharp::ScratchImage^ image*/);

		
		static void SaveToTGAFile(
			DirectXTexSharp::Image^ srcImage,
			System::String^ szFile,
			/*[Out]*/ DirectXTexSharp::TexMetadata^ metadata);

	};
	
	public ref class Conversion
	{
	public:
	
        static DirectXTexSharp::ScratchImage^ Convert(
            DirectXTexSharp::Image^ srcImage,
			DirectXTexSharp::DXGI_FORMAT_WRAPPED format,
            DirectXTexSharp::TEX_FILTER_FLAGS filter,
            float threshold/*,
            DirectXTexSharp::ScratchImage^ image*/);

     
		static DirectXTexSharp::ScratchImage^ Decompress(
			Image^ cImage,
			DirectXTexSharp::DXGI_FORMAT_WRAPPED format/*,
			DirectXTexSharp::ScratchImage^ image*/);
	
	};

    
	
}

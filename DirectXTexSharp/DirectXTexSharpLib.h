#pragma once

#include "DDSFLAGS.h"
#include "DXGI_FORMAT.h"
#include "Image.h"
#include "ScratchImage.h"
#include "TexMetadata.h"
#include "TEX_FILTER_FLAGS.h"

using namespace System;
using namespace System::Runtime::InteropServices;

namespace DirectXTexSharp {

	public ref class DirectXTexSharpLib
	{
		public:
			static long GetMetadataFromDDSFile(
				System::String^ szFile,
                DirectXTexSharp::DDSFLAGS flags,
				/*[Out]*/ DirectXTexSharp::TexMetadata^ metadata);
		
            static long GetMetadataFromDDSMemory(
                IntPtr^ pSource,
                int size,
                DirectXTexSharp::DDSFLAGS flags,
                DirectXTexSharp::TexMetadata^ metadata);
		
            static long Convert(
                DirectXTexSharp::Image^ srcImage,
				DirectXTexSharp::DXGI_FORMAT_WRAPPED format,
                DirectXTexSharp::TEX_FILTER_FLAGS filter,
                float threshold,
                DirectXTexSharp::ScratchImage^ image);

            static long LoadFromDDSMemory(
				IntPtr^ pSource,
				const int size,
				DirectXTexSharp::DDSFLAGS flags,
				DirectXTexSharp::TexMetadata^ metadata,
				DirectXTexSharp::ScratchImage^ image);

			static long SaveToTGAFile(
				DirectXTexSharp::Image^ srcImage,
				System::String^ szFile,
				/*[Out]*/ DirectXTexSharp::TexMetadata^ metadata);

			static long DirectXTexSharp::DirectXTexSharpLib::Decompress(
				Image^ cImage,
				DirectXTexSharp::DXGI_FORMAT_WRAPPED format,
				DirectXTexSharp::ScratchImage^ image);
		
	};

    
	
}

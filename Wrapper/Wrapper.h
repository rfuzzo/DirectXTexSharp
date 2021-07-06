#pragma once
#include "ManagedObject.h"

using namespace System;
using namespace System::Runtime::InteropServices;

namespace Wrapper {
	
    public enum class DDSFLAGS : unsigned long
    {
        DDS_FLAGS_NONE = 0x0,

        DDS_FLAGS_LEGACY_DWORD = 0x1,
        // Assume pitch is DWORD aligned instead of BYTE aligned (used by some legacy DDS files)

        DDS_FLAGS_NO_LEGACY_EXPANSION = 0x2,
        // Do not implicitly convert legacy formats that result in larger pixel sizes (24 bpp, 3:3:2, A8L8, A4L4, P8, A8P8)

        DDS_FLAGS_NO_R10B10G10A2_FIXUP = 0x4,
        // Do not use work-around for long-standing D3DX DDS file format issue which reversed the 10:10:10:2 color order masks

        DDS_FLAGS_FORCE_RGB = 0x8,
        // Convert DXGI 1.1 BGR formats to DXGI_FORMAT_R8G8B8A8_UNORM to avoid use of optional WDDM 1.1 formats

        DDS_FLAGS_NO_16BPP = 0x10,
        // Conversions avoid use of 565, 5551, and 4444 formats and instead expand to 8888 to avoid use of optional WDDM 1.2 formats

        DDS_FLAGS_EXPAND_LUMINANCE = 0x20,
        // When loading legacy luminance formats expand replicating the color channels rather than leaving them packed (L8, L16, A8L8)

        DDS_FLAGS_BAD_DXTN_TAILS = 0x40,
        // Some older DXTn DDS files incorrectly handle mipchain tails for blocks smaller than 4x4

        DDS_FLAGS_FORCE_DX10_EXT = 0x10000,
        // Always use the 'DX10' header extension for DDS writer (i.e. don't try to write DX9 compatible DDS files)

        DDS_FLAGS_FORCE_DX10_EXT_MISC2 = 0x20000,
        // DDS_FLAGS_FORCE_DX10_EXT including miscFlags2 information (result may not be compatible with D3DX10 or D3DX11)

        DDS_FLAGS_FORCE_DX9_LEGACY = 0x40000,
        // Force use of legacy header for DDS writer (will fail if unable to write as such)

        DDS_FLAGS_ALLOW_LARGE_FILES = 0x1000000,
        // Enables the loader to read large dimension .dds files (i.e. greater than known hardware requirements)
    };
	
    public enum class TEX_DIMENSION
        // Subset here matches D3D10_RESOURCE_DIMENSION and D3D11_RESOURCE_DIMENSION
    {
        TEX_DIMENSION_TEXTURE1D = 2,
        TEX_DIMENSION_TEXTURE2D = 3,
        TEX_DIMENSION_TEXTURE3D = 4,
    };
	

    public ref class TexMetadata : CLI::ManagedObject<DirectX::TexMetadata>
    {
    public:
        TexMetadata();
        property int width {
        public:
            int get() {
	            return m_Instance->width;
            }
        }
    	property int height {
        public:
            int get() {
	            return m_Instance->height;
            }
        }
    	property int depth {
        public:
            int get() {
	            return m_Instance->depth;
            }
        }
    	property int arraySize {
        public:
            int get() {
	            return m_Instance->arraySize;
            }
        }
    	property int mipLevels {
        public:
            int get() {
	            return m_Instance->mipLevels;
            }
        }
    	property UINT32 miscFlags {
        public:
            UINT32 get() {
	            return m_Instance->miscFlags;
            }
        }
    	property UINT32 miscFlags2 {
        public:
            UINT32 get() {
	            return m_Instance->miscFlags2;
            }
        }
    	property DXGI_FORMAT format {
        public:
            DXGI_FORMAT get() {
	            return m_Instance->format;
            }
        }
    	property TEX_DIMENSION dimension {
        public:
	        TEX_DIMENSION get() {
	            return static_cast<TEX_DIMENSION>(m_Instance->dimension);
            }
        }
    };
	
	public ref class DirectXTexSharp
	{
		public:
			static long GetMetadataFromDDSFile(
				System::String^ szFile,
				Wrapper::DDSFLAGS flags,
				/*[Out]*/ Wrapper::TexMetadata^ metadata);
            static long GetMetadataFromDDSMemory(
                IntPtr^ pSource,
                int size,
                Wrapper::DDSFLAGS flags,
                Wrapper::TexMetadata^ metadata);
		
	};

    
	
}

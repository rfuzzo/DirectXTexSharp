#pragma once

#include "DirectXTex.h"
#include "ManagedObject.h"
#include "DirectXTexEnums.h"
#include "DXGI_FORMAT.h"

namespace DirectXTexSharp {

    /*struct TexMetadata
    {
        size_t          width;
        size_t          height;     // Should be 1 for 1D textures
        size_t          depth;      // Should be 1 for 1D or 2D textures
        size_t          arraySize;  // For cubemap, this is a multiple of 6
        size_t          mipLevels;
        uint32_t        miscFlags;
        uint32_t        miscFlags2;
        DXGI_FORMAT     format;
        TEX_DIMENSION   dimension;

        size_t __cdecl ComputeIndex(_In_ size_t mip, _In_ size_t item, _In_ size_t slice) const noexcept;
        // Returns size_t(-1) to indicate an out-of-range error

        bool __cdecl IsCubemap() const noexcept { return (miscFlags & TEX_MISC_TEXTURECUBE) != 0; }
        // Helper for miscFlags

        bool __cdecl IsPMAlpha() const noexcept { return ((miscFlags2 & TEX_MISC2_ALPHA_MODE_MASK) == TEX_ALPHA_MODE_PREMULTIPLIED) != 0; }
        void __cdecl SetAlphaMode(TEX_ALPHA_MODE mode) noexcept { miscFlags2 = (miscFlags2 & ~static_cast<uint32_t>(TEX_MISC2_ALPHA_MODE_MASK)) | static_cast<uint32_t>(mode); }
        TEX_ALPHA_MODE __cdecl GetAlphaMode() const noexcept { return static_cast<TEX_ALPHA_MODE>(miscFlags2 & TEX_MISC2_ALPHA_MODE_MASK); }
        // Helpers for miscFlags2

        bool __cdecl IsVolumemap() const noexcept { return (dimension == TEX_DIMENSION_TEXTURE3D); }
        // Helper for dimension
    };*/

    public ref class TexMetadata : CLI::ManagedObject<DirectX::TexMetadata>
    {    	
		public:
			TexMetadata();
			TexMetadata(DirectX::TexMetadata& metadata);

        public:
            bool is_cubemap() { return m_instance_->IsCubemap(); }

    	property std::size_t width {
    		public:
    		std::size_t get() {
	            return m_instance_->width;
	        }
	    }
	    property std::size_t height {
			public:
	        std::size_t get() {
	            return m_instance_->height;
	        }
	    }
	    property std::size_t depth {
			public:
	        std::size_t get() {
	            return m_instance_->depth;
	        }
	    }
	    property std::size_t arraySize {
			public:
	        std::size_t get() {
	            return m_instance_->arraySize;
	        }
	    }
	    property std::size_t mipLevels {
			public:
	        std::size_t get() {
	            return m_instance_->mipLevels;
	        }
	    }
	    property UINT32 miscFlags {
			public:
	        UINT32 get() {
	            return m_instance_->miscFlags;
	        }
	    }
	    property UINT32 miscFlags2 {
			public:
	        UINT32 get() {
	            return m_instance_->miscFlags2;
	        }
	    }
	    property DirectXTexSharp::DXGI_FORMAT_WRAPPED format {
			public:
				DirectXTexSharp::DXGI_FORMAT_WRAPPED get() {
					return static_cast<DirectXTexSharp::DXGI_FORMAT_WRAPPED>(m_instance_->format);
				}
				void set(DirectXTexSharp::DXGI_FORMAT_WRAPPED value) {
					m_instance_->format = static_cast<DXGI_FORMAT>(value);
				}
	    }
	    property DirectXTexSharp::TEX_DIMENSION dimension {
			public:
				DirectXTexSharp::TEX_DIMENSION get() {
					return static_cast<DirectXTexSharp::TEX_DIMENSION>(m_instance_->dimension);
				}
        }
    };
}

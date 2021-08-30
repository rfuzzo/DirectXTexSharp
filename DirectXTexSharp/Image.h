#pragma once

#include "ManagedObject.h"
#include "TexMetadata.h"

namespace DirectXTexSharp {

    // Bitmap image container
    /*struct Image
    {
        size_t      width;
        size_t      height;
        DXGI_FORMAT format;
        size_t      rowPitch;
        size_t      slicePitch;
        uint8_t* pixels;
    };*/

    public ref class Image : CLI::ManagedObject<DirectX::Image> {
    	public:
    	Image();
        //Image(DirectX::Image* image);
        Image(DirectX::Image& image);

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
        property DirectXTexSharp::DXGI_FORMAT_WRAPPED format {
        	public:
                DirectXTexSharp::DXGI_FORMAT_WRAPPED get() {
        		return static_cast<DirectXTexSharp::DXGI_FORMAT_WRAPPED>(m_instance_->format);
        		}
        	}
        property std::size_t rowPitch {
        	public:
        		std::size_t get() {
        			return m_instance_->rowPitch;
        		}
        	}
        property std::size_t slicePitch {
        	public:
        		std::size_t get() {
        			return m_instance_->slicePitch;
        		}
        	}
        property System::IntPtr pixels {
        	public:
	        System::IntPtr get() {
        			return static_cast<System::IntPtr>(*m_instance_->pixels);
        		}
        	}
    };

}

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
        Image(DirectX::Image* image);

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
        property DirectXTexSharp::DXGI_FORMAT_WRAPPED format {
        	public:
                DirectXTexSharp::DXGI_FORMAT_WRAPPED get() {
        		return static_cast<DirectXTexSharp::DXGI_FORMAT_WRAPPED>(m_Instance->format);
        		}
        	}
        property int rowPitch {
        	public:
        		int get() {
        			return m_Instance->rowPitch;
        		}
        	}
        property int slicePitch {
        	public:
        		int get() {
        			return m_Instance->slicePitch;
        		}
        	}
        property System::IntPtr pixels {
        	public:
	        System::IntPtr get() {
        			return static_cast<System::IntPtr>(*m_Instance->pixels);
        		}
        	}
    };

}

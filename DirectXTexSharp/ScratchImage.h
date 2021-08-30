#pragma once

#include "Image.h"

namespace DirectXTexSharp {

    /*class ScratchImage
    {
    public:
        ScratchImage() noexcept
            : m_nimages(0), m_size(0), m_metadata{}, m_image(nullptr), m_memory(nullptr) {}
        ScratchImage(ScratchImage&& moveFrom) noexcept
            : m_nimages(0), m_size(0), m_metadata{}, m_image(nullptr), m_memory(nullptr) { *this = std::move(moveFrom); }
        ~ScratchImage() { Release(); }

        ScratchImage& __cdecl operator= (ScratchImage&& moveFrom) noexcept;

        ScratchImage(const ScratchImage&) = delete;
        ScratchImage& operator=(const ScratchImage&) = delete;

        HRESULT __cdecl Initialize(_In_ const TexMetadata& mdata, _In_ CP_FLAGS flags = CP_FLAGS_NONE) noexcept;

        HRESULT __cdecl Initialize1D(_In_ DXGI_FORMAT fmt, _In_ size_t length, _In_ size_t arraySize, _In_ size_t mipLevels, _In_ CP_FLAGS flags = CP_FLAGS_NONE) noexcept;
        HRESULT __cdecl Initialize2D(_In_ DXGI_FORMAT fmt, _In_ size_t width, _In_ size_t height, _In_ size_t arraySize, _In_ size_t mipLevels, _In_ CP_FLAGS flags = CP_FLAGS_NONE) noexcept;
        HRESULT __cdecl Initialize3D(_In_ DXGI_FORMAT fmt, _In_ size_t width, _In_ size_t height, _In_ size_t depth, _In_ size_t mipLevels, _In_ CP_FLAGS flags = CP_FLAGS_NONE) noexcept;
        HRESULT __cdecl InitializeCube(_In_ DXGI_FORMAT fmt, _In_ size_t width, _In_ size_t height, _In_ size_t nCubes, _In_ size_t mipLevels, _In_ CP_FLAGS flags = CP_FLAGS_NONE) noexcept;

        HRESULT __cdecl InitializeFromImage(_In_ const Image& srcImage, _In_ bool allow1D = false, _In_ CP_FLAGS flags = CP_FLAGS_NONE) noexcept;
        HRESULT __cdecl InitializeArrayFromImages(_In_reads_(nImages) const Image* images, _In_ size_t nImages, _In_ bool allow1D = false, _In_ CP_FLAGS flags = CP_FLAGS_NONE) noexcept;
        HRESULT __cdecl InitializeCubeFromImages(_In_reads_(nImages) const Image* images, _In_ size_t nImages, _In_ CP_FLAGS flags = CP_FLAGS_NONE) noexcept;
        HRESULT __cdecl Initialize3DFromImages(_In_reads_(depth) const Image* images, _In_ size_t depth, _In_ CP_FLAGS flags = CP_FLAGS_NONE) noexcept;

        void __cdecl Release() noexcept;

        bool __cdecl OverrideFormat(_In_ DXGI_FORMAT f) noexcept;

        const TexMetadata& __cdecl GetMetadata() const noexcept { return m_metadata; }
        const Image* __cdecl GetImage(_In_ size_t mip, _In_ size_t item, _In_ size_t slice) const noexcept;

        const Image* __cdecl GetImages() const noexcept { return m_image; }
        size_t __cdecl GetImageCount() const noexcept { return m_nimages; }

        uint8_t* __cdecl GetPixels() const noexcept { return m_memory; }
        size_t __cdecl GetPixelsSize() const noexcept { return m_size; }

        bool __cdecl IsAlphaAllOpaque() const noexcept;

    private:
        size_t      m_nimages;
        size_t      m_size;
        TexMetadata m_metadata;
        Image* m_image;
        uint8_t* m_memory;
    };*/

	public ref class ScratchImage sealed : CLI::ManagedObject<DirectX::ScratchImage> {

	    public:
	        ScratchImage();
	        ScratchImage(DirectX::ScratchImage& image);
    		
	        TexMetadata^ GetMetadata();

            array<byte>^ GetPixels();
	        std::size_t GetPixelsSize();

	        Image^ GetImages();
            std::size_t GetImageCount();

	};
}

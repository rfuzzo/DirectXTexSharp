#pragma once

namespace DirectXTexSharp {
	
    public enum class TEX_DIMENSION
        // Subset here matches D3D10_RESOURCE_DIMENSION and D3D11_RESOURCE_DIMENSION
    {
        TEX_DIMENSION_TEXTURE1D = 2,
        TEX_DIMENSION_TEXTURE2D = 3,
        TEX_DIMENSION_TEXTURE3D = 4,
    };
}
#include "TexMetadata.h"

using namespace DirectXTexSharp;

TexMetadata::TexMetadata() : ManagedObject(new DirectX::TexMetadata()) {
}

//TexMetadata::TexMetadata(DirectX::TexMetadata& metadata) : ManagedObject(&metadata) { 
//	
//}

TexMetadata::TexMetadata(DirectX::TexMetadata& metadata) : ManagedObject(new DirectX::TexMetadata()) {
	*this->m_instance_ = metadata;
}

//TexMetadata::TexMetadata(){
//}
//
//TexMetadata::TexMetadata(DirectX::TexMetadata metadata){
//
//	this->width = metadata.width;
//	this->height = metadata.height;
//	this->depth = metadata.depth;
//	this->arraySize = metadata.arraySize;
//	this->mipLevels = metadata.mipLevels;
//	this->miscFlags = metadata.miscFlags;
//	this->miscFlags2 = metadata.miscFlags2;
//	this->format = static_cast<DirectXTexSharp::DXGI_FORMAT_WRAPPED>(metadata.format);
//	this->dimension = static_cast<DirectXTexSharp::TEX_DIMENSION>(metadata.dimension);
//	
//}
//
//DirectX::TexMetadata& TexMetadata::ToNative()
//{
//	DirectX::TexMetadata native;
//
//	native.width = static_cast<size_t>(this->width);
//	native.height = static_cast<size_t>(this->height);
//	native.depth = static_cast<size_t>(this->depth);
//	native.arraySize = static_cast<size_t>(this->arraySize);
//	native.mipLevels = static_cast<size_t>(this->mipLevels);
//	native.miscFlags = static_cast<uint32_t>(this->miscFlags);
//	native.miscFlags2 = static_cast<uint32_t>(this->miscFlags2);
//	native.format = static_cast<::DXGI_FORMAT>(this->format);
//	native.dimension = static_cast<DirectX::TEX_DIMENSION>(this->dimension);
//
//	return native;
//}


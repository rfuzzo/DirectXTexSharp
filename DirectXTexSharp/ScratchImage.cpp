#include "ScratchImage.h"

using namespace DirectXTexSharp;

ScratchImage::ScratchImage() : ManagedObject(new DirectX::ScratchImage()) {

}

ScratchImage::ScratchImage(DirectX::ScratchImage& image) : ManagedObject(new DirectX::ScratchImage()) {
	*this->m_instance_ = std::move(image);
}



TexMetadata^ ScratchImage::GetMetadata()
{
	DirectX::TexMetadata native_metadata = m_instance_->GetMetadata();
	return gcnew DirectXTexSharp::TexMetadata(native_metadata);
}

array<byte>^ ScratchImage::GetPixels()
{
	auto dataSize = this->GetPixelsSize();
	auto ptr = m_instance_->GetPixels();

	array<byte>^ _Data = gcnew array<byte>(int(dataSize));

	for (int i = 0; i < _Data->Length; ++i)
		_Data[i] = ptr[i];

	return _Data;
}

std::size_t ScratchImage::GetPixelsSize()
{
	return m_instance_->GetPixelsSize();
}

Image^ ScratchImage::GetImages()
{
	const DirectX::Image* native_image = m_instance_->GetImages();	
	DirectX::Image* native_image_ptr = const_cast<DirectX::Image*>(native_image);
	return gcnew DirectXTexSharp::Image(*native_image_ptr);
}

std::size_t DirectXTexSharp::ScratchImage::GetImageCount()
{
	return m_instance_->GetImageCount();
}

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

System::IntPtr^ ScratchImage::GetPixels()
{
	return static_cast<System::IntPtr>(*m_instance_->GetPixels());
}

int ScratchImage::GetPixelsSize()
{
	return m_instance_->GetPixelsSize();
}

Image^ ScratchImage::GetImages()
{
	const DirectX::Image* native_image = m_instance_->GetImages();	
	DirectX::Image* native_image_ptr = const_cast<DirectX::Image*>(native_image);
	return gcnew DirectXTexSharp::Image(*native_image_ptr);
}

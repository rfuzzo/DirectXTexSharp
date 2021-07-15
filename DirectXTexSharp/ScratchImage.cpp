#include "ScratchImage.h"

using namespace DirectXTexSharp;

//ScratchImage::ScratchImage() /*: ManagedObject(new DirectX::ScratchImage())*/ {
//
//}

ScratchImage::ScratchImage(DirectX::ScratchImage& image)
{
	

	
}


//
//TexMetadata^ ScratchImage::GetMetadata()
//{
//	DirectX::TexMetadata native_metadata = m_Instance->GetMetadata();
//	//interior_ptr<DirectX::TexMetadata> ipi = &native_metadata;
//	return gcnew DirectXTexSharp::TexMetadata(native_metadata);
//}
//
//System::IntPtr^ ScratchImage::GetPixels()
//{
//	return static_cast<System::IntPtr>(*m_Instance->GetPixels());
//}
//
//int ScratchImage::GetPixelsSize()
//{
//	return m_Instance->GetPixelsSize();
//}
//
//Image^ ScratchImage::GetImages()
//{
//	const DirectX::Image* native_image = m_Instance->GetImages();
//	DirectX::Image* native_image_ptr = const_cast<DirectX::Image*>(native_image);
//	return gcnew DirectXTexSharp::Image(native_image_ptr);
//}

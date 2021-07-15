#include "Image.h"

using namespace DirectXTexSharp;

Image::Image() : ManagedObject(new DirectX::Image()) {

}

//Image::Image(DirectX::Image* image) : ManagedObject(image) {
//
//}

Image::Image(DirectX::Image& image) : ManagedObject(new DirectX::Image()) {
	*this->m_instance_ = image;
}
	

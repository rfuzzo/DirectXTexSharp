#include "Image.h"

using namespace DirectXTexSharp;

Image::Image() : ManagedObject(new DirectX::Image()) {

}

Image::Image(DirectX::Image* image) : ManagedObject(image) {

}
	

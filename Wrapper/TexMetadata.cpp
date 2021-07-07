#include "TexMetadata.h"

using namespace DirectXTexSharp;

TexMetadata::TexMetadata() : ManagedObject(new DirectX::TexMetadata()) {
}

TexMetadata::TexMetadata(DirectX::TexMetadata* metadata) : ManagedObject(metadata) {
	
}


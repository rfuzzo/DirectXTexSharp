#include "DirectXTex.h"

#include "Wrapper.h"

#include <msclr/marshal.h>
#include <msclr/marshal_cppstd.h>


using namespace Wrapper;

long Wrapper::DirectXTexSharp::GetMetadataFromDDSFile(
	System::String^ szFile, 
	Wrapper::DDSFLAGS flags,
	Wrapper::TexMetadata^ metadata)
{
	auto context = gcnew msclr::interop::marshal_context();
	const auto result = context->marshal_as<const wchar_t*>(szFile);
	
	return DirectX::GetMetadataFromDDSFile(
		result, 
		static_cast<DirectX::DDS_FLAGS> (flags), 
		*metadata->GetInstance());
}

long Wrapper::DirectXTexSharp::GetMetadataFromDDSMemory(
	IntPtr^ pSource,
	int size,
	Wrapper::DDSFLAGS flags,
	Wrapper::TexMetadata^ metadata)
{
	return DirectX::GetMetadataFromDDSMemory(
		static_cast<void*>(*pSource),
		size,
		static_cast<DirectX::DDS_FLAGS> (flags),
		*metadata->GetInstance());
}


TexMetadata::TexMetadata() : ManagedObject(new DirectX::TexMetadata())
{
	
}



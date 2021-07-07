# DirectXTexSharp
 A c++/CLI wrapper around DirectXTex.

## Installation

All stable and some pre-release packages are available on NuGet.

You can use the following command in the Package Manager Console:
```ps
Install-Package DirectXTexNet
```

| Package | NuGet Stable | NuGet Pre-release | Downloads |
| ------- | ------------ | ----------------- | --------- |
| [DirectXTexNet](https://www.nuget.org/packages/DirectXTexNet/) | [![DirectXTexNet](https://img.shields.io/nuget/v/DirectXTexNet.svg)](https://www.nuget.org/packages/DirectXTexNet/) | [![DirectXTexNet](https://img.shields.io/nuget/vpre/DirectXTexNet.svg)](https://www.nuget.org/packages/DirectXTexNet/) | [![DirectXTexNet](https://img.shields.io/nuget/dt/DirectXTexNet.svg)](https://www.nuget.org/packages/DirectXTexNet/) |

## Usage
```
using DirectXTexNet;


[TestMethod]
public void TestGetMetadataFromDDSFile()
{
    using (var metadata = new DirectXTexNet.TexMetadata())
    {
        var flags = DirectXTexNet.DDSFLAGS.DDS_FLAGS_NONE;

        var r = DirectXTexNet.Metadata.GetMetadataFromDDSFile(ddsPath, flags, metadata);

        Assert.AreEqual(0, r);
    }
}
```


## Currently implemented DirectXTex functions:
### DXGI Format Utilities
- [ ] ComputePitch
- [ ] ComputeScanlines
- [ ] ComputeScanlines


### Texture metadata
- [x] GetMetadataFromDDSMemory
- [x] GetMetadataFromDDSFile
- [ ] GetMetadataFromHDRMemory
- [ ] GetMetadataFromHDRFile
- [ ] GetMetadataFromTGAMemory
- [ ] GetMetadataFromTGAFile

### Image I/O
#### DDS operations
- [x] LoadFromDDSMemory
- [ ] LoadFromDDSFile
- [ ] SaveToDDSMemory
- [ ] SaveToDDSMemory
- [ ] SaveToDDSFile
- [ ] SaveToDDSFile

#### HDR operations

#### TGA operations
- [ ] LoadFromTGAMemory
- [ ] LoadFromTGAFile
- [ ] SaveToTGAMemory
- [x] SaveToTGAFile

### Texture conversion, resizing, mipmap generation, and block compression
- [ ] Resize
- [ ] Resize
- [x] Convert
- [ ] Convert
- [ ] ConvertToSinglePlane
- [ ] ConvertToSinglePlane
- [ ] GenerateMipMaps
- [ ] GenerateMipMaps
- [ ] GenerateMipMaps3D
- [ ] GenerateMipMaps3D
- [ ] ScaleMipMapsAlphaForCoverage

- [ ] Compress
- [ ] Compress
- [ ] Compress
- [ ] Compress
- [x] Decompress
- [ ] Decompress

### Normal map operations
- [ ] ComputeNormalMap
- [ ] ComputeNormalMap

### Misc image operations


## Currently implemented DirectXTex classes:
### structs
- [x] TexMetadata
- [x] Image
- [ ] Rect


### classes
- [x] ScratchImage
- [ ] Blob

### enums
- [x] DDS_FLAGS
- [x] TEX_DIMENSION
- [x] TEX_FILTER_FLAGS

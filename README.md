# DirectXTexSharp
 A c++/CLI wrapper around DirectXTex and Texconv. Available as managed nuget for x64 and x86 platforms.

## Installation

All stable and some pre-release packages are available on NuGet.

You can use the following command in the Package Manager Console:
```ps
Install-Package DirectXTexSharp
```

| Package | NuGet Stable | NuGet Pre-release | Downloads |
| ------- | ------------ | ----------------- | --------- |
| [DirectXTexSharp](https://www.nuget.org/packages/DirectXTexSharp/) | [![DirectXTexSharp](https://img.shields.io/nuget/v/DirectXTexSharp.svg)](https://www.nuget.org/packages/DirectXTexSharp/) | [![DirectXTexSharp](https://img.shields.io/nuget/vpre/DirectXTexSharp.svg)](https://www.nuget.org/packages/DirectXTexSharp/) | [![DirectXTexSharp](https://img.shields.io/nuget/dt/DirectXTexSharp)](https://www.nuget.org/packages/DirectXTexSharp/) |

## Usage
```
using DirectXTexSharp;


fixed (byte* ptr = span)
{
    var outDir = Path.Combine( new FileInfo(ddsPath).Directory.FullName, "out");
    Directory.CreateDirectory(outDir);
    var fileName = Path.GetFileNameWithoutExtension(ddsPath);
    var extension = filetype.ToString().ToLower();
    var newpath = Path.Combine(outDir, $"{fileName}.{extension}");

    var len = span.Length;

    // test direct saving
    DirectXTexSharp.Texcconv.ConvertAndSaveDdsImage(ptr, len, newpath, filetype, false, false);

    // test buffer saving
    var buffer = DirectXTexSharp.Texcconv.ConvertDdsImageToArray(ptr, len, filetype, false, false);
    var newpath2 = Path.Combine(outDir, $"{fileName}.2.{extension}");
    File.WriteAllBytes(newpath2, buffer);

}
```
## Currently implemented Texconv functions:
- [ ] ConvertAndSaveDdsImage
- [ ] ConvertDdsImageToArray

- [x] Flip/Rotate
- [ ] Resize
- [ ] Swizzle
- [ ] Color rotation
- [ ] Tonemap
- [x] Convert
- [ ] Convert NormalMaps
- [ ] ColorKey/ChromaKey
- [ ] Invert Y Channel
- [ ] Reconstruct Z Channel
- [ ] Determine whether preserve alpha coverage is required
- [ ] Generate mips
- [ ] Preserve mipmap alpha coverage
- [ ] Premultiplied alpha
- [ ] Compress
- [x] Set alpha mode


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

using System;
using System.IO;
using System.Runtime.InteropServices;
using DirectX;
using DirectXTexSharp;


namespace ConsoleApp
{
    class Program
    {
        static void Main(string[] args)
        {
            var ddsPath = @"X:\cp77\TEMP\rain_normal.dds";

            var a = TestGetMetadataFromDDSFile(ddsPath);
            var b = TestGetMetadataFromDDSMemory(ddsPath);
            var c = TestConvert(ddsPath);

            
        }

        private static long TestConvert(string ddsPath)
        {
            using (var fs = new FileStream(ddsPath, FileMode.Open, FileAccess.Read))
            {
                var ms = new MemoryStream();
                fs.Seek(0, SeekOrigin.Begin);
                fs.CopyTo(ms);

                var inputBytes = ms.ToArray();
                var inputHandle = GCHandle.Alloc(inputBytes, GCHandleType.Pinned);
                var inputAddress = inputHandle.AddrOfPinnedObject();

                var flags = DDSFLAGS.DDS_FLAGS_NONE;
                using (var metadata = new TexMetadata())
                using (var scratchImage = new ScratchImage())
                {
                    // load dds

                    DirectXTexSharpLib.LoadFromDDSMemory(
                        inputAddress,
                        inputBytes.Length,
                        flags,
                        metadata,
                        scratchImage);
                    var sourceImage = scratchImage.GetImages();
                    
                    // convert to DXGI_FORMAT_R8G8B8A8_UNORM

                    var format = DXGI_FORMAT_WRAPPED.DXGI_FORMAT_R8G8B8A8_UNORM;
                    using (var newscratchImage = new ScratchImage())
                    {
                        var result_decompress = DirectXTexSharpLib.Decompress(
                            sourceImage,
                            format,
                            newscratchImage
                            );

                        var uncompressedImage = newscratchImage.GetImages();
                        var newImageMD = newscratchImage.GetMetadata();

                        // save to tga

                        var newPath = Path.ChangeExtension(ddsPath, "tga");
                        var result_save = DirectXTexSharpLib.SaveToTGAFile(
                            uncompressedImage,
                            newPath,
                            newImageMD
                        );

                        return result_save;
                    }
                }
            }
        }

        private static TexMetadata TestGetMetadataFromDDSFile(string ddsPath)
        {
            var metadata = new TexMetadata();
            var flags = DDSFLAGS.DDS_FLAGS_NONE;

            var r = DirectXTexSharpLib.GetMetadataFromDDSFile(ddsPath, flags, metadata);
            return metadata;
        }

        private static TexMetadata TestGetMetadataFromDDSMemory(string ddsPath)
        {
            var flags = DDSFLAGS.DDS_FLAGS_NONE;
            var metadata = new TexMetadata();
            using (var fs = new FileStream(ddsPath, FileMode.Open, FileAccess.Read))
            {
                var ms = new MemoryStream();
                fs.Seek(0, SeekOrigin.Begin);
                fs.CopyTo(ms);

                var inputBytes = ms.ToArray();
                var inputHandle = GCHandle.Alloc(inputBytes, GCHandleType.Pinned);
                var inputAddress = inputHandle.AddrOfPinnedObject();

                var r = DirectXTexSharpLib.GetMetadataFromDDSMemory(
                    inputAddress,
                    inputBytes.Length,
                    flags,
                    metadata
                );
            }
            return metadata;
        }
    }
}

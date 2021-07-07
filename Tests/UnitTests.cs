using System.IO;
using System.Runtime.InteropServices;
using DirectXTexSharp;
using Microsoft.VisualStudio.TestTools.UnitTesting;

namespace Tests
{
    [TestClass]
    public class UnitTests
    {



        [TestMethod]
        public void TestSaveToTGAFile()
        {
            var ddsPath = @"X:\cp77\TEMP\rain_normal.dds";
            using (var fs = new FileStream(ddsPath, FileMode.Open, FileAccess.Read))
            using (var ms = new MemoryStream())
            using (var metadata = new TexMetadata())
            using (var scratchImage = new ScratchImage())
            {
                fs.Seek(0, SeekOrigin.Begin);
                fs.CopyTo(ms);

                var inputBytes = ms.ToArray();
                var inputHandle = GCHandle.Alloc(inputBytes, GCHandleType.Pinned);
                var inputAddress = inputHandle.AddrOfPinnedObject();

                var flags = DDSFLAGS.DDS_FLAGS_NONE;

                // load dds

                var resultLoad = DirectXTexSharpLib.LoadFromDDSMemory(
                    inputAddress,
                    inputBytes.Length,
                    flags,
                    metadata,
                    scratchImage);

                Assert.AreEqual(0, resultLoad);

                var sourceImage = scratchImage.GetImages();

                // convert to DXGI_FORMAT_R8G8B8A8_UNORM

                var format = DXGI_FORMAT_WRAPPED.DXGI_FORMAT_R8G8B8A8_UNORM;
                using (var newscratchImage = new ScratchImage())
                {
                    var resultDecompress = DirectXTexSharpLib.Decompress(
                        sourceImage,
                        format,
                        newscratchImage
                    );

                    Assert.AreEqual(0, resultDecompress);

                    var uncompressedImage = newscratchImage.GetImages();
                    var newImageMD = newscratchImage.GetMetadata();

                    // save to tga

                    var newPath = Path.ChangeExtension(ddsPath, "tga");
                    var resultSave = DirectXTexSharpLib.SaveToTGAFile(
                        uncompressedImage,
                        newPath,
                        newImageMD);

                    Assert.AreEqual(0, resultSave);
                    
                }
            }
        }

        [TestMethod]
        public void TestDecompress()
        {
            var ddsPath = @"X:\cp77\TEMP\rain_normal.dds";
            using (var fs = new FileStream(ddsPath, FileMode.Open, FileAccess.Read))
            using (var ms = new MemoryStream())
            using (var metadata = new TexMetadata())
            using (var scratchImage = new ScratchImage())
            {
                fs.Seek(0, SeekOrigin.Begin);
                fs.CopyTo(ms);

                var inputBytes = ms.ToArray();
                var inputHandle = GCHandle.Alloc(inputBytes, GCHandleType.Pinned);
                var inputAddress = inputHandle.AddrOfPinnedObject();

                var flags = DDSFLAGS.DDS_FLAGS_NONE;

                // load dds

                var resultLoad = DirectXTexSharpLib.LoadFromDDSMemory(
                    inputAddress,
                    inputBytes.Length,
                    flags,
                    metadata,
                    scratchImage);

                Assert.AreEqual(0, resultLoad);

                var sourceImage = scratchImage.GetImages();

                // convert to DXGI_FORMAT_R8G8B8A8_UNORM

                var format = DXGI_FORMAT_WRAPPED.DXGI_FORMAT_R8G8B8A8_UNORM;
                using (var newscratchImage = new ScratchImage())
                {
                    var r = DirectXTexSharpLib.Decompress(
                        sourceImage,
                        format,
                        newscratchImage
                    );

                    Assert.AreEqual(0, r);
                }
            }
        }

        [TestMethod]
        public void TestLoadFromDDSMemory()
        {
            var ddsPath = @"X:\cp77\TEMP\rain_normal.dds";

            using (var fs = new FileStream(ddsPath, FileMode.Open, FileAccess.Read))
            using (var metadata = new TexMetadata())
            using (var scratchImage = new ScratchImage())
            {
                var ms = new MemoryStream();
                fs.Seek(0, SeekOrigin.Begin);
                fs.CopyTo(ms);

                var inputBytes = ms.ToArray();
                var inputHandle = GCHandle.Alloc(inputBytes, GCHandleType.Pinned);
                var inputAddress = inputHandle.AddrOfPinnedObject();

                var flags = DDSFLAGS.DDS_FLAGS_NONE;

                var r = DirectXTexSharpLib.LoadFromDDSMemory(
                    inputAddress,
                    inputBytes.Length,
                    flags,
                    metadata,
                    scratchImage);

                Assert.AreEqual(0, r);
            }
        }

        [TestMethod]
        public void TestGetMetadataFromDDSFile()
        {
            var ddsPath = @"X:\cp77\TEMP\rain_normal.dds";
            using (var metadata = new TexMetadata())
            {
                var flags = DDSFLAGS.DDS_FLAGS_NONE;

                var r = DirectXTexSharpLib.GetMetadataFromDDSFile(ddsPath, flags, metadata);

                Assert.AreEqual(0, r);
            }
        }

        [TestMethod]
        public void TestGetMetadataFromDDSMemory()
        {
            var ddsPath = @"X:\cp77\TEMP\rain_normal.dds";

            using (var metadata = new TexMetadata())
            using (var fs = new FileStream(ddsPath, FileMode.Open, FileAccess.Read))
            {
                var ms = new MemoryStream();
                fs.Seek(0, SeekOrigin.Begin);
                fs.CopyTo(ms);

                var inputBytes = ms.ToArray();
                var inputHandle = GCHandle.Alloc(inputBytes, GCHandleType.Pinned);
                var inputAddress = inputHandle.AddrOfPinnedObject();

                var flags = DDSFLAGS.DDS_FLAGS_NONE;

                var r = DirectXTexSharpLib.GetMetadataFromDDSMemory(
                    inputAddress,
                    inputBytes.Length,
                    flags,
                    metadata
                );

                Assert.AreEqual(0, r);
            }
        }
    }
}

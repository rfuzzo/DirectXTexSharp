using System.IO;
using System.Runtime.InteropServices;
using DirectXTexSharp;
using Microsoft.VisualStudio.TestTools.UnitTesting;

namespace Tests
{
    [TestClass]
    public class ConversionTests
    {
        const string ddsPath = @"X:\cp77\TEMP\rain_normal.dds";

        [TestMethod]
        public void TestDecompress()
        {
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

                var resultLoad = DirectXTexSharp.IO.LoadFromDDSMemory(
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
                    var r = DirectXTexSharp.Conversion.Decompress(
                        sourceImage,
                        format,
                        newscratchImage
                    );

                    Assert.AreEqual(0, r);
                }
            }
        }
    }
}
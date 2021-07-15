using System;
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
            IntPtr inputAddress;
            int inputBytesLen = 0;

            using (var ms = new MemoryStream())
            {
                using (var fs = new FileStream(ddsPath, FileMode.Open, FileAccess.Read))
                {
                    fs.Seek(0, SeekOrigin.Begin);
                    fs.CopyTo(ms);
                }

                var inputBytes =  ms.ToArray();
                var inputHandle = GCHandle.Alloc(inputBytes, GCHandleType.Pinned);
                inputAddress = inputHandle.AddrOfPinnedObject();
                inputBytesLen = inputBytes.Length;
            }

            Assert.IsTrue(inputBytesLen > 0);

            var flags = DDSFLAGS.DDS_FLAGS_NONE;

            using (var scratchImage = DirectXTexSharp.IO.LoadFromDDSMemory(
                inputAddress,
                inputBytesLen,
                flags,
                null))
            {
                Assert.IsNotNull(scratchImage);

                var sourceImage = scratchImage.GetImages();

                // convert to DXGI_FORMAT_R8G8B8A8_UNORM

                var format = DXGI_FORMAT_WRAPPED.DXGI_FORMAT_R8G8B8A8_UNORM;
                using (var newscratchImage = DirectXTexSharp.Conversion.Decompress(
                    sourceImage,
                    format
                ))
                {
                    Assert.IsNotNull(newscratchImage);



                }
            }
        }
    }
}

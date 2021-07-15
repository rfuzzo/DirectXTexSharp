using System;
using System.IO;
using System.Runtime.InteropServices;
using DirectXTexSharp;
using Microsoft.VisualStudio.TestTools.UnitTesting;


namespace Tests
{
    [TestClass]
    public class IOTests
    {
        const string ddsPath = @"X:\cp77\TEMP\rain_normal.dds";


        [TestMethod]
        public void TestSaveToTGAFile()
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

                var inputBytes = ms.ToArray();
                var inputHandle = GCHandle.Alloc(inputBytes, GCHandleType.Pinned);
                inputAddress = inputHandle.AddrOfPinnedObject();
                inputBytesLen = inputBytes.Length;
            }

            Assert.IsTrue(inputBytesLen > 0);
            
            var flags = DDSFLAGS.DDS_FLAGS_NONE;

            //using (var metadata = new TexMetadata())
            using (var scratchImage = DirectXTexSharp.IO.LoadFromDDSMemory(
                inputAddress,
                inputBytesLen,
                flags,
                null))
            {
                Assert.IsNotNull(scratchImage);

                // convert to DXGI_FORMAT_R8G8B8A8_UNORM
                var format = DXGI_FORMAT_WRAPPED.DXGI_FORMAT_R8G8B8A8_UNORM;

                using (var sourceImage = scratchImage.GetImages())
                using (var newscratchImage = DirectXTexSharp.Conversion.Decompress(
                        sourceImage,
                        format))
                {

                    var uncompressedImage = newscratchImage.GetImages();

                    // save to tga
                    var newPath = Path.ChangeExtension(ddsPath, "tga");
                    DirectXTexSharp.IO.SaveToTGAFile(
                        uncompressedImage,
                        newPath,
                        null);
                }
            }
        }

        [TestMethod]
        public void TestLoadFromDDSMemory()
        {
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

                var image = DirectXTexSharp.IO.LoadFromDDSMemory(
                    inputAddress,
                    inputBytes.Length,
                    flags,
                    metadata);

                Assert.IsNotNull(image);

                var imageMetadata = image.GetMetadata();
                var imagePixelsSize = image.GetPixelsSize();
                var imageImages = image.GetImages();
                var imagePixels = image.GetPixels();
            }
        }




    }
}

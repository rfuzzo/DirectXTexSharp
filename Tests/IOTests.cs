//using System;
//using System.Buffers;
//using System.IO;
//using System.Runtime.InteropServices;
//using DirectXTexSharp;
//using Microsoft.VisualStudio.TestTools.UnitTesting;


//namespace Tests
//{
//    [TestClass]
//    public class IOTests
//    {
//        const string ddsPath = @"X:\cp77\TEMP\rain_normal.dds";

//        [TestMethod]
//        public unsafe void TestSaveToTgaFileSpan()
//        {
//            byte[] rentedBuffer = null;
//            try
//            {
//                int len;
//                var offset = 0;

//                using (var stream = File.OpenRead(ddsPath))
//                {
//                    len = checked((int)stream.Length);
//                    rentedBuffer = ArrayPool<byte>.Shared.Rent(len);
                    
//                    int readBytes;
//                    while (offset < len &&
//                           (readBytes = stream.Read(rentedBuffer, offset, len - offset)) > 0)
//                    {
//                        offset += readBytes;
//                    }
//                }

//                var span = new ReadOnlySpan<byte>(rentedBuffer, 0, len);

//                ProcessSpan(span);
//            }
//            finally
//            {
//                if (rentedBuffer is object)
//                    ArrayPool<byte>.Shared.Return(rentedBuffer);
//            }

//            void ProcessSpan(ReadOnlySpan<byte> span)
//            {
//                fixed (byte* ptr = span)
//                {
//                    var flags = DDSFLAGS.DDS_FLAGS_NONE;

//                    //using (var metadata = new TexMetadata())
//                    using (var scratchImage = DirectXTexSharp.IO.LoadFromDDSMemory(
//                        ptr,
//                        span.Length,
//                        flags,
//                        null))
//                    {
//                        Assert.IsNotNull(scratchImage);

//                        // convert to DXGI_FORMAT_R8G8B8A8_UNORM
//                        var format = DXGI_FORMAT_WRAPPED.DXGI_FORMAT_R8G8B8A8_UNORM;

//                        using (var sourceImage = scratchImage.GetImages())
//                        using (var newscratchImage = DirectXTexSharp.Conversion.Decompress(
//                            sourceImage,
//                            format))
//                        {

//                            var uncompressedImage = newscratchImage.GetImages();

//                            // save to tga
//                            var newPath = Path.ChangeExtension(ddsPath, "tga");
//                            DirectXTexSharp.IO.SaveToTGAFile(
//                                uncompressedImage,
//                                newPath,
//                                null);
//                        }
//                    }
//                }
//            }
//        }

//        [TestMethod]
//        public unsafe void TestLoadFromDDSMemory()
//        {
//            byte[] rentedBuffer = null;
//            try
//            {
//                int len;
//                var offset = 0;

//                using (var stream = File.OpenRead(ddsPath))
//                {
//                    len = checked((int)stream.Length);
//                    rentedBuffer = ArrayPool<byte>.Shared.Rent(len);

//                    int readBytes;
//                    while (offset < len &&
//                           (readBytes = stream.Read(rentedBuffer, offset, len - offset)) > 0)
//                    {
//                        offset += readBytes;
//                    }
//                }

//                var span = new ReadOnlySpan<byte>(rentedBuffer, 0, len);

//                ProcessSpan(span);
//            }
//            finally
//            {
//                if (rentedBuffer is object)
//                    ArrayPool<byte>.Shared.Return(rentedBuffer);
//            }

//            void ProcessSpan(ReadOnlySpan<byte> span)
//            {
//                fixed (byte* ptr = span)
//                {
//                    var flags = DDSFLAGS.DDS_FLAGS_NONE;

//                    //using (var metadata = new TexMetadata())
//                    using (var scratchImage = DirectXTexSharp.IO.LoadFromDDSMemory(
//                        ptr,
//                        span.Length,
//                        flags,
//                        null))
//                    {

//                        using (var imageMetadata = scratchImage.GetMetadata())
//                        {
//                        }
//                        var imagePixelsSize = scratchImage.GetPixelsSize();

//                        var imageImageCount = scratchImage.GetImageCount();
//                        using (var imageImages = scratchImage.GetImages())
//                        {
//                        }
//                        var imagePixels = scratchImage.GetPixels();

//                    }
//                }
//            }
//        }




//    }
//}

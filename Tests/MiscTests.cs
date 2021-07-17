using System;
using System.Buffers;
using System.IO;
using System.Runtime.InteropServices;
using DirectXTexSharp;
using Microsoft.VisualStudio.TestTools.UnitTesting;

namespace Tests
{
    [TestClass]
    public class MiscTests
    {
        const string ddsPath = @"X:\cp77\TEMP\rain_normal.dds";



        [TestMethod]
        public unsafe void TestReadFilePooled()
        {

            byte[] rentedBuffer = null;
            try
            {
                int len;
                var offset = 0;

                using (var stream = File.OpenRead(ddsPath))
                {
                    len = checked((int)stream.Length);
                    rentedBuffer = ArrayPool<byte>.Shared.Rent(len);

                    int readBytes;
                    while (offset < len &&
                           (readBytes = stream.Read(rentedBuffer, offset, len - offset)) > 0)
                    {
                        offset += readBytes;
                    }
                }

                var span = new ReadOnlySpan<byte>(rentedBuffer, 0, len);

                ProcessSpan(span);
            }
            finally
            {
                if (rentedBuffer is object)
                    ArrayPool<byte>.Shared.Return(rentedBuffer);
            }


            void ProcessSpan(ReadOnlySpan<byte> span)
            {
                fixed (byte* ptr = span)
                {
                    // load image from dds
                    var flags = DDSFLAGS.DDS_FLAGS_NONE;
                    using var scratchImage = DirectXTexSharp.IO.LoadFromDDSMemory(
                        ptr,
                        span.Length,
                        flags,
                        null);

                    //var srcimagePixels = scratchImage.GetPixels();


                    using var sourceImage = scratchImage.GetImages();

                    // 
                    var format = DXGI_FORMAT_WRAPPED.DXGI_FORMAT_R8G8B8A8_UNORM;
                    using var newscratchImage = DirectXTexSharp.Conversion.Decompress(
                        sourceImage,
                        format
                    );

                    using (var imageMetadata = newscratchImage.GetMetadata())
                    {
                    }
                    var imagePixelsSize = newscratchImage.GetPixelsSize();
                    using (var imageImages = newscratchImage.GetImages())
                    {
                    }
                    var imagePixels = newscratchImage.GetPixels();




                }
            }
        }



        [TestMethod]
        public unsafe void TestReadFile()
        {

            var buffer = new Span<byte>();
            using (var fileStream = new FileStream(ddsPath, FileMode.Open, FileAccess.Read))
            {
                int length = (int)fileStream.Length;

                buffer = new Span<byte>(new byte[length]);
                fileStream.Read(buffer);
            }

            fixed (byte* ptr = buffer)
            {
                var flags = DDSFLAGS.DDS_FLAGS_NONE;

                using var scratchImage = DirectXTexSharp.IO.LoadFromDDSMemory(
                    ptr,
                    buffer.Length,
                    flags,
                    null);
                using var sourceImage = scratchImage.GetImages();


                var format = DXGI_FORMAT_WRAPPED.DXGI_FORMAT_R8G8B8A8_UNORM;
                using var newscratchImage = DirectXTexSharp.Conversion.Decompress(
                    sourceImage,
                    format
                );

                using var newSourceImage = newscratchImage.GetImages();
            }


        }

    }
}

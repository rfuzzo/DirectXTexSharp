using System;
using System.Buffers;
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


        public delegate void SpanAction(ReadOnlySpan<byte> vs);

        public void ReadAndProcessFile(string path, SpanAction action)
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

                action(span);
            }
            finally
            {
                if (rentedBuffer is object)
                    ArrayPool<byte>.Shared.Return(rentedBuffer);
            }
        }


        [TestMethod]
        public unsafe void TestDecompress()
        {
            ReadAndProcessFile(ddsPath, ProcessSpan);

            void ProcessSpan(ReadOnlySpan<byte> span)
            {
                // get pointer to span in memory
                //fixed (byte* ptr = &MemoryMarshal.GetReference(buffer))
                fixed (byte* ptr = span)
                {
                    var flags = DDSFLAGS.DDS_FLAGS_NONE;

                    using (var scratchImage = DirectXTexSharp.IO.LoadFromDDSMemory(
                        ptr,
                        span.Length,
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

        [TestMethod]
        [DataRow(ESaveFileTypes.TGA)]
        [DataRow(ESaveFileTypes.PNG)]
        [DataRow(ESaveFileTypes.JPEG)]
        [DataRow(ESaveFileTypes.HDR)]
        [DataRow(ESaveFileTypes.BMP)]
        [DataRow(ESaveFileTypes.TIFF)]
        public unsafe void TestConvertDdsFile(ESaveFileTypes filetype)
        {
            ReadAndProcessFile(ddsPath, ProcessSpan);

            void ProcessSpan(ReadOnlySpan<byte> span)
            {
                fixed (byte* ptr = span)
                {
                    var flags = DDSFLAGS.DDS_FLAGS_NONE;

                    using (var scratchImage = DirectXTexSharp.IO.LoadFromDDSMemory(
                        ptr,
                        span.Length,
                        flags,
                        null))
                    {

                        var newpath = Path.ChangeExtension(ddsPath, filetype.ToString().ToLower());

                        DirectXTexSharp.Texcconv.ConvertDdsImage(scratchImage, newpath, filetype, false, false);

                    }
                }
            } 
        }
    }
}

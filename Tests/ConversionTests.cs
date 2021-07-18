using System;
using System.Buffers;
using System.Diagnostics;
using System.IO;
using System.Runtime.InteropServices;
using System.Threading;
using System.Threading.Tasks;
using DirectXTexSharp;
using Microsoft.VisualStudio.TestTools.UnitTesting;

namespace Tests
{
    [TestClass]
    public class ConversionTests
    {
        const string ddsPath = @"X:\cp77\TEMP\rain_normal.dds";
        const string infolder = @"X:\cp77\ASSETS\basegame_1_engine";


        public delegate void SpanAction(ReadOnlySpan<byte> vs);
        public void ReadAndProcessFile(string path, SpanAction action)
        {
            byte[] rentedBuffer = null;
            try
            {
                int len;
                var offset = 0;

                using (var stream = File.OpenRead(path))
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
                {
                    ArrayPool<byte>.Shared.Return(rentedBuffer);
                }
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
        //[DataRow(ESaveFileTypes.HDR)] // not working, disabled
        [DataRow(ESaveFileTypes.BMP)]
        [DataRow(ESaveFileTypes.TIFF)]
        public unsafe void TestConvertDdsFile(ESaveFileTypes filetype)
        {
            
            var files = Directory.GetFiles(infolder, "*.dds", SearchOption.AllDirectories);

            var succesfull = 0;

            Parallel.ForEach(files, item =>
            //foreach (var item in files)
            {
                ReadAndProcessFile(item, ProcessSpan);

                void ProcessSpan(ReadOnlySpan<byte> span)
                {
                    fixed (byte* ptr = span)
                    {
                        var len = span.Length;

                        //var buffer = DirectXTexSharp.Texconv.ConvertDdsImageToArray(ptr, len, filetype, false, false);
                        //if (buffer != null)
                        //{
                        //    succesfull++;
                        //}
                        //else
                        //{
                        //    Debug.WriteLine($"[{filetype.ToString()}] - {item}");
                        //}
                        //buffer = null;

                        var outDir = new FileInfo(item).Directory.FullName;
                        Directory.CreateDirectory(outDir);
                        var fileName = Path.GetFileNameWithoutExtension(item);
                        var extension = filetype.ToString().ToLower();
                        var newpath = Path.Combine(outDir, $"{fileName}.{extension}");

                        var hr = DirectXTexSharp.Texconv.ConvertAndSaveDdsImage(ptr, len, newpath, filetype, false, false);
                        if (hr == 0)
                        {
                            Interlocked.Increment(ref succesfull);
                        }
                        else
                        {
                            Debug.WriteLine($"[{filetype.ToString()}] - {item}");
                        }
                    }
                }

            }
            );

            Assert.AreEqual(files.Length, succesfull);
        }
    }
}

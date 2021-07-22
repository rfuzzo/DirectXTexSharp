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
        const string infolder = @".\Assets";


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
        [DataRow(@".\Assets\chicken_brown_d.dds")]
        [DataRow(@".\Assets\chicken_brown_n.dds")]
        public unsafe void TestDecompress(string ddsPath)
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
        [DataRow(ESaveFileTypes.BMP)]
        [DataRow(ESaveFileTypes.TIFF)]
        public unsafe void TestConvertFromDdsFile(ESaveFileTypes filetype)
        {
            var files = Directory.GetFiles(infolder, "*.dds", SearchOption.AllDirectories);

            var succesfull = 0;

            //Parallel.ForEach(files, item =>
            foreach (var item in files)
            {
                ReadAndProcessFile(item, ProcessSpan);

                void ProcessSpan(ReadOnlySpan<byte> span)
                {
                    fixed (byte* ptr = span)
                    {
                        var len = span.Length;

                        var buffer = DirectXTexSharp.Texconv.ConvertFromDdsArray(ptr, len, filetype, false, false);
                        if (buffer != null)
                        {
                            succesfull++;
                        }
                        else
                        {
                            Debug.WriteLine($"[{filetype.ToString()}] - {item}");
                        }
                        buffer = null;

                        //var outDir = new FileInfo(item).Directory.FullName;
                        //Directory.CreateDirectory(outDir);
                        //var fileName = Path.GetFileNameWithoutExtension(item);
                        //var extension = filetype.ToString().ToLower();
                        //var newpath = Path.Combine(outDir, $"{fileName}.{extension}");

                        //var hr = DirectXTexSharp.Texconv.ConvertAndSaveDdsImage(ptr, len, newpath, filetype, false, false);
                        //if (hr == 0)
                        //{
                        //    Interlocked.Increment(ref succesfull);
                        //}
                        //else
                        //{
                        //    Debug.WriteLine($"[{filetype.ToString()}] - {item}");
                        //}
                    }
                }

            }
            //);

            Assert.AreEqual(files.Length, succesfull);
        }

        [TestMethod]
        [DataRow(ESaveFileTypes.TGA)]
        [DataRow(ESaveFileTypes.PNG)]
        [DataRow(ESaveFileTypes.JPEG)]
        [DataRow(ESaveFileTypes.BMP)]
        [DataRow(ESaveFileTypes.TIFF)]
        public unsafe void TestConvertToDdsFile(ESaveFileTypes filetype)
        {
            var files = Directory.GetFiles(infolder, $"*.{filetype.ToString().ToLower()}", SearchOption.AllDirectories);

            var succesfull = 0;

            //Parallel.ForEach(files, item =>
            foreach (var item in files)
            {
                ReadAndProcessFile(item, ProcessSpan);

                void ProcessSpan(ReadOnlySpan<byte> span)
                {
                    fixed (byte* ptr = span)
                    {
                        var len = span.Length;
                        var format = DXGI_FORMAT_WRAPPED.DXGI_FORMAT_B8G8R8A8_UNORM;
                        

                        var buffer = DirectXTexSharp.Texconv.ConvertToDdsArray(ptr, len, filetype, format, false, false);
                        if (buffer != null && buffer.Length > 0)
                        {
                            succesfull++;
                        }
                        else
                        {
                            Debug.WriteLine($"[{filetype.ToString()}] - {item}");
                        }

                        // write to file
                        var outdir = Path.Combine( Environment.GetFolderPath(Environment.SpecialFolder.MyDocuments), "directxtexsharp");
                        Directory.CreateDirectory(outdir);
                        var test = Path.Combine(outdir, $"{Path.GetFileName(item)}.dds");
                        File.WriteAllBytes(test, buffer);


                        buffer = null;
                    }
                }

            }
            //);

            Assert.AreEqual(files.Length, succesfull);
        }
    }
}

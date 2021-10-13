using System;
using System.IO;
using System.Runtime.InteropServices;
using System.Threading;
using DirectXTexSharp;
//using DirectXTexSharp;
using Microsoft.VisualStudio.TestTools.UnitTesting;

namespace Tests
{
    [TestClass]
    public class MetadataTests
    {
        const string ddsPath = @"X:\cp77\TEMP\rain_normal.dds";


        [TestMethod]
        public void TestGetMetadataFromDDSFile()
        {
            var flags = DirectXTexSharp.DDSFLAGS.DDS_FLAGS_NONE;

            using (var metadata = DirectXTexSharp.Metadata.GetMetadataFromDDSFile(ddsPath, flags))
            {
                //Console.WriteLine("Before wait");
                //Console.WriteLine($"test: {metadata.width}");

                //Thread.Sleep(1000);

                //Console.WriteLine("After wait");
                //Console.WriteLine($"test: {metadata.width}");

                Assert.AreEqual(512, metadata.width);
            } 
        }

        [TestMethod]
        public void TestGetMetadataFromDDSMemory()
        {
            //using (var fs = new FileStream(ddsPath, FileMode.Open, FileAccess.Read))
            //{
            //    var ms = new MemoryStream();
            //    fs.Seek(0, SeekOrigin.Begin);
            //    fs.CopyTo(ms);

            //    var inputBytes = ms.ToArray();
            //    var inputHandle = GCHandle.Alloc(inputBytes, GCHandleType.Pinned);
            //    var inputAddress = inputHandle.AddrOfPinnedObject();

            //    var flags = DDSFLAGS.DDS_FLAGS_NONE;

            //    using (var metadata = DirectXTexSharp.Metadata.GetMetadataFromDDSMemory(
            //        inputAddress,
            //        inputBytes.Length,
            //        flags))
            //    {
            //        Assert.AreEqual(512, metadata.width);
            //    } 
            //}
        }
    }
}

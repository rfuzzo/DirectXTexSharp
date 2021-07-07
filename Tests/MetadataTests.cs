using System.IO;
using System.Runtime.InteropServices;
using DirectXTexSharp;
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
            using (var metadata = new DirectXTexSharp.TexMetadata())
            {
                var flags = DirectXTexSharp.DDSFLAGS.DDS_FLAGS_NONE;

                var r = DirectXTexSharp.Metadata.GetMetadataFromDDSFile(ddsPath, flags, metadata);

                Assert.AreEqual(0, r);
            }
        }

        [TestMethod]
        public void TestGetMetadataFromDDSMemory()
        {
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

                var r = DirectXTexSharp.Metadata.GetMetadataFromDDSMemory(
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

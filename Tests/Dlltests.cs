using System;
using System.Buffers;
using System.IO;
using System.Runtime.InteropServices;
using Microsoft.VisualStudio.TestTools.UnitTesting;

namespace Tests
{
    [TestClass]
    public class MiscTests
    {
        const string ddsPath = @"X:\cp77\TEMP\rain_normal.dds";


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

            

        }

    }
}

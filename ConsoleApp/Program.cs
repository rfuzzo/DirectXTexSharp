using System;
using DirectX;
using Wrapper;


namespace ConsoleApp
{
    class Program
    {
        static void Main(string[] args)
        {
            var metadata = new TexMetadata();
            var flags = DDSFLAGS.DDS_FLAGS_NONE;
            var ddsPath = @"X:\cp77\TEMP\rain_normal.dds";

            var z = DirectXTexSharp.GetMetadataFromDDSFile(ddsPath, flags, metadata);

        }
    }
}

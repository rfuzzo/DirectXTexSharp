using DirectXTexSharp;

namespace DirectXTex.Net
{
    public static class TexConv
    {
        public static int Test()
        {
            string ddsPath = @"X:\cp77\TEMP\rain_normal.dds";

            /*using*/ var metadata = new TexMetadata();
            var flags = DirectXTexSharp.DDSFLAGS.DDS_FLAGS_NONE;

            var r = DirectXTexSharp.Metadata.GetMetadataFromDDSFile(ddsPath, flags, metadata);

            return metadata.width;
        }


    }
}

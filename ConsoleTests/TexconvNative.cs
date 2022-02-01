using System;
using System.Runtime.InteropServices;
using static WolvenKit.Common.DDS.TexconvNative;

namespace WolvenKit.Common.DDS;

public static class BlobExtensions
{
    public static byte[] GetBytes(this Blob blob)
    {
        var Length = blob.Length;
        var Buffer = blob.Buffer;
        //return new ArraySegment<byte>(Buffer, 0, Length);
        var managedArray = new byte[Length];
        Marshal.Copy(Buffer, managedArray, 0, (int)Length);
        return managedArray;
    }
}

public static class TexconvNative
{

    [StructLayout(LayoutKind.Sequential)]
    public struct Blob
    {
        public IntPtr Buffer;
        public long Length;
    }


    [StructLayout(LayoutKind.Sequential)]
    public struct TexMetadata
    {
        public long width;
        public long height;     // Should be 1 for 1D textures
        public long depth;      // Should be 1 for 1D or 2D textures
        public long arraySize;  // For cubemap, this is a multiple of 6
        public long mipLevels;
        public int miscFlags;
        public int miscFlags2;
        public DXGI_FORMAT format;
        public TEX_DIMENSION dimension;
    }
    public enum ESaveFileTypes
    {
        BMP,
        JPEG,
        PNG,
        TGA,
        //HDR, //broken for some reason, disabling for now
        TIFF,
        /*WIC_CODEC_WMP,
        CODEC_HDP,
        CODEC_JXR,
        CODEC_PPM,
        CODEC_PFMm*/
    };

    public enum DDSFLAGS
    {
        DDS_FLAGS_NONE = 0x0,

        DDS_FLAGS_LEGACY_DWORD = 0x1,
        // Assume pitch is DWORD aligned instead of BYTE aligned (used by some legacy DDS files)

        DDS_FLAGS_NO_LEGACY_EXPANSION = 0x2,
        // Do not implicitly convert legacy formats that result in larger pixel sizes (24 bpp, 3:3:2, A8L8, A4L4, P8, A8P8)

        DDS_FLAGS_NO_R10B10G10A2_FIXUP = 0x4,
        // Do not use work-around for long-standing D3DX DDS file format issue which reversed the 10:10:10:2 color order masks

        DDS_FLAGS_FORCE_RGB = 0x8,
        // Convert DXGI 1.1 BGR formats to DXGI_FORMAT_R8G8B8A8_UNORM to avoid use of optional WDDM 1.1 formats

        DDS_FLAGS_NO_16BPP = 0x10,
        // Conversions avoid use of 565, 5551, and 4444 formats and instead expand to 8888 to avoid use of optional WDDM 1.2 formats

        DDS_FLAGS_EXPAND_LUMINANCE = 0x20,
        // When loading legacy luminance formats expand replicating the color channels rather than leaving them packed (L8, L16, A8L8)

        DDS_FLAGS_BAD_DXTN_TAILS = 0x40,
        // Some older DXTn DDS files incorrectly handle mipchain tails for blocks smaller than 4x4

        DDS_FLAGS_FORCE_DX10_EXT = 0x10000,
        // Always use the 'DX10' header extension for DDS writer (i.e. don't try to write DX9 compatible DDS files)

        DDS_FLAGS_FORCE_DX10_EXT_MISC2 = 0x20000,
        // DDS_FLAGS_FORCE_DX10_EXT including miscFlags2 information (result may not be compatible with D3DX10 or D3DX11)

        DDS_FLAGS_FORCE_DX9_LEGACY = 0x40000,
        // Force use of legacy header for DDS writer (will fail if unable to write as such)

        DDS_FLAGS_ALLOW_LARGE_FILES = 0x1000000,
        // Enables the loader to read large dimension .dds files (i.e. greater than known hardware requirements)
    };

    public enum TGA_FLAGS
    {
        TGA_FLAGS_NONE = 0x0,

        TGA_FLAGS_BGR = 0x1,
        // 24bpp files are returned as BGRX; 32bpp files are returned as BGRA

        TGA_FLAGS_ALLOW_ALL_ZERO_ALPHA = 0x2,
        // If the loaded image has an all zero alpha channel, normally we assume it should be opaque. This flag leaves it alone.

        TGA_FLAGS_IGNORE_SRGB = 0x10,
        // Ignores sRGB TGA 2.0 metadata if present in the file

        TGA_FLAGS_FORCE_SRGB = 0x20,
        // Writes sRGB metadata into the file reguardless of format (TGA 2.0 only)

        TGA_FLAGS_FORCE_LINEAR = 0x40,
        // Writes linear gamma metadata into the file reguardless of format (TGA 2.0 only)

        TGA_FLAGS_DEFAULT_SRGB = 0x80,
        // If no colorspace is specified in TGA 2.0 metadata, assume sRGB
    };

    [DllImport("lib/texconv.dll", EntryPoint = "ComputeRowPitch", CallingConvention = CallingConvention.StdCall)]
    private static extern long _ComputeRowPitch(DXGI_FORMAT format, long width, long height);
    public static long ComputeRowPitch(DXGI_FORMAT format, long width, long height) => _ComputeRowPitch(format, width, height);

    [DllImport("lib/texconv.dll", EntryPoint = "ComputeSlicePitch", CallingConvention = CallingConvention.StdCall)]
    private static extern long _ComputeSlicePitch(DXGI_FORMAT format, long width, long height);
    public static long ComputeSlicePitch(DXGI_FORMAT format, long width, long height) => _ComputeSlicePitch(format, width, height);

    [DllImport("lib/texconv.dll", EntryPoint = "BitsPerPixel", CallingConvention = CallingConvention.StdCall)]
    private static extern long _BitsPerPixel(DXGI_FORMAT format);
    public static long BitsPerPixel(DXGI_FORMAT format) => _BitsPerPixel(format);



    [DllImport("lib/texconv.dll", EntryPoint = "GetMetadataFromDDSFile", CallingConvention = CallingConvention.StdCall)]
    private static extern TexMetadata _GetMetadataFromDDSFile([MarshalAs(UnmanagedType.LPWStr)] string szFile, DDSFLAGS flags = DDSFLAGS.DDS_FLAGS_NONE);
    public static TexMetadata GetMetadataFromDDSFile([MarshalAs(UnmanagedType.LPWStr)] string szFile, DDSFLAGS flags = DDSFLAGS.DDS_FLAGS_NONE)
        => _GetMetadataFromDDSFile(szFile, flags);

    [DllImport("lib/texconv.dll", EntryPoint = "GetMetadataFromTGAFile", CallingConvention = CallingConvention.StdCall)]
    private static extern TexMetadata _GetMetadataFromTGAFile([MarshalAs(UnmanagedType.LPWStr)] string szFile, TGA_FLAGS flags = TGA_FLAGS.TGA_FLAGS_NONE);
    public static TexMetadata GetMetadataFromTGAFile([MarshalAs(UnmanagedType.LPWStr)] string szFile, TGA_FLAGS flags = TGA_FLAGS.TGA_FLAGS_NONE)
        => _GetMetadataFromTGAFile(szFile, flags);

    [DllImport("lib/texconv.dll", EntryPoint = "GetMetadataFromDDSMemory", CallingConvention = CallingConvention.StdCall)]
    private static extern TexMetadata _GetMetadataFromDDSMemory(byte[] pSource, int size, DDSFLAGS flags = DDSFLAGS.DDS_FLAGS_NONE);
    public static TexMetadata GetMetadataFromDDSMemory(byte[] pSource, DDSFLAGS flags = DDSFLAGS.DDS_FLAGS_NONE)
        => _GetMetadataFromDDSMemory(pSource, pSource.Length, flags);



    [DllImport("lib/texconv.dll", EntryPoint = "ConvertAndSaveDdsImage", CallingConvention = CallingConvention.StdCall)]
    private static extern int _ConvertAndSaveDdsImage(byte[] bytePtr, int len, [MarshalAs(UnmanagedType.LPWStr)] string szFile, ESaveFileTypes filetype,
        bool vflip = false, bool hflip = false);
    public static int ConvertAndSaveDdsImage(byte[] bytePtr, [MarshalAs(UnmanagedType.LPWStr)] string szFile, ESaveFileTypes filetype,
        bool vflip = false, bool hflip = false)
        => _ConvertAndSaveDdsImage(bytePtr, bytePtr.Length, szFile, filetype, vflip, hflip);


    [DllImport("lib/texconv.dll", EntryPoint = "ConvertFromDds", CallingConvention = CallingConvention.StdCall)]
    private static extern long _ConvertFromDds(byte[] inBuffer, int inBufferLength, ref Blob blob, ESaveFileTypes filetype, bool vflip = false, bool hflip = false);
    public static long ConvertFromDds(byte[] inBuffer, ref Blob blob, ESaveFileTypes filetype, bool vflip = false, bool hflip = false)
        => _ConvertFromDds(inBuffer, inBuffer.Length, ref blob, filetype, vflip, hflip);

  
    [DllImport("lib/texconv.dll", EntryPoint = "ConvertToDds", CallingConvention = CallingConvention.StdCall)]
    private static extern long _ConvertToDds(byte[] inBuff, int inBufferLength, ref Blob blob, ESaveFileTypes filetype, DXGI_FORMAT format, bool vflip, bool hflip);
    public static long ConvertToDds(byte[] inBuff, ref Blob blob, ESaveFileTypes filetype, DXGI_FORMAT format, bool vflip = false, bool hflip = false)
        => _ConvertToDds(inBuff, inBuff.Length, ref blob, filetype, format, vflip, hflip);

    public enum DXGI_FORMAT
    {
        DXGI_FORMAT_UNKNOWN,
        DXGI_FORMAT_R32G32B32A32_TYPELESS,
        DXGI_FORMAT_R32G32B32A32_FLOAT,
        DXGI_FORMAT_R32G32B32A32_UINT,
        DXGI_FORMAT_R32G32B32A32_SINT,
        DXGI_FORMAT_R32G32B32_TYPELESS,
        DXGI_FORMAT_R32G32B32_FLOAT,
        DXGI_FORMAT_R32G32B32_UINT,
        DXGI_FORMAT_R32G32B32_SINT,
        DXGI_FORMAT_R16G16B16A16_TYPELESS,
        DXGI_FORMAT_R16G16B16A16_FLOAT,
        DXGI_FORMAT_R16G16B16A16_UNORM,
        DXGI_FORMAT_R16G16B16A16_UINT,
        DXGI_FORMAT_R16G16B16A16_SNORM,
        DXGI_FORMAT_R16G16B16A16_SINT,
        DXGI_FORMAT_R32G32_TYPELESS,
        DXGI_FORMAT_R32G32_FLOAT,
        DXGI_FORMAT_R32G32_UINT,
        DXGI_FORMAT_R32G32_SINT,
        DXGI_FORMAT_R32G8X24_TYPELESS,
        DXGI_FORMAT_D32_FLOAT_S8X24_UINT,
        DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS,
        DXGI_FORMAT_X32_TYPELESS_G8X24_UINT,
        DXGI_FORMAT_R10G10B10A2_TYPELESS,
        DXGI_FORMAT_R10G10B10A2_UNORM,
        DXGI_FORMAT_R10G10B10A2_UINT,
        DXGI_FORMAT_R11G11B10_FLOAT,
        DXGI_FORMAT_R8G8B8A8_TYPELESS,
        DXGI_FORMAT_R8G8B8A8_UNORM,
        DXGI_FORMAT_R8G8B8A8_UNORM_SRGB,
        DXGI_FORMAT_R8G8B8A8_UINT,
        DXGI_FORMAT_R8G8B8A8_SNORM,
        DXGI_FORMAT_R8G8B8A8_SINT,
        DXGI_FORMAT_R16G16_TYPELESS,
        DXGI_FORMAT_R16G16_FLOAT,
        DXGI_FORMAT_R16G16_UNORM,
        DXGI_FORMAT_R16G16_UINT,
        DXGI_FORMAT_R16G16_SNORM,
        DXGI_FORMAT_R16G16_SINT,
        DXGI_FORMAT_R32_TYPELESS,
        DXGI_FORMAT_D32_FLOAT,
        DXGI_FORMAT_R32_FLOAT,
        DXGI_FORMAT_R32_UINT,
        DXGI_FORMAT_R32_SINT,
        DXGI_FORMAT_R24G8_TYPELESS,
        DXGI_FORMAT_D24_UNORM_S8_UINT,
        DXGI_FORMAT_R24_UNORM_X8_TYPELESS,
        DXGI_FORMAT_X24_TYPELESS_G8_UINT,
        DXGI_FORMAT_R8G8_TYPELESS,
        DXGI_FORMAT_R8G8_UNORM,
        DXGI_FORMAT_R8G8_UINT,
        DXGI_FORMAT_R8G8_SNORM,
        DXGI_FORMAT_R8G8_SINT,
        DXGI_FORMAT_R16_TYPELESS,
        DXGI_FORMAT_R16_FLOAT,
        DXGI_FORMAT_D16_UNORM,
        DXGI_FORMAT_R16_UNORM,
        DXGI_FORMAT_R16_UINT,
        DXGI_FORMAT_R16_SNORM,
        DXGI_FORMAT_R16_SINT,
        DXGI_FORMAT_R8_TYPELESS,
        DXGI_FORMAT_R8_UNORM,
        DXGI_FORMAT_R8_UINT,
        DXGI_FORMAT_R8_SNORM,
        DXGI_FORMAT_R8_SINT,
        DXGI_FORMAT_A8_UNORM,
        DXGI_FORMAT_R1_UNORM,
        DXGI_FORMAT_R9G9B9E5_SHAREDEXP,
        DXGI_FORMAT_R8G8_B8G8_UNORM,
        DXGI_FORMAT_G8R8_G8B8_UNORM,
        DXGI_FORMAT_BC1_TYPELESS,
        DXGI_FORMAT_BC1_UNORM,
        DXGI_FORMAT_BC1_UNORM_SRGB,
        DXGI_FORMAT_BC2_TYPELESS,
        DXGI_FORMAT_BC2_UNORM,
        DXGI_FORMAT_BC2_UNORM_SRGB,
        DXGI_FORMAT_BC3_TYPELESS,
        DXGI_FORMAT_BC3_UNORM,
        DXGI_FORMAT_BC3_UNORM_SRGB,
        DXGI_FORMAT_BC4_TYPELESS,
        DXGI_FORMAT_BC4_UNORM,
        DXGI_FORMAT_BC4_SNORM,
        DXGI_FORMAT_BC5_TYPELESS,
        DXGI_FORMAT_BC5_UNORM,
        DXGI_FORMAT_BC5_SNORM,
        DXGI_FORMAT_B5G6R5_UNORM,
        DXGI_FORMAT_B5G5R5A1_UNORM,
        DXGI_FORMAT_B8G8R8A8_UNORM,
        DXGI_FORMAT_B8G8R8X8_UNORM,
        DXGI_FORMAT_R10G10B10_XR_BIAS_A2_UNORM,
        DXGI_FORMAT_B8G8R8A8_TYPELESS,
        DXGI_FORMAT_B8G8R8A8_UNORM_SRGB,
        DXGI_FORMAT_B8G8R8X8_TYPELESS,
        DXGI_FORMAT_B8G8R8X8_UNORM_SRGB,
        DXGI_FORMAT_BC6H_TYPELESS,
        DXGI_FORMAT_BC6H_UF16,
        DXGI_FORMAT_BC6H_SF16,
        DXGI_FORMAT_BC7_TYPELESS,
        DXGI_FORMAT_BC7_UNORM,
        DXGI_FORMAT_BC7_UNORM_SRGB,
        DXGI_FORMAT_AYUV,
        DXGI_FORMAT_Y410,
        DXGI_FORMAT_Y416,
        DXGI_FORMAT_NV12,
        DXGI_FORMAT_P010,
        DXGI_FORMAT_P016,
        DXGI_FORMAT_420_OPAQUE,
        DXGI_FORMAT_YUY2,
        DXGI_FORMAT_Y210,
        DXGI_FORMAT_Y216,
        DXGI_FORMAT_NV11,
        DXGI_FORMAT_AI44,
        DXGI_FORMAT_IA44,
        DXGI_FORMAT_P8,
        DXGI_FORMAT_A8P8,
        DXGI_FORMAT_B4G4R4A4_UNORM,
        DXGI_FORMAT_P208,
        DXGI_FORMAT_V208,
        DXGI_FORMAT_V408,
        DXGI_FORMAT_FORCE_UINT
    }

    public enum D3D10_RESOURCE_DIMENSION
    {
        D3D10_RESOURCE_DIMENSION_UNKNOWN,
        D3D10_RESOURCE_DIMENSION_BUFFER,
        D3D10_RESOURCE_DIMENSION_TEXTURE1D,
        D3D10_RESOURCE_DIMENSION_TEXTURE2D,
        D3D10_RESOURCE_DIMENSION_TEXTURE3D
    }

    public enum TEX_DIMENSION
        // Subset here matches D3D10_RESOURCE_DIMENSION and D3D11_RESOURCE_DIMENSION
    {
        TEX_DIMENSION_TEXTURE1D = 2,
        TEX_DIMENSION_TEXTURE2D = 3,
        TEX_DIMENSION_TEXTURE3D = 4,
    };
}

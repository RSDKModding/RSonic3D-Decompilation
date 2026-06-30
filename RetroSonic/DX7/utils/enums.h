#ifndef DX7_ENUMS_H
#define DX7_ENUMS_H

enum TextureFormatSearchType : sbyte {
    TEXTURE_FMT_32BIT,
    TEXTURE_FMT_16BIT_ALPHA,
};

enum D3DZBufferResultType {
    D3D_DEVICE_NONE,
    D3D_DEVICE_TNL_HAL,
    D3D_DEVICE_HAL,
    D3D_DEVICE_RGB,
};

#endif // !DX7_ENUMS_H
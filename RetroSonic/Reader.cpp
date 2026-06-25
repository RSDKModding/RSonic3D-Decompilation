#include "RetroEngine.hpp"

IDirectDrawSurface7 *Load_PNG_File(const char *path, int a2)
{
    FIBITMAP *fBitmap   = FreeImage_Load(FIF_PNG, path);
    FIBITMAP *fBitmap32 = FreeImage_ConvertTo32Bits(fBitmap);
    int bmWidth         = FreeImage_GetWidth(fBitmap);
    int bmHeight        = FreeImage_GetHeight(fBitmap);

    D3DDEVICEDESC7 pDesc;
    if (dx7Device->GetCaps(&pDesc) < 0)
        return 0;

    DDSURFACEDESC2 surfaceDesc;
    ZeroMemory(&surfaceDesc, sizeof(surfaceDesc));

    surfaceDesc.dwSize = sizeof(surfaceDesc);

    if (a2 == 1) {
        if (bmWidth > 64) {
            switch (bmWidth) {
                case 128:
                    surfaceDesc.dwFlags        = 1183751;
                    surfaceDesc.ddsCaps.dwCaps = 4198408;
                    surfaceDesc.dwMipMapCount  = 4;
                    break;

                case 256:
                    surfaceDesc.dwFlags        = 1183751;
                    surfaceDesc.ddsCaps.dwCaps = 4198408;
                    surfaceDesc.dwMipMapCount  = 5;
                    break;

                case 512:
                    surfaceDesc.dwFlags        = 1183751;
                    surfaceDesc.ddsCaps.dwCaps = 4198408;
                    surfaceDesc.dwMipMapCount  = 6;
                    break;
            }
        }
        else {
            switch (bmWidth) {
                case 64:
                    surfaceDesc.dwFlags        = 1183751;
                    surfaceDesc.ddsCaps.dwCaps = 4198408;
                    surfaceDesc.dwMipMapCount  = 3;
                    break;
                case 8:
                    surfaceDesc.dwFlags        = 1052679;
                    surfaceDesc.ddsCaps.dwCaps = 4096;
                    a2                         = 0;
                    break;
                case 16:
                    surfaceDesc.dwFlags        = 1183751;
                    surfaceDesc.ddsCaps.dwCaps = 4096;
                    a2                         = 0;
                    break;
                case 32:
                    surfaceDesc.dwFlags        = 1183751;
                    surfaceDesc.ddsCaps.dwCaps = 4198408;
                    surfaceDesc.dwMipMapCount  = 2;
                    break;
            }
        }
    }
    else {
        surfaceDesc.dwFlags        = 1052679;
        surfaceDesc.ddsCaps.dwCaps = 4096;
    }

    surfaceDesc.dwWidth  = bmWidth;
    surfaceDesc.dwHeight = bmHeight;

    if (IsEqualGUID(pDesc.deviceGUID, IID_IDirect3DHALDevice) || IsEqualGUID(pDesc.deviceGUID, IID_IDirect3DTnLHalDevice))
        surfaceDesc.ddsCaps.dwCaps2 = 16;
    else
        surfaceDesc.ddsCaps.dwCaps |= DDSCAPS_TEXTURE;

    if (pDesc.dpcTriCaps.dwTextureCaps & D3DPTEXTURECAPS_POW2) {
        for (surfaceDesc.dwWidth = 1; bmWidth > surfaceDesc.dwWidth; surfaceDesc.dwWidth <<= 1);
        for (surfaceDesc.dwHeight = 1; bmHeight > surfaceDesc.dwHeight; surfaceDesc.dwHeight <<= 1);
    }

    if (pDesc.dpcTriCaps.dwTextureCaps & D3DPTEXTURECAPS_SQUAREONLY) {
        if (surfaceDesc.dwWidth <= surfaceDesc.dwHeight)
            surfaceDesc.dwWidth = surfaceDesc.dwHeight;
        else
            surfaceDesc.dwHeight = surfaceDesc.dwWidth;
    }

    retryTextureEnum = FALSE;
    dx7Device->EnumTextureFormats(lpEnumTextureFormats, &surfaceDesc.ddpfPixelFormat);
    if (!surfaceDesc.ddpfPixelFormat.dwRGBBitCount) {
        retryTextureEnum = TRUE;
        dx7Device->EnumTextureFormats(lpEnumTextureFormats, &surfaceDesc.ddpfPixelFormat);
        if (!surfaceDesc.ddpfPixelFormat.dwRGBBitCount)
            return 0;
    }

    IDirectDrawSurface7 *ppRenderTarget;
    dx7Device->GetRenderTarget(&ppRenderTarget);
    ppRenderTarget->GetDDInterface((LPVOID *)&dd);
    ppRenderTarget->Release();

    LPDIRECTDRAWSURFACE7 s;
    if (dd->CreateSurface(&surfaceDesc, &s, 0) < 0)
        return 0;

    DDSURFACEDESC2 lockDesc;
    memset(&lockDesc, 0, sizeof(lockDesc));
    lockDesc.dwSize = sizeof(lockDesc);

    WORD alphaMask = (WORD)surfaceDesc.ddpfPixelFormat.dwRGBAlphaBitMask;

    IDirectDrawSurface7 *surfacePtr = s;
    if (a2 == 1) {
        int vSize = bmWidth;
        for (int i = 0; i < surfaceDesc.dwMipMapCount; ++i) {
            if (bmWidth > vSize && vSize > 8) {
                fBitmap32 = FreeImage_Rescale(fBitmap32, vSize, vSize, FREE_IMAGE_FILTER::FILTER_BOX);
                bmWidth   = vSize;
                bmHeight  = vSize;
            }

            HDC hdc;
            surfacePtr->GetDC(&hdc);
            StretchDIBits(hdc, 0, 0, bmWidth, bmHeight, 0, 0, bmWidth, bmHeight, FreeImage_GetBits(fBitmap32), FreeImage_GetInfo(fBitmap32),
                          DIB_RGB_COLORS, SRCCOPY);
            surfacePtr->ReleaseDC(hdc);

            if (retryTextureEnum == 1) {
                surfacePtr->Lock(0, &lockDesc, DDLOCK_WAIT, 0);
                DWORD pitch = lockDesc.lPitch;
                BYTE *dst   = (BYTE *)lockDesc.lpSurface;
                int srcLine = vSize - 1;

                for (DWORD y = 0; y < bmHeight; ++y) {
                    WORD *p   = (WORD *)(dst + pitch * y);
                    BYTE *src = FreeImage_GetScanLine(fBitmap32, srcLine--);
                    for (DWORD x = 0; x < bmWidth; ++x) {
                        if (src[3] == 255)
                            *p |= alphaMask;
                        else
                            *p = 0;
                        ++p;
                        src += 4;
                    }
                }

                surfacePtr->Unlock(0);
            }

            DDSCAPS2 caps = { DDSCAPS_TEXTURE };
            if (surfacePtr->GetAttachedSurface(&caps, &surfacePtr) >= 0)
                surfacePtr->Release();

            vSize >>= 1;
        }
    }
    else {
        HDC hdc;
        s->GetDC(&hdc);
        StretchDIBits(hdc, 0, 0, bmWidth, bmHeight, 0, 0, bmWidth, bmHeight, FreeImage_GetBits(fBitmap32), FreeImage_GetInfo(fBitmap32),
                      DIB_RGB_COLORS, SRCCOPY);
        s->ReleaseDC(hdc);

        if (retryTextureEnum == 1) {
            s->Lock(0, &lockDesc, DDLOCK_WAIT, 0);
            DWORD pitch = lockDesc.lPitch;
            BYTE *dst   = (BYTE *)lockDesc.lpSurface;
            int srcLine = lockDesc.dwHeight - 1;
            for (DWORD y = 0; y < lockDesc.dwHeight; ++y) {
                WORD *p   = (WORD *)(dst + pitch * y);
                BYTE *src = FreeImage_GetScanLine(fBitmap32, srcLine--);
                for (DWORD x = 0; x < lockDesc.dwWidth; ++x) {
                    if (src[3] == 255)
                        *p |= alphaMask;
                    else
                        *p = 0;
                    ++p;
                    src += 4;
                }
            }
            s->Unlock(0);
        }
    }

    FreeImage_Unload(fBitmap32);
    FreeImage_Unload(fBitmap);
    return s;
}

void Load_LMF_File(LMF *lmf, const char *path)
{
    FILE *stream = fopen(path, "rb");
    LMF temp;
    uint8_t byte;
    fread(&byte, 1, 1, stream);
    temp.surfaceCount = byte;
    fread(&temp.variable_2, sizeof(temp.variable_2), 1, stream);
    fread(&temp.variable_3, sizeof(temp.variable_3), 1, stream);
    fread(&temp.variable_4, sizeof(temp.variable_4), 1, stream);
    fread(&temp.variable_5, sizeof(temp.variable_5), 1, stream);
    fread(&byte, 1, 1, stream);
    temp.variable_1 = byte;

    for (int i = 0; i < temp.surfaceCount; ++i) {
        fread(&byte, 1, 1, stream);
        temp.surfaceID[i] = byte;
    }

    int vertexInfoCount = temp.surfaceCount * temp.variable_3 * temp.variable_2 + 1;
    temp.drawList       = new LMF_VertexInfo[vertexInfoCount];
    int cellCount       = temp.variable_3 * temp.variable_2;
    LCollision          = new CollisionModel3D *[cellCount];

    for (int j = 0; j < cellCount; ++j) {
        LCollision[j] = newCollisionModel3D();
        for (int k = 0; k < temp.surfaceCount; ++k) {
            int index = k * cellCount + j;
            uint16_t vcount;
            fread(&vcount, sizeof(vcount), 1, stream);
            temp.drawList[index].vertexCount = vcount;

            int allocV                    = vcount + 1;
            temp.drawList[index].vertices = new D3DLVERTEX[allocV];
            memset(temp.drawList[index].vertices, 0, allocV * sizeof(D3DLVERTEX));
            temp.drawList[index].pVertexParams = new float[vcount];

            for (int v = 0; v < vcount; ++v) {
                float coords[8];
                for (int l = 0; l < 8; ++l) fread(&coords[l], sizeof(coords[l]), 1, stream);
                float param    = (coords[4] + 1.0f) * 0.375f + 0.25f;
                D3DCOLOR color = (uint32_t)(param * 255) | ((uint32_t)(param * 255) << 8) | ((uint32_t)(param * 255) << 16) | 0xFF000000;

                D3DVECTOR pos(coords[0], coords[1], coords[2]);

                temp.drawList[index].vertices[v]      = D3DLVERTEX(pos, color, 0, coords[6], coords[7]);
                temp.drawList[index].pVertexParams[v] = param;
            }

            uint16_t icount;
            fread(&icount, sizeof(icount), 1, stream);
            temp.drawList[index].indexCount = icount;
            temp.drawList[index].indexes    = new uint16_t[icount];

            for (int v = 0; v < icount; ++v) {
                fread(&temp.drawList[index].indexes[v], sizeof(uint16_t), 1, stream);
            }

            for (int t = 0; t < icount; t += 3) {
                int i0 = temp.drawList[index].indexes[t];
                int i1 = temp.drawList[index].indexes[t + 1];
                int i2 = temp.drawList[index].indexes[t + 2];
                LCollision[j]->addTriangle(&temp.drawList[index].vertices[i0].x, &temp.drawList[index].vertices[i1].x,
                                           &temp.drawList[index].vertices[i2].x);
            }
        }
        LCollision[j]->finalize();
    }

    fclose(stream);
    memcpy(lmf, &temp, sizeof(LMF));
}

void Load_TMF_File(TMF *tmf, const char *path)
{
    TMF model;

    FILE *stream = fopen(path, "rb");
    fseek(stream, 0, 0);

    fread(&model.numVertices, 2, 1, stream);

    model.vertices = new D3DTLVERTEX[model.numVertices + 1];
    if (model.vertices != nullptr)
        memset(model.vertices, 0, sizeof(D3DTLVERTEX));

    D3DTLVERTEX vert;
    int32_t readPos;

    for (int32_t i = 0; i < model.numVertices; ++i) {
        for (int32_t m = 0; m < 8; ++m) fread(&vert.sx + m, sizeof(D3DVALUE), 1, stream);
        memcpy(&model.vertices[i], &vert, sizeof(D3DTLVERTEX));
    }

    fread(&readPos, 1, 1, stream);
    model.numIndices = (uint8_t)readPos;

    fread(&readPos, 1, 1, stream);
    model.numIndices += (uint8_t)readPos << 8;

    model.indices = new uint16_t[model.numIndices + 2];
    for (int32_t i = 0; i < model.numIndices; ++i) fread(&model.indices[i], 2, 1, stream);

    fclose(stream);
    memcpy(tmf, &model, sizeof(TMF));
}

void Load_ANI_File(Animation *animationPtr, const char *path)
{
    Animation animation;
    char boneName[256];

    FILE *stream = fopen(path, "rb");
    fseek(stream, 0, SEEK_SET);

    uint8_t frameCount;
    uint16_t boneCount;
    fread(&frameCount, sizeof(uint8_t), 1, stream);
    fread(&boneCount, sizeof(uint16_t), 1, stream);

    for (int i = 0; i < frameCount; ++i) {
        uint8_t nameLen;
        fread(&nameLen, sizeof(uint8_t), 1, stream);
        fread(boneName, sizeof(uint8_t), nameLen, stream);

        fread(&animation.frames[i].position.x, sizeof(float), 1, stream);
        fread(&animation.frames[i].position.y, sizeof(float), 1, stream);
        fread(&animation.frames[i].position.z, sizeof(float), 1, stream);

        fread(&animation.frames[i].vertexCount, sizeof(uint16_t), 1, stream);
        animation.frames[i].vertexIDs = new uint16_t[animation.frames[i].vertexCount];

        for (int j = 0; j < animation.frames[i].vertexCount; ++j) {
            fread(&animation.frames[i].vertexIDs[j], sizeof(uint16_t), 1, stream);
        }

        for (int j = 0; j < boneCount; ++j) {
            uint8_t idk;
            uint16_t val;

            fread(&idk, sizeof(uint8_t), 1, stream);
            fread(&val, sizeof(uint16_t), 1, stream);
            animation.frames[i].rotX[j] = (float)((idk ? (int)val : -(int)val) * (RETRO_PI / 180.0));

            fread(&idk, sizeof(uint8_t), 1, stream);
            fread(&val, sizeof(uint16_t), 1, stream);
            animation.frames[i].rotY[j] = (float)((idk ? (int)val : -(int)val) * (RETRO_PI / 180.0));

            fread(&idk, sizeof(uint8_t), 1, stream);
            fread(&val, sizeof(uint16_t), 1, stream);
            animation.frames[i].rotZ[j] = (float)((idk ? (int)val : -(int)val) * (RETRO_PI / 180.0));
        }
    }

    // field_BFA8 -> frameIDCount

    fread(&animation.field_BFA8, sizeof(uint16_t), 1, stream);
    animation.frameIDs = new uint8_t[animation.field_BFA8];

    for (int i = 0; i < animation.field_BFA8; ++i) fread(&animation.frameIDs[i], sizeof(uint8_t), 1, stream);

    uint8_t ab90Count;
    fread(&ab90Count, sizeof(uint8_t), 1, stream);

    for (int i = 0; i < ab90Count; ++i) {
        // also a name
        uint8_t nameLen;
        fread(&nameLen, sizeof(uint8_t), 1, stream);
        fread(boneName, sizeof(uint8_t), nameLen, stream); // animation/pose names?

        fread(&animation.array_AB90[i].field_201, sizeof(uint8_t), 1, stream);
        fread(&animation.array_AB90[i].field_200, sizeof(uint8_t), 1, stream);
        fread(&animation.array_AB90[i].count, sizeof(uint8_t), 1, stream);

        for (int j = 0; j < animation.array_AB90[i].count; ++j) fread(&animation.array_AB90[i].array_2[j], sizeof(uint16_t), 1, stream);
    }

    animation.field_BFAA = 0;
    animation.field_BFAB = 0;

    fclose(stream);
    memcpy(animationPtr, &animation, sizeof(Animation));
}

void LoadFile(FileInfo *file, const char *path)
{
    FILE *stream = fopen(path, "rb");
    fseek(stream, 0, 2);

    int len = ftell(stream);

    FileInfo *alloc = new FileInfo[len + 1];

    fseek(stream, 0, 0);

    for (int i = 0; i < len; ++i) {
        fread(alloc + i, 1, 1, stream);
    }

    fclose(stream);
    memcpy(file, alloc, sizeof(FileInfo));
}
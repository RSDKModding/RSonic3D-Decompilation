#include "RetroEngine.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb-image/stb_image.h>

LevelDirectoryEntry *LDirectory;

void LoadFile(FileInfo *file, const char *path)
{
    FileIO *handle = fOpen(path, "rb");
    fSeek(handle, 0, 2);

    file->size = fTell(handle);
    file->data = new byte[file->size + 1];
    fSeek(handle, 0, 0);

    for (int i = 0; i < file->size; ++i) {
        fRead(&file->data[i], 1, 1, handle);
    }

    fClose(handle);
}

void LoadTexture(Texture **texturePtr, const char *path, bool useTexMips)
{
#if RETRO_USE_ORIGINAL_CODE
    FIBITMAP *image = FreeImage_Load(FIF_PNG, path);
    if (image == NULL)
        return;

    FIBITMAP *image32 = FreeImage_ConvertTo32Bits(image);
    if (image32 == NULL) {
        FreeImage_Unload(image);
        return;
    }

    int width  = FreeImage_GetWidth(image);
    int height = FreeImage_GetHeight(image);

    D3DDEVICEDESC7 d3dd;
    if (D3DDevice->GetCaps(&d3dd) < 0)
        return;

    DDSURFACEDESC2 ddsd;
    MEM_ZERO(&ddsd, sizeof(ddsd));

    ddsd.dwSize = sizeof(ddsd);

    if (useTexMips == true) {
        switch (width) {
            case 8:
                ddsd.dwFlags        = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH | DDSD_PIXELFORMAT | DDSD_TEXTURESTAGE;
                ddsd.ddsCaps.dwCaps = DDSCAPS_TEXTURE;

                useTexMips = false;
                break;

            case 16:
                ddsd.dwFlags        = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH | DDSD_PIXELFORMAT | DDSD_MIPMAPCOUNT | DDSD_TEXTURESTAGE;
                ddsd.ddsCaps.dwCaps = DDSCAPS_TEXTURE;

                useTexMips = false;
                break;

            case 32:
                ddsd.dwFlags        = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH | DDSD_PIXELFORMAT | DDSD_MIPMAPCOUNT | DDSD_TEXTURESTAGE;
                ddsd.ddsCaps.dwCaps = DDSCAPS_TEXTURE | DDSCAPS_MIPMAP | DDSCAPS_COMPLEX;
                ddsd.dwMipMapCount  = 2;
                break;

            case 64:
                ddsd.dwFlags        = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH | DDSD_PIXELFORMAT | DDSD_MIPMAPCOUNT | DDSD_TEXTURESTAGE;
                ddsd.ddsCaps.dwCaps = DDSCAPS_TEXTURE | DDSCAPS_MIPMAP | DDSCAPS_COMPLEX;
                ddsd.dwMipMapCount  = 3;
                break;

            case 128:
                ddsd.dwFlags        = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH | DDSD_PIXELFORMAT | DDSD_MIPMAPCOUNT | DDSD_TEXTURESTAGE;
                ddsd.ddsCaps.dwCaps = DDSCAPS_TEXTURE | DDSCAPS_MIPMAP | DDSCAPS_COMPLEX;
                ddsd.dwMipMapCount  = 4;
                break;

            case 256:
                ddsd.dwFlags        = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH | DDSD_PIXELFORMAT | DDSD_MIPMAPCOUNT | DDSD_TEXTURESTAGE;
                ddsd.ddsCaps.dwCaps = DDSCAPS_TEXTURE | DDSCAPS_MIPMAP | DDSCAPS_COMPLEX;
                ddsd.dwMipMapCount  = 5;
                break;

            case 512:
                ddsd.dwFlags        = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH | DDSD_PIXELFORMAT | DDSD_MIPMAPCOUNT | DDSD_TEXTURESTAGE;
                ddsd.ddsCaps.dwCaps = DDSCAPS_TEXTURE | DDSCAPS_MIPMAP | DDSCAPS_COMPLEX;
                ddsd.dwMipMapCount  = 6;
                break;

            default: break;
        }
    }
    else {
        ddsd.dwFlags        = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH | DDSD_PIXELFORMAT | DDSD_TEXTURESTAGE;
        ddsd.ddsCaps.dwCaps = DDSCAPS_TEXTURE;
    }

    ddsd.dwWidth  = width;
    ddsd.dwHeight = height;

    if (d3dd.deviceGUID == IID_IDirect3DHALDevice || d3dd.deviceGUID == IID_IDirect3DTnLHalDevice)
        ddsd.ddsCaps.dwCaps2 = DDSCAPS_FLIP;
    else
        ddsd.ddsCaps.dwCaps |= DDSCAPS_TEXTURE;

    if (d3dd.dpcTriCaps.dwTextureCaps & D3DPTEXTURECAPS_POW2) {
        ddsd.dwWidth = 1;
        while (width > ddsd.dwWidth) {
            ddsd.dwWidth = ddsd.dwWidth * 2;
        }

        ddsd.dwHeight = 1;
        while (height > ddsd.dwHeight) {
            ddsd.dwHeight = ddsd.dwHeight * 2;
        }
    }

    if (d3dd.dpcTriCaps.dwTextureCaps & D3DPTEXTURECAPS_SQUAREONLY) {
        if (ddsd.dwWidth <= ddsd.dwHeight)
            ddsd.dwWidth = ddsd.dwHeight;
        else
            ddsd.dwHeight = ddsd.dwWidth;
    }

    TexFmtSearchType = TEXTURE_FMT_32BIT;
    D3DDevice->EnumTextureFormats(TextureSearchCallback, &ddsd.ddpfPixelFormat);
    if (!ddsd.ddpfPixelFormat.dwRGBBitCount) {

        TexFmtSearchType = TEXTURE_FMT_16BIT_ALPHA;
        D3DDevice->EnumTextureFormats(TextureSearchCallback, &ddsd.ddpfPixelFormat);
        if (!ddsd.ddpfPixelFormat.dwRGBBitCount)
            return;
    }

    IDirectDrawSurface7 *D3DRenderTarget;
    D3DDevice->GetRenderTarget(&D3DRenderTarget);

    D3DRenderTarget->GetDDInterface((void **)(&DDraw));
    D3DRenderTarget->Release();

    if (FAILED(DDraw->CreateSurface(&ddsd, (IDirectDrawSurface7 **)(texturePtr), 0)))
        return;

    DDSURFACEDESC2 ddsd2;
    MEM_ZERO(&ddsd2, sizeof(ddsd2));

    ddsd2.dwSize = sizeof(ddsd2);

    short alphaBitMask = ddsd.ddpfPixelFormat.dwRGBAlphaBitMask;

    IDirectDrawSurface7 *texture = (IDirectDrawSurface7 *)(*texturePtr);
    if (useTexMips == true) {
        int texMipSize = width;
        for (int i = 0; i < ddsd.dwMipMapCount; ++i) {
            if (width > texMipSize && texMipSize > 8) {
                image32 = FreeImage_Rescale(image32, texMipSize, texMipSize, FREE_IMAGE_FILTER::FILTER_BOX);
                width   = texMipSize;
                height  = texMipSize;
            }

            HDC hdc;
            texture->GetDC(&hdc);

            byte *data       = FreeImage_GetBits(image32);
            BITMAPINFO *info = FreeImage_GetInfo(image32);
            StretchDIBits(hdc, 0, 0, width, height, 0, 0, width, height, data, info, DIB_RGB_COLORS, SRCCOPY);

            texture->ReleaseDC(hdc);

            if (TexFmtSearchType == TEXTURE_FMT_16BIT_ALPHA) {
                texture->Lock(0, &ddsd2, DDLOCK_WAIT, 0);

                struct ColorBGRA {
                    byte b;
                    byte g;
                    byte r;
                    byte a;
                };

                int line = texMipSize - 1;
                for (int y = 0; y < height; ++y) {
                    auto *dst = (short *)((byte *)(ddsd2.lpSurface) + ddsd2.lPitch * y);
                    auto *src = (ColorBGRA *)(FreeImage_GetScanLine(image32, line--));

                    for (int x = 0; x < width; ++x) {
                        dst[x] = 0;
                        if (src[x].a == 0xFF)
                            dst[x] |= alphaBitMask;
                    }
                }

                texture->Unlock(0);
            }

            DDSCAPS2 caps = { DDSCAPS_TEXTURE };
            if (texture->GetAttachedSurface(&caps, &texture) >= 0)
                texture->Release();

            texMipSize >>= 1;
        }
    }
    else {
        HDC hdc;
        texture->GetDC(&hdc);

        byte *data       = FreeImage_GetBits(image32);
        BITMAPINFO *info = FreeImage_GetInfo(image32);
        StretchDIBits(hdc, 0, 0, width, height, 0, 0, width, height, data, info, DIB_RGB_COLORS, SRCCOPY);

        texture->ReleaseDC(hdc);

        if (TexFmtSearchType == TEXTURE_FMT_16BIT_ALPHA) {
            texture->Lock(NULL, &ddsd2, DDLOCK_WAIT, NULL);

            struct ColorBGRA {
                byte b;
                byte g;
                byte r;
                byte a;
            };

            int line = ddsd2.dwHeight - 1;
            for (int y = 0; y < ddsd2.dwHeight; ++y) {
                auto *dst = (short *)((byte *)(ddsd2.lpSurface) + ddsd2.lPitch * y);
                auto *src = (ColorBGRA *)(FreeImage_GetScanLine(image32, line--));

                for (int x = 0; x < ddsd2.dwWidth; ++x) {
                    dst[x] = 0;
                    if (src[x].a == 0xFF)
                        dst[x] |= alphaBitMask;
                }
            }

            texture->Unlock(NULL);
        }
    }

    FreeImage_Unload(image32);
    FreeImage_Unload(image);
#else
    int width    = 0;
    int height   = 0;
    int channels = 0;
    if (stbi_uc *data = stbi_load(path, &width, &height, &channels, 4)) {
        Texture *texture = (*texturePtr) = new Texture();

        texture->vtbl   = NULL;
        texture->width  = width;
        texture->height = height;

        glGenTextures(1, &texture->id);
        glBindTexture(GL_TEXTURE_2D, texture->id);

        if (useTexMips == true)
            glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_BGRA, texture->width, texture->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

        if (useTexMips == true)
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        else
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
#endif
}

void LoadLevelModel(LMF *model, const char *path)
{
    memset(model, 0, sizeof(*model));

    FileIO *stream = fOpen(path, "rb");
    fRead(&model->surfaceCount, sizeof(model->surfaceCount), 1, stream);
    fRead(&model->columns, sizeof(model->columns), 1, stream);
    fRead(&model->rows, sizeof(model->rows), 1, stream);
    fRead(&model->startX, sizeof(model->startX), 1, stream);
    fRead(&model->startZ, sizeof(model->startZ), 1, stream);
    fRead(&model->unused, sizeof(model->unused), 1, stream);

    for (int i = 0; i < model->surfaceCount; ++i) {
        fRead(&model->surfaceID[i], sizeof(model->surfaceID[i]), 1, stream);
    }

    LCollision = new CollisionModel3D **[model->rows];
    for (int row = 0; row < model->rows; ++row) {
        LCollision[row] = new CollisionModel3D *[model->columns];
    }

    model->tiles = new LMFMesh **[model->surfaceCount];
    for (int s = 0; s < model->surfaceCount; ++s) {
        model->tiles[s] = new LMFMesh *[model->rows];
        for (int row = 0; row < model->rows; ++row) {
            model->tiles[s][row] = new LMFMesh[model->columns]();
        }
    }

    for (int y = 0; y < model->rows; ++y) {
        for (int c = 0; c < model->columns; ++c) {
            LCollision[y][c] = newCollisionModel3D();

            for (int s = 0; s < model->surfaceCount; ++s) {
                LMFMesh *tile = &model->tiles[s][y][c];

                fRead(&tile->numVertices, sizeof(tile->numVertices), 1, stream);
                tile->vertices = new LVertex[tile->numVertices + 1]();
                tile->colors   = new float[tile->numVertices];

                for (int v = 0; v < tile->numVertices; ++v) {
                    // did you know that this is straight up allowed in C++?
                    // LMF vertices are identical to D3DLVERTEX/LVertex layout-wise
                    // though, we're using a seperate struct here due to color being float instead of D3DCOLOR (int)
                    struct _ {
                        Vector3D v;
                        float reserved;
                        float color;
                        float specular;
                        float tu;
                        float tv;
                    } vert;

                    fRead(&vert, sizeof(vert), 1, stream);

                    vert.color += 1.0f;
                    vert.color *= 0.375f;
                    vert.color += 0.25f;

                    Color color = PACK_ARGB_F(1.0f, vert.color, vert.color, vert.color);

                    tile->vertices[v] = { vert.v, color, 0, vert.tu, vert.tv };
                    tile->colors[v]   = vert.color;
                }

                fRead(&tile->numIndices, sizeof(tile->numIndices), 1, stream);
                tile->indices = new ushort[tile->numIndices];

                for (int v = 0; v < tile->numIndices; ++v) {
                    fRead(&tile->indices[v], sizeof(tile->indices[v]), 1, stream);
                }

                for (int t = 0; t < tile->numIndices; t += 3) {
                    float *vert[3] = { NULL, NULL, NULL };
                    for (int v = 0; v < 3; ++v) {
                        vert[v] = &tile->vertices[tile->indices[t + v]].x;
                    }

                    LCollision[y][c]->addTriangle(vert[0], vert[1], vert[2]);
                }
            }

            LCollision[y][c]->finalize();
        }
    }

    fClose(stream);
}

void SetLevelDirectory(const char *text, byte length, int index)
{
    for (int i = 0; i < length; ++i) {
        LDirectory[index - 1].levelName[i] = text[i];
    }

    LDirectory[index - 1].levelNameLen = length;
}

void SetActNumber(const char *text, byte length, int index)
{
    for (int i = 0; i < length; ++i) {
        LDirectory[index - 1].actNum[i] = text[i];
    }

    LDirectory[index - 1].actNumLen = length;
}

void AllocateDirectories(int size) { LDirectory = new LevelDirectoryEntry[size + 1]; }

void LoadDirectoryFile(FileInfo *file, int id, const char *fileName, int fileNameLen)
{
    char path[0x20];
    StrCopy(path, "Data/Levels/");

    int r = 12; // "Data/Levels/" length
    for (int i = 0; i < LDirectory[id].levelNameLen; ++i) {
        path[r++] = LDirectory[id].levelName[i];
    }

    for (int i = 0; i < fileNameLen; ++i) {
        path[r++] = fileName[i];
    }

    path[r] = '\0';

    LoadFile(file, fileName);
}

void LoadDirectoryActFile(FileInfo *file, int id, const char *fileName, int fileNameLen)
{
    char path[0x20];
    StrCopy(path, "Data/Levels/");

    char act[0x08];
    strcpy(act, "/Act");

    int r = 12; // "Data/Levels/" length
    for (int i = 0; i < LDirectory[id].levelNameLen; ++i) {
        path[r++] = LDirectory[id].levelName[i];
    }

    for (int i = 0; i < 4; ++i) {
        path[r++] = act[i];
    }

    for (int i = 0; i < LDirectory[id].actNumLen; ++i) {
        path[r++] = LDirectory[id].actNum[i];
    }

    for (int i = 0; i < fileNameLen; ++i) {
        path[r++] = fileName[i];
    }

    path[r] = '\0';

    LoadFile(file, fileName);
}

void LoadDirectoryGraphic(int id, const char *fileName, int fileNameLen)
{
    char path[0x20];
    StrCopy(path, "Data/Levels/");

    int r = 12; // "Data/Levels/" length
    for (int i = 0; i < LDirectory[id].levelNameLen; ++i) {
        path[r++] = LDirectory[id].levelName[i];
    }

    for (int i = 0; i < fileNameLen; ++i) {
        path[r++] = fileName[i];
    }

    path[r] = '\0';

    // No LoadZoneTiles :C
}

void CreateDirectories()
{
    memcpy(&LDirectory[0], "R1", 2);
    LDirectory[0].levelNameLen = 2;

    memcpy(&LDirectory[1], "R1", 2);
    LDirectory[1].levelNameLen = 2;

    memcpy(&LDirectory[2], "R1", 2);
    LDirectory[2].levelNameLen = 2;

    memcpy(&LDirectory[3], "R2", 2);
    LDirectory[3].levelNameLen = 2;

    memcpy(&LDirectory[4], "R2", 2);
    LDirectory[4].levelNameLen = 2;

    memcpy(&LDirectory[5], "R2", 2);
    LDirectory[5].levelNameLen = 2;
}

void Load_TMF_File(TMF *model, const char *path)
{
    memset(model, 0, sizeof(*model));

    FileIO *stream = fopen(path, "rb");
    fSeek(stream, 0, 0);

    fRead(&model->numVertices, 2, 1, stream);
    model->vertices = new Vertex[model->numVertices + 1];
    if (model->vertices != NULL)
        memset(model->vertices, 0, sizeof(*model->vertices));

    for (int i = 0; i < model->numVertices; ++i) {
        fRead(&model->vertices[i], sizeof(model->vertices[i]), 1, stream);
    }

    fRead(&model->numIndices, sizeof(model->numIndices), 1, stream);
    model->indices = new ushort[model->numIndices + 2];
    for (int i = 0; i < model->numIndices; ++i) {
        fRead(&model->indices[i], sizeof(model->indices[i]), 1, stream);
    }

    fClose(stream);
}

void Load_ANI_File(Animation *animation, const char *path)
{
    memset(animation, 0, sizeof(*animation));

    char boneName[256];

    FileIO *stream = fopen(path, "rb");
    fSeek(stream, 0, SEEK_SET);

    byte frameCount;
    ushort nodeCount;
    fRead(&frameCount, sizeof(byte), 1, stream);
    fRead(&nodeCount, sizeof(ushort), 1, stream);

    for (int i = 0; i < frameCount; ++i) {
        byte nameLen;
        fRead(&nameLen, sizeof(byte), 1, stream);
        fRead(boneName, sizeof(byte), nameLen, stream);

        fRead(&animation->nodes[i].position.x, sizeof(float), 1, stream);
        fRead(&animation->nodes[i].position.y, sizeof(float), 1, stream);
        fRead(&animation->nodes[i].position.z, sizeof(float), 1, stream);

        fRead(&animation->nodes[i].vertexCount, sizeof(ushort), 1, stream);
        animation->nodes[i].vertexIDs = new ushort[animation->nodes[i].vertexCount];

        for (int j = 0; j < animation->nodes[i].vertexCount; ++j) {
            fRead(&animation->nodes[i].vertexIDs[j], sizeof(ushort), 1, stream);
        }

        for (int j = 0; j < nodeCount; ++j) {
            byte idk;
            ushort val;

            fRead(&idk, sizeof(byte), 1, stream);
            fRead(&val, sizeof(ushort), 1, stream);
            animation->nodes[i].rotX[j] = (float)((idk ? (int)val : -(int)val) * (RSDK_PI / 180.0));

            fRead(&idk, sizeof(byte), 1, stream);
            fRead(&val, sizeof(ushort), 1, stream);
            animation->nodes[i].rotY[j] = (float)((idk ? (int)val : -(int)val) * (RSDK_PI / 180.0));

            fRead(&idk, sizeof(byte), 1, stream);
            fRead(&val, sizeof(ushort), 1, stream);
            animation->nodes[i].rotZ[j] = (float)((idk ? (int)val : -(int)val) * (RSDK_PI / 180.0));
        }
    }

    fRead(&animation->frameIDCount, sizeof(ushort), 1, stream);
    animation->frameIDs = new byte[animation->frameIDCount];

    for (int i = 0; i < animation->frameIDCount; ++i) {
        fRead(&animation->frameIDs[i], sizeof(byte), 1, stream);
    }

    byte stateCount;
    fRead(&stateCount, sizeof(byte), 1, stream);

    for (int i = 0; i < stateCount; ++i) {
        byte nameLen;
        fRead(&nameLen, sizeof(byte), 1, stream);
        fRead(&boneName, sizeof(byte), nameLen, stream);

        fRead(&animation->states[i].frameDuration, sizeof(byte), 1, stream);
        fRead(&animation->states[i].loopIndex, sizeof(byte), 1, stream);
        fRead(&animation->states[i].frameCount, sizeof(byte), 1, stream);

        for (int j = 0; j < animation->states[i].frameCount; ++j) {
            fRead(&animation->states[i].array_2[j], sizeof(ushort), 1, stream);
        }
    }

    animation->field_BFAA = 0;
    animation->field_BFAB = 0;

    fClose(stream);
}

#if 0
int *__cdecl TODO_READER_FUNC_401A87(int *a1, char *FileName)
{
  int v3; // [esp+Ch] [ebp-BFD0h]
  int v4; // [esp+Ch] [ebp-BFD0h]
  int v5; // [esp+10h] [ebp-BFCCh] BYREF
  int j; // [esp+14h] [ebp-BFC8h]
  int i; // [esp+18h] [ebp-BFC4h]
  FileIO *Stream; // [esp+1Ch] [ebp-BFC0h]
  int Buffer; // [esp+20h] [ebp-BFBCh] BYREF
  R3D::Animation v10; // [esp+24h] [ebp-BFB8h] BYREF
  int v11; // [esp+BFD8h] [ebp-4h] BYREF

  Stream = fopen(FileName, "rb");
  fSeek(Stream, 0, 0);
  fRead(&Buffer, 1u, 1u, Stream);
  v3 = Buffer;
  fRead(&v11, 2u, 1u, Stream);
  for ( i = 0; i < v3; ++i )
  {
    fRead(&Buffer, 1u, 1u, Stream);
    v5 = Buffer;
    for ( j = 0; j < v5; ++j )
      fRead(&Buffer, 1u, 1u, Stream);
    fRead(&v10.frames[i].field_8, 4u, 1u, Stream);
    fRead(&v10.frames[i].field_8.y, 4u, 1u, Stream);
    fRead(&v10.frames[i].field_8.z, 4u, 1u, Stream);
    fRead(&v5, 2u, 1u, Stream);
    v10.frames[i].count = v5;
    for ( j = 0; j < v10.frames[i].count; ++j )
    {
      fRead(&v5, 2u, 1u, Stream);
      v10.frames[i].field_0[j] = v5;
    }
    for ( j = 0; j < v11; ++j )
    {
      fRead(&v5, 2u, 1u, Stream);
      v10.frames[i].rotX[j] = v5 * (3.1415927 / 180.0);
      fRead(&v5, 2u, 1u, Stream);
      v10.frames[i].rotY[j] = v5 * (3.1415927 / 180.0);
      fRead(&v5, 2u, 1u, Stream);
      v10.frames[i].rotZ[j] = v5 * (3.1415927 / 180.0);
    }
  }
  fRead(&v5, 2u, 1u, Stream);
  v10.field_BFA8 = v5;
  v10.frameIDs = operator new(v5);
  for ( i = 0; i < v10.field_BFA8; ++i )
    fRead(&v10.frameIDs[i], 1u, 1u, Stream);
  fRead(&Buffer, 1u, 1u, Stream);
  v4 = Buffer;
  for ( i = 0; i < v4; ++i )
  {
    fRead(&Buffer, 1u, 1u, Stream);
    v5 = Buffer;
    for ( j = 0; j < v5; ++j )
      fRead(&Buffer, 1u, 1u, Stream);
    fRead(&v10.array_AB90[i].field_201, 1u, 1u, Stream);
    fRead(&v10.array_AB90[i].field_200, 1u, 1u, Stream);
    fRead(&v10.array_AB90[i], 1u, 1u, Stream);
    for ( j = 0; j < v10.array_AB90[i].count; ++j )
      fRead(&v10.array_AB90[i].array_2[j], 2u, 1u, Stream);
  }
  fClose(Stream);
  qmemcpy(a1, &v10, 0xBFB4u);
  return a1;
}
#endif
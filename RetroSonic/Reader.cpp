#include "RetroEngine.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb-image/stb_image.h>

LevelDirectoryEntry *LDirectory;

void LoadFile(FileInfo *file, const char *path)
{
    READER_PATH_BUFFER(path);

    FileIO *handle = fOpen(buffer, "rb");
    fSeek(handle, 0, SEEK_END);
    file->size = fTell(handle);
    file->data = new byte[file->size + 1];
    fSeek(handle, 0, SEEK_SET);

    for (int i = 0; i < file->size; ++i) {
        fRead(&file->data[i], 1, 1, handle);
    }

    fClose(handle);
}

void LoadTexture(Texture **texturePtr, const char *path, bool useTexMips)
{
    READER_PATH_BUFFER(path);

#if RETRO_USE_ORIGINAL_CODE
    FIBITMAP *image = FreeImage_Load(FIF_PNG, buffer);
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

    D3DTexFmtSearchType = TEXTURE_FMT_32BIT;
    D3DDevice->EnumTextureFormats(TextureSearchCallback, &ddsd.ddpfPixelFormat);
    if (!ddsd.ddpfPixelFormat.dwRGBBitCount) {

        D3DTexFmtSearchType = TEXTURE_FMT_16BIT_ALPHA;
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

            if (D3DTexFmtSearchType == TEXTURE_FMT_16BIT_ALPHA) {
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

        if (D3DTexFmtSearchType == TEXTURE_FMT_16BIT_ALPHA) {
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

    if (stbi_uc *data = stbi_load(buffer, &width, &height, &channels, 4)) {
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
    MEM_ZEROP(model);
    READER_PATH_BUFFER(path);

    FileIO *handle = fOpen(buffer, "rb");

    fRead(&model->layers, sizeof(byte), 1, handle);
    fRead(&model->columns, sizeof(ushort), 1, handle);
    fRead(&model->rows, sizeof(ushort), 1, handle);
    fRead(&model->x, sizeof(float), 1, handle);
    fRead(&model->z, sizeof(float), 1, handle);
    fRead(&model->numTextures, sizeof(byte), 1, handle);

    for (int i = 0; i < model->layers; ++i) {
        fRead(&model->textureIDs[i], sizeof(byte), 1, handle);
    }

    // This and model->tiles were originally allocated as flat arrays, changed 'em here
    // so we can access their multidimensional data naturally
    LCollision = new CollisionModel3D **[model->rows];
    for (int row = 0; row < model->rows; ++row) {
        LCollision[row] = new CollisionModel3D *[model->columns];
    }

    // Instead of having to manually calculate it
    model->tiles = new LMFMesh **[model->layers];
    for (int s = 0; s < model->layers; ++s) {
        model->tiles[s] = new LMFMesh *[model->rows];
        for (int row = 0; row < model->rows; ++row) {
            model->tiles[s][row] = new LMFMesh[model->columns]();
        }
    }

    for (int y = 0; y < model->rows; ++y) {
        for (int c = 0; c < model->columns; ++c) {
            LCollision[y][c] = newCollisionModel3D();

            for (int l = 0; l < model->layers; ++l) {
                LMFMesh *tile = &model->tiles[l][y][c];

                fRead(&tile->numVertices, sizeof(ushort), 1, handle);
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

                    fRead(&vert, sizeof(_), 1, handle);

                    vert.color += 1.0f;
                    vert.color *= 0.375f;
                    vert.color += 0.25f;

                    Color color = PACK_ARGB_F(1.0f, vert.color, vert.color, vert.color);

                    tile->vertices[v] = { vert.v, color, 0, vert.tu, vert.tv };
                    tile->colors[v]   = vert.color;
                }

                fRead(&tile->numIndices, sizeof(ushort), 1, handle);
                tile->indices = new ushort[tile->numIndices];

                for (int v = 0; v < tile->numIndices; ++v) {
                    fRead(&tile->indices[v], sizeof(tile->indices[v]), 1, handle);
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

    fClose(handle);
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
#if RETRO_USE_MOD_LOADER
    char path[0x100];
#else
    char path[0x20];
#endif

    StrCopy(path, "Data/Levels/");

    int r = 12; // "Data/Levels/" length
    for (int i = 0; i < LDirectory[id].levelNameLen; ++i) {
        path[r++] = LDirectory[id].levelName[i];
    }

    for (int i = 0; i < fileNameLen; ++i) {
        path[r++] = fileName[i];
    }

    path[r] = '\0';

#if !RETRO_USE_ORIGINAL_CODE && RETRO_USE_MOD_LOADER
    ModFilePath(path);
#endif

    LoadFile(file, path);
}

void LoadDirectoryActFile(FileInfo *file, int id, const char *fileName, int fileNameLen)
{
#if RETRO_USE_MOD_LOADER
    char path[0x100];
#else
    char path[0x20];
#endif

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

#if !RETRO_USE_ORIGINAL_CODE && RETRO_USE_MOD_LOADER
    ModFilePath(path);
#endif

    LoadFile(file, path);
}

void LoadDirectoryGraphic(int id, const char *fileName, int fileNameLen)
{
#if RETRO_USE_MOD_LOADER
    char path[0x100];
#else
    char path[0x20];
#endif

    StrCopy(path, "Data/Levels/");

    int r = 12; // "Data/Levels/" length
    for (int i = 0; i < LDirectory[id].levelNameLen; ++i) {
        path[r++] = LDirectory[id].levelName[i];
    }

    for (int i = 0; i < fileNameLen; ++i) {
        path[r++] = fileName[i];
    }

    path[r] = '\0';

#if !RETRO_USE_ORIGINAL_CODE && RETRO_USE_MOD_LOADER
    ModFilePath(path);
#endif

    // No LoadZoneTiles
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

void LoadModel(TMF *model, const char *path)
{
    MEM_ZEROP(model);
    READER_PATH_BUFFER(path);

    FileIO *handle = fOpen(buffer, "rb");
    fSeek(handle, 0, SEEK_SET);

    fRead(&model->numVertices, sizeof(ushort), 1, handle);
    model->vertices = new Vertex[model->numVertices + 1];
    if (model->vertices != NULL)
        MEM_ZEROP(model->vertices);

    for (int i = 0; i < model->numVertices; ++i) {
        fRead(&model->vertices[i], sizeof(Vertex), 1, handle);
    }

    fRead(&model->numIndices, sizeof(ushort), 1, handle);
    model->indices = new ushort[model->numIndices + 2];
    for (int i = 0; i < model->numIndices; ++i) {
        fRead(&model->indices[i], sizeof(ushort), 1, handle);
    }

    fClose(handle);
}

void LoadAnimationFile(Animator *animator, const char *path)
{
    MEM_ZEROP(animator);
    READER_PATH_BUFFER(path);

    FileIO *handle = fOpen(path, "rb");
    fSeek(handle, 0, SEEK_SET);

    byte nodeCount;
    ushort poseCount;
    fRead(&nodeCount, sizeof(byte), 1, handle);
    fRead(&poseCount, sizeof(ushort), 1, handle);

    for (int i = 0; i < nodeCount; ++i) {
        AnimatorPart *node = &animator->nodes[i];

        byte nameLength;  // Used by the animation editor, unused by the game
        char name[0x100]; // Used by the animation editor, unused by the game

        fRead(&nameLength, sizeof(byte), 1, handle);
        fRead(name, sizeof(byte), nameLength, handle);

        fRead(&node->x, sizeof(float), 1, handle);
        fRead(&node->y, sizeof(float), 1, handle);
        fRead(&node->z, sizeof(float), 1, handle);

        fRead(&node->numIndices, sizeof(ushort), 1, handle);
        node->indices = new ushort[node->numIndices];

        for (int k = 0; k < node->numIndices; ++k) {
            fRead(&node->indices[k], sizeof(ushort), 1, handle);
        }

        for (int j = 0; j < poseCount; ++j) {
            byte poseFlipped;
            ushort poseValue;

            fRead(&poseFlipped, sizeof(byte), 1, handle);
            fRead(&poseValue, sizeof(ushort), 1, handle);
            node->ZPosing[j] = -TO_RADIAN(poseValue);
            if (poseFlipped != false)
                node->ZPosing[j] = -node->ZPosing[j];

            fRead(&poseFlipped, sizeof(byte), 1, handle);
            fRead(&poseValue, sizeof(ushort), 1, handle);
            node->YPosing[j] = -TO_RADIAN(poseValue);
            if (poseFlipped != false)
                node->YPosing[j] = -node->YPosing[j];

            fRead(&poseFlipped, sizeof(byte), 1, handle);
            fRead(&poseValue, sizeof(ushort), 1, handle);
            node->XPosing[j] = -TO_RADIAN(poseValue);
            if (poseFlipped != false)
                node->XPosing[j] = -node->XPosing[j];
        }
    }

    fRead(&animator->nodeCount, sizeof(ushort), 1, handle);
    animator->nodeIndices = new byte[animator->nodeCount];

    for (int i = 0; i < animator->nodeCount; ++i) {
        fRead(&animator->nodeIndices[i], sizeof(byte), 1, handle);
    }

    byte numStates;
    fRead(&numStates, sizeof(byte), 1, handle);

    for (int i = 0; i < numStates; ++i) {
        AnimatorState *state = &animator->states[i];

        byte nameLength;  // Used by the animation editor, unused by the game
        char name[0x100]; // Used by the animation editor, unused by the game

        fRead(&nameLength, sizeof(byte), 1, handle);
        fRead(name, sizeof(byte), nameLength, handle);

        fRead(&state->frameDuration, sizeof(byte), 1, handle);
        fRead(&state->loopIndex, sizeof(byte), 1, handle);
        fRead(&state->frameCount, sizeof(byte), 1, handle);

        for (int k = 0; k < state->frameCount; ++k) {
            fRead(&state->indices[k], sizeof(ushort), 1, handle);
        }
    }

    animator->animationID   = 0;
    animator->nextAnimation = 0;

    fClose(handle);
}

void LoadAnimationFile2(Animator *animator, const char *path)
{
    MEM_ZEROP(animator);
    READER_PATH_BUFFER(path);

    FileIO *handle = fOpen(buffer, "rb");
    fSeek(handle, 0, SEEK_SET);

    byte nodeCount;
    ushort poseCount;
    fRead(&nodeCount, sizeof(byte), 1, handle);
    fRead(&poseCount, sizeof(ushort), 1, handle);

    for (int i = 0; i < nodeCount; ++i) {
        AnimatorPart *node = &animator->nodes[i];

        byte nameLength;  // Used by the animation editor, unused by the game
        char name[0x100]; // Used by the animation editor, unused by the game

        fRead(&nameLength, sizeof(byte), 1, handle);

        // This one reads each character individually
        for (int n = 0; n < nameLength; ++n) {
            fRead(&name, sizeof(byte), 1, handle);
        }

        fRead(&node->x, sizeof(float), 1, handle);
        fRead(&node->y, sizeof(float), 1, handle);
        fRead(&node->z, sizeof(float), 1, handle);

        fRead(&node->numIndices, sizeof(ushort), 1, handle);
        node->indices = new ushort[node->numIndices];

        for (int k = 0; k < node->numIndices; ++k) {
            fRead(&node->indices[k], sizeof(ushort), 1, handle);
        }

        for (int j = 0; j < poseCount; ++j) {
            byte poseFlipped;
            ushort poseValue;

            fRead(&poseFlipped, sizeof(byte), 1, handle);
            fRead(&poseValue, sizeof(ushort), 1, handle);
            node->ZPosing[j] = -TO_RADIAN(poseValue);
            if (poseFlipped != false)
                node->ZPosing[j] = -node->ZPosing[j];

            fRead(&poseFlipped, sizeof(byte), 1, handle);
            fRead(&poseValue, sizeof(ushort), 1, handle);
            node->YPosing[j] = -TO_RADIAN(poseValue);
            if (poseFlipped != false)
                node->YPosing[j] = -node->YPosing[j];

            fRead(&poseFlipped, sizeof(byte), 1, handle);
            fRead(&poseValue, sizeof(ushort), 1, handle);
            node->XPosing[j] = -TO_RADIAN(poseValue);
            if (poseFlipped != false)
                node->XPosing[j] = -node->XPosing[j];
        }
    }

    fRead(&animator->nodeCount, sizeof(ushort), 1, handle);
    animator->nodeIndices = new byte[animator->nodeCount];

    for (int i = 0; i < animator->nodeCount; ++i) {
        fRead(&animator->nodeIndices[i], sizeof(byte), 1, handle);
    }

    byte numStates;
    fRead(&numStates, sizeof(byte), 1, handle);

    for (int i = 0; i < numStates; ++i) {
        AnimatorState *state = &animator->states[i];

        byte nameLength;  // Used by the animation editor, unused by the game
        char name[0x100]; // Used by the animation editor, unused by the game

        fRead(&nameLength, sizeof(byte), 1, handle);

        // This one reads each character individually
        for (int n = 0; n < nameLength; ++n) {
            fRead(&name, sizeof(byte), 1, handle);
        }

        fRead(&state->frameDuration, sizeof(byte), 1, handle);
        fRead(&state->loopIndex, sizeof(byte), 1, handle);
        fRead(&state->frameCount, sizeof(byte), 1, handle);

        for (int k = 0; k < state->frameCount; ++k) {
            fRead(&state->indices[k], sizeof(ushort), 1, handle);
        }
    }

    // Doesn't reset animationID

    fClose(handle);
}
#include "RetroEngine.hpp"

LevelDirectoryEntry *LDirectory;

void LoadFile(FileInfo *file, const char *path)
{
    FILE *handle = fopen(path, "rb");
    fseek(handle, 0, 2);

    file->size = ftell(handle);
    file->data = new byte[file->size + 1];
    fseek(handle, 0, 0);

    for (int i = 0; i < file->size; ++i) {
        fread(&file->data[i], 1, 1, handle);
    }

    fclose(handle);
}

void LoadTexture(Texture **texture, const char *path, bool useTexMips)
{
#if RETRO_USE_ORIGINAL_CODE
    FIBITMAP *fBitmap = FreeImage_Load(FIF_PNG, path);
    if (fBitmap == NULL)
        return;

    FIBITMAP *fBitmap32 = FreeImage_ConvertTo32Bits(fBitmap);
    if (fBitmap32 == NULL) {
        FreeImage_Unload(fBitmap);
        return;
    }

    int bmWidth  = FreeImage_GetWidth(fBitmap);
    int bmHeight = FreeImage_GetHeight(fBitmap);

    D3DDEVICEDESC7 pDesc;
    if (D3DDevice->GetCaps(&pDesc) < 0)
        return;

    DDSURFACEDESC2 surfaceDesc;
    MEM_ZERO(&surfaceDesc, sizeof(surfaceDesc));

    surfaceDesc.dwSize = sizeof(surfaceDesc);

    if (useTexMips == true) {
        if (bmWidth > 64) {
            switch (bmWidth) {
                case 128:
                    surfaceDesc.dwFlags        = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH | DDSD_PIXELFORMAT | DDSD_MIPMAPCOUNT | DDSD_TEXTURESTAGE;
                    surfaceDesc.ddsCaps.dwCaps = 4198408;
                    surfaceDesc.dwMipMapCount  = 4;
                    break;

                case 256:
                    surfaceDesc.dwFlags        = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH | DDSD_PIXELFORMAT | DDSD_MIPMAPCOUNT | DDSD_TEXTURESTAGE;
                    surfaceDesc.ddsCaps.dwCaps = 4198408;
                    surfaceDesc.dwMipMapCount  = 5;
                    break;

                case 512:
                    surfaceDesc.dwFlags        = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH | DDSD_PIXELFORMAT | DDSD_MIPMAPCOUNT | DDSD_TEXTURESTAGE;
                    surfaceDesc.ddsCaps.dwCaps = 4198408;
                    surfaceDesc.dwMipMapCount  = 6;
                    break;
            }
        }
        else {
            switch (bmWidth) {
                case 64:
                    surfaceDesc.dwFlags        = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH | DDSD_PIXELFORMAT | DDSD_MIPMAPCOUNT | DDSD_TEXTURESTAGE;
                    surfaceDesc.ddsCaps.dwCaps = 4198408;
                    surfaceDesc.dwMipMapCount  = 3;
                    break;
                case 8:
                    surfaceDesc.dwFlags        = 1052679;
                    surfaceDesc.ddsCaps.dwCaps = 4096;
                    useTexMips                 = false;
                    break;
                case 16:
                    surfaceDesc.dwFlags        = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH | DDSD_PIXELFORMAT | DDSD_MIPMAPCOUNT | DDSD_TEXTURESTAGE;
                    surfaceDesc.ddsCaps.dwCaps = 4096;
                    useTexMips                 = false;
                    break;
                case 32:
                    surfaceDesc.dwFlags        = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH | DDSD_PIXELFORMAT | DDSD_MIPMAPCOUNT | DDSD_TEXTURESTAGE;
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

    if (pDesc.deviceGUID == IID_IDirect3DHALDevice || pDesc.deviceGUID == IID_IDirect3DTnLHalDevice)
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

    TexFmtSearchType = TEXTURE_FMT_32BIT;
    D3DDevice->EnumTextureFormats(TextureSearchCallback, &surfaceDesc.ddpfPixelFormat);
    if (!surfaceDesc.ddpfPixelFormat.dwRGBBitCount) {
        TexFmtSearchType = TEXTURE_FMT_16BIT_ALPHA;
        D3DDevice->EnumTextureFormats(TextureSearchCallback, &surfaceDesc.ddpfPixelFormat);
        if (!surfaceDesc.ddpfPixelFormat.dwRGBBitCount)
            return;
    }

    IDirectDrawSurface7 *ppRenderTarget;
    D3DDevice->GetRenderTarget(&ppRenderTarget);
    ppRenderTarget->GetDDInterface((LPVOID *)&DDraw);
    ppRenderTarget->Release();

    if (DDraw->CreateSurface(&surfaceDesc, (IDirectDrawSurface7 **)(texture), 0) < 0)
        return;

    DDSURFACEDESC2 lockDesc;
    memset(&lockDesc, 0, sizeof(lockDesc));
    lockDesc.dwSize = sizeof(lockDesc);

    WORD alphaMask = (WORD)surfaceDesc.ddpfPixelFormat.dwRGBAlphaBitMask;

    IDirectDrawSurface7 *surfacePtr = (IDirectDrawSurface7 *)(*texture);
    if (useTexMips == true) {
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

            if (TexFmtSearchType == TEXTURE_FMT_16BIT_ALPHA) {
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
        ((IDirectDrawSurface7 *)(*texture))->GetDC(&hdc);
        StretchDIBits(hdc, 0, 0, bmWidth, bmHeight, 0, 0, bmWidth, bmHeight, FreeImage_GetBits(fBitmap32), FreeImage_GetInfo(fBitmap32),
                      DIB_RGB_COLORS, SRCCOPY);
        ((IDirectDrawSurface7 *)(*texture))->ReleaseDC(hdc);

        if (TexFmtSearchType == TEXTURE_FMT_16BIT_ALPHA) {
            ((IDirectDrawSurface7 *)(*texture))->Lock(0, &lockDesc, DDLOCK_WAIT, 0);

            byte *dstRow = (byte *)lockDesc.lpSurface;
            int srcLine  = lockDesc.dwHeight - 1;

            for (int y = 0; y < lockDesc.dwHeight; ++y, --srcLine, dstRow += lockDesc.lPitch) {
                ushort *p = (ushort *)dstRow;
                byte *src = FreeImage_GetScanLine(fBitmap32, srcLine);

                for (int x = 0; x < lockDesc.dwWidth; ++x, ++p, src += 4) {
                    *p = (src[3] == 255) ? (*p | alphaMask) : 0;
                }
            }

            ((IDirectDrawSurface7 *)(*texture))->Unlock(0);
        }
    }

    FreeImage_Unload(fBitmap32);
    FreeImage_Unload(fBitmap);
#else
#if RETRO_USE_SDL3
    SDL_Surface *image = SDL_LoadPNG(path);
#else
    SDL_Surface *image = IMG_Load(path);
#endif
    if (image == NULL)
        return;

#if RETRO_USE_SDL3
    SDL_Surface *image32 = SDL_ConvertSurface(image, SDL_PIXELFORMAT_BGRA32);
    SDL_DestroySurface(image);
#else
    SDL_Surface *image32 = SDL_ConvertSurfaceFormat(image, SDL_PIXELFORMAT_BGRA32, 0);
    SDL_FreeSurface(image);
#endif
    if (image32 == NULL)
        return;

    (*texture) = new Texture();

    (*texture)->vtbl   = NULL;
    (*texture)->width  = image32->w;
    (*texture)->height = image32->h;

    glGenTextures(1, &(*texture)->id);
    glBindTexture(GL_TEXTURE_2D, (*texture)->id);

    if (useTexMips == true)
        glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (*texture)->width, (*texture)->height, 0, GL_BGRA, GL_UNSIGNED_BYTE, image32->pixels);

    if (useTexMips == true)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    else
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

#if RETRO_USE_SDL3
    SDL_DestroySurface(image32);
#else
    SDL_FreeSurface(image32);
#endif
#endif
}

void LoadLevelModel(LMF *model, const char *path)
{
    memset(model, 0, sizeof(*model));

    FILE *stream = fopen(path, "rb");
    fread(&model->surfaceCount, sizeof(model->surfaceCount), 1, stream);
    fread(&model->columns, sizeof(model->columns), 1, stream);
    fread(&model->rows, sizeof(model->rows), 1, stream);
    fread(&model->startX, sizeof(model->startX), 1, stream);
    fread(&model->startZ, sizeof(model->startZ), 1, stream);
    fread(&model->unused, sizeof(model->unused), 1, stream);

    for (int i = 0; i < model->surfaceCount; ++i) {
        fread(&model->surfaceID[i], sizeof(model->surfaceID[i]), 1, stream);
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

                fread(&tile->numVertices, sizeof(tile->numVertices), 1, stream);
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

                    fread(&vert, sizeof(vert), 1, stream);

                    vert.color += 1.0f;
                    vert.color *= 0.375f;
                    vert.color += 0.25f;

                    Color color = PACK_ARGB_F(1.0f, vert.color, vert.color, vert.color);

                    tile->vertices[v] = { vert.v, color, 0, vert.tu, vert.tv };
                    tile->colors[v]   = vert.color;
                }

                fread(&tile->numIndices, sizeof(tile->numIndices), 1, stream);
                tile->indices = new ushort[tile->numIndices];

                for (int v = 0; v < tile->numIndices; ++v) {
                    fread(&tile->indices[v], sizeof(tile->indices[v]), 1, stream);
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

    fclose(stream);
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

    FILE *stream = fopen(path, "rb");
    fseek(stream, 0, 0);

    fread(&model->numVertices, 2, 1, stream);
    model->vertices = new Vertex[model->numVertices + 1];
    if (model->vertices != NULL)
        memset(model->vertices, 0, sizeof(*model->vertices));

    for (int i = 0; i < model->numVertices; ++i) {
        fread(&model->vertices[i], sizeof(model->vertices[i]), 1, stream);
    }

    fread(&model->numIndices, sizeof(model->numIndices), 1, stream);
    model->indices = new ushort[model->numIndices + 2];
    for (int i = 0; i < model->numIndices; ++i) {
        fread(&model->indices[i], sizeof(model->indices[i]), 1, stream);
    }

    fclose(stream);
}

void Load_ANI_File(Animation *animation, const char *path)
{
    memset(animation, 0, sizeof(*animation));

    char boneName[256];

    FILE *stream = fopen(path, "rb");
    fseek(stream, 0, SEEK_SET);

    byte frameCount;
    ushort nodeCount;
    fread(&frameCount, sizeof(byte), 1, stream);
    fread(&nodeCount, sizeof(ushort), 1, stream);

    for (int i = 0; i < frameCount; ++i) {
        byte nameLen;
        fread(&nameLen, sizeof(byte), 1, stream);
        fread(boneName, sizeof(byte), nameLen, stream);

        fread(&animation->nodes[i].position.x, sizeof(float), 1, stream);
        fread(&animation->nodes[i].position.y, sizeof(float), 1, stream);
        fread(&animation->nodes[i].position.z, sizeof(float), 1, stream);

        fread(&animation->nodes[i].vertexCount, sizeof(ushort), 1, stream);
        animation->nodes[i].vertexIDs = new ushort[animation->nodes[i].vertexCount];

        for (int j = 0; j < animation->nodes[i].vertexCount; ++j) {
            fread(&animation->nodes[i].vertexIDs[j], sizeof(ushort), 1, stream);
        }

        for (int j = 0; j < nodeCount; ++j) {
            byte idk;
            ushort val;

            fread(&idk, sizeof(byte), 1, stream);
            fread(&val, sizeof(ushort), 1, stream);
            animation->nodes[i].rotX[j] = (float)((idk ? (int)val : -(int)val) * (RSDK_PI / 180.0));

            fread(&idk, sizeof(byte), 1, stream);
            fread(&val, sizeof(ushort), 1, stream);
            animation->nodes[i].rotY[j] = (float)((idk ? (int)val : -(int)val) * (RSDK_PI / 180.0));

            fread(&idk, sizeof(byte), 1, stream);
            fread(&val, sizeof(ushort), 1, stream);
            animation->nodes[i].rotZ[j] = (float)((idk ? (int)val : -(int)val) * (RSDK_PI / 180.0));
        }
    }

    fread(&animation->frameIDCount, sizeof(ushort), 1, stream);
    animation->frameIDs = new byte[animation->frameIDCount];

    for (int i = 0; i < animation->frameIDCount; ++i) {
        fread(&animation->frameIDs[i], sizeof(byte), 1, stream);
    }

    byte stateCount;
    fread(&stateCount, sizeof(byte), 1, stream);

    for (int i = 0; i < stateCount; ++i) {
        byte nameLen;
        fread(&nameLen, sizeof(byte), 1, stream);
        fread(&boneName, sizeof(byte), nameLen, stream);

        fread(&animation->states[i].frameDuration, sizeof(byte), 1, stream);
        fread(&animation->states[i].loopIndex, sizeof(byte), 1, stream);
        fread(&animation->states[i].frameCount, sizeof(byte), 1, stream);

        for (int j = 0; j < animation->states[i].frameCount; ++j) {
            fread(&animation->states[i].array_2[j], sizeof(ushort), 1, stream);
        }
    }

    animation->field_BFAA = 0;
    animation->field_BFAB = 0;

    fclose(stream);
}

#if 0
int *__cdecl TODO_READER_FUNC_401A87(int *a1, char *FileName)
{
  int v3; // [esp+Ch] [ebp-BFD0h]
  int v4; // [esp+Ch] [ebp-BFD0h]
  int v5; // [esp+10h] [ebp-BFCCh] BYREF
  int j; // [esp+14h] [ebp-BFC8h]
  int i; // [esp+18h] [ebp-BFC4h]
  FILE *Stream; // [esp+1Ch] [ebp-BFC0h]
  int Buffer; // [esp+20h] [ebp-BFBCh] BYREF
  R3D::Animation v10; // [esp+24h] [ebp-BFB8h] BYREF
  int v11; // [esp+BFD8h] [ebp-4h] BYREF

  Stream = fopen(FileName, "rb");
  fseek(Stream, 0, 0);
  fread(&Buffer, 1u, 1u, Stream);
  v3 = Buffer;
  fread(&v11, 2u, 1u, Stream);
  for ( i = 0; i < v3; ++i )
  {
    fread(&Buffer, 1u, 1u, Stream);
    v5 = Buffer;
    for ( j = 0; j < v5; ++j )
      fread(&Buffer, 1u, 1u, Stream);
    fread(&v10.frames[i].field_8, 4u, 1u, Stream);
    fread(&v10.frames[i].field_8.y, 4u, 1u, Stream);
    fread(&v10.frames[i].field_8.z, 4u, 1u, Stream);
    fread(&v5, 2u, 1u, Stream);
    v10.frames[i].count = v5;
    for ( j = 0; j < v10.frames[i].count; ++j )
    {
      fread(&v5, 2u, 1u, Stream);
      v10.frames[i].field_0[j] = v5;
    }
    for ( j = 0; j < v11; ++j )
    {
      fread(&v5, 2u, 1u, Stream);
      v10.frames[i].rotX[j] = v5 * (3.1415927 / 180.0);
      fread(&v5, 2u, 1u, Stream);
      v10.frames[i].rotY[j] = v5 * (3.1415927 / 180.0);
      fread(&v5, 2u, 1u, Stream);
      v10.frames[i].rotZ[j] = v5 * (3.1415927 / 180.0);
    }
  }
  fread(&v5, 2u, 1u, Stream);
  v10.field_BFA8 = v5;
  v10.frameIDs = operator new(v5);
  for ( i = 0; i < v10.field_BFA8; ++i )
    fread(&v10.frameIDs[i], 1u, 1u, Stream);
  fread(&Buffer, 1u, 1u, Stream);
  v4 = Buffer;
  for ( i = 0; i < v4; ++i )
  {
    fread(&Buffer, 1u, 1u, Stream);
    v5 = Buffer;
    for ( j = 0; j < v5; ++j )
      fread(&Buffer, 1u, 1u, Stream);
    fread(&v10.array_AB90[i].field_201, 1u, 1u, Stream);
    fread(&v10.array_AB90[i].field_200, 1u, 1u, Stream);
    fread(&v10.array_AB90[i], 1u, 1u, Stream);
    for ( j = 0; j < v10.array_AB90[i].count; ++j )
      fread(&v10.array_AB90[i].array_2[j], 2u, 1u, Stream);
  }
  fclose(Stream);
  qmemcpy(a1, &v10, 0xBFB4u);
  return a1;
}
#endif
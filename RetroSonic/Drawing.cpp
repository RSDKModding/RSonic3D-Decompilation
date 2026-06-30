#include "RetroEngine.hpp"

tagRECT rect_420488;
tagRECT clientRect;
tagRECT windowBounds;

D3DMATRIX MatrixSonicAni_4C8990[36];
D3DMATRIX MatrixSonicAni_4C9290[36];

D3DMATRIX MatrixSonicModel;
D3DMATRIX MatrixInversed;
D3DMATRIX MatrixWorld;
D3DMATRIX MatrixView;
D3DMATRIX MatrixProjection;
D3DMATRIX MatrixIdentity;

LPDIRECTDRAWSURFACE7 surfaceList[10];
LPDIRECTDRAWSURFACE7 surface_4C9D3C;
LPDIRECTDRAWSURFACE7 surface_439D40;
LPDIRECTDRAWSURFACE7 surface_4C9D44;

char *StrRenderFailError;
TextureFormatSearchType TexFmtSearchType;

char WindowMode      = 1;
int32_t dword_41F0C4 = 1;

sbyte ResX = 80;
sbyte ResY = 82;
sbyte ColourDepth;


void UpdateWindowRect(HWND hWnd)
{
    if (WindowMode == 1) {
        GetClientRect(hWnd, &clientRect);
        GetClientRect(hWnd, &windowBounds);
        ClientToScreen(hWnd, (POINT *)&windowBounds.left);
        ClientToScreen(hWnd, (POINT *)&windowBounds.right);
    }
}

void ResetWindow(HWND hWnd)
{
    if (WindowMode == 1)
        GetWindowRect(hWnd, &rect_420488);

    RenderDevice::RefreshSurfaces(hWnd);
}

void DrawTitleModel(char type)
{
    using namespace RenderDevice; // TODO: REMOVE

    RenderDevice::SetMaterial(&material_420520);

    if (type == 1) {
        RenderDevice::SetTexture(0, surface3DLogo);
        RenderDevice::DrawIndexed(RENDER_FVF_VERTEX, LogoMdl.vertices, LogoMdl.numVertices, LogoMdl.indices, LogoMdl.numIndices);
    }
    else if (type == 0) {
        RenderDevice::SetTexture(0, surfaceTestZoneBG);
        RenderDevice::DrawIndexed(RENDER_FVF_VERTEX, BackgroundMdl.vertices, BackgroundMdl.numVertices, BackgroundMdl.indices,
                                  BackgroundMdl.numIndices);
    }
}

void CopyMatrix_4C9B90_4C9C50() { memcpy(&MatrixSonicModel, &MatrixIdentity, sizeof(MatrixSonicModel)); }

void SonicMat_WorldTransform()
{
    using namespace RenderDevice; // TODO: REMOVE

    RenderDevice::SetTransform(RENDER_TRANSFORM_WORLD, &MatrixSonicModel);
}

void ReleaseSurfaceID(uint8_t id)
{
    if (surfaceList[id]) {
        surfaceList[id]->Release();
        surfaceList[id] = NULL;
    }
}

void ReleaseModelSurfaces()
{
    using namespace RenderDevice; // TODO: REMOVE

    for (int32_t i = 0; i < 5; ++i) {
        if (surfaceCharacters[i]) {
            surfaceCharacters[i]->Release();
            surfaceCharacters[i] = NULL;
        }
    }

    if (unk_420500) {
        unk_420500->Release();
        unk_420500 = NULL;
    }

    if (surfaceSonic) {
        surfaceSonic->Release();
        surfaceSonic = NULL;
    }

    if (surfaceTestZoneBG) {
        surfaceTestZoneBG->Release();
        surfaceTestZoneBG = NULL;
    }

    if (surface3DLogo) {
        surface3DLogo->Release();
        surface3DLogo = NULL;
    }

    if (surfaceMText) {
        surfaceMText->Release();
        surfaceMText = NULL;
    }

    if (surfaceShadow) {
        surfaceShadow->Release();
        surfaceShadow = NULL;
    }

    if (surface_4C9D44) {
        surface_4C9D44->Release();
        surface_4C9D44 = NULL;
    }

    if (D3DDevice) {
        IDirect3DDevice7_Release(D3DDevice);
        D3DDevice = NULL;
    }

    if (D3D) {
        D3D->Release();
        D3D = NULL;
    }
}

void ReleaseGraphicsAPI()
{
    using namespace RenderDevice; // TODO: REMOVE

    ReleaseModelSurfaces();

    for (int i = 0; i < 10; ++i) {
        ReleaseSurfaceID(i);
    }

    if (DDraw != NULL) {
        IDirectDraw7_Release(DDraw);
        DDraw = NULL;
    }

    if (surface_439D40 != NULL) {
        IDirectDrawSurface7_Release(surface_439D40);
        surface_439D40 = NULL;
    }

    if (surface_4C9D3C != NULL) {
        IDirectDrawSurface7_Release(surface_4C9D3C);
        surface_4C9D3C = NULL;
    }
}
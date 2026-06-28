#include "RetroEngine.hpp"

tagRECT rect_420488  = { 0, 0, 0, 0 };
tagRECT clientRect   = { 0, 0, 0, 0 };
tagRECT windowBounds = { 0, 0, 0, 0 };

D3DMATRIX matrix_4C8990[36];
D3DMATRIX matrix_4C9290[36];
D3DMATRIX matSonicMdl;
D3DMATRIX matrixBackgroundTransform;
D3DMATRIX matProject;
D3DMATRIX matWorld;
D3DMATRIX matrix_4C9C90;
D3DMATRIX matView;
LPDIRECTDRAWSURFACE7 surfaceList[10];

LPDIRECTDRAW7 DDraw;
HRESULT ddState = S_FALSE;

LPDIRECTDRAWSURFACE7 surface_4C9D3C;
LPDIRECTDRAWSURFACE7 surface_439D40;
LPDIRECTDRAWSURFACE7 surface_4C9D44;
IDirect3DDevice7 *D3DDevice;
IDirect3D7 *d3d;

char *StrRenderFailError;
TextureFormatSearchType TexFmtSearchType;

char WindowMode      = 1;
int32_t dword_41F0C4 = 1;

sbyte ResX = 80;
sbyte ResY = 82;
sbyte ColourDepth;

BOOL TryInitDirectDraw(HWND hWnd)
{
    ddState = DirectDrawCreateEx(NULL, (LPVOID *)&DDraw, IID_IDirectDraw7, NULL);
    if (ddState) {
        StrRenderFailError = (char *)"DirectDrawCreate FAILED";
        return FALSE;
    }

    ddState = InitDrawSurface(hWnd);
    return ddState >= 0;
}

BOOL TryInitRenderDevice() { return InitScreen() != FALSE; }

BOOL InitDrawSurface(HWND hWnd)
{
    BOOL result;
    HRESULT r;
    IDirectDrawClipper *ddclipper;
    DDSURFACEDESC2 surfaceDesc_1;
    DDSURFACEDESC2 surfaceDesc_2;
    DDSCAPS2 caps;
    DDBLTFX bltFx_1;
    DDBLTFX bltFx_2;

    if (WindowMode == 1) {
        r = DDraw->SetCooperativeLevel(hWnd, 8);
        UpdateWindowRect(hWnd);

        memset(&surfaceDesc_1, 0, sizeof(surfaceDesc_1));
        surfaceDesc_1.dwSize         = 124;
        surfaceDesc_1.dwFlags        = 1;
        surfaceDesc_1.ddsCaps.dwCaps = 8704;

        r = DDraw->CreateSurface(&surfaceDesc_1, &surface_4C9D3C, NULL);
        r = DDraw->CreateClipper(0, &ddclipper, 0);

        ddclipper->SetHWnd(0, hWnd);
        surface_4C9D3C->SetClipper(ddclipper);

        ddclipper->Release();
        ddclipper = NULL;

        memset(&surfaceDesc_2, 0, sizeof(surfaceDesc_2));
        surfaceDesc_2.dwSize         = 124;
        surfaceDesc_2.dwFlags        = 7;
        surfaceDesc_2.ddsCaps.dwCaps = 8256;
        surfaceDesc_2.dwWidth        = 640;
        surfaceDesc_2.dwHeight       = 480;

        r = DDraw->CreateSurface(&surfaceDesc_2, &surface_439D40, NULL);
    }
    else {
        r = DDraw->SetCooperativeLevel(hWnd, 17);
        r = DDraw->SetDisplayMode(ResX, ResY, 32, 0, 0);

        memset(&surfaceDesc_1, 0, sizeof(surfaceDesc_1));
        surfaceDesc_1.dwSize            = 124;
        surfaceDesc_1.dwFlags           = 33;
        surfaceDesc_1.ddsCaps.dwCaps    = 8728;
        surfaceDesc_1.dwBackBufferCount = 1;

        r = DDraw->CreateSurface(&surfaceDesc_1, &surface_4C9D3C, 0);

        caps.dwCaps  = 4;
        caps.dwCaps2 = 0;
        caps.dwCaps3 = 0;
        caps.dwCaps4 = 0;

        r = surface_4C9D3C->GetAttachedSurface(&caps, &surface_439D40);

        memset(&bltFx_1, 0, sizeof(bltFx_1));
        bltFx_1.dwSize      = 100;
        bltFx_1.dwFillColor = 0;

        surface_439D40->Blt(0, 0, 0, 16778240, &bltFx_1);
    }

    DDraw->GetDisplayMode(&surfaceDesc_1);

    if (surfaceDesc_1.ddpfPixelFormat.dwRGBBitCount == 24 || surfaceDesc_1.ddpfPixelFormat.dwRGBBitCount == 8) {
        MessageBoxA(hWnd, "This Colour Depth is not suitable for Retro-Sonic. Please use either 16bit or 32bit colour mode", "Colour Depth", 0x10);
        result = FALSE;
    }
    else {
        memset(&bltFx_2, 0, sizeof(bltFx_2));
        bltFx_2.dwSize      = 100;
        bltFx_2.dwFillColor = 0;

        surface_439D40->Blt(0, 0, 0, 16778240, &bltFx_2);

        if (HasGameStarted != TRUE || TryInitRenderDevice())
            result = FALSE;
        else
            result = FALSE;
    }

    return result;
}

BOOL InitScreen()
{
    SupportsZBufferFmt = false;
    if (DDraw->QueryInterface(IID_IDirect3D7, (LPVOID *)&d3d) < 0)
        return FALSE;

    DDPIXELFORMAT fmt;
    memset(&fmt, 0, sizeof(fmt));

    fmt.dwSize  = 32;
    fmt.dwFlags = 1024;

    d3d->EnumZBufferFormats(IID_IDirect3DTnLHalDevice, EnumZBufferFormatsCallback, &fmt);
    if (SupportsZBufferFmt == true) {
        D3DDeviceType = D3D_DEVICE_TNL_HAL;
    }
    else {
        d3d->EnumZBufferFormats(IID_IDirect3DHALDevice, EnumZBufferFormatsCallback, &fmt);
        if (SupportsZBufferFmt == true) {
            D3DDeviceType = D3D_DEVICE_HAL;
        }
        else {
            d3d->EnumZBufferFormats(IID_IDirect3DRGBDevice, EnumZBufferFormatsCallback, &fmt);
            if (SupportsZBufferFmt == true)
                D3DDeviceType = D3D_DEVICE_RGB;
        }
    }

    DDSURFACEDESC2 ddSurfaceDesc;
    memset(&ddSurfaceDesc, 0, sizeof(ddSurfaceDesc));

    ddSurfaceDesc.dwSize  = 124;
    ddSurfaceDesc.dwFlags = 4103;

    if (D3DDeviceType <= -3)
        ddSurfaceDesc.ddsCaps.dwCaps = 133120;
    else
        ddSurfaceDesc.ddsCaps.dwCaps = 147456;

    if (WindowMode == 1) {
        ddSurfaceDesc.dwWidth  = 640;
        ddSurfaceDesc.dwHeight = 480;
    }
    else if (WindowMode == 0) {
        ddSurfaceDesc.dwWidth  = ResX;
        ddSurfaceDesc.dwHeight = ResY;
    }

    memcpy(&ddSurfaceDesc.ddpfPixelFormat, &fmt, sizeof(ddSurfaceDesc.ddpfPixelFormat));
    if (!DDraw->CreateSurface(&ddSurfaceDesc, &surface_4C9D44, 0))
        surface_439D40->AddAttachedSurface(surface_4C9D44);

    if (d3d->CreateDevice(IID_IDirect3DTnLHalDevice, surface_439D40, &D3DDevice)) {
        if (d3d->CreateDevice(IID_IDirect3DHALDevice, surface_439D40, &D3DDevice)) {
            if (d3d->CreateDevice(IID_IDirect3DRGBDevice, surface_439D40, &D3DDevice)) {
                return 0;
            }
            dword_41F0C4 = 0;
        }
        else {
            dword_41F0C4 = 1;
        }
    }
    else {
        dword_41F0C4 = 1;
    }

    D3DVIEWPORT7 pViewport;
    pViewport.dwX = 0;
    pViewport.dwY = 0;

    if (WindowMode) {
        pViewport.dwWidth  = 640;
        pViewport.dwHeight = 480;
    }
    else {
        pViewport.dwWidth  = ResX;
        pViewport.dwHeight = ResY;
    }

    pViewport.dvMinZ = 0.0;
    pViewport.dvMaxZ = 1.0;

    if (D3DDevice->SetViewport(&pViewport))
        return FALSE;

    IdentityMatrix(&matWorld);

    D3DMATRIX mTemp;
    memcpy(&mTemp, &matWorld, sizeof(mTemp));
    D3DDevice->SetTransform(D3DTRANSFORMSTATE_WORLD, &mTemp);

    memcpy(&matView, &matWorld, sizeof(matView));
    D3DDevice->SetTransform(D3DTRANSFORMSTATE_VIEW, &matView);

    memset(&matProject, 0, sizeof(matProject));
    MatrixPerspective(&matProject, TO_RADIAN(45.0f), 0.75f, 1.0f, 1000.0f);

    D3DLIGHT7 d3dLight;
    memset(&d3dLight, 0, sizeof(d3dLight));

    d3dLight.dvDirection.x = 0.0f;
    d3dLight.dvDirection.y = -0.89999998f;
    d3dLight.dvDirection.z = 0.1f;
    d3dLight.dltType       = D3DLIGHT_DIRECTIONAL;
    d3dLight.dcvDiffuse.r  = 0.80000001f;
    d3dLight.dcvDiffuse.g  = 0.80000001f;
    d3dLight.dcvDiffuse.b  = 0.80000001f;
    d3dLight.dcvAmbient.r  = 0.44999999f;
    d3dLight.dcvAmbient.g  = 0.44999999f;
    d3dLight.dcvAmbient.b  = 0.44999999f;
    d3dLight.dcvSpecular.r = 1.0f;
    d3dLight.dcvSpecular.g = 1.0f;
    d3dLight.dcvSpecular.b = 1.0f;

    D3DDevice->SetLight(0, &d3dLight);
    D3DDevice->LightEnable(0, TRUE);

    material_420520.dcvAmbient.r  = 1.0f;
    material_420520.dcvAmbient.g  = 1.0f;
    material_420520.dcvAmbient.b  = 1.0f;
    material_420520.dcvAmbient.a  = 1.0f;
    material_420520.dcvDiffuse    = material_420520.dcvAmbient;
    material_420520.dcvSpecular.r = 0.25f;
    material_420520.dcvSpecular.g = 0.25f;
    material_420520.dcvSpecular.b = 1.0f;
    material_420520.dcvSpecular.a = 0.25f;
    material_420520.power         = 80.0f;

    D3DDevice->SetTransform(D3DTRANSFORMSTATE_PROJECTION, &matProject);
    D3DDevice->SetRenderState(D3DRENDERSTATE_DITHERENABLE, 1);

    D3DDevice->SetTextureStageState(0, D3DTSS_MINFILTER, D3DTOP_SELECTARG1);
    D3DDevice->SetTextureStageState(0, D3DTSS_MAGFILTER, D3DTOP_SELECTARG1);
    D3DDevice->SetTextureStageState(0, D3DTSS_MIPFILTER, D3DTOP_SELECTARG1);

    D3DDevice->SetRenderState(D3DRENDERSTATE_ZENABLE, TRUE);
    D3DDevice->SetRenderState(D3DRENDERSTATE_LIGHTING, TRUE);

    surfaceTestZoneBG = Load_PNG_File("Data/Levels/TestZone/BG.png", 0);
    surfaceSonic      = Load_PNG_File("Data/Characters/Sonic.png", 0);
    surface3DLogo     = Load_PNG_File("Data/Title/Logo.png", 0);
    surfaceShadow     = Load_PNG_File("Data/Objects/Shadow.png", 0);

    float_420428 = 4.0f;
    float_42042C = 4.0f;
    float_420430 = 4.0f;

    D3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
    D3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTOP_SELECTARG1);
    D3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTOP_DISABLE);

    D3DDevice->SetRenderState(D3DRENDERSTATE_ALPHATESTENABLE, 1);
    D3DDevice->SetRenderState(D3DRENDERSTATE_ALPHAREF, 0);
    D3DDevice->SetRenderState(D3DRENDERSTATE_ALPHAFUNC, 7);
    D3DDevice->SetRenderState(D3DRENDERSTATE_SRCBLEND, 5);
    D3DDevice->SetRenderState(D3DRENDERSTATE_DESTBLEND, 6);
    D3DDevice->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, 1);
    D3DDevice->SetRenderState(D3DRENDERSTATE_DESTBLEND, 6);

    if (!dword_41F0C4)
        D3DDevice->SetRenderState(D3DRENDERSTATE_TEXTUREPERSPECTIVE, 0);

    return TRUE;
}

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

    ddState = RefreshSurfaces(hWnd);
}

HRESULT BeginScene() { return IDirect3DDevice7_BeginScene(D3DDevice); }

HRESULT EndScene() { return IDirect3DDevice7_EndScene(D3DDevice); }

void DrawTitleModel(char type)
{
    IDirect3DDevice7_SetMaterial(D3DDevice, &material_420520);

    if (type == 1) {
        IDirect3DDevice7_SetTexture(D3DDevice, 0, surface3DLogo);
        IDirect3DDevice7_DrawIndexedPrimitive(D3DDevice, D3DPT_TRIANGLELIST, D3DFVF_VERTEX, LogoMdl.vertices, LogoMdl.numVertices, LogoMdl.indices,
                                              LogoMdl.numIndices, 0);
    }
    else if (type == 0) {
        IDirect3DDevice7_SetTexture(D3DDevice, 0, surfaceTestZoneBG);
        IDirect3DDevice7_DrawIndexedPrimitive(D3DDevice, D3DPT_TRIANGLELIST, D3DFVF_VERTEX, BackgroundMdl.vertices, BackgroundMdl.numVertices,
                                              BackgroundMdl.indices, BackgroundMdl.numIndices, 0);
    }
}

void CopyMatrix_4C9B90_4C9C50() { memcpy(&matSonicMdl, &matWorld, sizeof(matSonicMdl)); }

void SonicMat_WorldTransform() { IDirect3DDevice7_SetTransform(D3DDevice, D3DTRANSFORMSTATE_WORLD, &matSonicMdl); }

HRESULT Render_ClearScreen(D3DCOLOR color) { return IDirect3DDevice7_Clear(D3DDevice, 0, 0, 3, color, 1.0f, 0); }

void SetFade(float a1, float a2, float a3, float a4)
{
    D3DLVERTEX vertices[4];
    memset(&vertices, 0, sizeof(vertices));

    WORD indices[6];
    indices[0] = 0;
    indices[1] = 1;
    indices[2] = 2;
    indices[3] = 0;
    indices[4] = 2;
    indices[5] = 3;

    D3DCOLOR color = (__int64)(a3 * 255.0) | ((unsigned int)(__int64)(a2 * 255.0) << 8) | ((unsigned int)(__int64)(a1 * 255.0) << 16)
                     | ((unsigned int)(__int64)(a4 * 255.0) << 24);

    D3DVECTOR position_0(-1.0f, -1.0f, 1.0f);
    D3DVECTOR position_1(-1.0f, 1.0f, 1.0f);
    D3DVECTOR position_2(1.0f, 1.0f, 1.0f);
    D3DVECTOR position_3(1.0f, -1.0f, 1.0f);

    vertices[0] = D3DLVERTEX(position_0, color, 0.0f, 0.0f, 1.0f);
    vertices[1] = D3DLVERTEX(position_1, color, 0.0f, 0.0f, 0.0f);
    vertices[2] = D3DLVERTEX(position_2, color, 0.0f, 1.0f, 0.0f);
    vertices[3] = D3DLVERTEX(position_3, color, 0.0f, 1.0f, 0.0f);

    IDirect3DDevice7_BeginScene(D3DDevice);
    IDirect3DDevice7_SetTexture(D3DDevice, 0, NULL);
    IDirect3DDevice7_DrawIndexedPrimitive(D3DDevice, D3DPT_TRIANGLELIST, D3DFVF_LVERTEX, vertices, 4, indices, 6, 0);
    IDirect3DDevice7_EndScene(D3DDevice);
}

void FlipScreen()
{
    if (WindowMode == 1)
        IDirectDrawSurface7_Blt(surface_4C9D3C, &windowBounds, surface_439D40, 0, 0x1000000, 0);
    else if (WindowMode == 0)
        IDirectDrawSurface7_Flip(surface_4C9D3C, 0, 1);
}

void ReleaseSurfaceID(uint8_t id)
{
    if (surfaceList[id]) {
        surfaceList[id]->Release();
        surfaceList[id] = NULL;
    }
}

HRESULT RefreshSurfaces(HWND hWnd)
{
    DDraw->SetCooperativeLevel(hWnd, 8);

    for (int32_t i = 0; i < 5; ++i) {
        if (surfaceCharacters[i]) {
            surfaceCharacters[i]->Release();
            surfaceCharacters[i] = NULL;
        }
    }

    if (HasGameStarted == TRUE)
        ReleaseModelSurfaces();

    if (surface_439D40) {
        surface_439D40->Release();
        surface_439D40 = NULL;
    }

    if (WindowMode) {
        if (WindowMode == 1) {
            WindowMode = 0;
            ShowWindow(hWnd, 0);
            SetWindowLongA(hWnd, -16, 0x80000000);
            hMenu = GetMenu(hWnd);
            SetMenu(hWnd, 0);
            ShowCursor(0);
        }
    }
    else {
        WindowMode = 1;
        ShowWindow(hWnd, 0);
        SetWindowLongA(hWnd, -16, 13565952);
        SetMenu(hWnd, hMenu);
        ShowCursor(1);
    }

    ShowWindow(hWnd, 5);

    if (surface_4C9D3C) {
        surface_4C9D3C->Release();
        surface_4C9D3C = NULL;
    }

    if (WindowMode == 1) {
        SetWindowPos(hWnd, HWND_NOTOPMOST, rect_420488.left, rect_420488.top, rect_420488.right - rect_420488.left,
                     rect_420488.bottom - rect_420488.top, 0x40);
    }

    return InitDrawSurface(hWnd);
}

void ReleaseModelSurfaces()
{
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

    if (d3d) {
        d3d->Release();
        d3d = NULL;
    }
}

void ReleaseGraphicsAPI()
{
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
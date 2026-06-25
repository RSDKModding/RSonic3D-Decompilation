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

LPDIRECTDRAW7 dd;
HRESULT ddState = S_FALSE;

LPDIRECTDRAWSURFACE7 surface_4C9D3C;
LPDIRECTDRAWSURFACE7 surface_439D40;
LPDIRECTDRAWSURFACE7 surface_4C9D44;
IDirect3DDevice7 *dx7Device;
IDirect3D7 *d3d;

char *StrRenderFailError;
char retryTextureEnum;

char WindowMode      = 1;
int32_t dword_41F0C4 = 1;

sbyte ResX = 80;
sbyte ResY = 82;
sbyte ColourDepth;

BOOL TryInitDirectDraw(HWND hWnd)
{
    ddState = DirectDrawCreateEx(NULL, (LPVOID *)&dd, IID_IDirectDraw7, NULL);
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
        r = dd->SetCooperativeLevel(hWnd, 8);
        UpdateWindowRect(hWnd);

        memset(&surfaceDesc_1, 0, sizeof(surfaceDesc_1));
        surfaceDesc_1.dwSize         = 124;
        surfaceDesc_1.dwFlags        = 1;
        surfaceDesc_1.ddsCaps.dwCaps = 8704;

        r = dd->CreateSurface(&surfaceDesc_1, &surface_4C9D3C, NULL);
        r = dd->CreateClipper(0, &ddclipper, 0);

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

        r = dd->CreateSurface(&surfaceDesc_2, &surface_439D40, NULL);
    }
    else {
        r = dd->SetCooperativeLevel(hWnd, 17);
        r = dd->SetDisplayMode(ResX, ResY, 32, 0, 0);

        memset(&surfaceDesc_1, 0, sizeof(surfaceDesc_1));
        surfaceDesc_1.dwSize            = 124;
        surfaceDesc_1.dwFlags           = 33;
        surfaceDesc_1.ddsCaps.dwCaps    = 8728;
        surfaceDesc_1.dwBackBufferCount = 1;

        r = dd->CreateSurface(&surfaceDesc_1, &surface_4C9D3C, 0);

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

    dd->GetDisplayMode(&surfaceDesc_1);

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
    data_4C9D80 = 0;
    if (dd->QueryInterface(IID_IDirect3D7, (LPVOID *)&d3d) < 0)
        return FALSE;

    DDPIXELFORMAT fmt;
    memset(&fmt, 0, sizeof(fmt));

    fmt.dwSize  = 32;
    fmt.dwFlags = 1024;

    d3d->EnumZBufferFormats(IID_IDirect3DTnLHalDevice, ldUnknownCallback, &fmt);

    if (data_4C9D80 == 1) {
        data_4C9D84 = 1;
    }
    else {
        d3d->EnumZBufferFormats(IID_IDirect3DHALDevice, ldUnknownCallback, &fmt);

        if (data_4C9D80 == 1) {
            data_4C9D84 = 2;
        }
        else {
            d3d->EnumZBufferFormats(IID_IDirect3DRGBDevice, ldUnknownCallback, &fmt);

            if (data_4C9D80 == 1)
                data_4C9D84 = 3;
        }
    }

    DDSURFACEDESC2 ddSurfaceDesc;
    memset(&ddSurfaceDesc, 0, sizeof(ddSurfaceDesc));

    ddSurfaceDesc.dwSize  = 124;
    ddSurfaceDesc.dwFlags = 4103;

    if (data_4C9D84 <= -3)
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
    if (!dd->CreateSurface(&ddSurfaceDesc, &surface_4C9D44, 0))
        surface_439D40->AddAttachedSurface(surface_4C9D44);

    if (d3d->CreateDevice(IID_IDirect3DTnLHalDevice, surface_439D40, &dx7Device)) {
        if (d3d->CreateDevice(IID_IDirect3DHALDevice, surface_439D40, &dx7Device)) {
            if (d3d->CreateDevice(IID_IDirect3DRGBDevice, surface_439D40, &dx7Device)) {
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

    if (dx7Device->SetViewport(&pViewport))
        return FALSE;

    IdentityMatrix(&matWorld);

    D3DMATRIX mTemp;
    memcpy(&mTemp, &matWorld, sizeof(mTemp));
    dx7Device->SetTransform(D3DTRANSFORMSTATE_WORLD, &mTemp);

    memcpy(&matView, &matWorld, sizeof(matView));
    dx7Device->SetTransform(D3DTRANSFORMSTATE_VIEW, &matView);

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

    dx7Device->SetLight(0, &d3dLight);
    dx7Device->LightEnable(0, TRUE);

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

    dx7Device->SetTransform(D3DTRANSFORMSTATE_PROJECTION, &matProject);
    dx7Device->SetRenderState(D3DRENDERSTATE_DITHERENABLE, 1);

    dx7Device->SetTextureStageState(0, D3DTSS_MINFILTER, D3DTOP_SELECTARG1);
    dx7Device->SetTextureStageState(0, D3DTSS_MAGFILTER, D3DTOP_SELECTARG1);
    dx7Device->SetTextureStageState(0, D3DTSS_MIPFILTER, D3DTOP_SELECTARG1);

    dx7Device->SetRenderState(D3DRENDERSTATE_ZENABLE, TRUE);
    dx7Device->SetRenderState(D3DRENDERSTATE_LIGHTING, TRUE);

    surfaceTestZoneBG = Load_PNG_File("Data/Levels/TestZone/BG.png", 0);
    surfaceSonic      = Load_PNG_File("Data/Characters/Sonic.png", 0);
    surface3DLogo     = Load_PNG_File("Data/Title/Logo.png", 0);
    surfaceShadow     = Load_PNG_File("Data/Objects/Shadow.png", 0);

    float_420428 = 4.0f;
    float_42042C = 4.0f;
    float_420430 = 4.0f;

    dx7Device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
    dx7Device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTOP_SELECTARG1);
    dx7Device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTOP_DISABLE);

    dx7Device->SetRenderState(D3DRENDERSTATE_ALPHATESTENABLE, 1);
    dx7Device->SetRenderState(D3DRENDERSTATE_ALPHAREF, 0);
    dx7Device->SetRenderState(D3DRENDERSTATE_ALPHAFUNC, 7);
    dx7Device->SetRenderState(D3DRENDERSTATE_SRCBLEND, 5);
    dx7Device->SetRenderState(D3DRENDERSTATE_DESTBLEND, 6);
    dx7Device->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, 1);
    dx7Device->SetRenderState(D3DRENDERSTATE_DESTBLEND, 6);

    if (!dword_41F0C4)
        dx7Device->SetRenderState(D3DRENDERSTATE_TEXTUREPERSPECTIVE, 0);

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

LPDIRECTDRAWSURFACE7 CreateBMPSurfaceFromHandle(IDirect3DDevice7 *device, HANDLE handle)
{
    D3DDEVICEDESC7 pDesc;
    if (IDirect3DDevice7_GetCaps(device, &pDesc) < 0)
        return 0;

    BITMAP bmp;
    GetObjectA(handle, 24, &bmp);

    LONG bmpWidth  = bmp.bmWidth;
    LONG bmpHeight = bmp.bmHeight;

    DDSURFACEDESC2 surfaceDesc;
    memset(&surfaceDesc, 0, sizeof(surfaceDesc));
    surfaceDesc.dwSize         = 124;
    surfaceDesc.dwFlags        = 1052679;
    surfaceDesc.ddsCaps.dwCaps = 4096;
    surfaceDesc.dwWidth        = bmp.bmWidth;
    surfaceDesc.dwHeight       = bmp.bmHeight;

    if (IsEqualGUID(pDesc.deviceGUID, IID_IDirect3DHALDevice))
        surfaceDesc.ddsCaps.dwCaps2 = 16;
    else if (IsEqualGUID(pDesc.deviceGUID, IID_IDirect3DTnLHalDevice))
        surfaceDesc.ddsCaps.dwCaps2 = 16;
    else
        surfaceDesc.ddsCaps.dwCaps |= 2048;

    if (pDesc.dpcTriCaps.dwTextureCaps & 2) {
        for (surfaceDesc.dwWidth = 1; bmpWidth > surfaceDesc.dwWidth; surfaceDesc.dwWidth *= 2);
        for (surfaceDesc.dwHeight = 1; bmpHeight > surfaceDesc.dwHeight; surfaceDesc.dwHeight *= 2);
    }

    if (pDesc.dpcTriCaps.dwTextureCaps & 32) {
        if (surfaceDesc.dwWidth <= surfaceDesc.dwHeight)
            surfaceDesc.dwWidth = surfaceDesc.dwHeight;
        else
            surfaceDesc.dwHeight = surfaceDesc.dwWidth;
    }

    retryTextureEnum = FALSE;
    IDirect3DDevice7_EnumTextureFormats(device, lpEnumTextureFormats, &surfaceDesc.ddpfPixelFormat);

    if (!surfaceDesc.ddpfPixelFormat.dwRGBBitCount) {
        retryTextureEnum = TRUE;
        IDirect3DDevice7_EnumTextureFormats(device, lpEnumTextureFormats, &surfaceDesc.ddpfPixelFormat);

        if (!surfaceDesc.ddpfPixelFormat.dwRGBBitCount)
            return FALSE;
    }

    IDirectDrawSurface7 *ppRenderTarget;
    IDirect3DDevice7_GetRenderTarget(device, &ppRenderTarget);

    ppRenderTarget->GetDDInterface((LPVOID *)&dd);
    ppRenderTarget->Release();

    IDirectDrawSurface7 *ddSurface;
    if (dd->CreateSurface(&surfaceDesc, &ddSurface, 0) >= 0) {
        dd->Release();

        HDC hdc = CreateCompatibleDC(0);
        if (hdc) {
            SelectObject(hdc, handle);

            HDC ddHDC;
            if (ddSurface->GetDC(&ddHDC) >= 0) {
                BitBlt(ddHDC, 0, 0, bmp.bmWidth, bmp.bmHeight, hdc, 0, 0, 0xCC0020);
                ddSurface->ReleaseDC(ddHDC);
            }

            DeleteDC(hdc);
            return ddSurface;
        }

        ddSurface->Release();
        return FALSE;
    }

    dd->Release();
    return FALSE;
}

HRESULT CALLBACK lpEnumTextureFormats(LPDDPIXELFORMAT pddpf, LPVOID pContext)
{
    char result; // al

    if ((pddpf->dwFlags & 0xE0000) != 0)
        return 1;
    if (pddpf->dwFourCC)
        return 1;
    result = retryTextureEnum;
    if (!retryTextureEnum) {
        if ((pddpf->dwFlags & 1) == 0 || pddpf->dwRGBBitCount != 32)
            return 1;
        goto LABEL_10;
    }
    if (retryTextureEnum == 1) {
        if ((pddpf->dwFlags & 1) == 0 || pddpf->dwRGBBitCount != 16)
            return 1;
        if (pddpf->dwRGBAlphaBitMask != 1 && pddpf->dwRGBAlphaBitMask != 0x8000)
            return 1;
    LABEL_10:
        memcpy(pContext, pddpf, 0x20u);
        result = 0;
    }
    return result;
}

HRESULT CALLBACK ldUnknownCallback(LPDDPIXELFORMAT pddpf, LPVOID pContext)
{
    LPDDPIXELFORMAT fmt = (LPDDPIXELFORMAT)pContext;

    if (!pddpf || !fmt)
        return DDENUMRET_CANCEL;

    if (pddpf->dwFlags != fmt->dwFlags || pddpf->dwRGBBitCount != 24)
        return D3DENUMRET_OK;

    memcpy(fmt, pddpf, sizeof(DDPIXELFORMAT));
    data_4C9D80 = 1;
    return DDENUMRET_CANCEL;
}

HRESULT BeginScene() { return IDirect3DDevice7_BeginScene(dx7Device); }

HRESULT EndScene() { return IDirect3DDevice7_EndScene(dx7Device); }

void DrawTitleModel(char type)
{
    IDirect3DDevice7_SetMaterial(dx7Device, &material_420520);

    if (type == 1) {
        IDirect3DDevice7_SetTexture(dx7Device, 0, surface3DLogo);
        IDirect3DDevice7_DrawIndexedPrimitive(dx7Device, D3DPT_TRIANGLELIST, D3DFVF_VERTEX, logoTMF.vertices, logoTMF.numVertices, logoTMF.indices,
                                              logoTMF.numIndices, 0);
    }
    else if (type == 0) {
        IDirect3DDevice7_SetTexture(dx7Device, 0, surfaceTestZoneBG);
        IDirect3DDevice7_DrawIndexedPrimitive(dx7Device, D3DPT_TRIANGLELIST, D3DFVF_VERTEX, backgroundTMF.vertices, backgroundTMF.numVertices,
                                              backgroundTMF.indices, backgroundTMF.numIndices, 0);
    }
}

void CopyMatrix_4C9B90_4C9C50() { memcpy(&matSonicMdl, &matWorld, sizeof(matSonicMdl)); }

void SonicMat_WorldTransform() { IDirect3DDevice7_SetTransform(dx7Device, D3DTRANSFORMSTATE_WORLD, &matSonicMdl); }

HRESULT Render_ClearScreen(D3DCOLOR color) { return IDirect3DDevice7_Clear(dx7Device, 0, 0, 3, color, 1.0f, 0); }

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

    IDirect3DDevice7_BeginScene(dx7Device);
    IDirect3DDevice7_SetTexture(dx7Device, 0, NULL);
    IDirect3DDevice7_DrawIndexedPrimitive(dx7Device, D3DPT_TRIANGLELIST, D3DFVF_LVERTEX, vertices, 4, indices, 6, 0);
    IDirect3DDevice7_EndScene(dx7Device);
}

void FlipScreen()
{
    if (WindowMode == 1)
        IDirectDrawSurface7_Blt(surface_4C9D3C, &windowBounds, surface_439D40, 0, 0x1000000, 0);
    else if (WindowMode == 0)
        IDirectDrawSurface7_Flip(surface_4C9D3C, 0, 1);
}

void LoadBitmapToSurface(LPCSTR name, int32_t id)
{
    HANDLE h = LoadImageA(0, name, 0, 0, 0, 0x50);

    BITMAP bmp;
    GetObjectA(h, 24, &bmp);

    LONG bmpWidth  = bmp.bmWidth;
    LONG bmpHeight = bmp.bmHeight;

    DDSURFACEDESC2 surfaceDesc;
    memset(&surfaceDesc, 0, sizeof(surfaceDesc));
    surfaceDesc.dwSize         = 124;
    surfaceDesc.dwFlags        = 7;
    surfaceDesc.ddsCaps.dwCaps = 64;
    surfaceDesc.dwWidth        = bmp.bmWidth;
    surfaceDesc.dwHeight       = bmp.bmHeight;

    dd->CreateSurface(&surfaceDesc, &surfaceList[id], 0);

    HDC hdc;
    surfaceList[id]->GetDC(&hdc);

    HDC hdcSrc = CreateCompatibleDC(hdc);
    SelectObject(hdcSrc, h);
    BitBlt(hdc, 0, 0, bmpWidth, bmpHeight, hdcSrc, 0, 0, 0xCC0020);

    surfaceList[id]->ReleaseDC(hdc);
    DeleteDC(hdcSrc);
    DeleteObject(h);
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
    dd->SetCooperativeLevel(hWnd, 8);

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

    if (dx7Device) {
        IDirect3DDevice7_Release(dx7Device);
        dx7Device = NULL;
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

    if (dd != NULL) {
        IDirectDraw7_Release(dd);
        dd = NULL;
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
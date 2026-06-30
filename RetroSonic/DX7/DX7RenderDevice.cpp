#include "RetroEngine.hpp"

IDirect3D7 *RenderDevice::D3D;
IDirect3DDevice7 *RenderDevice::D3DDevice;
IDirectDraw7 *RenderDevice::DDraw;

HWND RenderDevice::HWnd;
HINSTANCE RenderDevice::HInst;

bool RenderDevice::InitGraphicsAPI()
{
    if (FAILED(DirectDrawCreateEx(NULL, (LPVOID *)(&DDraw), IID_IDirectDraw7, NULL))) {
        MessageBox(HWnd, "DirectDrawCreate FAILED", 0, MB_ICONHAND);
        return FALSE;
    }

    return InitDirect3D(HWnd) >= 0;
}

bool RenderDevice::InitDirect3D(HWND hWnd)
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

        if (HasGameStarted != TRUE || InitScreen() != false)
            result = FALSE;
        else
            result = FALSE;
    }

    return result;
}

bool RenderDevice::InitScreen()
{
    SupportsZBufferFmt = false;
    if (DDraw->QueryInterface(IID_IDirect3D7, (LPVOID *)&D3D) < 0)
        return FALSE;

    DDPIXELFORMAT fmt;
    memset(&fmt, 0, sizeof(fmt));

    fmt.dwSize  = 32;
    fmt.dwFlags = 1024;

    D3D->EnumZBufferFormats(IID_IDirect3DTnLHalDevice, EnumZBufferFormatsCallback, &fmt);
    if (SupportsZBufferFmt == true) {
        D3DDeviceType = D3D_DEVICE_TNL_HAL;
    }
    else {
        D3D->EnumZBufferFormats(IID_IDirect3DHALDevice, EnumZBufferFormatsCallback, &fmt);
        if (SupportsZBufferFmt == true) {
            D3DDeviceType = D3D_DEVICE_HAL;
        }
        else {
            D3D->EnumZBufferFormats(IID_IDirect3DRGBDevice, EnumZBufferFormatsCallback, &fmt);
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

    if (D3D->CreateDevice(IID_IDirect3DTnLHalDevice, surface_439D40, &D3DDevice)) {
        if (D3D->CreateDevice(IID_IDirect3DHALDevice, surface_439D40, &D3DDevice)) {
            if (D3D->CreateDevice(IID_IDirect3DRGBDevice, surface_439D40, &D3DDevice)) {
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

    IdentityMatrix(&MatrixIdentity);

    D3DMATRIX mTemp;
    memcpy(&mTemp, &MatrixIdentity, sizeof(mTemp));
    RenderDevice::SetTransform(RENDER_TRANSFORM_WORLD, &mTemp);

    memcpy(&MatrixView, &MatrixIdentity, sizeof(MatrixView));
    RenderDevice::SetTransform(RENDER_TRANSFORM_VIEW, &MatrixView);

    memset(&MatrixProjection, 0, sizeof(MatrixProjection));
    MatrixPerspective(&MatrixProjection, TO_RADIAN(45.0f), 0.75f, 1.0f, 1000.0f);

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

    RenderDevice::SetLight(0, &d3dLight);
    RenderDevice::LightEnable(0, true);

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

    RenderDevice::SetTransform(RENDER_TRANSFORM_PROJECTION, &MatrixProjection);
    RenderDevice::SetRenderState(RENDER_STATE_DITHERENABLE, true);

    RenderDevice::SetTextureStageState(0, TEXTURE_STATE_MINFILTER, D3DTOP_SELECTARG1);
    RenderDevice::SetTextureStageState(0, TEXTURE_STATE_MAGFILTER, D3DTOP_SELECTARG1);
    RenderDevice::SetTextureStageState(0, TEXTURE_STATE_MIPFILTER, D3DTOP_SELECTARG1);

    RenderDevice::SetRenderState(RENDER_STATE_ZENABLE, true);
    RenderDevice::SetRenderState(RENDER_STATE_LIGHTING, true);

    LoadTexture(surfaceTestZoneBG, "Data/Levels/TestZone/BG.png", 0);
    LoadTexture(surfaceSonic, "Data/Characters/Sonic.png", 0);
    LoadTexture(surface3DLogo, "Data/Title/Logo.png", 0);
    LoadTexture(surfaceShadow, "Data/Objects/Shadow.png", 0);

    float_420428 = 4.0f;
    float_42042C = 4.0f;
    float_420430 = 4.0f;

    RenderDevice::SetTextureStageState(0, TEXTURE_STATE_ALPHAOP, D3DTOP_MODULATE);
    RenderDevice::SetTextureStageState(0, TEXTURE_STATE_ALPHAARG1, D3DTOP_SELECTARG1);
    RenderDevice::SetTextureStageState(0, TEXTURE_STATE_ALPHAARG2, D3DTOP_DISABLE);

    RenderDevice::SetRenderState(RENDER_STATE_ALPHATESTENABLE, 1);
    RenderDevice::SetRenderState(RENDER_STATE_ALPHAREF, 0);
    RenderDevice::SetRenderState(RENDER_STATE_ALPHAFUNC, 7);
    RenderDevice::SetRenderState(RENDER_STATE_SRCBLEND, 5);
    RenderDevice::SetRenderState(RENDER_STATE_DESTBLEND, 6);
    RenderDevice::SetRenderState(RENDER_STATE_ALPHABLENDENABLE, 1);
    RenderDevice::SetRenderState(RENDER_STATE_DESTBLEND, 6);

    if (!dword_41F0C4)
        RenderDevice::SetRenderState(RENDER_STATE_TEXTUREPERSPECTIVE, 0);

    return TRUE;
}

void RenderDevice::FlipScreen()
{
    switch (WindowMode) {
        case 0: surface_4C9D3C->Flip(0, 1); break;
        case 1: surface_4C9D3C->Blt(&windowBounds, surface_439D40, 0, 0x1000000, 0); break;
        default: break;
    }
}

HRESULT RenderDevice::RefreshSurfaces(HWND hWnd)
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

    return InitDirect3D(hWnd);
}

void RenderDevice::BeginScene() { D3DDevice->BeginScene(); }
void RenderDevice::EndScene() { D3DDevice->EndScene(); }
void RenderDevice::Clear(int color) { D3DDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, color, 1.0f, 0); }

void RenderDevice::SetTexture(int id, IDirectDrawSurface7 *pTexture) { D3DDevice->SetTexture(id, pTexture); }
void RenderDevice::SetMaterial(D3DMATERIAL7 *pMaterial) { D3DDevice->SetMaterial(pMaterial); }
void RenderDevice::SetLight(int id, D3DLIGHT7 *pLight) { D3DDevice->SetLight(id, pLight); }

void RenderDevice::SetTransform(RenderTransform type, D3DMATRIX *pMatrix)
{
    switch (type) {
        case RENDER_TRANSFORM_WORLD: D3DDevice->SetTransform(D3DTRANSFORMSTATE_WORLD, pMatrix); break;
        case RENDER_TRANSFORM_VIEW: D3DDevice->SetTransform(D3DTRANSFORMSTATE_VIEW, pMatrix); break;
        case RENDER_TRANSFORM_PROJECTION: D3DDevice->SetTransform(D3DTRANSFORMSTATE_PROJECTION, pMatrix); break;
        default: break;
    }
}

void RenderDevice::SetRenderState(RenderState type, int value)
{
    switch (type) {
        case RENDER_STATE_LIGHTING: D3DDevice->SetRenderState(D3DRENDERSTATE_LIGHTING, value); break;
        case RENDER_STATE_ZENABLE: D3DDevice->SetRenderState(D3DRENDERSTATE_ZENABLE, value); break;
        case RENDER_STATE_ALPHAFUNC: D3DDevice->SetRenderState(D3DRENDERSTATE_ALPHAFUNC, value); break;
        case RENDER_STATE_ALPHAREF: D3DDevice->SetRenderState(D3DRENDERSTATE_ALPHAREF, value); break;
        case RENDER_STATE_SRCBLEND: D3DDevice->SetRenderState(D3DRENDERSTATE_SRCBLEND, value); break;
        case RENDER_STATE_DESTBLEND: D3DDevice->SetRenderState(D3DRENDERSTATE_DESTBLEND, value); break;
        case RENDER_STATE_SPECULARENABLE: D3DDevice->SetRenderState(D3DRENDERSTATE_SPECULARENABLE, value); break;
        case RENDER_STATE_ALPHABLENDENABLE: D3DDevice->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, value); break;
        case RENDER_STATE_ALPHATESTENABLE: D3DDevice->SetRenderState(D3DRENDERSTATE_ALPHATESTENABLE, value); break;
        case RENDER_STATE_DITHERENABLE: D3DDevice->SetRenderState(D3DRENDERSTATE_DITHERENABLE, value); break;
        case RENDER_STATE_TEXTUREPERSPECTIVE: D3DDevice->SetRenderState(D3DRENDERSTATE_TEXTUREPERSPECTIVE, value); break;
        default: break;
    }
}

void RenderDevice::SetTextureStageState(int stage, TextureStageState type, int value)
{
    switch (type) {
        case TEXTURE_STATE_TEXCOORDINDEX: D3DDevice->SetTextureStageState(stage, D3DTSS_TEXCOORDINDEX, value); break;
        case TEXTURE_STATE_MINFILTER: D3DDevice->SetTextureStageState(stage, D3DTSS_MINFILTER, value); break;
        case TEXTURE_STATE_MAGFILTER: D3DDevice->SetTextureStageState(stage, D3DTSS_MAGFILTER, value); break;
        case TEXTURE_STATE_MIPFILTER: D3DDevice->SetTextureStageState(stage, D3DTSS_MIPFILTER, value); break;
        case TEXTURE_STATE_ALPHAOP: D3DDevice->SetTextureStageState(stage, D3DTSS_ALPHAOP, value); break;
        case TEXTURE_STATE_ALPHAARG1: D3DDevice->SetTextureStageState(stage, D3DTSS_ALPHAARG1, value); break;
        case TEXTURE_STATE_ALPHAARG2: D3DDevice->SetTextureStageState(stage, D3DTSS_ALPHAARG2, value); break;
        default: break;
    }
}

void RenderDevice::LightEnable(int id, bool enabled) { D3DDevice->LightEnable(id, enabled); }

void RenderDevice::DrawIndexed(RenderFVF type, void *pVertices, int numVertices, void *pIndices, int numIndices)
{
    int fvf = type ? D3DFVF_LVERTEX : D3DFVF_VERTEX;
    D3DDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, fvf, pVertices, numVertices, (ushort *)(pIndices), numIndices, 0);
}

void RenderDevice::DrawFade(float r, float g, float b, float a)
{
    D3DLVERTEX vertices[4];
    ZeroMemory(vertices, sizeof(vertices));

    ushort indices[] = { 0, 1, 2, 0, 2, 3 };

    D3DCOLOR color = TO_ARGB_F(a, r, g, b);

    vertices[0] = { { -1.0f, -1.0f, 1.0f }, color, 0, 0.0f, 1.0f };
    vertices[1] = { { -1.0f, +1.0f, 1.0f }, color, 0, 0.0f, 0.0f };
    vertices[2] = { { +1.0f, +1.0f, 1.0f }, color, 0, 1.0f, 0.0f };
    vertices[3] = { { +1.0f, -1.0f, 1.0f }, color, 0, 1.0f, 0.0f };

    BeginScene();
    SetTexture(0, NULL);
    DrawIndexed(RENDER_FVF_LVERTEX, vertices, 4, indices, 6);
    EndScene();
}
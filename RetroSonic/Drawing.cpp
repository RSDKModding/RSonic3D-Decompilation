#include "RetroEngine.hpp"

#if RETRO_USE_ORIGINAL_CODE
tagRECT rect_420488;
tagRECT clientRect;
tagRECT windowBounds;
#endif

Matrix3D MatrixWorld;
Matrix3D MatrixView;
Matrix3D MatrixProjection;
Matrix3D MatrixIdentity;

TextureFormatSearchType TexFmtSearchType;

char WindowMode  = 1;
int dword_41F0C4 = 1;

int ResX = 80;
int ResY = 82;
sbyte ColourDepth;

#if RETRO_USE_ORIGINAL_CODE
IDirect3D7 *D3D;
IDirect3DDevice7 *D3DDevice;
IDirectDraw7 *DDraw;

IDirectDrawSurface7 *FrontBuffer;
IDirectDrawSurface7 *BackBuffer;
IDirectDrawSurface7 *ZBuffer;

int D3DSupportsZBufferFmt;
byte D3DDeviceType;

HWND HWnd;
HMENU HMenu;
HINSTANCE HInst;
int NCmdShow;
#else
SDL_Window *Window;
SDL_GLContext GLContext;
#endif

bool EngineRunning = false;

bool InitGraphicsAPI()
{
#if RETRO_USE_ORIGINAL_CODE
    if (FAILED(DirectDrawCreateEx(NULL, (LPVOID *)(&DDraw), IID_IDirectDraw7, NULL))) {
        MessageBox(HWnd, "DirectDrawCreate FAILED", 0, MB_ICONHAND);
        return false;
    }

    // Non-original, but this is apparently required for FreeImage's static library to work on Windows
    FreeImage_Initialise();

    return InitDirect3D() >= 0;
#else
    return InitScreen();
#endif
}

void ReleaseGraphicsAPI()
{
    ReleaseModelSurfaces();
    for (int i = 0; i < 10; ++i) {
        ReleaseSurfaceID(i);
    }

#if RETRO_USE_ORIGINAL_CODE
    if (DDraw != NULL) {
        DDraw->Release();
        DDraw = NULL;
    }

    if (BackBuffer != NULL) {
        BackBuffer->Release();
        BackBuffer = NULL;
    }

    if (FrontBuffer != NULL) {
        FrontBuffer->Release();
        FrontBuffer = NULL;
    }

    // Non-original, but this is apparently required for FreeImage's static library to work on Windows
    FreeImage_DeInitialise();
#else
    if (GLContext != NULL) {
#if RETRO_USE_SDL3
        SDL_GL_DestroyContext(GLContext);
#elif RETRO_USE_SDL2
        SDL_GL_DeleteContext(GLContext);
#endif
        GLContext = NULL;
    }

    SDL_Quit();
#endif
}

#if RETRO_USE_ORIGINAL_CODE
bool InitDirect3D()
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
        r = DDraw->SetCooperativeLevel(HWnd, 8);
        UpdateWindowRect(HWnd);

        memset(&surfaceDesc_1, 0, sizeof(surfaceDesc_1));
        surfaceDesc_1.dwSize         = 124;
        surfaceDesc_1.dwFlags        = 1;
        surfaceDesc_1.ddsCaps.dwCaps = 8704;

        r = DDraw->CreateSurface(&surfaceDesc_1, &FrontBuffer, NULL);
        r = DDraw->CreateClipper(0, &ddclipper, 0);

        ddclipper->SetHWnd(0, HWnd);
        FrontBuffer->SetClipper(ddclipper);

        ddclipper->Release();
        ddclipper = NULL;

        memset(&surfaceDesc_2, 0, sizeof(surfaceDesc_2));
        surfaceDesc_2.dwSize         = 124;
        surfaceDesc_2.dwFlags        = 7;
        surfaceDesc_2.ddsCaps.dwCaps = 8256;
        surfaceDesc_2.dwWidth        = SCREEN_XSIZE * 2;
        surfaceDesc_2.dwHeight       = SCREEN_YSIZE * 2;

        r = DDraw->CreateSurface(&surfaceDesc_2, &BackBuffer, NULL);
    }
    else {
        r = DDraw->SetCooperativeLevel(HWnd, 17);
        r = DDraw->SetDisplayMode(ResX, ResY, 32, 0, 0);

        memset(&surfaceDesc_1, 0, sizeof(surfaceDesc_1));
        surfaceDesc_1.dwSize            = 124;
        surfaceDesc_1.dwFlags           = 33;
        surfaceDesc_1.ddsCaps.dwCaps    = 8728;
        surfaceDesc_1.dwBackBufferCount = 1;

        r = DDraw->CreateSurface(&surfaceDesc_1, &FrontBuffer, 0);

        caps.dwCaps  = 4;
        caps.dwCaps2 = 0;
        caps.dwCaps3 = 0;
        caps.dwCaps4 = 0;

        r = FrontBuffer->GetAttachedSurface(&caps, &BackBuffer);

        memset(&bltFx_1, 0, sizeof(bltFx_1));
        bltFx_1.dwSize      = 100;
        bltFx_1.dwFillColor = 0;

        BackBuffer->Blt(0, 0, 0, 16778240, &bltFx_1);
    }

    DDraw->GetDisplayMode(&surfaceDesc_1);

    if (surfaceDesc_1.ddpfPixelFormat.dwRGBBitCount == 24 || surfaceDesc_1.ddpfPixelFormat.dwRGBBitCount == 8) {
        MessageBoxA(HWnd, "This Colour Depth is not suitable for Retro-Sonic. Please use either 16bit or 32bit colour mode", "Colour Depth", 0x10);
    }
    else {
        memset(&bltFx_2, 0, sizeof(bltFx_2));
        bltFx_2.dwSize      = 100;
        bltFx_2.dwFillColor = 0;

        BackBuffer->Blt(0, 0, 0, 16778240, &bltFx_2);

        if (EngineRunning != true || InitScreen() != false) {
            // nothing
        }
    }

    return false;
}
#endif

bool InitScreen()
{
#if RETRO_USE_ORIGINAL_CODE
    SupportsZBufferFmt = false;
    if (DDraw->QueryInterface(IID_IDirect3D7, (LPVOID *)&D3D) < 0)
        return false;

    DDPIXELFORMAT fmt;
    memset(&fmt, 0, sizeof(fmt));

    fmt.dwSize  = 32;
    fmt.dwFlags = 1024;

    D3D->EnumZBufferFormats(IID_IDirect3DTnLHalDevice, EnumZBufferFormatsCallback, &fmt);
    if (D3DSupportsZBufferFmt == true) {
        D3DDeviceType = D3D_DEVICE_TNL_HAL;
    }
    else {
        D3D->EnumZBufferFormats(IID_IDirect3DHALDevice, EnumZBufferFormatsCallback, &fmt);
        if (D3DSupportsZBufferFmt == true) {
            D3DDeviceType = D3D_DEVICE_HAL;
        }
        else {
            D3D->EnumZBufferFormats(IID_IDirect3DRGBDevice, EnumZBufferFormatsCallback, &fmt);
            if (D3DSupportsZBufferFmt == true)
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
        ddSurfaceDesc.dwWidth  = SCREEN_XSIZE * 2;
        ddSurfaceDesc.dwHeight = SCREEN_YSIZE * 2;
    }
    else if (WindowMode == 0) {
        ddSurfaceDesc.dwWidth  = ResX;
        ddSurfaceDesc.dwHeight = ResY;
    }

    memcpy(&ddSurfaceDesc.ddpfPixelFormat, &fmt, sizeof(ddSurfaceDesc.ddpfPixelFormat));
    if (SUCCEEDED(DDraw->CreateSurface(&ddSurfaceDesc, &ZBuffer, 0))) {
        BackBuffer->AddAttachedSurface(ZBuffer);
    }

    if (FAILED(D3D->CreateDevice(IID_IDirect3DTnLHalDevice, BackBuffer, &D3DDevice))) {
        if (FAILED(D3D->CreateDevice(IID_IDirect3DHALDevice, BackBuffer, &D3DDevice))) {
            if (FAILED(D3D->CreateDevice(IID_IDirect3DRGBDevice, BackBuffer, &D3DDevice)))
                return false;

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
        pViewport.dwWidth  = SCREEN_XSIZE * 2;
        pViewport.dwHeight = SCREEN_YSIZE * 2;
    }
    else {
        pViewport.dwWidth  = ResX;
        pViewport.dwHeight = ResY;
    }

    pViewport.dvMinZ = 0.0;
    pViewport.dvMaxZ = 1.0;

    if (FAILED(D3DDevice->SetViewport(&pViewport)))
        return false;
#else
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);

    GLfloat m[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, m);
    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
#endif

    IdentityMatrix(&MatrixIdentity);

    Matrix3D MWorld;
    memcpy(&MWorld, &MatrixIdentity, sizeof(MWorld));
    SetRenderTransform(RENDER_TRANSFORM_WORLD, &MWorld);

    memcpy(&MatrixView, &MatrixIdentity, sizeof(MatrixView));
    SetRenderTransform(RENDER_TRANSFORM_VIEW, &MatrixView);

    memset(&MatrixProjection, 0, sizeof(MatrixProjection));
    MatrixPerspective(&MatrixProjection, TO_RADIAN(45.0f), 0.75f, 1.0f, 1000.0f);

    Light light = {};
    light.type  = LIGHT_DIRECTIONAL;

    light.direction = { 0.0f, -0.9f, 0.1f };
    light.diffuse   = { 0.8f, 0.8f, 0.8f, 0.0f };
    light.ambient   = { 0.450f, 0.450f, 0.450f, 0.0f };
    light.specular  = { 1.0f, 1.0f, 1.0f, 0.0f };

    SetRenderLight(0, &light);
    EnableLight(0, true);

    material_420520.ambient = { 1.0f, 1.0f, 1.0f, 1.0f };
    material_420520.diffuse = material_420520.ambient;

    material_420520.specular = { 0.25f, 0.25f, 1.0f, 0.25f };
    material_420520.power    = 80.0f;

    SetRenderTransform(RENDER_TRANSFORM_PROJECTION, &MatrixProjection);
    SetRenderState(RENDER_STATE_DITHERENABLE, true);

    SetRenderTextureStageState(0, TEXTURE_STATE_MINFILTER, TEXTURE_VALUE_SELECTARG1);
    SetRenderTextureStageState(0, TEXTURE_STATE_MAGFILTER, TEXTURE_VALUE_SELECTARG1);
    SetRenderTextureStageState(0, TEXTURE_STATE_MIPFILTER, TEXTURE_VALUE_SELECTARG1);

    SetRenderState(RENDER_STATE_ZENABLE, true);
    SetRenderState(RENDER_STATE_LIGHTING, true);

    LoadTexture(&surfaceTestZoneBG, "Data/Levels/TestZone/BG.png", false);
    LoadTexture(&surfaceSonic, "Data/Characters/Sonic.png", false);
    LoadTexture(&surface3DLogo, "Data/Title/Logo.png", false);
    LoadTexture(&surfaceShadow, "Data/Objects/Shadow.png", false);

    float_420428 = 4.0f;
    float_42042C = 4.0f;
    float_420430 = 4.0f;

    SetRenderTextureStageState(0, TEXTURE_STATE_ALPHAOP, TEXTURE_VALUE_MODULATE);
    SetRenderTextureStageState(0, TEXTURE_STATE_ALPHAARG1, TEXTURE_VALUE_SELECTARG1);
    SetRenderTextureStageState(0, TEXTURE_STATE_ALPHAARG2, TEXTURE_VALUE_DISABLE);

    SetRenderState(RENDER_STATE_ALPHATESTENABLE, 1);
    SetRenderState(RENDER_STATE_ALPHAREF, 0);
    SetRenderState(RENDER_STATE_ALPHAFUNC, 7);
    SetRenderState(RENDER_STATE_SRCBLEND, 5);
    SetRenderState(RENDER_STATE_DESTBLEND, 6);
    SetRenderState(RENDER_STATE_ALPHABLENDENABLE, 1);
    SetRenderState(RENDER_STATE_DESTBLEND, 6);

#if RETRO_USE_ORIGINAL_CODE
    if (!dword_41F0C4)
        SetRenderState(RENDER_STATE_TEXTUREPERSPECTIVE, false);
#endif

    return true;
}

void FlipScreen()
{
#if RETRO_USE_ORIGINAL_CODE
    switch (WindowMode) {
        case 0: FrontBuffer->Flip(NULL, DDFLIP_WAIT); break;
        case 1: FrontBuffer->Blt(&windowBounds, BackBuffer, NULL, DDBLT_WAIT, NULL); break;
        default: break;
    }
#else
    ImGuiDoMenuBar();
    SDL_GL_SwapWindow(Window);
#endif
}

void SetScreenResolution(sbyte windowResolution, sbyte fullscreenResolution, sbyte colourDepth)
{
    switch (windowResolution) {
        case SCALE_1X:
            ResX = SCREEN_XSIZE * 1;
            ResY = SCREEN_YSIZE * 1;
            break;

        case SCALE_2X:
            ResX = SCREEN_XSIZE * 2;
            ResY = SCREEN_YSIZE * 2;
            break;

        case SCALE_3X:
            ResX = SCREEN_XSIZE * 3;
            ResY = SCREEN_YSIZE * 3;
            break;

        case SCALE_4X:
            ResX = SCREEN_XSIZE * 4;
            ResY = SCREEN_YSIZE * 4;
            break;

        default: break;
    }

    switch (fullscreenResolution) {
        case SCALE_1X:
            ResX = SCREEN_XSIZE * 1;
            ResY = SCREEN_YSIZE * 1;
            break;

        case SCALE_2X:
            ResX = SCREEN_XSIZE * 2;
            ResY = SCREEN_YSIZE * 2;
            break;

        default: break;
    }

    ColourDepth = colourDepth;
}

void ToggleScreenMode()
{
#if RETRO_USE_ORIGINAL_CODE
    DDraw->SetCooperativeLevel(HWnd, 8);
#endif

    for (int i = 0; i < 10; ++i) {
        if (surfaceCharacters2[i] != NULL) {
            surfaceCharacters2[i]->Release();
            surfaceCharacters2[i] = NULL;
        }
    }

    if (EngineRunning == true)
        ReleaseModelSurfaces();

#if RETRO_USE_ORIGINAL_CODE
    if (BackBuffer != NULL) {
        BackBuffer->Release();
        BackBuffer = NULL;
    }
#endif

    switch (WindowMode) {
        case 0: WindowMode = 1;
#if RETRO_USE_ORIGINAL_CODE
            ShowWindow(HWnd, SW_HIDE);
            SetWindowLongA(HWnd, GWL_STYLE, WS_OVERLAPPEDWINDOW);
            SetMenu(HWnd, HMenu);
            ShowCursor(true);
#else
            SDL_SetWindowFullscreen(Window, 0);
            SDL_SetWindowSize(Window, ResX, ResY);
#endif
            break;

        case 1: WindowMode = 0;
#if RETRO_USE_ORIGINAL_CODE
            ShowWindow(HWnd, SW_HIDE);
            SetWindowLongA(HWnd, GWL_STYLE, WS_POPUP);
            HMenu = GetMenu(HWnd);
            SetMenu(HWnd, NULL);
            ShowCursor(false);
#else
            SDL_SetWindowFullscreen(Window, SDL_WINDOW_FULLSCREEN);
            SDL_SetWindowSize(Window, ResX, ResY);
#endif
            break;

        default: break;
    }

#if RETRO_USE_ORIGINAL_CODE
    ShowWindow(HWnd, SW_SHOW);

    if (FrontBuffer != NULL) {
        FrontBuffer->Release();
        FrontBuffer = NULL;
    }

    if (WindowMode == 1) {
        SetWindowPos(HWnd, HWND_NOTOPMOST, rect_420488.left, rect_420488.top, rect_420488.right - rect_420488.left,
                     rect_420488.bottom - rect_420488.top, 0x40);
    }

    InitDirect3D();
#else
    InitScreen();
#endif
}

void ClearScreen(Color color)
{
#if RETRO_USE_ORIGINAL_CODE
    D3DDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, color, 1.0f, 0);
#else
    float b = UNPACK_B_F(color);
    float g = UNPACK_G_F(color);
    float r = UNPACK_R_F(color);
    float a = UNPACK_A_F(color);

    glClearColor(r, g, b, a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
#endif
}

void BeginScene()
{
#if RETRO_USE_ORIGINAL_CODE
    D3DDevice->BeginScene();
#endif
}

void EndScene()
{
#if RETRO_USE_ORIGINAL_CODE
    D3DDevice->EndScene();
#endif
}

void SetRenderTexture(int id, Texture *pTexture)
{
#if RETRO_USE_ORIGINAL_CODE
    D3DDevice->SetTexture(id, (IDirectDrawSurface7 *)(pTexture));
#else
    if (pTexture != NULL && pTexture->id) {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, pTexture->id);
    }
    else {
        glDisable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
#endif
}

void SetRenderMaterial(Material *pMaterial)
{
#if RETRO_USE_ORIGINAL_CODE
    D3DDevice->SetMaterial((D3DMATERIAL7 *)(pMaterial));
#else
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, (const GLfloat *)(&pMaterial->ambient));
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, (const GLfloat *)(&pMaterial->diffuse));
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, (const GLfloat *)(&pMaterial->specular));
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, pMaterial->power);
    glColor4f(pMaterial->diffuse.r, pMaterial->diffuse.g, pMaterial->diffuse.b, pMaterial->diffuse.a);
#endif
}

void SetRenderLight(int id, Light *pLight)
{
#if RETRO_USE_ORIGINAL_CODE
    D3DDevice->SetLight(id, (D3DLIGHT7 *)(pLight));
#else
    glLightfv(GL_LIGHT0 + id, GL_AMBIENT, (const GLfloat *)(&pLight->ambient));
    glLightfv(GL_LIGHT0 + id, GL_DIFFUSE, (const GLfloat *)(&pLight->diffuse));
    glLightfv(GL_LIGHT0 + id, GL_SPECULAR, (const GLfloat *)(&pLight->specular));

    switch (pLight->type) {
        case LIGHT_DIRECTIONAL: {
            GLfloat position[] = { -pLight->direction.x, -pLight->direction.y, -pLight->direction.z, 0.0f };
            glLightfv(GL_LIGHT0 + id, GL_POSITION, position);
            break;
        }

        default: break;
    }
#endif
}

void SetRenderTransform(RenderTransform type, Matrix3D *pMatrix)
{
#if !RETRO_USE_ORIGINAL_CODE
    static Matrix3D ViewTemp  = Matrix3D::Identity;
    static Matrix3D WorldTemp = Matrix3D::Identity;
#endif

    switch (type) {
        case RENDER_TRANSFORM_WORLD:
#if RETRO_USE_ORIGINAL_CODE
            D3DDevice->SetTransform(D3DTRANSFORMSTATE_WORLD, (D3DMATRIX *)(pMatrix));
#else
            glMatrixMode(GL_MODELVIEW);
            memcpy(&WorldTemp, pMatrix, sizeof(WorldTemp));

            glLoadMatrixf(&ViewTemp.m[0][0]);
            glMultMatrixf(&WorldTemp.m[0][0]);
#endif
            break;

        case RENDER_TRANSFORM_VIEW:
#if RETRO_USE_ORIGINAL_CODE
            D3DDevice->SetTransform(D3DTRANSFORMSTATE_VIEW, (D3DMATRIX *)(pMatrix));
#else
            glMatrixMode(GL_MODELVIEW);
            memcpy(&ViewTemp, pMatrix, sizeof(ViewTemp));

            glLoadMatrixf(&ViewTemp.m[0][0]);
            glMultMatrixf(&WorldTemp.m[0][0]);
#endif
            break;

        case RENDER_TRANSFORM_PROJECTION:
#if RETRO_USE_ORIGINAL_CODE
            D3DDevice->SetTransform(D3DTRANSFORMSTATE_PROJECTION, (D3DMATRIX *)(pMatrix));
#else
            glMatrixMode(GL_PROJECTION);
            glLoadMatrixf(&pMatrix->m[0][0]);
#endif
            break;

        default: break;
    }
}

void SetRenderState(RenderState type, int value)
{
#if !RETRO_USE_ORIGINAL_CODE
    static GLenum GLAlphaFunc = GL_ALWAYS;
    static GLenum GLAlphaRef  = 0.0f;

    static GLenum GLSourceBlendFunc = GL_ONE;
    static GLenum GLDestBlendFunc   = GL_ZERO;
#endif

    switch (type) {
        case RENDER_STATE_LIGHTING:
#if RETRO_USE_ORIGINAL_CODE
            D3DDevice->SetRenderState(D3DRENDERSTATE_LIGHTING, value);
#else
            if (value != 0)
                glEnable(GL_LIGHTING);
            else
                glDisable(GL_LIGHTING);
#endif
            break;

        case RENDER_STATE_ZENABLE:
#if RETRO_USE_ORIGINAL_CODE
            D3DDevice->SetRenderState(D3DRENDERSTATE_ZENABLE, value);
#else
            if (value != 0)
                glEnable(GL_DEPTH_TEST);
            else
                glDisable(GL_DEPTH_TEST);
#endif
            break;

        case RENDER_STATE_ALPHAFUNC:
#if RETRO_USE_ORIGINAL_CODE
            D3DDevice->SetRenderState(D3DRENDERSTATE_ALPHAFUNC, value);
#else
            GLAlphaFunc = GL_NEVER + (GLenum)(value - 1);
            glAlphaFunc((GLenum)GLAlphaFunc, GLAlphaRef);
#endif
            break;

        case RENDER_STATE_ALPHAREF:
#if RETRO_USE_ORIGINAL_CODE
            D3DDevice->SetRenderState(D3DRENDERSTATE_ALPHAREF, value);
#else
            GLAlphaRef = (float)(value) / 255.0f;
            glAlphaFunc(GLAlphaFunc, GLAlphaRef);
#endif
            break;

        case RENDER_STATE_SRCBLEND:
#if RETRO_USE_ORIGINAL_CODE
            D3DDevice->SetRenderState(D3DRENDERSTATE_SRCBLEND, value);
#else
            switch (value) {
                case 1: GLSourceBlendFunc = GL_ZERO; break;
                case 2: GLSourceBlendFunc = GL_ONE; break;
                case 5: GLSourceBlendFunc = GL_SRC_ALPHA; break;
                case 6: GLSourceBlendFunc = GL_ONE_MINUS_SRC_ALPHA; break;
                default: break;
            }

            glBlendFunc(GLSourceBlendFunc, GLDestBlendFunc);
#endif
            break;

        case RENDER_STATE_DESTBLEND:
#if RETRO_USE_ORIGINAL_CODE
            D3DDevice->SetRenderState(D3DRENDERSTATE_DESTBLEND, value);
#else
            switch (value) {
                case 1: GLDestBlendFunc = GL_ZERO; break;
                case 2: GLDestBlendFunc = GL_ONE; break;
                case 5: GLDestBlendFunc = GL_SRC_ALPHA; break;
                case 6: GLDestBlendFunc = GL_ONE_MINUS_SRC_ALPHA; break;
                default: break;
            }

            glBlendFunc(GLSourceBlendFunc, GLDestBlendFunc);
#endif
            break;

        case RENDER_STATE_SPECULARENABLE:
#if RETRO_USE_ORIGINAL_CODE
            D3DDevice->SetRenderState(D3DRENDERSTATE_SPECULARENABLE, value);
#else
            if (value != 0)
                glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR);
            else
                glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SINGLE_COLOR);
#endif
            break;

        case RENDER_STATE_ALPHABLENDENABLE:
#if RETRO_USE_ORIGINAL_CODE
            D3DDevice->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, value);
#else
            if (value != 0)
                glEnable(GL_BLEND);
            else
                glDisable(GL_BLEND);
#endif
            break;

        case RENDER_STATE_ALPHATESTENABLE:
#if RETRO_USE_ORIGINAL_CODE
            D3DDevice->SetRenderState(D3DRENDERSTATE_ALPHATESTENABLE, value);
#else
            if (value != 0)
                glEnable(GL_ALPHA_TEST);
            else
                glDisable(GL_ALPHA_TEST);
#endif
            break;

        case RENDER_STATE_DITHERENABLE:
#if RETRO_USE_ORIGINAL_CODE
            D3DDevice->SetRenderState(D3DRENDERSTATE_DITHERENABLE, value);
#else
            if (value != 0)
                glEnable(GL_DITHER);
            else
                glDisable(GL_DITHER);
#endif
            break;

        case RENDER_STATE_TEXTUREPERSPECTIVE:
#if RETRO_USE_ORIGINAL_CODE
            D3DDevice->SetRenderState(D3DRENDERSTATE_TEXTUREPERSPECTIVE, value);
#else
            if (value != 0)
                glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
            else
                glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);
#endif
            break;

        default: break;
    }
}

void SetRenderTextureStageState(int stage, TextureStageState type, int value)
{
    switch (type) {
        // i only did the shiny one for GL and then gave up
        // the other ones arent too important anyways
        case TEXTURE_STATE_TEXCOORDINDEX:
#if RETRO_USE_ORIGINAL_CODE
            D3DDevice->SetTextureStageState(stage, D3DTSS_TEXCOORDINDEX, value);
#else
            switch (value) {
                default:
                case TEXTURE_VALUE_PASSTHRU:
                    glDisable(GL_TEXTURE_GEN_S);
                    glDisable(GL_TEXTURE_GEN_T);
                    break;

                case TEXTURE_VALUE_CAMERASPACE_NORMAL:
                    glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_NORMAL_MAP);
                    glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_NORMAL_MAP);

                    glEnable(GL_TEXTURE_GEN_S);
                    glEnable(GL_TEXTURE_GEN_T);
                    break;

                case TEXTURE_VALUE_CAMERASPACE_POSITION:
                    glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
                    glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);

                    glEnable(GL_TEXTURE_GEN_S);
                    glEnable(GL_TEXTURE_GEN_T);
                    break;

                case TEXTURE_VALUE_CAMERASPACE_REFLECTIONVECTOR:
                    glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP);
                    glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP);

                    glEnable(GL_TEXTURE_GEN_S);
                    glEnable(GL_TEXTURE_GEN_T);
                    break;
            }
#endif
            break;

        case TEXTURE_STATE_MINFILTER:
#if RETRO_USE_ORIGINAL_CODE
            D3DDevice->SetTextureStageState(stage, D3DTSS_MINFILTER, value);
#endif
            break;

        case TEXTURE_STATE_MAGFILTER:
#if RETRO_USE_ORIGINAL_CODE
            D3DDevice->SetTextureStageState(stage, D3DTSS_MAGFILTER, value);
#endif
            break;

        case TEXTURE_STATE_MIPFILTER:
#if RETRO_USE_ORIGINAL_CODE
            D3DDevice->SetTextureStageState(stage, D3DTSS_MIPFILTER, value);
#endif
            break;

        case TEXTURE_STATE_ALPHAOP:
#if RETRO_USE_ORIGINAL_CODE
            D3DDevice->SetTextureStageState(stage, D3DTSS_ALPHAOP, value);
#endif
            break;

        case TEXTURE_STATE_ALPHAARG1:
#if RETRO_USE_ORIGINAL_CODE
            D3DDevice->SetTextureStageState(stage, D3DTSS_ALPHAARG1, value);
#endif
            break;

        case TEXTURE_STATE_ALPHAARG2:
#if RETRO_USE_ORIGINAL_CODE
            D3DDevice->SetTextureStageState(stage, D3DTSS_ALPHAARG2, value);
#endif
            break;

        default: break;
    }
}

void EnableLight(int id, bool enabled)
{
#if RETRO_USE_ORIGINAL_CODE
    D3DDevice->LightEnable(id, enabled);
#else
    if (enabled)
        glEnable(GL_LIGHT0 + id);
    else
        glDisable(GL_LIGHT0 + id);
#endif
}

void EnableVSync(bool enabled)
{
#if !RETRO_USE_ORIGINAL_CODE
    SDL_GL_SetSwapInterval(enabled);
#endif
}

void DrawIndexedPrimitive(RenderFVF type, void *pVertices, int numVertices, void *pIndices, int numIndices)
{
#if RETRO_USE_ORIGINAL_CODE
    int fvf = type ? D3DFVF_LVERTEX : D3DFVF_VERTEX;
    D3DDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, fvf, pVertices, numVertices, (ushort *)(pIndices), numIndices, 0);
#else
    if (!pVertices || !pIndices || numVertices <= 0 || numIndices <= 0)
        return;

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    GLboolean lightEnabled = glIsEnabled(GL_LIGHTING);

    switch (type) {
        case RENDER_FVF_VERTEX: {
            Vertex *vertex = (Vertex *)(pVertices);

            glDisableClientState(GL_COLOR_ARRAY);
            glEnableClientState(GL_NORMAL_ARRAY);

            glVertexPointer(3, GL_FLOAT, sizeof(*vertex), &vertex->x);
            glNormalPointer(GL_FLOAT, sizeof(*vertex), &vertex->nx);
            glTexCoordPointer(2, GL_FLOAT, sizeof(*vertex), &vertex->tu);
            break;
        }

        case RENDER_FVF_LVERTEX: {
            LVertex *vertex = (LVertex *)(pVertices);

            glDisable(GL_LIGHTING);

            glEnableClientState(GL_COLOR_ARRAY);
            glDisableClientState(GL_NORMAL_ARRAY);

            glVertexPointer(3, GL_FLOAT, sizeof(*vertex), &vertex->x);
            glColorPointer(GL_BGRA, GL_UNSIGNED_BYTE, sizeof(*vertex), &vertex->color);
            glTexCoordPointer(2, GL_FLOAT, sizeof(*vertex), &vertex->tu);
            break;
        }

        default: break;
    }

    glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_SHORT, pIndices);

    if (type == RENDER_FVF_LVERTEX && lightEnabled)
        glEnable(GL_LIGHTING);
#endif
}

void SetFade(float r, float g, float b, float a)
{
    LVertex vertices[4];
    MEM_ZERO(vertices, sizeof(vertices));

    ushort indices[] = { 0, 1, 2, 0, 2, 3 };

    Color color = PACK_ARGB_F(a, r, g, b);

    vertices[0] = { { -1.0f, -1.0f, 1.0f }, color, 0, 0.0f, 1.0f };
    vertices[1] = { { -1.0f, +1.0f, 1.0f }, color, 0, 0.0f, 0.0f };
    vertices[2] = { { +1.0f, +1.0f, 1.0f }, color, 0, 1.0f, 0.0f };
    vertices[3] = { { +1.0f, -1.0f, 1.0f }, color, 0, 1.0f, 0.0f };

    BeginScene();
    SetRenderTexture(0, NULL);
    DrawIndexedPrimitive(RENDER_FVF_LVERTEX, vertices, 4, indices, 6);
    EndScene();
}

void Texture::Release()
{
#if RETRO_USE_ORIGINAL_CODE
    ((IDirectDrawSurface7 *)(this))->Release();
#else
    glDeleteTextures(1, &this->id);
    this->id = 0;
#endif
}

// i'm not sure what this is for?
void ReleaseSurfaceID(byte id)
{
    if (surfaceCharacters2[id]) {
        surfaceCharacters2[id]->Release();
        surfaceCharacters2[id] = NULL;
    }
}

void ReleaseModelSurfaces()
{
    for (int i = 0; i < 5; ++i) {
        if (surfaceCharacters[i]) {
            surfaceCharacters[i]->Release();
            surfaceCharacters[i] = NULL;
        }
    }

    if (TextureUnused != NULL) {
        TextureUnused->Release();
        TextureUnused = NULL;
    }

    if (surfaceSonic != NULL) {
        surfaceSonic->Release();
        surfaceSonic = NULL;
    }

    if (surfaceTestZoneBG != NULL) {
        surfaceTestZoneBG->Release();
        surfaceTestZoneBG = NULL;
    }

    if (surface3DLogo != NULL) {
        surface3DLogo->Release();
        surface3DLogo = NULL;
    }

    if (surfaceMText != NULL) {
        surfaceMText->Release();
        surfaceMText = NULL;
    }

    if (surfaceShadow != NULL) {
        surfaceShadow->Release();
        surfaceShadow = NULL;
    }

#if RETRO_USE_ORIGINAL_CODE
    if (ZBuffer != NULL) {
        ZBuffer->Release();
        ZBuffer = NULL;
    }

    if (D3DDevice != NULL) {
        D3DDevice->Release();
        D3DDevice = NULL;
    }

    if (D3D != NULL) {
        D3D->Release();
        D3D = NULL;
    }
#endif
}
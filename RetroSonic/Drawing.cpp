#include "RetroEngine.hpp"

#if !RETRO_USE_ORIGINAL_CODE
int SCREEN_XSIZE;
#endif

Matrix3D MatrixInversed;
Matrix3D MatrixWorld;
Matrix3D MatrixView;
Matrix3D MatrixProjection;
Matrix3D MatrixIdentity;

int ResX = 1024;
int ResY = 768;
byte ColourDepth;
bool WindowMode = true;

#if RETRO_USE_ORIGINAL_CODE
IDirect3D7 *D3D;
IDirect3DDevice7 *D3DDevice;
IDirectDraw7 *DDraw;

IDirectDrawSurface7 *FrontBuffer;
IDirectDrawSurface7 *BackBuffer;
IDirectDrawSurface7 *ZBuffer;

bool D3DSupportsZBufferFmt;
bool D3DUseHardwareRendering = true;
byte D3DDeviceType;
byte D3DTexFmtSearchType;

tagRECT ClientRect;
tagRECT WindowRect;
tagRECT WindowStoreRect;

HWND HWnd;
HMENU HMenu;

HINSTANCE HInst;
int NCmdShow;
#elif RETRO_USE_SDL3 || RETRO_USE_SDL2
SDL_Window *Window;
SDL_GLContext GLContext;
#elif RETRO_USE_SDL1
SDL_Surface *Window;
#endif

bool GameRunning = false;

bool InitGraphicsAPI()
{
#if RETRO_USE_ORIGINAL_CODE
    if (FAILED(DirectDrawCreateEx(NULL, (void **)(&DDraw), IID_IDirectDraw7, NULL))) {
        MessageBox(HWnd, "DirectDrawCreate FAILED", 0, MB_ICONHAND);
        return false;
    }

    // Non-original, but this is apparently required for FreeImage's static library to work on Windows
    FreeImage_Initialise();

    return SUCCEEDED(InitDirectDraw());
#else
    return InitScreen();
#endif
}

void ReleaseGraphicsAPI()
{
    ReleaseModelSurfaces();
    for (int i = 0; i < 10; ++i) {
        ReleaseCharacterUITexture(i);
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

#if RETRO_USE_SDL3
    if (GLContext != NULL) {
        SDL_GL_DestroyContext(GLContext);
        GLContext = NULL;
    }
#elif RETRO_USE_SDL2
    if (GLContext != NULL) {
        SDL_GL_DeleteContext(GLContext);
        GLContext = NULL;
    }
#endif

    SDL_Quit();
#endif
}

#if RETRO_USE_ORIGINAL_CODE
bool InitDirectDraw()
{
    DDSURFACEDESC2 ddsdf;
    DDSURFACEDESC2 ddsdb;

    if (WindowMode == true) {
        DDraw->SetCooperativeLevel(HWnd, DDSCL_NORMAL);
        UpdateEngineWindowRect(HWnd);

        MEM_ZERO(&ddsdf, sizeof(ddsdf));

        ddsdf.dwSize = sizeof(ddsdf);

        ddsdf.dwFlags        = DDSD_CAPS;
        ddsdf.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE | DDSCAPS_3DDEVICE;

        DDraw->CreateSurface(&ddsdf, &FrontBuffer, NULL);

        IDirectDrawClipper *DDClipper;
        DDraw->CreateClipper(0, &DDClipper, NULL);

        DDClipper->SetHWnd(0, HWnd);
        FrontBuffer->SetClipper(DDClipper);

        DDClipper->Release();
        DDClipper = NULL;

        MEM_ZERO(&ddsdb, sizeof(ddsdb));

        ddsdb.dwSize = sizeof(ddsdb);

        ddsdb.dwFlags        = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH;
        ddsdb.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_3DDEVICE;

        ddsdb.dwWidth  = SCREEN_XSIZE * 2;
        ddsdb.dwHeight = SCREEN_YSIZE * 2;

        DDraw->CreateSurface(&ddsdb, &BackBuffer, NULL);
    }
    else {
        DDraw->SetCooperativeLevel(HWnd, DDSCL_FULLSCREEN | DDSCL_EXCLUSIVE);
        DDraw->SetDisplayMode(ResX, ResY, 32, 0, 0);

        MEM_ZERO(&ddsdf, sizeof(ddsdf));

        ddsdf.dwSize = sizeof(ddsdf);

        ddsdf.dwFlags           = DDSD_CAPS | DDSD_BACKBUFFERCOUNT;
        ddsdf.ddsCaps.dwCaps    = DDSCAPS_PRIMARYSURFACE | DDSCAPS_COMPLEX | DDSCAPS_FLIP | DDSCAPS_3DDEVICE;
        ddsdf.dwBackBufferCount = 1;

        DDraw->CreateSurface(&ddsdf, &FrontBuffer, 0);

        DDSCAPS2 ddcaps;
        MEM_ZERO(&ddcaps, sizeof(ddcaps));

        ddcaps.dwCaps = DDSCAPS_BACKBUFFER;

        FrontBuffer->GetAttachedSurface(&ddcaps, &BackBuffer);

        DDBLTFX ddbltfx;
        MEM_ZERO(&ddbltfx, sizeof(ddbltfx));

        ddbltfx.dwSize      = sizeof(ddbltfx);
        ddbltfx.dwFillColor = 0;

        BackBuffer->Blt(NULL, NULL, NULL, DDBLT_WAIT | DDBLT_COLORFILL, &ddbltfx);
    }

    DDraw->GetDisplayMode(&ddsdf);

    if (ddsdf.ddpfPixelFormat.dwRGBBitCount == 24 || ddsdf.ddpfPixelFormat.dwRGBBitCount == 8) {
        MessageBox(HWnd, "This Colour Depth is not suitable for Retro-Sonic. Please use either 16bit or 32bit colour mode", "Colour Depth",
                   MB_ICONERROR);
    }
    else {
        DDBLTFX ddbltfx;
        MEM_ZERO(&ddbltfx, sizeof(ddbltfx));

        ddbltfx.dwSize      = sizeof(ddbltfx);
        ddbltfx.dwFillColor = 0;

        BackBuffer->Blt(NULL, NULL, NULL, DDBLT_WAIT | DDBLT_COLORFILL, &ddbltfx);

        if (!GameRunning || InitScreen()) {
            // this branch is empty for an amazing reason
        }
    }

    return false;
}
#endif

bool InitScreen()
{
#if RETRO_USE_ORIGINAL_CODE
    D3DSupportsZBufferFmt = false;
    if (FAILED(DDraw->QueryInterface(IID_IDirect3D7, (void **)(&D3D))))
        return false;

    DDPIXELFORMAT format;
    MEM_ZERO(&format, sizeof(format));

    format.dwSize  = sizeof(format);
    format.dwFlags = DDPF_ZBUFFER;

    D3D->EnumZBufferFormats(IID_IDirect3DTnLHalDevice, EnumZBufferFormatsCallback, &format);
    if (D3DSupportsZBufferFmt == true) {
        D3DDeviceType = D3D_DEVICE_TNL_HAL;
    }
    else {
        D3D->EnumZBufferFormats(IID_IDirect3DHALDevice, EnumZBufferFormatsCallback, &format);
        if (D3DSupportsZBufferFmt == true) {
            D3DDeviceType = D3D_DEVICE_HAL;
        }
        else {
            D3D->EnumZBufferFormats(IID_IDirect3DRGBDevice, EnumZBufferFormatsCallback, &format);
            if (D3DSupportsZBufferFmt == true)
                D3DDeviceType = D3D_DEVICE_RGB;
        }
    }

    DDSURFACEDESC2 description;
    MEM_ZERO(&description, sizeof(description));

    description.dwSize  = sizeof(description);
    description.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH | DDSD_PIXELFORMAT;

    if (D3DDeviceType <= D3D_DEVICE_UNINITALIZED)
        description.ddsCaps.dwCaps = DDSCAPS_SYSTEMMEMORY | DDSCAPS_ZBUFFER;
    else
        description.ddsCaps.dwCaps = DDSCAPS_VIDEOMEMORY | DDSCAPS_ZBUFFER;

    if (WindowMode == true) {
        description.dwWidth  = SCREEN_XSIZE * 2;
        description.dwHeight = SCREEN_YSIZE * 2;
    }
    else if (WindowMode == false) {
        description.dwWidth  = ResX;
        description.dwHeight = ResY;
    }

    memcpy(&description.ddpfPixelFormat, &format, sizeof(description.ddpfPixelFormat));
    if (SUCCEEDED(DDraw->CreateSurface(&description, &ZBuffer, NULL)))
        BackBuffer->AddAttachedSurface(ZBuffer);

    if (SUCCEEDED(D3D->CreateDevice(IID_IDirect3DTnLHalDevice, BackBuffer, &D3DDevice)))
        D3DUseHardwareRendering = true;
    else if (SUCCEEDED(D3D->CreateDevice(IID_IDirect3DHALDevice, BackBuffer, &D3DDevice)))
        D3DUseHardwareRendering = true;
    else if (SUCCEEDED(D3D->CreateDevice(IID_IDirect3DRGBDevice, BackBuffer, &D3DDevice)))
        /* The game will most likely crash if hardware rendering isn't avaliable */
        D3DUseHardwareRendering = false;
    else
        /* :skull: */
        return false;

    D3DVIEWPORT7 viewport;
    viewport.dwX = 0;
    viewport.dwY = 0;

    if (WindowMode) {
        viewport.dwWidth  = SCREEN_XSIZE * 2;
        viewport.dwHeight = SCREEN_YSIZE * 2;
    }
    else {
        viewport.dwWidth  = ResX;
        viewport.dwHeight = ResY;
    }

    viewport.dvMinZ = 0.0f;
    viewport.dvMaxZ = 1.0f;

    if (FAILED(D3DDevice->SetViewport(&viewport)))
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

    MEM_ZERO(&MatrixProjection, sizeof(MatrixProjection));
    MatrixPerspective(&MatrixProjection, TO_RADIAN(45.0f), SCREEN_YSIZE_F / SCREEN_XSIZE_F, 1.0f, 1000.0f);

    Light light = {};
    light.type  = LIGHT_DIRECTIONAL;

    light.direction = { 0.00f, -0.90f, 0.10f /* */ };
    light.diffuse   = { 0.80f, +0.80f, 0.80f, 0.0f };
    light.ambient   = { 0.45f, +0.45f, 0.45f, 0.0f };
    light.specular  = { 1.00f, +1.00f, 1.00f, 0.0f };

    SetRenderLight(0, &light);
    EnableLight(0, true);

    RenderMaterial.ambient = { 1.0f, 1.0f, 1.0f, 1.0f };
    RenderMaterial.diffuse = RenderMaterial.ambient;

    RenderMaterial.specular = { 0.25f, 0.25f, 1.0f, 0.25f };
    RenderMaterial.power    = 80.0f;

    SetRenderTransform(RENDER_TRANSFORM_PROJECTION, &MatrixProjection);
    SetRenderState(RENDER_STATE_DITHERENABLE, true);

    SetRenderTextureStageState(0, TEXTURE_STATE_MINFILTER, TEXTURE_VALUE_SELECTARG1);
    SetRenderTextureStageState(0, TEXTURE_STATE_MAGFILTER, TEXTURE_VALUE_SELECTARG1);
    SetRenderTextureStageState(0, TEXTURE_STATE_MIPFILTER, TEXTURE_VALUE_SELECTARG1);

    SetRenderState(RENDER_STATE_ZENABLE, true);
    SetRenderState(RENDER_STATE_LIGHTING, true);

    LoadTexture(&BGTexture, "Data/Levels/TestZone/BG.png", false);
    LoadTexture(&SonicTexture, "Data/Characters/Sonic.png", false);
    LoadTexture(&LogoTexture, "Data/Title/Logo.png", false);
    LoadTexture(&ShadowTexture, "Data/Objects/Shadow.png", false);

    UnusedVector = { 4.0f, 4.0f, 4.0f };

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
    if (!D3DUseHardwareRendering)
        SetRenderState(RENDER_STATE_TEXTUREPERSPECTIVE, false);
#endif

    return true;
}

void FlipScreen()
{
#if RETRO_USE_ORIGINAL_CODE
    if (WindowMode == true)
        FrontBuffer->Blt(&WindowRect, BackBuffer, NULL, DDBLT_WAIT, NULL);
    else if (WindowMode == false)
        FrontBuffer->Flip(NULL, DDFLIP_WAIT);
#elif RETRO_USE_SDL3 || RETRO_USE_SDL2
    SDL_GL_SwapWindow(Window);
#elif RETRO_USE_SDL1
    SDL_GL_SwapBuffers();
#endif
}

void SetScreenResolution(sbyte windowResolution, sbyte fullscreenResolution, sbyte colourDepth)
{
#if RETRO_USE_ORIGINAL_CODE
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
#else
    ResX = SCREEN_XSIZE * Settings.windowScale;
    ResY = SCREEN_YSIZE * Settings.windowScale;
#endif

    ColourDepth = colourDepth;
}

void ToggleScreenMode()
{
#if RETRO_USE_ORIGINAL_CODE
    DDraw->SetCooperativeLevel(HWnd, DDSCL_NORMAL);

    for (int i = 0; i < 10; ++i) {
        ReleaseCharacterUITexture(i);
    }

    if (GameRunning == true)
        ReleaseModelSurfaces();

    if (BackBuffer != NULL) {
        BackBuffer->Release();
        BackBuffer = NULL;
    }
#endif

    if (WindowMode == true) {
        WindowMode = false;
#if RETRO_USE_ORIGINAL_CODE
        ShowWindow(HWnd, SW_HIDE);
        SetWindowLongA(HWnd, GWL_STYLE, WS_POPUP);
        HMenu = GetMenu(HWnd);
        SetMenu(HWnd, NULL);
        ShowCursor(false);
#elif RETRO_USE_SDL3
        SDL_SetWindowFullscreen(Window, true);
        SDL_HideCursor();
#elif RETRO_USE_SDL2
        SDL_SetWindowFullscreen(Window, SDL_WINDOW_FULLSCREEN_DESKTOP);
        SDL_ShowCursor(SDL_DISABLE);
#elif RETRO_USE_SDL1
        Window = SDL_SetVideoMode(ResX, ResY, 32, SDL_OPENGL | SDL_FULLSCREEN);
        SDL_ShowCursor(SDL_DISABLE);
#endif
    }
    else if (WindowMode == false) {
        WindowMode = true;
#if RETRO_USE_ORIGINAL_CODE
        ShowWindow(HWnd, SW_HIDE);
        SetWindowLongA(HWnd, GWL_STYLE, WS_OVERLAPPEDWINDOW);
        SetMenu(HWnd, HMenu);
        ShowCursor(true);
#elif RETRO_USE_SDL3
        SDL_SetWindowFullscreen(Window, false);
        SDL_ShowCursor();
#elif RETRO_USE_SDL2
        SDL_SetWindowFullscreen(Window, 0);
        SDL_ShowCursor(SDL_ENABLE);
#elif RETRO_USE_SDL1
        Window = SDL_SetVideoMode(ResX, ResY, 32, SDL_OPENGL);
        SDL_ShowCursor(SDL_ENABLE);
#endif
    }

#if RETRO_USE_ORIGINAL_CODE
    ShowWindow(HWnd, SW_SHOW);

    if (FrontBuffer != NULL) {
        FrontBuffer->Release();
        FrontBuffer = NULL;
    }

    if (WindowMode == true) {
        tagRECT *rect = &WindowStoreRect;
        SetWindowPos(HWnd, HWND_NOTOPMOST, rect->left, rect->top, rect->right - rect->left, rect->bottom - rect->top, SWP_SHOWWINDOW);
    }

    InitDirectDraw();
#elif RETRO_USE_SDL1
    glViewport(0, 0, ResX, ResY);

    // Textures need to be reloaded
    InitScreen();
    LoadObjectAssets();
    LoadLevelAssets();
    LoadFontAssets();
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

void SetRenderTexture(int id, Texture *texture)
{
#if RETRO_USE_ORIGINAL_CODE
    D3DDevice->SetTexture(id, (IDirectDrawSurface7 *)(texture));
#else
    if (texture != NULL && texture->id) {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texture->id);
    }
    else {
        glDisable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
#endif
}

void SetRenderMaterial(Material *material)
{
#if RETRO_USE_ORIGINAL_CODE
    D3DDevice->SetMaterial((D3DMATERIAL7 *)(material));
#else
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, (const GLfloat *)(&material->ambient));
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, (const GLfloat *)(&material->diffuse));
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, (const GLfloat *)(&material->specular));
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, material->power);
    glColor4f(material->diffuse.r, material->diffuse.g, material->diffuse.b, material->diffuse.a);
#endif
}

void SetRenderLight(int id, Light *light)
{
#if RETRO_USE_ORIGINAL_CODE
    D3DDevice->SetLight(id, (D3DLIGHT7 *)(light));
#else
    glLightfv(GL_LIGHT0 + id, GL_AMBIENT, (const GLfloat *)(&light->ambient));
    glLightfv(GL_LIGHT0 + id, GL_DIFFUSE, (const GLfloat *)(&light->diffuse));
    glLightfv(GL_LIGHT0 + id, GL_SPECULAR, (const GLfloat *)(&light->specular));

    switch (light->type) {
        case LIGHT_DIRECTIONAL: {
            GLfloat position[] = { -light->direction.x, -light->direction.y, -light->direction.z, 0.0f };
            glLightfv(GL_LIGHT0 + id, GL_POSITION, position);
            break;
        }

        default: break;
    }
#endif
}

void SetRenderTransform(RenderTransform type, Matrix3D *matrix)
{
#if !RETRO_USE_ORIGINAL_CODE
    static Matrix3D ViewTemp  = Matrix3D::Identity;
    static Matrix3D WorldTemp = Matrix3D::Identity;
#endif

    switch (type) {
        case RENDER_TRANSFORM_WORLD:
#if RETRO_USE_ORIGINAL_CODE
            D3DDevice->SetTransform(D3DTRANSFORMSTATE_WORLD, (D3DMATRIX *)(matrix));
#else
            glMatrixMode(GL_MODELVIEW);
            memcpy(&WorldTemp, matrix, sizeof(WorldTemp));

            glLoadMatrixf(&ViewTemp.m[0][0]);
            glMultMatrixf(&WorldTemp.m[0][0]);
#endif
            break;

        case RENDER_TRANSFORM_VIEW:
#if RETRO_USE_ORIGINAL_CODE
            D3DDevice->SetTransform(D3DTRANSFORMSTATE_VIEW, (D3DMATRIX *)(matrix));
#else
            glMatrixMode(GL_MODELVIEW);
            memcpy(&ViewTemp, matrix, sizeof(ViewTemp));

            glLoadMatrixf(&ViewTemp.m[0][0]);
            glMultMatrixf(&WorldTemp.m[0][0]);
#endif
            break;

        case RENDER_TRANSFORM_PROJECTION:
#if RETRO_USE_ORIGINAL_CODE
            D3DDevice->SetTransform(D3DTRANSFORMSTATE_PROJECTION, (D3DMATRIX *)(matrix));
#else
            glMatrixMode(GL_PROJECTION);
            glLoadMatrixf(&matrix->m[0][0]);
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
            glAlphaFunc(GLAlphaFunc, GLAlphaRef);
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
#else
            switch (value) {
                default:
                case TEXTURE_VALUE_SELECTARG1: glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); break;
                case TEXTURE_VALUE_DISABLE: glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); break;
            }
#endif
            break;

        case TEXTURE_STATE_MAGFILTER:
#if RETRO_USE_ORIGINAL_CODE
            D3DDevice->SetTextureStageState(stage, D3DTSS_MAGFILTER, value);
#else
            switch (value) {
                default:
                case TEXTURE_VALUE_SELECTARG1: glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); break;
                case TEXTURE_VALUE_DISABLE: glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); break;
            }
#endif
            break;

        case TEXTURE_STATE_MIPFILTER:
#if RETRO_USE_ORIGINAL_CODE
            D3DDevice->SetTextureStageState(stage, D3DTSS_MIPFILTER, value);
#else
            switch (value) {
                default:
                case TEXTURE_VALUE_SELECTARG1: glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); break;
                case TEXTURE_VALUE_DISABLE: glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); break;
            }
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

void DrawFace(RenderFVF type, void *vertices, int numVertices, void *indices, int numIndices)
{
#if RETRO_USE_ORIGINAL_CODE
    int fvf = type ? D3DFVF_LVERTEX : D3DFVF_VERTEX;
    D3DDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, fvf, vertices, numVertices, (ushort *)(indices), numIndices, 0);
#else
    if (!vertices || !indices || numVertices <= 0 || numIndices <= 0)
        return;

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    GLboolean lightEnabled = glIsEnabled(GL_LIGHTING);

    switch (type) {
        case RENDER_FVF_VERTEX: {
            Vertex *vertex = (Vertex *)(vertices);

            glDisableClientState(GL_COLOR_ARRAY);
            glEnableClientState(GL_NORMAL_ARRAY);

            glVertexPointer(3, GL_FLOAT, sizeof(*vertex), &vertex->x);
            glNormalPointer(GL_FLOAT, sizeof(*vertex), &vertex->nx);
            glTexCoordPointer(2, GL_FLOAT, sizeof(*vertex), &vertex->tu);
            break;
        }

        case RENDER_FVF_LVERTEX: {
            LVertex *vertex = (LVertex *)(vertices);

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

    glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_SHORT, indices);

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
    DrawFace(RENDER_FVF_LVERTEX, vertices, 4, indices, 6);
    EndScene();
}

void Texture::Release()
{
#if RETRO_USE_ORIGINAL_CODE
    IDirectDrawSurface7 *texture = (IDirectDrawSurface7 *)(this);
    texture->Release();
#else
    glDeleteTextures(1, &this->id);
    this->id = 0;
#endif
}

void ReleaseCharacterUITexture(byte id)
{
    if (CharacterUITexture[id]) {
        CharacterUITexture[id]->Release();
        CharacterUITexture[id] = NULL;
    }
}

void ReleaseModelSurfaces()
{
    for (int i = 0; i < CHARACTER_COUNT; ++i) {
        if (CharacterTexture[i]) {
            CharacterTexture[i]->Release();
            CharacterTexture[i] = NULL;
        }
    }

    if (UnusedTexture != NULL) {
        UnusedTexture->Release();
        UnusedTexture = NULL;
    }

    if (SonicTexture != NULL) {
        SonicTexture->Release();
        SonicTexture = NULL;
    }

    if (BGTexture != NULL) {
        BGTexture->Release();
        BGTexture = NULL;
    }

    if (LogoTexture != NULL) {
        LogoTexture->Release();
        LogoTexture = NULL;
    }

    if (FontTexture != NULL) {
        FontTexture->Release();
        FontTexture = NULL;
    }

    if (ShadowTexture != NULL) {
        ShadowTexture->Release();
        ShadowTexture = NULL;
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
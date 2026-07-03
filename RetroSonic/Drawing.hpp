#ifndef DRAWING_H
#define DRAWING_H

#include "RetroEngine.hpp"

#define PACK_A_I(v) ((Color)(((v) & 0xFF) << 24))
#define PACK_R_I(v) ((Color)(((v) & 0xFF) << 16))
#define PACK_G_I(v) ((Color)(((v) & 0xFF) << 8))
#define PACK_B_I(v) ((Color)(((v) & 0xFF) << 0))

#define UNPACK_A_I(v) (((v) >> 24) & 0xFF)
#define UNPACK_R_I(v) (((v) >> 16) & 0xFF)
#define UNPACK_G_I(v) (((v) >> 8) & 0xFF)
#define UNPACK_B_I(v) (((v) >> 0) & 0xFF)

#define PACK_A_F(v) (PACK_A_I((Color)((v) * 255.0f)))
#define PACK_R_F(v) (PACK_R_I((Color)((v) * 255.0f)))
#define PACK_G_F(v) (PACK_G_I((Color)((v) * 255.0f)))
#define PACK_B_F(v) (PACK_B_I((Color)((v) * 255.0f)))

#define UNPACK_B_F(v) ((float)(UNPACK_B_I(v)) / 255.0f)
#define UNPACK_G_F(v) ((float)(UNPACK_G_I(v)) / 255.0f)
#define UNPACK_R_F(v) ((float)(UNPACK_R_I(v)) / 255.0f)
#define UNPACK_A_F(v) ((float)(UNPACK_A_I(v)) / 255.0f)

#define PACK_ARGB_I(a, r, g, b) (PACK_A_I(a) | PACK_R_I(r) | PACK_G_I(g) | PACK_B_I(b))
#define PACK_ARGB_F(a, r, g, b) (PACK_A_F(a) | PACK_R_F(r) | PACK_G_F(g) | PACK_B_F(b))

#if !RETRO_USE_ORIGINAL_CODE
extern int SCREEN_XSIZE;
#endif

enum RenderTransform {
    RENDER_TRANSFORM_WORLD,
    RENDER_TRANSFORM_VIEW,
    RENDER_TRANSFORM_PROJECTION,
};

enum RenderState {
    RENDER_STATE_LIGHTING,
    RENDER_STATE_ZENABLE,
    RENDER_STATE_ALPHAFUNC,
    RENDER_STATE_ALPHAREF,
    RENDER_STATE_SRCBLEND,
    RENDER_STATE_DESTBLEND,
    RENDER_STATE_SPECULARENABLE,
    RENDER_STATE_ALPHABLENDENABLE,
    RENDER_STATE_ALPHATESTENABLE,
    RENDER_STATE_DITHERENABLE,
    RENDER_STATE_TEXTUREPERSPECTIVE,
};

enum RenderFVF {
    RENDER_FVF_VERTEX,
    RENDER_FVF_LVERTEX,
};

enum TextureStageState {
    TEXTURE_STATE_TEXCOORDINDEX,
    TEXTURE_STATE_MINFILTER,
    TEXTURE_STATE_MAGFILTER,
    TEXTURE_STATE_MIPFILTER,
    TEXTURE_STATE_ALPHAOP,
    TEXTURE_STATE_ALPHAARG1,
    TEXTURE_STATE_ALPHAARG2,
};

enum TextureStageValue {
    TEXTURE_VALUE_DISABLE    = 1,
    TEXTURE_VALUE_SELECTARG1 = 2,
    TEXTURE_VALUE_MODULATE   = 4,

    TEXTURE_VALUE_PASSTHRU                     = 0x00000000,
    TEXTURE_VALUE_CAMERASPACE_NORMAL           = 0x00010000,
    TEXTURE_VALUE_CAMERASPACE_POSITION         = 0x00020000,
    TEXTURE_VALUE_CAMERASPACE_REFLECTIONVECTOR = 0x00030000,
};

enum TextureFormatSearchType : sbyte {
    TEXTURE_FMT_32BIT,
    TEXTURE_FMT_16BIT_ALPHA,
};

enum D3DZBufferResultType {
    D3D_DEVICE_NONE,
    D3D_DEVICE_TNL_HAL,
    D3D_DEVICE_HAL,
    D3D_DEVICE_RGB,

    D3D_DEVICE_UNINITALIZED = -3,
};

enum LightType {
    LIGHT_POINT       = 1,
    LIGHT_SPOT        = 2,
    LIGHT_DIRECTIONAL = 3,
    LIGHT_FORCE_S32   = 0x7FFFFFFF,
};

using Color = uint;

struct ColorValue {
    float r;
    float g;
    float b;
    float a;
};

struct LVertex {
    float x;
    float y;
    float z;
    int reserved;
    Color color;
    Color specular;
    float tu;
    float tv;

    LVertex() {}

    LVertex(const Vector3D &v, Color color, Color specular, float tu, float tv)
    {
        this->x        = v.x;
        this->y        = v.y;
        this->z        = v.z;
        this->reserved = 0;
        this->color    = color;
        this->specular = specular;
        this->tu       = tu;
        this->tv       = tv;
    }
};

struct Vertex {
    float x;
    float y;
    float z;
    float nx;
    float ny;
    float nz;
    float tu;
    float tv;

    Vertex() {}

    Vertex(const Vector3D &v, const Vector3D &n, float tu, float tv)
    {
        this->x  = v.x;
        this->y  = v.y;
        this->z  = v.z;
        this->nx = n.x;
        this->ny = n.y;
        this->nz = n.z;
        this->tu = tu;
        this->tv = tv;
    }
};

struct Material {
    ColorValue diffuse;
    ColorValue ambient;
    ColorValue specular;
    ColorValue emissive;
    float power;
};

struct Light {
    LightType type;
    ColorValue diffuse;
    ColorValue specular;
    ColorValue ambient;
    Vector3D position;
    Vector3D direction;
    float range;
    float falloff;
    float attenuation[3];
    float theta;
    float phi;
};

struct Texture {
    void *vtbl;
    uint id;
    int width;
    int height;

    void Release();
};

extern Matrix3D MatrixInversed;
extern Matrix3D MatrixWorld;
extern Matrix3D MatrixView;
extern Matrix3D MatrixProjection;
extern Matrix3D MatrixIdentity;

extern char WindowMode;

extern int ResX;
extern int ResY;
extern sbyte ColourDepth;

#if RETRO_USE_ORIGINAL_CODE
extern IDirect3D7 *D3D;
extern IDirect3DDevice7 *D3DDevice;
extern IDirectDraw7 *DDraw;

extern IDirectDrawSurface7 *FrontBuffer;
extern IDirectDrawSurface7 *BackBuffer;
extern IDirectDrawSurface7 *ZBuffer;

extern tagRECT ClientRect;
extern tagRECT WindowRect;
extern tagRECT WindowStoreRect;

extern bool D3DSupportsZBufferFmt;
extern bool D3DUseHardwareRendering;
extern byte D3DDeviceType;
extern byte D3DTexFmtSearchType;

extern HWND HWnd;
extern HMENU HMenu;

extern HINSTANCE HInst;
extern int NCmdShow;
#elif RETRO_USE_SDL3 || RETRO_USE_SDL2
extern SDL_Window *Window;
extern SDL_GLContext GLContext;
#elif RETRO_USE_SDL1
extern SDL_Surface *Window;
#endif

extern bool EngineRunning;

bool InitGraphicsAPI();
void ReleaseGraphicsAPI();
#if RETRO_USE_ORIGINAL_CODE
bool InitDirect3D();
#endif

bool InitScreen();
void FlipScreen();

void SetScreenResolution(sbyte resolution, sbyte windowResolution, sbyte colourDepth);
void ToggleScreenMode();
void ClearScreen(Color color);

void BeginScene();
void EndScene();

void SetRenderTexture(int id, Texture *pTexture);
void SetRenderMaterial(Material *pMaterial);
void SetRenderLight(int id, Light *pLight);
void SetRenderTransform(RenderTransform type, Matrix3D *pMatrix);
void SetRenderState(RenderState type, int value);
void SetRenderTextureStageState(int stage, TextureStageState type, int value);
void EnableLight(int id, bool enabled);

void DrawIndexedPrimitive(RenderFVF type, void *pVertices, int numVertices, void *pIndices, int numIndices);
void SetFade(float r, float g, float b, float a);

void ReleaseCharacterUITexture(byte id);
void ReleaseModelSurfaces();
void ReleaseGraphicsAPI();

#endif // !DRAWING_H
#ifndef R_RENDER_H
#define R_RENDER_H

#include "RetroEngine.hpp"

#define R3D_SCALE(base, scale) (base * scale)

#define TO_ARGB(a, r, g, b) ((D3DCOLOR)((((a) & 0xFF) << 24) | (((r) & 0xFF) << 16) | (((g) & 0xFF) << 8) | ((b) & 0xFF)))

#define TO_ARGB_F(a, r, g, b)                                                                                                                        \
    ((D3DCOLOR)((((DWORD)((a) * 255.0f)) & 0xFF) << 24 | (((DWORD)((r) * 255.0f)) & 0xFF) << 16 | (((DWORD)((g) * 255.0f)) & 0xFF) << 8              \
                | (((DWORD)((b) * 255.0f)) & 0xFF)))

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
    TEXTURE_VALUE_MODULATE,
    TEXTURE_VALUE_SELECTARG1,
    TEXTURE_VALUE_DISABLE,
};

extern tagRECT rect_420488;
extern tagRECT clientRect;
extern tagRECT windowBounds;

extern D3DMATRIX MatrixSonicModel;
extern D3DMATRIX MatrixSonicAni_4C8990[36];
extern D3DMATRIX MatrixSonicAni_4C9290[36];

extern D3DMATRIX MatrixInversed;
extern D3DMATRIX MatrixWorld;
extern D3DMATRIX MatrixView;
extern D3DMATRIX MatrixProjection;
extern D3DMATRIX MatrixIdentity;

extern LPDIRECTDRAWSURFACE7 surfaceList[10];
extern LPDIRECTDRAWSURFACE7 surface_4C9D3C;
extern LPDIRECTDRAWSURFACE7 surface_439D40;
extern LPDIRECTDRAWSURFACE7 surface_4C9D44;

extern char *StrRenderFailError;
extern TextureFormatSearchType TexFmtSearchType;

extern char WindowMode;
extern int32_t dword_41F0C4;

extern sbyte ResX;
extern sbyte ResY;
extern sbyte ColourDepth;

void UpdateWindowRect(HWND hWnd);
void ResetWindow(HWND hWnd);

void DrawTitleModel(char type);
void CopyMatrix_4C9B90_4C9C50();
void SonicMat_WorldTransform();
void ReleaseSurfaceID(uint8_t id);

HRESULT RefreshSurfaces(HWND hWnd);
void ReleaseModelSurfaces();
void ReleaseGraphicsAPI();

#endif // !R_RENDER_H
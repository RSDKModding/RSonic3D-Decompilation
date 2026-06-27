#ifndef R_RENDER_H
#define R_RENDER_H

#include "RetroEngine.hpp"

#define R3D_SCALE(base, scale) (base * scale)

extern tagRECT rect_420488;
extern tagRECT clientRect;
extern tagRECT windowBounds;

extern D3DMATRIX matrix_4C8990[36];
extern D3DMATRIX matrix_4C9290[36];
extern D3DMATRIX matSonicMdl;
extern D3DMATRIX matrixBackgroundTransform;
extern D3DMATRIX matProject;
extern D3DMATRIX matWorld;
extern D3DMATRIX matrix_4C9C90;
extern D3DMATRIX matView;
extern LPDIRECTDRAWSURFACE7 surfaceList[10];

extern LPDIRECTDRAW7 DDraw;
extern HRESULT ddState;
extern LPDIRECTDRAWSURFACE7 surface_4C9D3C;
extern LPDIRECTDRAWSURFACE7 surface_439D40;
extern LPDIRECTDRAWSURFACE7 surface_4C9D44;
extern IDirect3DDevice7 *D3DDevice;
extern IDirect3D7 *d3d;

extern char *StrRenderFailError;
extern TextureFormatSearchType TexFmtSearchType;

extern char WindowMode;
extern int32_t dword_41F0C4;

extern sbyte ResX;
extern sbyte ResY;
extern sbyte ColourDepth;

BOOL TryInitDirectDraw(HWND hWnd);
BOOL TryInitRenderDevice();
BOOL InitDrawSurface(HWND hWnd);
BOOL InitScreen();
void UpdateWindowRect(HWND hWnd);
void ResetWindow(HWND hWnd);

HRESULT BeginScene();
HRESULT EndScene();

void DrawTitleModel(char type);
void CopyMatrix_4C9B90_4C9C50();
void SonicMat_WorldTransform();
HRESULT Render_ClearScreen(D3DCOLOR color);

void SetFade(float a1, float a2, float a3, float a4);
void FlipScreen();
void ReleaseSurfaceID(uint8_t id);

HRESULT RefreshSurfaces(HWND hWnd);
void ReleaseModelSurfaces();
void ReleaseGraphicsAPI();

#endif // !R_RENDER_H
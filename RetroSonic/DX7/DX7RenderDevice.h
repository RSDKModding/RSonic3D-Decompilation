#ifndef RENDER_DEVICE_H
#define RENDER_DEVICE_H

#include "Drawing.hpp"

namespace RenderDevice
{
    extern IDirect3D7 *D3D;
    extern IDirect3DDevice7 *D3DDevice;
    extern IDirectDraw7 *DDraw;

    extern HWND HWnd;
    extern HINSTANCE HInst;

    bool InitGraphicsAPI();
    bool InitDirect3D(HWND hWnd);
    bool InitScreen();
    void FlipScreen();

    HRESULT RefreshSurfaces(HWND hWnd);

    void BeginScene();
    void EndScene();
    void Clear(int color);

    void SetTexture(int stage, IDirectDrawSurface7 *pTexture);
    void SetMaterial(D3DMATERIAL7 *pMaterial);
    void SetLight(int id, D3DLIGHT7 *pLight);
    void SetTransform(RenderTransform type, D3DMATRIX *pMatrix);
    void SetTextureStageState(int stage, TextureStageState type, int value);
    void SetRenderState(RenderState type, int value);

    void LightEnable(int id, bool enabled);

    void DrawIndexed(RenderFVF type, void *pVertices, int numVertices, void *pIndices, int numIndices);
    void DrawFade(float r, float g, float b, float a);
};

#endif // !RENDER_DEVICE_H
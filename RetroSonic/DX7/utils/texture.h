#ifndef DX7_TEXTURE_H
#define DX7_TEXTURE_H

HRESULT CALLBACK TextureSearchCallback(DDPIXELFORMAT *pddpf, VOID *param);
HRESULT WINAPI EnumZBufferFormatsCallback(DDPIXELFORMAT *pddpf, VOID *pddpfDesired);

IDirectDrawSurface7 *CreateTextureFromBitmap(IDirect3DDevice7 *pd3dDevice, HBITMAP hbm);
IDirectDrawSurface7 *CreateTexture(IDirect3DDevice7 *pd3dDevice, CHAR *strName);

#endif // !DX7_TEXTURE_H
#ifndef DX7_DDUTIL_H
#define DX7_DDUTIL_H

#if RETRO_USE_ORIGINAL_CODE
void DDLoadBitmap(LPCSTR szBitmap, int id);
inline void DDCopyBitmap(IDirectDrawSurface7 *pdds, HBITMAP hbm, int dwWidth, int dwHeight, int dx, int dy);
#endif

#endif // !DX7_DDUTIL_H
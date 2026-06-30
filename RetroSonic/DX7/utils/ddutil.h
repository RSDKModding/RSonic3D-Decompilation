#ifndef DX7_DDUTIL_H
#define DX7_DDUTIL_H

IDirectDrawSurface7 *DDLoadBitmap(LPCSTR szBitmap, int id);
inline void DDCopyBitmap(IDirectDrawSurface7 *pdds, HBITMAP hbm, int dwWidth, int dwHeight, int dx, int dy);

#endif // !DX7_DDUTIL_H
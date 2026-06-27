//-----------------------------------------------------------------------------
// File: ddutil.cpp
//
// Desc: Routines for loading bitmap and palettes from resources
//
//
//
// Copyright (c) 1995-1999 Microsoft Corporation. All rights reserved.
//-----------------------------------------------------------------------------

#include "RetroEngine.hpp"

//-----------------------------------------------------------------------------
// Name: DDLoadBitmap()
// Desc: Create a DirectDrawSurface from a bitmap resource.
//-----------------------------------------------------------------------------
IDirectDrawSurface7 *DDLoadBitmap(LPCSTR szBitmap, int id)
{
    BITMAP bm;
    DDSURFACEDESC2 ddsd;

    //
    //  Try to load the bitmap as a file
    //
    HBITMAP hbm = (HBITMAP)LoadImage(NULL, szBitmap, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_DEFAULTSIZE);

    //
    // Get size of the bitmap
    //
    GetObject(hbm, sizeof(bm), &bm);

    //
    // Create a DirectDrawSurface for this bitmap
    //
    ZeroMemory(&ddsd, sizeof(ddsd));
    ddsd.dwSize         = sizeof(ddsd);
    ddsd.dwFlags        = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH;
    ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN;
    ddsd.dwWidth        = bm.bmWidth;
    ddsd.dwHeight       = bm.bmHeight;

    DDraw->CreateSurface(&ddsd, &surfaceList[id], NULL);

    DDCopyBitmap(surfaceList[id], hbm, ddsd.dwWidth, ddsd.dwHeight, 0, 0);
    DeleteObject(hbm);
}

//-----------------------------------------------------------------------------
// Name: DDCopyBitmap()
// Desc: Draw a bitmap into a DirectDrawSurface
//-----------------------------------------------------------------------------
void DDCopyBitmap(IDirectDrawSurface7 *pdds, HBITMAP hbm, int dwWidth, int dwHeight, int dx, int dy)
{
    HDC hdc;
    pdds->GetDC(&hdc);

    //
    // Select bitmap into a memoryDC so we can use it.
    //
    HDC hdcImage = CreateCompatibleDC(hdc);
    SelectObject(hdcImage, hbm);

    BitBlt(hdc, 0, 0, dwWidth, dwHeight, hdcImage, dx, dy, SRCCOPY);
    pdds->ReleaseDC(hdc);

    DeleteDC(hdcImage);
}
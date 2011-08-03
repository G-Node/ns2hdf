///////////////////////////////////////////////////////////////////////////////////////////////////
//
// Copyright (C) 2003-2005  Neuroshare Project                                                         
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// A copy of the GNU Lesser General Public License can be obtained by writing to:
//  Free Software Foundation, Inc.,
//  59 Temple Place, Suite 330,
//  Boston, MA  02111-1307
//  USA
//
// Contact information:
//  Kirk Korver
//  CyberKinetics, Inc.,
//  391 G Chipeta Way
//  Salt Lake City,  UT  84108
//  USA
//  kkorver@cyberkineticsinc.com
//
// Website:
//  www.neuroshare.org
//
// All other copyrights on this material are replaced by this license agreeement.
//
///////////////////////////////////////////////////////////////////////////////////////////////////
// $Workfile: aviUtil.h $
//
// $Date: 2005/03/16 16:54:31 $
//
// $History: aviUtil.h $
// 
// *****************  Version 2  *****************
// User: Abranner     Date: 6/11/04    Time: 1:16p
// Updated in $/Neuroshare/PowerNAP
// Class to save AVIs (written by Rod VanAmburgh and published on
// www.codeproject.com). Fixed problem with resetting a variable after
// saving an AVI.
// 
// 
// $NoKeywords: $
//
//////////////////////////////////////////////////////////////////////

#ifndef AVIUTIL_H_INCLUDED
#define AVIUTIL_H_INCLUDED

#include "stdafx.h"

#define AVIIF_KEYFRAME  0x00000010L // this frame is a key frame.
#define PW_WINDOW        1 
#define PW_CLIENT        2 

#include <memory.h>
#include <mmsystem.h>
#include <vfw.h>


BOOL AVI_Init();

BOOL AVI_FileOpenWrite(PAVIFILE * pfile, char *filename);

DWORD getFOURCC(CString value);

// Fill in the header for the video stream....
// The video stream will run in rate ths of a second....
BOOL AVI_CreateStream(PAVIFILE pfile, PAVISTREAM * ps, int rate, // sample/second
                      unsigned long buffersize, int rectwidth, int rectheight,
                      CString _compressor);

CString getFOURCCVAsString(DWORD value);

CString dumpAVICOMPRESSOPTIONS(AVICOMPRESSOPTIONS opts);

BOOL AVI_SetOptions(PAVISTREAM * ps, PAVISTREAM * psCompressed, LPBITMAPINFOHEADER lpbi,
                    CString _compressor);

BOOL AVI_SetText(PAVIFILE pfile, PAVISTREAM psText, char *szText, int width, int height, int TextHeight);

BOOL AVI_AddFrame(PAVISTREAM psCompressed, int time, LPBITMAPINFOHEADER lpbi);

BOOL AVI_AddText(PAVISTREAM psText, int time, char *szText);

BOOL AVI_CloseStream(PAVISTREAM ps, PAVISTREAM psCompressed, PAVISTREAM psText);

BOOL AVI_CloseFile(PAVIFILE pfile);

BOOL AVI_Exit();

HANDLE  MakeDib( HBITMAP hbitmap, UINT bits );

HBITMAP CopyScreenToBitmap(LPRECT lpRect);

// Author & Date:   Almut Branner   27 April 2004
// Purpose: Copy the window to a bitmap. This is part of the Microsoft Source Code Examples.
HBITMAP CopyWindowToBitmap(CWnd* wnd , HWND hWnd, WORD fPrintArea);

// Initialization... 
bool START_AVI(CString file_name);

//Now we can add frames
// ie. ADD_FRAME_FROM_DIB_TO_AVI(yourDIB, "CVID", 25);
bool ADD_FRAME_FROM_DIB_TO_AVI(HANDLE dib, CString _compressor, int _frameRate);

// The end... 
bool STOP_AVI();

#endif // include guard
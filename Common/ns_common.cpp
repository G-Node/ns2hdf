///////////////////////////////////////////////////////////////////////////////////////////////////
//
// Copyright (C) 2003-2005 Neuroshare Project                                                         
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
// $Workfile: ns_common.cpp $
//
// Authors       : Kirk Korver
//
// $Date: 2005/03/16 16:54:31 $
//
// $History: ns_common.cpp $
// 
// *****************  Version 2  *****************
// User: Kkorver      Date: 5/19/04    Time: 1:39p
// Updated in $/Neuroshare/PowerNAP
// 
// *****************  Version 1  *****************
// User: Kkorver      Date: 10/21/03   Time: 4:30p
// Created in $/Neuroshare/PowerNAP
// Added ReplaceWindowControl()
// 
// $NoKeywords: $
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ns_common.h"


// Purpose: Replace a static control with this control
// Inputs:
//  rParent - the window containing this control
//  rcNewWindow - the window that is to be created
//  nControlID - the ID of the control (from the resource editor)
void ReplaceWindowControl(CWnd & rParent, CWnd & rcNewWindow, int nControlID)
{
    CWnd * pStatic = rParent.GetDlgItem(nControlID);

    CRect rctWindowSize;

    DWORD frmstyle   = pStatic->GetStyle();
    DWORD frmexstyle = pStatic->GetExStyle();

    pStatic->GetWindowRect(rctWindowSize);              // Get window coord.
    rParent.ScreenToClient(rctWindowSize);              // change to client coord
    pStatic->DestroyWindow();

    rcNewWindow.CreateEx(frmexstyle, NULL, NULL, frmstyle, rctWindowSize, &rParent, nControlID);
}

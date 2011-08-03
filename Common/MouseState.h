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
// $Workfile: MouseState.h $
//
// Authors       : Kirk Korver
//
// $Date: 2005/03/16 16:54:31 $
//
// $History: MouseState.h $
// 
// *****************  Version 2  *****************
// User: Kkorver      Date: 5/19/04    Time: 1:39p
// Updated in $/Neuroshare/PowerNAP
// 
// *****************  Version 1  *****************
// User: Kkorver      Date: 10/20/03   Time: 10:16a
// Created in $/Neuroshare/nsClassifier
// Class to allow different "mouse" states (base class)
// 
// $NoKeywords: $
//
//////////////////////////////////////////////////////////////////////


#ifndef MOUSESTATE_H_INCLUDED
#define MOUSESTATE_H_INCLUDED

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

template <typename T>
class MouseState
{
public:
    MouseState() : m_pParent(NULL), m_bDragging(false) {}

    virtual void OnLButtonDown(UINT nFlags, CPoint point) { m_bDragging = true; }
    virtual void OnLButtonUp(UINT nFlags, CPoint point) { m_bDragging = false; }
    virtual void OnMouseMove(UINT nFlags, CPoint point) { if (m_bDragging) OnDragging(nFlags, point); }
    virtual void OnDragging(UINT nFlags, CPoint point) {}

    void SetParent(T * pParent) { m_pParent = pParent; }

protected:
    T * m_pParent;
    bool m_bDragging;
};


#endif //Include Guards
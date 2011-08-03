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

#ifndef SEGMENTSRAM_H_INCLUDED
#define SEGMENTSRAM_H_INCLUDED

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Datas.h"

class SegmentsRam : public Segments
{
public:
    SegmentsRam(DWORD dwEntityID, CString strLabel, ns_SEGMENTINFO segInfo, ns_SEGSOURCEINFO segSrc);

    virtual void GetInfo(ns_SEGMENTINFO & rInfo) const;         // Get the "segment" info
    virtual void GetInfo(VECTORSEGSOURCEINFO & rvInfo) const;   // Get info about the "sources" of each segment

	virtual DWORD GetEntityID() const;      // Get the Entity ID
    virtual CString GetLabel() const;
	virtual uint32 GetIndexByTime(double dTime, GIBT nFlag) const;

    virtual ns_RESULT GetIndexByTime(double dTime, GIBT nFlag, uint32 & dwIndex) const;    // most control

private:
	// This templage function was mooched from Datas!
    template <class T>
    ns_RESULT GetIndexByTime(T & rVector, double dTime, GIBT nFlag, uint32 & nIndex) const; 

	DWORD m_dwEntityID;
	CString m_strLabel;
	ns_SEGMENTINFO m_segInfo;
	ns_SEGSOURCEINFO m_segSrc;
};

#endif // include guards

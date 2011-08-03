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

#include "stdafx.h"
#include "SegmentsRam.h"

SegmentsRam::SegmentsRam(DWORD dwEntityID, CString strLabel, ns_SEGMENTINFO segInfo, ns_SEGSOURCEINFO segSrc)
{
	m_dwEntityID = dwEntityID;
	m_strLabel = strLabel;
	memcpy(&m_segInfo, &segInfo, sizeof(ns_SEGMENTINFO));
	memcpy(&m_segSrc, &segSrc, sizeof(ns_SEGSOURCEINFO));
}

void SegmentsRam::GetInfo(ns_SEGMENTINFO & rInfo) const
{
	memcpy(&rInfo, &m_segInfo, sizeof(ns_SEGMENTINFO));
}

void SegmentsRam::GetInfo(VECTORSEGSOURCEINFO & rvInfo) const
{
	for(UINT i = 0; i < m_segInfo.dwSourceCount; ++i)
		rvInfo.push_back(m_segSrc);
}

DWORD SegmentsRam::GetEntityID() const
{
	return m_dwEntityID;
}

CString SegmentsRam::GetLabel() const
{
	return m_strLabel;
}

uint32 SegmentsRam::GetIndexByTime(double dTime, GIBT nFlag) const
{
	uint32 dwIndex = 0;

    // TODO: This needs to changed to search what is loaded. In the case
    // where we have "deleted" some "time" from the file and not saved, then
    // this searching will return indices from the file, and not what is loaded
    ns_RESULT ret = GetIndexByTime(dTime, nFlag, dwIndex);
    if (ret != ns_OK)
    {
        // Ok, so I'm likely either before the first, or after the last. So try the closest
        ret = GetIndexByTime(dTime, GIBT_CLOSEST, dwIndex);
        ASSERT(ret == ns_OK);
    }

    return dwIndex;
}

ns_RESULT SegmentsRam::GetIndexByTime(double dTime, GIBT nFlag, uint32 & dwIndex) const
{
	return GetIndexByTime(m_vSegments, dTime, nFlag, dwIndex);
}

// Author & Date:   Kirk Korver     30 Jan 2004
// Purpose: predicate to compare a catalog entry with a time.
struct TimeLess
{
    // return TRUE if less; FALSE, otherwise
    template <class T>
    bool operator()(T & lhs, double dTime)
    {
        return lhs->GetTime() < dTime;
    }
};


// Author & Date:   Kirk Korver     29 Dec 2004
// Purpose: do all of the searching by time for the entities we are looking for
template <class T>
ns_RESULT SegmentsRam::GetIndexByTime(T & rVector, double dTime, GIBT nFlag, uint32 & nIndex) const
{
    T::const_iterator it = 
        std::lower_bound(rVector.begin(), rVector.end(), dTime, TimeLess());

    // What happens if I've run off the end?
    if (it == rVector.end())
    {
        switch (nFlag)
        {
            case GIBT_BEFORE:     // less than or equal to specified time
            case GIBT_CLOSEST:    // closest time 
                nIndex = uint32(it - rVector.begin() - 1);    // The "last" entry is before the end and is closest
                return ns_OK;

            case GIBT_AFTER:      // greater than or equal to specified time
                return ns_BADINDEX;
        }
    }

    // Was I lucky?...exact match
    if ((*it)->GetTime() == dTime)
    {
        nIndex = uint32(it - rVector.begin());       // can't be closer than exact match
        return ns_OK;
    }
    
    
    switch (nFlag)
    {
    case GIBT_BEFORE:     // less than or equal to specified time
        if (it == rVector.begin())        // can't be before the beginning
        {
            return ns_BADINDEX;
        }
        else
        {
            // Ok, so here is "after"
            nIndex = uint32(it - rVector.begin() - 1);
            return ns_OK;
        }

    case GIBT_CLOSEST:    // closest time 
        if (it == rVector.begin())
        {
            nIndex = uint32(it - rVector.begin());
            return ns_OK;
        }
        else
        {
            double dDistToCurrent = (*it)->GetTime() - dTime;
            --it;
            double dDistToPrev = dTime - (*it)->GetTime();

            nIndex = uint32((dDistToCurrent < dDistToPrev) ? it - rVector.begin() + 1 : it - rVector.begin() );
            return ns_OK;
        }

    case GIBT_AFTER:      // greater than or equal to specified time
        nIndex = uint32(it - rVector.begin());
        return ns_OK;
    }

    return ns_LIBERROR;
}
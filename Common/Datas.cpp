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
//  Cyberkinetics, Inc.,
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
//
// $Author: kirkkorver $
// $Date: 2005/05/18 19:17:50 $
// $Revision: 1.13 $
// $Source: /cvsroot/neuroshare/Suite/Common/Datas.cpp,v $
//
///////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ns_common.h"
#include <vector>
#include <algorithm>
#include <boost/bind.hpp>

#include "Datas.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


CString Datas::GetLabel() const
{
    //return CString(Data::m_pDoc->GetEntity(m_dwEntityID).isEntityInfo.szEntityLabel);
	ns_ENTITYINFO sInfo;
	m_isFile.GetEntityInfo(m_dwEntityID, sInfo);
    return CString(sInfo.szEntityLabel);
}


// Author & Date:   Kirk Korver     01 Dec 2004
// Purpose: find the closest index to this time point
// Inputs:
//  dTime - the time we are looking for
// Outputs:
//  the Index corresponding with this time point
uint32 Datas::GetIndexByTime(double dTime) const
{
    uint32 nIndex;

    // TODO: This needs to changed to search what is loaded. In the case
    // where we have "deleted" some "time" from the file and not saved, then
    // this searching will return indices from the file, and not what is loaded
    ns_RESULT ret = GetIndexByTime(dTime, GIBT_CLOSEST, nIndex);
    ASSERT(ret == ns_OK);

    return nIndex;
}


// Author & Date:   Kirk Korver     01 Dec 2004
// Purpose: find an index. First try to find one using the "flag" logic,
//  if this were to fail, then use the "closest" logic
// Inputs:
//  dTime - the time we are looking for
//  nFlag - one of : ns_BEFORE, ns_AFTER or ns_CLOSEST
// Outputs:
//  the Index corresponding with this time point
uint32 Datas::GetIndexByTime(double dTime, GIBT nFlag) const
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
ns_RESULT Datas::GetIndexByTime(T & rVector, double dTime, GIBT nFlag, uint32 & nIndex) const
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


//////////////////////////////////////////////////////////////////////////////



// Author & Date:   Kirk Korver     Oct 17, 2003
// Purpose: Function to retrieve the ns_SEGMENTINFO structure
// Inputs:  Reference to the structure receiving the ns_SEGMENTINFO
void SegmentsFile::GetInfo(ns_SEGMENTINFO & rInfo) const
{
    //ASSERT(Data::m_pDoc->m_icFileVector.empty() == false);

    // TODO: there is no guarantee that this data file is the right one. The
    //  "file" needs to be stored with each "entity"
    NsFile const & rFile = GetFile(); //Data::m_pDoc->m_icFileVector[0]->icFile;

	rFile.GetSegmentInfo(Datas::GetEntityID(), rInfo);
}


// Author & Date:   Kirk Korver     29 Dec 2004
// Purpose: find the index most associated with this time. See
// Inputs:
//  dTime - the time we are looking for
//  nFlag - tells us if we want before, after or closest
// Outputs:
//  dwIndex - the offset in the list of entites that matches this time
//  returns ns_OK if life is good
ns_RESULT SegmentsFile::GetIndexByTime(double dTime, GIBT nFlag, uint32 & dwIndex) const
{
    return Datas::GetIndexByTime(m_vSegments, dTime, nFlag, dwIndex);
}


// Author & Date:   Kirk Korver     Oct 17, 2003
// Purpose: Function to retrieve a vector of ns_SEGSOURCEINFO structures
// Inputs:  Reference to the vector receiving the ns_SEGSOURCEINFO structures
void SegmentsFile::GetInfo(VECTORSEGSOURCEINFO & rvInfo) const
{
    //ASSERT(Data::m_pDoc->m_icFileVector.empty() == false);

    // TODO: there is no guarantee that this data file is the right one. The
    //  "file" needs to be stored with each "entity"
    ns_SEGMENTINFO isSegInfo;
    NsFile rFile = GetFile(); //Data::m_pDoc->m_icFileVector[0]->icFile;

	rFile.GetSegmentInfo(Datas::GetEntityID(), isSegInfo);
    
    for (UINT nSource = 0; nSource < isSegInfo.dwSourceCount; ++nSource)
    {
        ns_SEGSOURCEINFO isSegSourceInfo;

        rFile.GetSegmentSourceInfo(Datas::GetEntityID(), nSource, isSegSourceInfo);

        rvInfo.push_back(isSegSourceInfo);
    } 
}

DWORD SegmentsFile::GetEntityID() const
{
	return Datas::GetEntityID();
}

CString SegmentsFile::GetLabel() const
{
	return Datas::GetLabel();
}

uint32 SegmentsFile::GetIndexByTime(double dTime, GIBT nFlag) const
{
	return Datas::GetIndexByTime(dTime, nFlag);
}

//////////////////////////////////////////////////////////////////////////////



struct SameUnit : std::binary_function <UnitMasks, SPDataSegment, bool>
{
    bool operator()(UnitMasks enUnit, const SPDataSegment & rSeg) const
    {
        return rSeg->GetUnitID() == DWORD(enUnit);
    }
};

// Author & Date:   Kirk Korver     10 Mar 2004
// Purpose: delete every segment of this unit
void Segments::DeleteTheseUnits(UnitMasks enUnit)
{
    // Remove if   Unit == enUnit
    //
    // the bind<bool> is needed to call the operator that returns bool
    m_vSegments.erase(std::remove_if(m_vSegments.begin(), m_vSegments.end(),
        boost::bind<bool>(SameUnit(), enUnit, _1)), m_vSegments.end());
}

struct DataSegTimeLess
{
    bool operator()(const SPDataSegment & lhs, double dTime)  // needed for lower_bound
    {
        return lhs->GetTime() < dTime;
    }

    bool operator()(double dTime, const SPDataSegment & rhs)  // needed for upper_bound
    {
        return dTime < rhs->GetTime();
    }
};

// Author & Date:   Kirk Korver     27 Apr 2004
// Purpose: delete every segment between these two times
void Segments::DeleteTimeSpan(double dTimeStart, double dTimeStop)
{
    ASSERT(dTimeStart <= dTimeStop);

    SEGMENTLIST::iterator itBegin;
    SEGMENTLIST::iterator itEnd;

    itBegin = std::upper_bound(m_vSegments.begin(), m_vSegments.end(), 
                               dTimeStart, DataSegTimeLess());

    itEnd   = std::lower_bound(m_vSegments.begin(), m_vSegments.end(), 
                               dTimeStop, DataSegTimeLess());

    m_vSegments.erase(itBegin, itEnd);
}

// Author & Date:   Kirk Korver     10 Mar 2004
// Purpose: remove "holes" in the unit classifications
// Outputs:
//  TRUE means data changed; FALSE, no changes
bool Segments::CompactUnits()
{
    struct Mapping
    {
        UnitMasks enOrigMask;
        UnitMasks enNewMask;
        DWORD nCount;
    } asMap[] = 
    { // original      new (ignore)  count          // It is important
        {UNIT_1_MASK,  UNIT_1_MASK,  0},            // that these are "in order"
        {UNIT_2_MASK,  UNIT_2_MASK,  0},
        {UNIT_3_MASK,  UNIT_3_MASK,  0},
        {UNIT_4_MASK,  UNIT_4_MASK,  0},
        {UNIT_5_MASK,  UNIT_5_MASK,  0},
        {UNIT_6_MASK,  UNIT_6_MASK,  0},
        {UNIT_7_MASK,  UNIT_7_MASK,  0},
        {UNIT_8_MASK,  UNIT_8_MASK,  0},
        {UNIT_9_MASK,  UNIT_9_MASK,  0},
        {UNIT_10_MASK, UNIT_10_MASK, 0},
    };

    SEGMENTLIST::iterator it;
    int i;
    // First get the counts for each of the units
    for (it = m_vSegments.begin(); it != m_vSegments.end(); ++it)
    {
        for (i = 0; i < ARRAY_SIZE(asMap); ++i)
        {
            if (DWORD(asMap[i].enOrigMask) == (*it)->GetUnitID())
            {
                ++asMap[i].nCount;
                break;
            }
        }
    }


    // Test to see if we have a hole, does the "compacting" as well

                // numbers   zero, if a number follows, then we have a "hole"
    enum CuState { CU_NUMBER, CU_ZERO, CU_HOLE};
    CuState enCuState = CU_NUMBER;
    for (i = 0; i < ARRAY_SIZE(asMap); ++i)
    {
        if (enCuState == CU_NUMBER)
        {
            if (asMap[i].nCount == 0)
                enCuState = CU_ZERO;
        }
        else
        {
            if (asMap[i].nCount != 0)
            {
                enCuState = CU_HOLE;
                break;
            }
        }
    }

    if (enCuState != CU_HOLE)       // no holes, no changes
        return false;


    // Now, "compact" these counts
    int nIdxLastUnit = 0;
    for (i = 0; i < ARRAY_SIZE(asMap); ++i)
    {
        if (asMap[i].nCount != 0)
        {
            asMap[i].enNewMask = asMap[nIdxLastUnit++].enOrigMask;
        }
    }

    // Now go through and make these changes happen
    for (it = m_vSegments.begin(); it != m_vSegments.end(); ++it)
    {
        for (i = 0; i < ARRAY_SIZE(asMap); ++i)
        {
            if (DWORD(asMap[i].enOrigMask) == (*it)->GetUnitID())
            {
                (*it)->SetUnitID(asMap[i].enNewMask);
                break;
            }
        }
    }
    return true;
}

// Author & Date:   Kirk Korver     10 Mar 2004
// Purpose: reclassify every "from" unit as the "to" unit
//  enFrom - the unit we are to change
//  enTo - what this unit should be classified as
void Segments::ReclassifyUnits(UnitMasks enFrom, UnitMasks enTo)
{
    for (SEGMENTLIST::iterator it = m_vSegments.begin(); it != m_vSegments.end(); ++it)
    {
        if ((*it)->GetUnitID() == DWORD(enFrom))
        {
            (*it)->SetUnitID(enTo);
        }
    }
}

void Segments::push_back(DataSegment *seg) 
{ 
	SPDataSegment DataSegList(seg);
	m_vSegments.push_back(DataSegList); 
}

//////////////////////////////////////////////////////////////////////////////

// Author & Date:   Kirk Korver     20 May 2004
// Purpose: Get the info about this event
void Events::GetInfo(ns_EVENTINFO & rInfo) const
{
    //ASSERT(Data::m_pDoc->m_icFileVector.empty() == false);

    // TODO: there is no guarantee that this data file is the right one. The
    //  "file" needs to be stored with each "entity"
    NsFile rFile = GetFile(); //Data::m_pDoc->m_icFileVector[0]->icFile;

    rFile.GetEventInfo(GetEntityID(), rInfo);
}

#pragma message("This is bad programming....GetLabel() isn't virtual")
// Author & Date: Kirk Korver       17 May 2005
// Purpose: get the label of this event
CString Events::GetLabel() const
{
    return m_strLabel;
}


// Author & Date:   Kirk Korver     29 Dec 2004
// Purpose: find the index most associated with this time. See
// Inputs:
//  dTime - the time we are looking for
//  nFlag - tells us if we want before, after or closest
// Outputs:
//  dwIndex - the offset in the list of entites that matches this time
//  returns ns_OK if life is good
ns_RESULT Events::GetIndexByTime(double dTime, GIBT nFlag, uint32 & dwIndex) const
{
    return Datas::GetIndexByTime(m_vEvents, dTime, nFlag, dwIndex);
}

void Events::push_back(DataEvent *event)
{
	SPDataEvent DataEveList(event);
	m_vEvents.push_back(DataEveList); 
}

//////////////////////////////////////////////////////////////////////////////




// Author & Date:   Kirk Korver     Oct 17, 2003
// Purpose: get all of the "info" about this entity
void Analogs::GetInfo(ns_ANALOGINFO & rInfo) const
{
    //ASSERT(Data::m_pDoc->m_icFileVector.empty() == false);

    // TODO: there is no guarantee that this data file is the right one. The
    //  "file" needs to be stored with each "entity"
    NsFile rFile = GetFile(); //Data::m_pDoc->m_icFileVector[0]->icFile;

    rFile.GetAnalogInfo(GetEntityID(), rInfo);
}


// Author & Date:   Kirk Korver     Oct 17, 2003
// Purpose: Function to retrieve the sampling rate of this waveform
// Output:  The sampling rate in Hz.
double Analogs::GetSampleRate() const
{
    ns_ANALOGINFO isInfo;
    GetInfo(isInfo);
    return isInfo.dSampleRate;
}

// Author & Date:   Kirk Korver     29 Dec 2004
// Purpose: find the index most associated with this time. See
// Inputs:
//  dTime - the time we are looking for
//  nFlag - tells us if we want before, after or closest
// Outputs:
//  dwIndex - the offset in the list of entites that matches this time
//  returns ns_OK if life is good
ns_RESULT Analogs::GetIndexByTime(double dTime, GIBT nFlag, uint32 & dwIndex) const
{
    NsFile rFile = GetFile();

    // TODO: This needs to changed to search what is loaded. In the case
    // where we have "deleted" some "time" from the file and not saved, then
    // this searching will return indices from the file, and not what is loaded
    ns_RESULT ret = rFile.GetIndexByTime(GetEntityID(), dTime, nFlag, &dwIndex);

    DEBUG_CODE
    (
        TRACE("GetIndexByTime failed\n");
    )

    return ret;
}

void Analogs::push_back(DataAnalog *seg) 
{ 
	SPDataAnalog DataAnaList(seg);
	m_vAnalogs.push_back(DataAnaList); 
}

//////////////////////////////////////////////////////////////////////////////


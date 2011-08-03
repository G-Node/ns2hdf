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
//#include "PowerNAP.h"
#include "DataFileSegment.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// Inputs:
//  dwIdxFile - the index of the file in the document's file vector
//  dwIdxEntity - in the correct type, this is the "offset" into that types entity vector
//  dwIndex - the index (or 1st index if a collection) that corresponds to this data
DataFileSegment::DataFileSegment(FileInfo * pcFile, Segments * pcSegments, DWORD dwIndex) : 
    Data(pcFile, dwIndex),
    m_pcSegments(pcSegments)
{
    GetSegmentData(NULL, NULL, 0, NULL, (uint32 *)(&m_dwUnitID) );
}

DataFileSegment::~DataFileSegment()
{

}

// Author & Date:   Kirk Korver     25 Feb 2004
// Purpose: Get data about this segment
// Inputs:
//  cbDataBufferSize - the number of bytes for the "pdData" field
// Outputs:
//  pdTimeStamp - the time of this segment (can be NULL)
//  pdData - The actual data of the wave form (can be NULL)
//  pdwSampleCount - how many samples were actually returned (in # of doubles)
//  pdwUnitID - unit classification code
ns_RESULT DataFileSegment::GetSegmentData(double *pdTimeStamp, 
                                      double *pdData, uint32 cbDataBufferSize, uint32 *pdwSampleCount, 
                                      uint32 *pdwUnitID)
{
	
    return GetFile().GetSegmentData(GetEntityID(), m_dwIndex, 
            pdTimeStamp, pdData, cbDataBufferSize, pdwSampleCount, pdwUnitID);
}


inline DWORD DataFileSegment::GetEntityID() const
{
    return m_pcSegments->GetEntityID();
}


//////////////////////////////////////////////////////////////////////
// Functions
//////////////////////////////////////////////////////////////////////

// Author & Date:   Almut Branner   Sept 11, 2003
// Purpose: Function to retrieve the timestamp of this segment entity
// Outputs: The timestamp
double DataFileSegment::GetTime() const
{
    return GetTimeByIndex(GetEntityID(), m_dwIndex);
}


// Author & Date:   Almut Branner   Sept 11, 2003
// Purpose: Function to retrieve the entity's unit ID
// Outputs: The unit ID
DWORD DataFileSegment::GetUnitID() const
{
    return m_dwUnitID;
}


// Author & Date:   Almut Branner   Sept 11, 2003
// Purpose: Function to set the entity's unit ID
// Inputs:  The unit ID
void DataFileSegment::SetUnitID(DWORD dwUnitID)
{ 
    // Save the unit ID 
    m_dwUnitID = dwUnitID;
}


// Author & Date:   Almut Branner   Sept 11, 2003
// Purpose: Function to retrieve the segment waveform
// Inputs:  Reference to the vector receiving the waveform
void DataFileSegment::GetWave(WAVEFORM & rvWaveform) const
{ 
    DWORD dwEntityID = GetEntityID();
    double dTimeStamp;
    uint32 dwSampleCount;
    uint32 dwUnitID;

    ns_SEGMENTINFO isSegInfo;
    NsFile & rcFile = GetFile();
	if (ns_OK != rcFile.GetSegmentInfo(dwEntityID, isSegInfo))
        return;
    rvWaveform.resize(isSegInfo.dwMaxSampleCount);

    if (ns_OK == rcFile.GetSegmentData(dwEntityID, 
            m_dwIndex, &dTimeStamp, &rvWaveform[0], isSegInfo.dwMaxSampleCount * sizeof (double), 
            &dwSampleCount, &dwUnitID))
    {
        rvWaveform.resize(dwSampleCount);
    }
    else
    {
        rvWaveform.clear();
    }
    return;
}


// Author & Date:   Almut Branner   Sept 11, 2003
// Purpose: Function to retrieve the number of data points in a waveform
// Outputs: The number of data points in a waveform
DWORD DataFileSegment::GetNumOfPoints() const
{
    ns_SEGMENTINFO isSegInfo;
    DWORD dwNumPoints = 0;

    if (ns_OK == GetFile().GetSegmentInfo(GetEntityID(), isSegInfo))
        dwNumPoints = isSegInfo.dwMaxSampleCount;

    return dwNumPoints;
}



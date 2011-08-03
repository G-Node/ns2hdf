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
#include "DataRamSegment.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

DataRamSegment::DataRamSegment() 
{
	Clear();
}


DataRamSegment::DataRamSegment(const DataRamSegment &seg) 
{
	m_dwUnitID = seg.GetUnitID();
	m_dwEntityID = seg.GetEntityID();
	m_dTime = seg.GetTime();
	seg.GetWave(m_vWaveform);
}


DataRamSegment::~DataRamSegment()
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
ns_RESULT DataRamSegment::GetSegmentData(double *pdTimeStamp, 
                                      double *pdData, uint32 cbDataBufferSize, uint32 *pdwSampleCount, 
                                      uint32 *pdwUnitID)
{
	return ns_LIBERROR;
}

inline DWORD DataRamSegment::GetEntityID() const
{
    return m_dwEntityID;
}


//////////////////////////////////////////////////////////////////////
// Functions
//////////////////////////////////////////////////////////////////////

// Author & Date:   Almut Branner   Sept 11, 2003
// Purpose: Function to retrieve the timestamp of this segment entity
// Outputs: The timestamp
double DataRamSegment::GetTime() const
{
	return m_dTime;
}


// Author & Date:   Almut Branner   Sept 11, 2003
// Purpose: Function to retrieve the entity's unit ID
// Outputs: The unit ID
DWORD DataRamSegment::GetUnitID() const
{
    return m_dwUnitID;
}


// Author & Date:   Almut Branner   Sept 11, 2003
// Purpose: Function to set the entity's unit ID
// Inputs:  The unit ID
void DataRamSegment::SetUnitID(DWORD dwUnitID)
{ 
    // Save the unit ID 
    m_dwUnitID = dwUnitID;
}


// Author & Date:   Almut Branner   Sept 11, 2003
// Purpose: Function to retrieve the segment waveform
// Inputs:  Reference to the vector receiving the waveform
void DataRamSegment::GetWave(WAVEFORM & rvWaveform) const
{ 
	rvWaveform.clear();
	rvWaveform.insert(rvWaveform.begin(), m_vWaveform.begin(), m_vWaveform.end());
}

void DataRamSegment::AddWave(WAVEFORM::iterator begin, WAVEFORM::iterator end)
{ 
	m_vWaveform.insert(m_vWaveform.end(), begin, end);
}

// Author & Date:   Almut Branner   Sept 11, 2003
// Purpose: Function to retrieve the number of data points in a waveform
// Outputs: The number of data points in a waveform
DWORD DataRamSegment::GetNumOfPoints() const
{
	return (DWORD) m_vWaveform.size();
}

void DataRamSegment::SetTime(double dTime)
{
	m_dTime = dTime;
}

void DataRamSegment::SetEntityID(DWORD dwEntityID)
{
	m_dwEntityID = dwEntityID;
}

void DataRamSegment::Clear()
{
	m_dwUnitID = 0;
	m_dwEntityID = -1;
	m_dTime = -1.0;
	m_vWaveform.clear();
}

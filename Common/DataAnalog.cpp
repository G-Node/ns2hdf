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
//
// $Workfile: DataAnalog.cpp $
//
// Description   : 
//
// Authors       : Almut Branner
//
// $Date: 2005/03/16 16:54:30 $
//
// $History: DataAnalog.cpp $
// 
// *****************  Version 17  *****************
// User: Kkorver      Date: 5/19/04    Time: 1:39p
// Updated in $/Neuroshare/PowerNAP
// 
// *****************  Version 16  *****************
// User: Kkorver      Date: 4/23/04    Time: 1:16p
// Updated in $/Neuroshare/PowerNAP
// Fixed problem with using   v.begin()  instead of &v[0]
// 
// *****************  Version 15  *****************
// User: Abranner     Date: 4/22/04    Time: 4:38p
// Updated in $/Neuroshare/PowerNAP
// Changes to move to Visual C++ .NET.
// 
// *****************  Version 14  *****************
// User: Kkorver      Date: 3/04/04    Time: 5:29p
// Updated in $/Neuroshare/PowerNAP
// Converted to use SharedPointers instead of Indexes
// 
// *****************  Version 13  *****************
// User: Kkorver      Date: 3/04/04    Time: 9:38a
// Updated in $/Neuroshare/PowerNAP
// Use new GetEntity()
// 
// *****************  Version 12  *****************
// User: Kkorver      Date: 2/27/04    Time: 2:42p
// Updated in $/Neuroshare/PowerNAP
// Added GetTimeByIndex(), GetTimeFirst() and GetTimeLast()
// 
// *****************  Version 11  *****************
// User: Kkorver      Date: 10/21/03   Time: 2:43p
// Updated in $/Neuroshare/PowerNAP
// Renamed program to PowerNAP
// 
// *****************  Version 10  *****************
// User: Kkorver      Date: 10/17/03   Time: 11:51a
// Updated in $/Neuroshare/nsClassifier
// updated the AnalogData to match the new Data format
// 
// *****************  Version 9  *****************
// User: Kkorver      Date: 10/17/03   Time: 9:48a
// Updated in $/Neuroshare/nsClassifier
// Updated the "data" class to no longer store the Entity ID
// 
// *****************  Version 8  *****************
// User: Abranner     Date: 10/15/03   Time: 2:20p
// Updated in $/Neuroshare/nsClassifier
// Did some formatting.
// 
// *****************  Version 7  *****************
// User: Kkorver      Date: 10/14/03   Time: 10:42a
// Updated in $/Neuroshare/nsClassifier
// Updated data class to no longer hold "type"
// 
// *****************  Version 6  *****************
// User: Abranner     Date: 10/14/03   Time: 10:32a
// Updated in $/Neuroshare/nsClassifier
// Added function GetSampleRate().
// 
// *****************  Version 5  *****************
// User: Abranner     Date: 9/11/03    Time: 5:12p
// Updated in $/Neuroshare/nsClassifier
// Added test IsThisSegment. Analog data is loaded in.
// 
// *****************  Version 4  *****************
// User: Abranner     Date: 9/11/03    Time: 2:57p
// Updated in $/Neuroshare/nsClassifier
// Added function headers and made DataAnalog functions working.
// 
// *****************  Version 3  *****************
// User: Abranner     Date: 9/11/03    Time: 12:09p
// Updated in $/Neuroshare/nsClassifier
// Document now stores EntityID instead of EntityInList. This accomodates
// sorting of the list in FIU.
// 
// *****************  Version 2  *****************
// User: Abranner     Date: 9/09/03    Time: 3:03p
// Updated in $/Neuroshare/nsClassifier
// Changed the document to be able to incoorperate more entity types and
// loading more channels at a time. Waveforms are not stored in the
// document anymore. The WaveformList was turned into a SegmentList.
// 
// *****************  Version 1  *****************
// User: Abranner     Date: 9/03/03    Time: 11:07a
// Created in $/Neuroshare/nsClassifier
// 
// 
// $NoKeywords: $
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DataAnalog.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// Inputs:
//  pcFile - the file we should read from
//  pcAnalogs - the Analog entity we belong to
//  dwIndexStart - first index of this analog waveform 
//  dwIndexCount - number of valid indices for this analog waveform
DataAnalog::DataAnalog(FileInfo * pcFile, Analogs * pcAnalogs, DWORD dwIndexStart, DWORD dwIndexCount) :
    Data(pcFile, dwIndexStart),
    m_pcAnalogs(pcAnalogs),
    m_dwIndexCount(dwIndexCount)
{
}

DataAnalog::~DataAnalog()
{

}

//////////////////////////////////////////////////////////////////////
// Functions
//////////////////////////////////////////////////////////////////////

// Author & Date:   Almut Branner   Sept 11, 2003
// Purpose: Function to retrieve the analog entity's timestamp
// Outputs: The timestamp
double DataAnalog::GetTimeByIndex(DWORD dwIndex) const
{
    return Data::GetTimeByIndex(GetEntityID(), dwIndex);
}


// Author & Date:   Almut Branner   Sept 11, 2003
// Purpose: Function to retrieve the analog waveform 
// Inputs:  Reference to the vector receiving the waveform
void DataAnalog::GetWave(WAVEFORM & rvWaveform)
{
    DWORD dwEntityID = GetEntityID();
	uint32 dwContCount;
	NsFile & rFile = GetFile();
	ns_ENTITYINFO entInfo;

	rFile.GetEntityInfo(dwEntityID, entInfo);

    rvWaveform.clear();
    rvWaveform.resize(entInfo.dwItemCount);

    if (ns_OK != rFile.GetAnalogData(dwEntityID, 0, entInfo.dwItemCount, &dwContCount, &rvWaveform[0]))
    {
        AfxMessageBox("Error reading analog waveform", MB_ICONEXCLAMATION, 0);
    }
}

// Author & Date:   Kirk Korver     25 Feb 2004
// Purpose: Get the entity ID of this entity
inline DWORD DataAnalog::GetEntityID() const
{
    return m_pcAnalogs->GetEntityID();
}

// Author & Date:   Chris Silva     08 Dec 2004
// Purpose: Fills rcWaveform from dwStartIndex to dwIndexCount
ns_RESULT DataAnalog::GetAnalogData(uint32 dwStartIndex, uint32 dwIndexCount, uint32 *pdwContCount, WAVEFORM & rvWaveform)
{
    NsFile & rFile = GetFile();

    rvWaveform.clear();
    rvWaveform.resize(dwIndexCount);

    return rFile.GetAnalogData(GetEntityID(), dwStartIndex, dwIndexCount, pdwContCount, &rvWaveform[0]);
}
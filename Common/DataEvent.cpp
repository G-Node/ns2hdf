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
// $Workfile: DataEvent.cpp $
//
// $Date: 2005/03/16 16:54:30 $
//
// $History: DataEvent.cpp $
// 
// *****************  Version 1  *****************
// User: Kkorver      Date: 5/24/04    Time: 2:37p
// Created in $/Neuroshare/PowerNAP
// Anomaly #43
// Added the ability to save events
// 
// $NoKeywords: $
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DataEvent.h"

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
//  dwIndexStart - The index of this event
DataEvent::DataEvent(FileInfo * pcFile, Events * pcEvents, DWORD dwIndex) :
    Data(pcFile, dwIndex),
    m_pcEvents(pcEvents)
{
}

DataEvent::~DataEvent()
{
}

//////////////////////////////////////////////////////////////////////
// Functions
//////////////////////////////////////////////////////////////////////

// Author & Date:   Kirk Korver     25 Feb 2004
// Purpose: Get the entity ID of this entity
inline DWORD DataEvent::GetEntityID() const
{
    return m_pcEvents->GetEntityID();
}

// Author & Date:   Kirk Korver     19 May 2004
// Purpose: Function to retrieve the timestamp of this event entity
// Outputs: The timestamp
double DataEvent::GetTime() const
{
    return GetTimeByIndex(GetEntityID(), m_dwIndex);
}

// Author & Date:   Kirk Korver     24 May 2004
// Purpose: get the data associated with this event
// Inputs:
//  pdTimeStamp - what time did this event happen
//  pData - the data we got
//  cbDataSize - how many bytes are available in pData
//  pcbDataRetSize - how many bytes did we get?
// Outputs:
//  TRUE means life is good; FALSE, otherwise
bool DataEvent::GetData(double *pdTimeStamp, void *pData, uint32 cbDataSize, uint32 *pcbDataRetSize) const
{ 	
    ns_RESULT ret = GetFile().GetEventData(GetEntityID(), m_dwIndex, pdTimeStamp, pData, cbDataSize, pcbDataRetSize);
    return ret == ns_OK;
}


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
// $Workfile: DataEvent.h $
//
// $Date: 2005/03/16 16:54:31 $
//
// $History: DataEvent.h $
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

#ifndef DATAEVENT_H_INCLUDED
#define DATAEVENT_H_INCLUDED

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <boost/shared_ptr.hpp>
#include "Data.h"
#include "Datas.h"
#include "CommonTypes.h"

class Events;

typedef boost::shared_ptr<Events>   SPEvents;
typedef std::vector <SPEvents>      EVENTENTITYLIST;

// This stores the information for a single event.
class DataEvent : public Data  
{
public:
	DataEvent(FileInfo * pcFile, Events * pcEvents, DWORD dwIndex);
	virtual ~DataEvent();

    DWORD GetEntityID() const;                                  // Get the EntityID of this entity
    double GetTime() const;                                     // Retrieve the timestamp
    
    // most general purpose function to read data
    bool GetData(double *pdTimeStamp, void *pData, uint32 cbDataSize, uint32 *pcbDataRetSize) const; 
    
    // Convenient function to read out data of a known size
    template <typename _T>
    bool GetData(double *pdTimeStamp, _T * pData);

protected:
    Events * m_pcEvents;                                        // point to the analogs of which this is a member
    //TODO: add a 'type' here to say what kind of event this is
};



template <typename _T>
bool DataEvent::GetData(double *pdTimeStamp, _T * pData)
{
    const cbWant = sizeof(*pData);
    uint32 cbGot;

    if (GetData(pdTimeStamp, pData, cbWant, &cbGot) != ns_OK)
        return false;

    if (cbGot != cbWant)
        return false;

    return true;
}



#endif // include guard

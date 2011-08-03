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


#ifndef DATARAMSEGMENT_H_INCLUDED
#define DATARAMSEGMENT_H_INCLUDED

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DataSegment.h"

class DataRamSegment : public DataSegment  
{
public:
	DataRamSegment();
	DataRamSegment(const DataRamSegment &seg);
	virtual ~DataRamSegment();

    double GetTime() const;                                     // Retrieve the timestamp
    void GetWave(WAVEFORM & rvWaveform) const;                  // Retrieve the segment waveform
    DWORD GetNumOfPoints() const;                               // Retrieve the number of data points in a waveform

    DWORD GetUnitID() const;    // Retrieve the Unit ID
    void SetUnitID(DWORD dwUnitID);             // Set the Unit ID


    ns_RESULT GetSegmentData(double *pdTimeStamp, 
                             double *pdData, uint32 dwDataBufferSize, uint32 *pdwSampleCount, 
                             uint32 *pdwUnitID);

    DWORD GetEntityID() const;                                  // Get the EntityID of this entity

	void AddWave(WAVEFORM::iterator begin, WAVEFORM::iterator end);
	void SetTime(double dTime);
	void SetEntityID(DWORD dwEntityID);
	void Clear();

protected:
    DWORD m_dwUnitID;                   // Which "sorted unit" does this spike belong to
	WAVEFORM m_vWaveform;
	DWORD m_dwEntityID;
	double m_dTime;
};

#endif // include guard

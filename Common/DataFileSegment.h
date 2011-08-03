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


#ifndef DATAFILESEGMENT_H_INCLUDED
#define DATAFILESEGMENT_H_INCLUDED

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <vector>
#include <boost/shared_ptr.hpp>
#include "Data.h"
#include "Datas.h"
#include "DataSegment.h"
#include "CommonTypes.h"

class Segments;

class DataFileSegment : public DataSegment, Data 
{
public:
	DataFileSegment(FileInfo * pcFile, Segments * pcSegments, DWORD dwIndex);
	virtual ~DataFileSegment();

    double GetTime() const;                                     // Retrieve the timestamp
    void GetWave(WAVEFORM & rvWaveform) const;                  // Retrieve the segment waveform
    DWORD GetNumOfPoints() const;                               // Retrieve the number of data points in a waveform

    DWORD GetUnitID() const;    // Retrieve the Unit ID
    void SetUnitID(DWORD dwUnitID);             // Set the Unit ID


    ns_RESULT GetSegmentData(double *pdTimeStamp, 
                             double *pdData, uint32 dwDataBufferSize, uint32 *pdwSampleCount, 
                             uint32 *pdwUnitID);

    DWORD GetEntityID() const;                                  // Get the EntityID of this entity

protected:
    Segments * m_pcSegments;            // points to the segments of which this is a member
    DWORD m_dwUnitID;                   // Which "sorted unit" does this spike belong to
};

#endif // include guard

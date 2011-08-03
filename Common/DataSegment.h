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
// $Workfile: DataSegment.h $
//
// Description   : 
//
// Authors       : Almut Branner
//
// $Date: 2005/05/11 23:36:24 $
//
// $History: DataSegment.h $
// 
// *****************  Version 14  *****************
// User: Kkorver      Date: 5/19/04    Time: 1:39p
// Updated in $/Neuroshare/PowerNAP
// 
// *****************  Version 13  *****************
// User: Kkorver      Date: 3/04/04    Time: 5:28p
// Updated in $/Neuroshare/PowerNAP
// Converted to use SharedPointers instead of Indexes
// 
// *****************  Version 12  *****************
// User: Kkorver      Date: 2/27/04    Time: 1:56p
// Updated in $/Neuroshare/PowerNAP
// The means to get the EntityID is now stored locally
// Added GetSegmentData()
// Added GetEntityID()
// 
// *****************  Version 11  *****************
// User: Kkorver      Date: 10/17/03   Time: 11:53a
// Updated in $/Neuroshare/nsClassifier
// Moved All of the GetInfo()'s to  Class Segments
// 
// *****************  Version 10  *****************
// User: Kkorver      Date: 10/17/03   Time: 9:48a
// Updated in $/Neuroshare/nsClassifier
// Updated the "data" class to no longer store the Entity ID
// 
// *****************  Version 9  *****************
// User: Kkorver      Date: 10/16/03   Time: 1:01p
// Updated in $/Neuroshare/nsClassifier
// Added "const" to functions that don't modify variables
// 
// *****************  Version 8  *****************
// User: Abranner     Date: 10/15/03   Time: 2:21p
// Updated in $/Neuroshare/nsClassifier
// Changed constructor to either accept dwUnitID or read it from the file.
// 
// *****************  Version 7  *****************
// User: Kkorver      Date: 10/14/03   Time: 10:42a
// Updated in $/Neuroshare/nsClassifier
// Updated data class to no longer hold "type"
// 
// *****************  Version 6  *****************
// User: Abranner     Date: 9/11/03    Time: 2:57p
// Updated in $/Neuroshare/nsClassifier
// Added function headers and made DataAnalog functions working.
// 
// *****************  Version 5  *****************
// User: Abranner     Date: 9/11/03    Time: 12:09p
// Updated in $/Neuroshare/nsClassifier
// Document now stores EntityID instead of EntityInList. This accomodates
// sorting of the list in FIU.
// 
// *****************  Version 4  *****************
// User: Abranner     Date: 9/11/03    Time: 9:54a
// Updated in $/Neuroshare/nsClassifier
// Changed Get... and SetUnitID().
// 
// *****************  Version 3  *****************
// User: Kkorver      Date: 9/11/03    Time: 7:04a
// Updated in $/Neuroshare/nsClassifier
// Added GetInfo()
// 
// *****************  Version 2  *****************
// User: Abranner     Date: 9/09/03    Time: 3:03p
// Updated in $/Neuroshare/nsClassifier
// Changed the document to be able to incoorperate more entity types and
// loading more channels at a time. Waveforms are not stored in the
// document anymore. The WaveformList was turned into a SegmentList.
// 
// *****************  Version 1  *****************
// User: Abranner     Date: 9/03/03    Time: 11:16a
// Created in $/Neuroshare/nsClassifier
// 
// 
// $NoKeywords: $
//
//////////////////////////////////////////////////////////////////////

#ifndef DATASEGMENT_H_INCLUDED
#define DATASEGMENT_H_INCLUDED

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <vector>
#include <boost/shared_ptr.hpp>
#include "Data.h"
#include "Datas.h"
#include "CommonTypes.h"

class Segments;

typedef boost::shared_ptr<Segments> SPSegments;
typedef std::vector <SPSegments>    SEGMENTENTITYLIST;
typedef std::vector <ns_SEGSOURCEINFO> VECTORSEGSOURCEINFO;

class DataSegment 
{
public:
    virtual double GetTime() const = 0;                                     // Retrieve the timestamp
    virtual void GetWave(WAVEFORM & rvWaveform) const = 0;                  // Retrieve the segment waveform
    virtual DWORD GetNumOfPoints() const = 0;                               // Retrieve the number of data points in a waveform

    virtual DWORD GetUnitID() const = 0;    // Retrieve the Unit ID
    virtual void SetUnitID(DWORD dwUnitID) = 0;             // Set the Unit ID


    virtual ns_RESULT GetSegmentData(double *pdTimeStamp, 
									 double *pdData, uint32 dwDataBufferSize, uint32 *pdwSampleCount, 
									 uint32 *pdwUnitID) = 0;

    virtual DWORD GetEntityID() const = 0;                                  // Get the EntityID of this entity
};

#endif // include guard

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
// $Workfile: DataAnalog.h $
//
// Description   : 
//
// Authors       : Almut Branner
//
// $Date: 2005/03/16 16:54:30 $
//
// $History: DataAnalog.h $
// 
// *****************  Version 11  *****************
// User: Kkorver      Date: 5/19/04    Time: 1:39p
// Updated in $/Neuroshare/PowerNAP
// 
// *****************  Version 10  *****************
// User: Kkorver      Date: 3/04/04    Time: 5:28p
// Updated in $/Neuroshare/PowerNAP
// Converted to use SharedPointers instead of Indexes
// 
// *****************  Version 9  *****************
// User: Kkorver      Date: 2/27/04    Time: 2:15p
// Updated in $/Neuroshare/PowerNAP
// Added GetTimeByIndex(), GetTimeFirst() and GetTimeLast()
// 
// *****************  Version 8  *****************
// User: Kkorver      Date: 10/17/03   Time: 11:52a
// Updated in $/Neuroshare/nsClassifier
// updated the AnalogData to match the new Data format
// 
// *****************  Version 7  *****************
// User: Abranner     Date: 10/15/03   Time: 2:20p
// Updated in $/Neuroshare/nsClassifier
// Did some formatting.
// 
// *****************  Version 6  *****************
// User: Kkorver      Date: 10/14/03   Time: 10:42a
// Updated in $/Neuroshare/nsClassifier
// Updated data class to no longer hold "type"
// 
// *****************  Version 5  *****************
// User: Abranner     Date: 10/14/03   Time: 10:32a
// Updated in $/Neuroshare/nsClassifier
// Added function GetSampleRate().
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

#ifndef DATAANALOG_H_INCLUDED
#define DATAANALOG_H_INCLUDED

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <vector>
#include <boost/shared_ptr.hpp>
#include "Data.h"
#include "Datas.h"
#include "CommonTypes.h"

class Analogs;

typedef boost::shared_ptr<Analogs>  SPAnalogs;
typedef std::vector <SPAnalogs>     ANALOGENTITYLIST;

// This stores the information for an analog waveform. A single data analog
// actually comprises
class DataAnalog : public Data  
{
public:
	DataAnalog(FileInfo * pcFile, Analogs * pcAnalogs, DWORD dwIndexStart, DWORD dwIndexCount);
	virtual ~DataAnalog();

    double GetTimeByIndex(DWORD dwIndex) const;             // Retrieve the timestamp
    double GetTimeFirst() const                             // ^^^ of the 1st entry
        { return GetTimeByIndex(m_dwIndex); }
    double GetTimeLast() const                              // ^^^ of the "last" entry
        { return GetTimeByIndex(m_dwIndex + m_dwIndexCount - 1); }

                                                            // Might take this out because it is usually zero.
	void GetWave(WAVEFORM & rvWaveform);                    // Retrieve the analog waveform

    DWORD GetEntityID() const;                              // Get the EntityID of this entity

    ns_RESULT GetAnalogData(uint32 dwStartIndex, uint32 dwIndexCount, uint32 *pdwContCount, WAVEFORM & rvWaveform);

protected:
    Analogs * m_pcAnalogs;                                  // point to the analogs of which this is a member
    DWORD m_dwIndexCount;                                   // This is the number of valid indices in this analog entity
};

#endif // include guard

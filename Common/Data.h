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
// $Workfile: Data.h $
//
// Description   : 
//
// Authors       : Almut Branner
//
// $Date: 2005/05/17 19:54:37 $
//
// $History: Data.h $
// 
// *****************  Version 13  *****************
// User: Kkorver      Date: 5/19/04    Time: 1:38p
// Updated in $/Neuroshare/PowerNAP
// 
// *****************  Version 12  *****************
// User: Kkorver      Date: 3/04/04    Time: 5:29p
// Updated in $/Neuroshare/PowerNAP
// Converted to use SharedPointers instead of Indexes
// 
// *****************  Version 11  *****************
// User: Kkorver      Date: 2/27/04    Time: 2:04p
// Updated in $/Neuroshare/PowerNAP
// Made GetFile() a const function
// 
// *****************  Version 10  *****************
// User: Kkorver      Date: 2/27/04    Time: 1:45p
// Updated in $/Neuroshare/PowerNAP
// Added GetFile() and some comments
// 
// *****************  Version 9  *****************
// User: Kkorver      Date: 2/24/04    Time: 12:49p
// Updated in $/Neuroshare/PowerNAP
// Added GetTimeByIndex()
// 
// *****************  Version 8  *****************
// User: Kkorver      Date: 10/21/03   Time: 2:43p
// Updated in $/Neuroshare/PowerNAP
// Renamed program to PowerNAP
// 
// *****************  Version 7  *****************
// User: Kkorver      Date: 10/17/03   Time: 9:48a
// Updated in $/Neuroshare/nsClassifier
// Updated the "data" class to no longer store the Entity ID
// 
// *****************  Version 6  *****************
// User: Kkorver      Date: 10/14/03   Time: 10:42a
// Updated in $/Neuroshare/nsClassifier
// Updated data class to no longer hold "type"
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
// User: Abranner     Date: 9/11/03    Time: 9:05a
// Updated in $/Neuroshare/nsClassifier
// Made EntityID protected and added function to access it.
// 
// *****************  Version 2  *****************
// User: Abranner     Date: 9/09/03    Time: 3:03p
// Updated in $/Neuroshare/nsClassifier
// Changed the document to be able to incoorperate more entity types and
// loading more channels at a time. Waveforms are not stored in the
// document anymore. The WaveformList was turned into a SegmentList.
// 
// 
// $NoKeywords: $
//
//////////////////////////////////////////////////////////////////////

#ifndef DATA_H_INCLUDED
#define DATA_H_INCLUDED

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NsFile.h"

class Data  
{
public:
    Data(FileInfo * pcFile, DWORD dwIndex);
	virtual ~Data();

    DWORD GetIndex()        // give me the index for this particular item
        { return m_dwIndex; }
	
protected:
	NsFile & GetFile() const;       // Get the file class. Needed for many fxns.
    double GetTimeByIndex(DWORD dwEntityID, DWORD dwIndex) const;

    // All of these are stored as "Indices" as opposed to pointers. The reason is that
    // as the master lists get larger (and relocate in RAM), theEn pointers would no 
    // longer be valid. The "INDEX" will remain valid though.
//    DWORD m_dwIdxFile;              // this is the Index of the "file" in the FileList
//    DWORD m_dwIdxEntity;            // In the vector of entities for this type, this is the offset to it
    FileInfo * m_pcFile;              // This is the file our data is in
    DWORD m_dwIndex;                // This is the index (or starting index) for this entry. Can be used for GetTimeByIndex();
};

#endif // include guard

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
// $Workfile: NsLibrary.h $
//
// Description   : interface for the NsLibrary class
//
// Authors       : Kirk Korver
//
// $Date: 2005/03/16 16:54:31 $
//
// $History: NsLibrary.h $
// 
// *****************  Version 12  *****************
// User: Kkorver      Date: 5/19/04    Time: 1:39p
// Updated in $/Neuroshare/PowerNAP
// 
// *****************  Version 11  *****************
// User: Kkorver      Date: 5/12/03    Time: 11:14a
// Updated in $/Neuroshare/nsClassifier
// OpenFile() now accepts the DLL to use to open
// 
// *****************  Version 10  *****************
// User: Kirk         Date: 3/27/03    Time: 1:56p
// Updated in $/Neuroshare/nsClassifier
// Removed LibraryImpMgr from members
// Added Copy Constructor
// Added = operator
// 
// *****************  Version 9  *****************
// User: Kirk         Date: 3/25/03    Time: 2:11p
// Updated in $/Neuroshare/nsClassifier
// Rename the NsFile iterators
// 
// *****************  Version 8  *****************
// User: Kirk         Date: 3/04/03    Time: 2:23p
// Updated in $/Neuroshare/VC Source Code Example
// Made GetLastErrorMsg() a const function
// 
// *****************  Version 7  *****************
// User: Kirk         Date: 3/03/03    Time: 12:18p
// Updated in $/Neuroshare/VC Source Code Example
// First implementation of the new Library framework
// 
// *****************  Version 6  *****************
// User: Kirk         Date: 2/24/03    Time: 2:15p
// Updated in $/Neuroshare/VC Source Code Example
// Updated to use the new version 1.0 API
// 
// *****************  Version 5  *****************
// User: Kirk         Date: 1/27/03    Time: 2:17p
// Updated in $/Neuroshare/VC Source Code Example
// NeuroshareLibrary.h is now NsFile.h
// NsLibrary is now NsFile
// NsLibraryMgr is now MsFileMgr
// 
// *****************  Version 4  *****************
// User: Kirk         Date: 1/27/03    Time: 1:05p
// Updated in $/Neuroshare/VC Source Code Example
// Added Required Headers
// Added IsOK()
//
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef NSLIBRARY_H_INCLUDED   // our standard include guards
#define NSLIBRARY_H_INCLUDED

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "nsAPIdllimp.h"
#include "NsLibraryImpMgr.h"

////////////////////////////////// Forward declarations ///////////////////////



// This is the class that abstracts out a single library.
class NsLibrary
{
public:
    NsLibrary(NsLibraryImp const * pcLibraryImp = NULL);
    NsLibrary(NsLibrary const & rhs);
    ~NsLibrary();

    NsLibrary & operator = (const NsLibrary & rhs);


    bool IsOK() const
        { return m_pcLibraryImp == NULL ? false : true; }


    // Purpose: Replace the libraryImp with this one.
    // Inputs: 
    //  pcLibraryImp - the new library imp
    void SetLibraryImp(NsLibraryImp const * pcLibraryImp);

    // Purpose: Release the current LibraryImp
    void Release();



    // Purpose: Open this data file
    // Inputs:
    //  pszFilename - the name of the data file to open
    //  pszDllName - the name of the DLL to use to open this data file
    //               when this parameter is NULL then we use the data file extension
    //               to find the appropriate DLL
    // Outputs:
    //  hFile - handle to the file after it is open
    //  returns ns_OK if all is well
    ns_RESULT OpenFile(const char *pszFilename, uint32 *hFile, const char * pszDllName = NULL);
    

    ///////////////////////////////////////////////////////////////////////////////    


    // All of these functions map exactly into the function prototypes for the neuroshare API,
    // look there for the documentation on what they do.

    ns_RESULT GetLibraryInfo(ns_LIBRARYINFO *pLibraryInfo, uint32 dwLibraryInfoSize) const
        { _ASSERT (m_pcLibraryImp); return m_pcLibraryImp->GetLibraryInfo(pLibraryInfo, dwLibraryInfoSize); }
    
    ns_RESULT GetFileInfo(uint32 hFile, ns_FILEINFO *pFileInfo, uint32 dwFileInfoSize) const
        { _ASSERT (m_pcLibraryImp); return m_pcLibraryImp->GetFileInfo(hFile, pFileInfo, dwFileInfoSize); }
    
    ns_RESULT CloseFile(uint32 hFile) const 
        { _ASSERT (m_pcLibraryImp); return m_pcLibraryImp->CloseFile(hFile); }
    
    ns_RESULT GetEntityInfo(uint32 hFile, uint32 dwEntityID, ns_ENTITYINFO *pEntityInfo, uint32 dwEntityInfoSize) const
        { _ASSERT (m_pcLibraryImp); return m_pcLibraryImp->GetEntityInfo(hFile, dwEntityID, pEntityInfo, dwEntityInfoSize); }
    
    ns_RESULT GetEventInfo(uint32 hFile, uint32 dwEntityID, ns_EVENTINFO *pEventInfo, uint32 dwEventInfoSize) const
        { _ASSERT (m_pcLibraryImp); return m_pcLibraryImp->GetEventInfo(hFile, dwEntityID, pEventInfo, dwEventInfoSize); }
    
    ns_RESULT GetEventData(uint32 hFile, uint32 dwEntityID, uint32 nIndex, double *pdTimeStamp, void *pData, uint32 dwDataSize, uint32 *pdwDataRetSize) const 
        { _ASSERT (m_pcLibraryImp); return m_pcLibraryImp->GetEventData(hFile, dwEntityID, nIndex, pdTimeStamp, pData, dwDataSize, pdwDataRetSize); }
    
    ns_RESULT GetAnalogInfo(uint32 hFile, uint32 dwEntityID, ns_ANALOGINFO *pAnalogInfo, uint32 dwAnalogInfoSize) const
        { _ASSERT (m_pcLibraryImp); return m_pcLibraryImp->GetAnalogInfo(hFile, dwEntityID, pAnalogInfo, dwAnalogInfoSize); }
    
    ns_RESULT GetAnalogData(uint32 hFile, uint32 dwEntityID, uint32 dwStartIndex, uint32 dwIndexCount, uint32 *pdwContCount, double *pData) const
        { _ASSERT (m_pcLibraryImp); return m_pcLibraryImp->GetAnalogData(hFile, dwEntityID, dwStartIndex, dwIndexCount, pdwContCount, pData); }
    
    ns_RESULT GetSegmentInfo(uint32 hFile, uint32 dwEntityID, ns_SEGMENTINFO *pSegmentInfo, uint32 dwSegmentInfoSize) const
        { _ASSERT (m_pcLibraryImp); return m_pcLibraryImp->GetSegmentInfo(hFile, dwEntityID, pSegmentInfo, dwSegmentInfoSize); }
    
    ns_RESULT GetSegmentSourceInfo(uint32 hFile, uint32 dwEntityID, uint32 dwSourceID, ns_SEGSOURCEINFO *pSourceInfo, uint32 dwSourceInfoSize) const
        { _ASSERT (m_pcLibraryImp); return m_pcLibraryImp->GetSegmentSourceInfo(hFile, dwEntityID, dwSourceID, pSourceInfo, dwSourceInfoSize); }
    
    ns_RESULT GetSegmentData(uint32 hFile, uint32 dwEntityID, int32 nIndex, double *pdTimeStamp, double *pdData, uint32 dwDataBufferSize, uint32 *pdwSampleCount, uint32 *pdwUnitID) const
        { _ASSERT (m_pcLibraryImp); return m_pcLibraryImp->GetSegmentData(hFile, dwEntityID, nIndex, pdTimeStamp, pdData, dwDataBufferSize, pdwSampleCount, pdwUnitID); }
    
    ns_RESULT GetNeuralInfo(uint32 hFile, uint32 dwEntityID, ns_NEURALINFO *pNeuralInfo, uint32 dwNeuralInfoSize) const
        { _ASSERT (m_pcLibraryImp); return m_pcLibraryImp->GetNeuralInfo(hFile, dwEntityID, pNeuralInfo, dwNeuralInfoSize); }
    
    ns_RESULT GetNeuralData(uint32 hFile, uint32 dwEntityID, uint32 dwStartIndex, uint32 dwIndexCount, double *pdData) const
        { _ASSERT (m_pcLibraryImp); return m_pcLibraryImp->GetNeuralData(hFile, dwEntityID, dwStartIndex, dwIndexCount, pdData); }
    
    ns_RESULT GetIndexByTime(uint32 hFile, uint32 dwEntityID, double dTime, int32 nFlag, uint32 *pdwIndex) const
        { _ASSERT (m_pcLibraryImp); return m_pcLibraryImp->GetIndexByTime(hFile, dwEntityID, dTime, nFlag, pdwIndex); }
    
    ns_RESULT GetTimeByIndex(uint32 hFile, uint32 dwEntityID, uint32 dwIndex, double *pdTime) const
        { _ASSERT (m_pcLibraryImp); return m_pcLibraryImp->GetTimeByIndex(hFile, dwEntityID, dwIndex, pdTime); }
    
    ns_RESULT GetLastErrorMsg(char *pszMsgBuffer, uint32 dwMsgBufferSize) const
        { _ASSERT (m_pcLibraryImp); return m_pcLibraryImp->GetLastErrorMsg(pszMsgBuffer, dwMsgBufferSize); }



private:

    // This is where the actual library calls are made
    NsLibraryImp const * m_pcLibraryImp; 
};



#endif  // NSLIBRARY_H_INCLUDED   // our standard include guards

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
// $Workfile: NsFile.h $
//
// Description   : interface for the NsFile class
//
// Authors       : Kirk Korver
//
// $Date: 2005/03/16 16:54:31 $
//
// $History: NsFile.h $
// 
// *****************  Version 15  *****************
// User: Kkorver      Date: 5/19/04    Time: 1:39p
// Updated in $/Neuroshare/PowerNAP
// 
// *****************  Version 14  *****************
// User: Abranner     Date: 2/06/04    Time: 12:41p
// Updated in $/Neuroshare/PowerNAP
// Update data access functions and fixed bugs.
// 
// *****************  Version 13  *****************
// User: Kkorver      Date: 5/12/03    Time: 11:13a
// Updated in $/Neuroshare/nsClassifier
// Added OpenFileUsingThisDLL()
// 
// *****************  Version 12  *****************
// User: Kirk         Date: 3/27/03    Time: 1:54p
// Updated in $/Neuroshare/nsClassifier
// Mods to allow multiple data files open at same time
// 
// *****************  Version 11  *****************
// User: Kirk         Date: 3/25/03    Time: 2:11p
// Updated in $/Neuroshare/nsClassifier
// Rename the NsFile iterators
// 
// *****************  Version 10  *****************
// User: Kirk         Date: 3/21/03    Time: 9:45a
// Updated in $/Neuroshare/Data Editor
// Updated the uses of sizeof
// 
// *****************  Version 9  *****************
// User: Kirk         Date: 3/19/03    Time: 3:55p
// Updated in $/Neuroshare/Data Editor
// Many functions now take a reference instead of a pointer
// 
// *****************  Version 8  *****************
// User: Kirk         Date: 3/04/03    Time: 2:24p
// Updated in $/Neuroshare/VC Source Code Example
// Removed spurious #define
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
// Added required headers
// Renamed a few classes
//
// $NoKeywords: $
//
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef NSFILE_H_INCLUDED
#define NSFILE_H_INCLUDED

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NsLibrary.h"
#include "nsAPItypes.h"

// This class is the main interface to the Neuroshare libraries. The other classes
// are just helper files - except for the library manager.
class NsFile  
{
public:

	NsFile();
	~NsFile();

    // Comparison operator
    bool operator == (const NsFile & rhs) const { return (m_hDataFile == rhs.m_hDataFile); }

    // return TRUE if this library is OK to use; FALSE, otherwise.
    bool IsOK() const
        { return m_icLibrary.IsOK() && m_hDataFile != INVALID_HANDLE; }

    // Purpose: Open a data file while explicitely naming the DLL to use
    // Inputs:
    //  szDataPathName - the fully qualified path name of the data file to open
    //  szDllPathName - the fully qualified path name of the DLL to use to interpret this data file
    // Outputs:
    //  ns_OK if the file opened properly
    ns_RESULT OpenFileUsingThisDLL(LPCSTR szDataName, LPCSTR szDllPathName);

    // Open a file using the DLLs that are already located
    // returns ns_OK if sucessful
    ns_RESULT OpenFile(LPCSTR szDataName)
        { return m_icLibrary.OpenFile(szDataName, &m_hDataFile); }

    ns_RESULT CloseFile();


    ///////////////////////////////////////////////////////////////////////////////    

    // All of these functions map exactly into the function prototypes for the neuroshare API,
    // look there for the documentation on what they do.
    
    
    ns_RESULT GetLibraryInfo(ns_LIBRARYINFO & LibraryInfo) const
        { return m_icLibrary.GetLibraryInfo(& LibraryInfo, sizeof *&LibraryInfo); }

    ns_RESULT GetFileInfo(ns_FILEINFO & FileInfo) const
        { _ASSERT(m_hDataFile != INVALID_HANDLE); 
          return m_icLibrary.GetFileInfo(m_hDataFile, &FileInfo, sizeof *&FileInfo); }
    
    
    ns_RESULT GetEntityInfo(uint32 dwEntityID, ns_ENTITYINFO & EntityInfo) const
        { _ASSERT(m_hDataFile != INVALID_HANDLE); 
          return m_icLibrary.GetEntityInfo(m_hDataFile, dwEntityID, &EntityInfo, sizeof *&EntityInfo); }
    
    ns_RESULT GetEventInfo(uint32 dwEntityID, ns_EVENTINFO & EventInfo) const
        { _ASSERT(m_hDataFile != INVALID_HANDLE);  
          return m_icLibrary.GetEventInfo(m_hDataFile, dwEntityID, &EventInfo, sizeof *&EventInfo); }
    
    ns_RESULT GetEventData(uint32 dwEntityID, uint32 nIndex, double *pdTimeStamp, void *pData, uint32 dwDataSize, uint32 *pdwDataRetSize) const 
        { _ASSERT(m_hDataFile != INVALID_HANDLE); 
          return m_icLibrary.GetEventData(m_hDataFile, dwEntityID, nIndex, pdTimeStamp, pData, dwDataSize, pdwDataRetSize); }
    
    ns_RESULT GetAnalogInfo(uint32 dwEntityID, ns_ANALOGINFO & AnalogInfo) const 
        { _ASSERT(m_hDataFile != INVALID_HANDLE); 
          return m_icLibrary.GetAnalogInfo(m_hDataFile, dwEntityID, &AnalogInfo, sizeof *&AnalogInfo); }
    
    ns_RESULT GetAnalogData(uint32 dwEntityID, uint32 dwStartIndex, uint32 dwIndexCount, uint32 *pdwContCount, double *pData) const
        { _ASSERT(m_hDataFile != INVALID_HANDLE); 
          return m_icLibrary.GetAnalogData(m_hDataFile, dwEntityID, dwStartIndex, dwIndexCount, pdwContCount, pData); }
    
    ns_RESULT GetSegmentInfo(uint32 dwEntityID, ns_SEGMENTINFO & SegmentInfo) const
        { _ASSERT(m_hDataFile != INVALID_HANDLE); 
          return m_icLibrary.GetSegmentInfo(m_hDataFile, dwEntityID, &SegmentInfo, sizeof *&SegmentInfo); }
    
    ns_RESULT GetSegmentSourceInfo(uint32 dwEntityID, uint32 dwSourceID, ns_SEGSOURCEINFO & SourceInfo) const
        { _ASSERT(m_hDataFile != INVALID_HANDLE); 
          return m_icLibrary.GetSegmentSourceInfo(m_hDataFile, dwEntityID, dwSourceID, &SourceInfo, sizeof *&SourceInfo); }
    
    ns_RESULT GetSegmentData(uint32 dwEntityID, int32 nIndex, double *pdTimeStamp, double *pdData, uint32 dwDataBufferSize, uint32 *pdwSampleCount, uint32 *pdwUnitID) const
        { _ASSERT(m_hDataFile != INVALID_HANDLE); 
          return m_icLibrary.GetSegmentData(m_hDataFile, dwEntityID, nIndex, pdTimeStamp, pdData, dwDataBufferSize, pdwSampleCount, pdwUnitID); }
    
    ns_RESULT GetNeuralInfo(uint32 dwEntityID, ns_NEURALINFO & NeuralInfo) const
        { _ASSERT(m_hDataFile != INVALID_HANDLE); 
          return m_icLibrary.GetNeuralInfo(m_hDataFile, dwEntityID, &NeuralInfo, sizeof *&NeuralInfo); }
    
    ns_RESULT GetNeuralData(uint32 dwEntityID, uint32 dwStartIndex, uint32 dwIndexCount, double *pdData) const
        { _ASSERT(m_hDataFile != INVALID_HANDLE); 
          return m_icLibrary.GetNeuralData(m_hDataFile, dwEntityID, dwStartIndex, dwIndexCount, pdData); }
    
    ns_RESULT GetIndexByTime(uint32 dwEntityID, double dTime, int32 nFlag, uint32 *pdwIndex) const
        { _ASSERT(m_hDataFile != INVALID_HANDLE); 
          return m_icLibrary.GetIndexByTime(m_hDataFile, dwEntityID, dTime, nFlag, pdwIndex); }

    ns_RESULT GetTimeByIndex(uint32 dwEntityID, uint32 dwIndex, double *pdTime) const
        { _ASSERT(m_hDataFile != INVALID_HANDLE); 
          return m_icLibrary.GetTimeByIndex(m_hDataFile, dwEntityID, dwIndex, pdTime); }

    ns_RESULT GetLastErrorMsg(char *pszMsgBuffer, uint32 dwMsgBufferSize) const
        { return m_icLibrary.GetLastErrorMsg(pszMsgBuffer, dwMsgBufferSize); }


private:
    NsLibrary m_icLibrary;
    uint32 m_hDataFile;

    // The value that means that the handle returned is invalid
    enum { INVALID_HANDLE = 0 };

};

// This is stored as a shared pointer. So when the last instance is deleted, then
// we know it is time to close the data file.
struct FileInfo
{
    CString strFilePathName;        // Fully qualified name of the file
    ns_FILEINFO isFileInfo;         // FileInfo read from the file
    NsFile icFile;                  // The actual data file to read

    ~FileInfo()
        { icFile.CloseFile(); }
};

#endif // NSFILE_H_INCLUDED
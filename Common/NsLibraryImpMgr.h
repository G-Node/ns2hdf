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
// $Workfile: NsLibraryImpMgr.h $
//
// Description   : interface for the NsLibraryManager class
//
// Authors       : Kirk Korver
//
// $Date: 2005/07/14 15:56:32 $
//
// $History: NsLibraryImpMgr.h $
// 
// *****************  Version 14  *****************
// User: Kkorver      Date: 5/19/04    Time: 1:39p
// Updated in $/Neuroshare/PowerNAP
// 
// *****************  Version 13  *****************
// User: Kkorver      Date: 2/19/04    Time: 4:07p
// Updated in $/Neuroshare/PowerNAP
// There is an 'ADD' button on the library details. This button will set
// the correct settings in the registry so the DLL will be remembered
// 
// *****************  Version 12  *****************
// User: Kkorver      Date: 2/18/04    Time: 1:08p
// Updated in $/Neuroshare/VC Source Code Example
// Updated so AppPath is passed in
// 
// *****************  Version 11  *****************
// User: Kkorver      Date: 9/16/03    Time: 11:41a
// Updated in $/Neuroshare/nsClassifier
// Added GetExtensions()
// 
// *****************  Version 10  *****************
// User: Kkorver      Date: 9/11/03    Time: 1:57p
// Updated in $/Neuroshare/nsClassifier
// The NsLibraryElement now stores all of the extensions
// 
// *****************  Version 9  *****************
// User: Kkorver      Date: 5/12/03    Time: 11:17a
// Updated in $/Neuroshare/nsClassifier
// Added AddLibraryToSystem()
// 
// *****************  Version 8  *****************
// User: Kkorver      Date: 4/24/03    Time: 1:14p
// Updated in $/Neuroshare/nsClassifier
// Added GetElementList() and GetLibraryList()
// 
// *****************  Version 7  *****************
// User: Kkorver      Date: 4/11/03    Time: 10:48a
// Updated in $/Neuroshare/nsClassifier
// Added the abillty to see how many libaries are currently loaded
// 
// *****************  Version 6  *****************
// User: Kirk         Date: 3/27/03    Time: 1:56p
// Updated in $/Neuroshare/nsClassifier
// Added IncrementReferenceCount()
// 
// *****************  Version 5  *****************
// User: Kirk         Date: 3/25/03    Time: 2:11p
// Updated in $/Neuroshare/nsClassifier
// Rename the NsFile iterators
// 
// *****************  Version 4  *****************
// User: Kirk         Date: 3/05/03    Time: 2:02p
// Updated in $/Neuroshare/VC Source Code Example
// AddDllToList() now accepts a LPCSTR instead of a CString
// 
// *****************  Version 3  *****************
// User: Kirk         Date: 3/04/03    Time: 2:22p
// Updated in $/Neuroshare/VC Source Code Example
// Removed unnecessary include
// Fixed typo's in the comments
// Added comments to GetLibrary....()
// 
// *****************  Version 2  *****************
// User: Kirk         Date: 3/04/03    Time: 12:29p
// Updated in $/Neuroshare/VC Source Code Example
// In GetLastErrorMsg(), Assert was checking the wrong variable
// 
// *****************  Version 1  *****************
// User: Kirk         Date: 3/03/03    Time: 12:19p
// Created in $/Neuroshare/VC Source Code Example
// These are the library implementation manager classes as well as all
// supporting classes
// 
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef NSLIBRARYIMPMGR_H_INCLUDED   // our standard include guards
#define NSLIBRARYIMPMGR_H_INCLUDED

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "nsAPIdllimp.h"
#include <atlstr.h>

#include <list>
#include <vector>

////////////////////////////////// Forward declarations ///////////////////////

// This is the actual implementation of the library class.
class NsLibraryImp
{
public:
    //NsLibraryImp(LPCSTR szDllName);   Don't let us be constructed normally, the constructor is private
    ~NsLibraryImp();


    // return TRUE if the DLL was loaded properly; FALSE, otherwise
    bool IsOK() const
        { return m_hDLL != 0; }




    // All of these functions map exactly into the function prototypes for the neuroshare API,
    // look there for the documentation on what they do.

    ns_RESULT GetLibraryInfo(ns_LIBRARYINFO *pLibraryInfo, uint32 dwLibraryInfoSize) const
        { _ASSERT(m_pfGetLibraryInfo); return m_pfGetLibraryInfo(pLibraryInfo, dwLibraryInfoSize); }
    
    ns_RESULT OpenFile(const char *pszFilename, uint32 *hFile) const
        { _ASSERT(m_pfOpenFile); return m_pfOpenFile(pszFilename, hFile); }
    
    ns_RESULT GetFileInfo(uint32 hFile, ns_FILEINFO *pFileInfo, uint32 dwFileInfoSize) const
        { _ASSERT(m_pfGetFileInfo); return m_pfGetFileInfo(hFile, pFileInfo, dwFileInfoSize); }
    
    ns_RESULT CloseFile(uint32 hFile) const 
        { _ASSERT(m_pfCloseFile);  return m_pfCloseFile(hFile); }
    
    ns_RESULT GetEntityInfo(uint32 hFile, uint32 dwEntityID, ns_ENTITYINFO *pEntityInfo, uint32 dwEntityInfoSize) const
        { _ASSERT(m_pfGetEntityInfo); return m_pfGetEntityInfo(hFile, dwEntityID, pEntityInfo, dwEntityInfoSize); }
    
    ns_RESULT GetEventInfo(uint32 hFile, uint32 dwEntityID, ns_EVENTINFO *pEventInfo, uint32 dwEventInfoSize) const
        { _ASSERT(m_pfGetEventInfo); return m_pfGetEventInfo(hFile, dwEntityID, pEventInfo, dwEventInfoSize); }
    
    ns_RESULT GetEventData(uint32 hFile, uint32 dwEntityID, uint32 nIndex, double *pdTimeStamp, void *pData, uint32 dwDataSize, uint32 *pdwDataRetSize) const 
        { _ASSERT(m_pfGetEventData); return m_pfGetEventData(hFile, dwEntityID, nIndex, pdTimeStamp, pData, dwDataSize, pdwDataRetSize); }
    
    ns_RESULT GetAnalogInfo(uint32 hFile, uint32 dwEntityID, ns_ANALOGINFO *pAnalogInfo, uint32 dwAnalogInfoSize) const
        { _ASSERT(m_pfGetAnalogInfo); return m_pfGetAnalogInfo(hFile, dwEntityID, pAnalogInfo, dwAnalogInfoSize); }
    
    ns_RESULT GetAnalogData(uint32 hFile, uint32 dwEntityID, uint32 dwStartIndex, uint32 dwIndexCount, uint32 *pdwContCount, double *pData) const
        { _ASSERT(m_pfGetAnalogData); return m_pfGetAnalogData(hFile, dwEntityID, dwStartIndex, dwIndexCount, pdwContCount, pData); }
    
    ns_RESULT GetSegmentInfo(uint32 hFile, uint32 dwEntityID, ns_SEGMENTINFO *pSegmentInfo, uint32 dwSegmentInfoSize) const
        { _ASSERT(m_pfGetSegmentInfo); return m_pfGetSegmentInfo(hFile, dwEntityID, pSegmentInfo, dwSegmentInfoSize); }
    
    ns_RESULT GetSegmentSourceInfo(uint32 hFile, uint32 dwEntityID, uint32 dwSourceID, ns_SEGSOURCEINFO *pSourceInfo, uint32 dwSourceInfoSize) const
        { _ASSERT(m_pfGetSegmentSourceInfo); return m_pfGetSegmentSourceInfo(hFile, dwEntityID, dwSourceID, pSourceInfo, dwSourceInfoSize); }
    
    ns_RESULT GetSegmentData(uint32 hFile, uint32 dwEntityID, int32 nIndex, double *pdTimeStamp, double *pdData, uint32 dwDataBufferSize, uint32 *pdwSampleCount, uint32 *pdwUnitID) const
        { _ASSERT(m_pfGetSegmentData); return m_pfGetSegmentData(hFile, dwEntityID, nIndex, pdTimeStamp, pdData, dwDataBufferSize, pdwSampleCount, pdwUnitID); }
    
    ns_RESULT GetNeuralInfo(uint32 hFile, uint32 dwEntityID, ns_NEURALINFO *pNeuralInfo, uint32 dwNeuralInfoSize) const
        { _ASSERT(m_pfGetNeuralInfo); return m_pfGetNeuralInfo(hFile, dwEntityID, pNeuralInfo, dwNeuralInfoSize); }
    
    ns_RESULT GetNeuralData(uint32 hFile, uint32 dwEntityID, uint32 dwStartIndex, uint32 dwIndexCount, double *pdData) const
        { _ASSERT(m_pfGetNeuralData); return m_pfGetNeuralData(hFile, dwEntityID, dwStartIndex, dwIndexCount, pdData); }
    
    ns_RESULT GetIndexByTime(uint32 hFile, uint32 dwEntityID, double dTime, int32 nFlag, uint32 *pdwIndex) const
        { _ASSERT(m_pfGetIndexByTime); return m_pfGetIndexByTime(hFile, dwEntityID, dTime, nFlag, pdwIndex); }
    
    ns_RESULT GetTimeByIndex(uint32 hFile, uint32 dwEntityID, uint32 dwIndex, double *pdTime) const
        { _ASSERT(m_pfGetTimeByIndex); return m_pfGetTimeByIndex(hFile, dwEntityID, dwIndex, pdTime); }
    
    ns_RESULT GetLastErrorMsg(char *pszMsgBuffer, uint32 dwMsgBufferSize) const
        { _ASSERT(m_pfGetLastErrorMsg); return m_pfGetLastErrorMsg(pszMsgBuffer, dwMsgBufferSize); }



protected:
    HINSTANCE m_hDLL;       // The handle to the DLL


    // Purpose: This is the constructor. We don't want to be normally constructed, only by the
    //  LibraryImpManager.
    // Inputs:
    //  szDllName - the name of the DLL to open. NULL is ok as well.
    NsLibraryImp(LPCSTR szDllName);

    NsLibraryImp(NsLibraryImp const & rhs);
    NsLibraryImp & operator = (NsLibraryImp const & rhs);


    // Purpose: Load the DLL of this name. It is really intended to be called for the
    //  LibImp Manager. That is why it is over here
    // Inputs:
    //  szDllName - the name of the DLL to open
    // Returns:
    //  TRUE if life is good; FALSE, otherwise
    bool LoadDLL(LPCSTR szDllName);

    // Purpose: Clean everything up
    void CleanUp();



    // These are all of the function pointers to the loaded DLL
    NS_GETLIBRARYINFO m_pfGetLibraryInfo;
    NS_OPENFILE m_pfOpenFile;
    NS_GETFILEINFO m_pfGetFileInfo;
    NS_CLOSEFILE m_pfCloseFile;
    NS_GETENTITYINFO m_pfGetEntityInfo;
    NS_GETEVENTINFO m_pfGetEventInfo;
    NS_GETEVENTDATA m_pfGetEventData;
    NS_GETANALOGINFO m_pfGetAnalogInfo;
    NS_GETANALOGDATA m_pfGetAnalogData;
    NS_GETSEGMENTINFO m_pfGetSegmentInfo;
    NS_GETSEGMENTSOURCEINFO m_pfGetSegmentSourceInfo;
    NS_GETSEGMENTDATA m_pfGetSegmentData;
    NS_GETNEURALINFO m_pfGetNeuralInfo;
    NS_GETNEURALDATA m_pfGetNeuralData;
    NS_GETINDEXBYTIME m_pfGetIndexByTime;
    NS_GETTIMEBYINDEX m_pfGetTimeByIndex;
    NS_GETLASTERRORMSG m_pfGetLastErrorMsg;

    friend class NsLibraryImpMgr;
    friend class NsLibraryList;
    friend class NsLibraryElement;
};


// These are the individual elements of the list below
class NsLibraryElement
{
public:
    NsLibraryElement();
    ~NsLibraryElement();

    CString m_strDllPathName;           // fully qualified path + name of the DLL

    NsLibraryImp * GetLibraryImp();     // Get a copy of the library, and increment the reference count
    void Release();                     // Reduce the reference count and delete if necessary
    void IncrementReferenceCount();     // Increment the reference count

    bool CanReadFileType(LPCSTR szExtension);   // Tell me if I can understand this file type

    const std::vector<CString> & GetExtensions() const { return m_vExtensions; }

private:
    NsLibraryImp * m_pcLibraryImp;      // Pointer to the one and only instance of the DLL wrapper
    UINT m_nLibraryImpRefCount;         // current number of uses of this DLL wrapper
    std::vector<CString> m_vExtensions; // All of the extensions that this library can understand

    friend class NsLibraryList;
};

typedef std::list <NsLibraryElement> ElementList;




// This is a collection of elements
class NsLibraryList
{
public:
    NsLibraryList(LPCSTR szAppPathName);    // Fully qualified name of the exe running
    ~NsLibraryList();


    // Purpose: find a matching element in the list
    // Inputs:
    //  szDllPathName - fully qualified name of the DLL to open
    // Outputs:
    //  The element in the list if found; NULL, otherwise
    NsLibraryElement * FindPathMatch(LPCSTR szDllPathName);


    // Purpose: find a matching element in the list
    // Inputs:
    //  szDataExtension - extension of data file to match (e.g.  "nev")
    // Outputs:
    //  The element in the list if found; NULL, otherwise
    NsLibraryElement * FindExtensionMatch(LPCSTR szDataExtension);

    // Purpose: find a matching element in the list
    // Inputs:
    //  pcLibraryImp - pointer to the library to match up
    // Outputs:
    //  The element in the list if found; NULL, otherwise
    NsLibraryElement * FindLibraryMatch(NsLibraryImp const * pcLibraryImp);


    // Purpose: Tell me how many entries there are in this list
    // Outputs:
    //  The number of elements in the list. 0 if the list is empty
    int Size();

    const ElementList & GetElementList() const { return m_icList; }

private:
    // This is a single element. It needs to become a full list
    ElementList m_icList;

    // Purpose: Add to the list of elements by searching the local directory
    // Inputs:
    //  szPath - the path to look for DLLs in
    void AddListLocal(LPCSTR szPath);

    // Purpose: Add to the list of elements by searching the registry
    void AddListRegistry();

    // Purpose: Add this named DLL to the list of available DLLs
    // Inputs:
    //  szName - the name of the DLL to add
    void AddDllToList(LPCSTR szName);

    friend class NsLibraryImpMgr;
};




// This is the library manager. It is the major manager
class NsLibraryImpMgr
{
public:
    NsLibraryImpMgr(LPCSTR szAppPath) :  // fully qualified path. No terminating slash
      m_icLibraryList(szAppPath) {}

    // Think basic constructor. It needs to called once 
    static void CreateMgr(LPCSTR szAppPath);    // Think basic constructor (fully qualified path, no end slash)
    static void DestroyMgr();                   // Think basic destructor

    // Reduce the reference count of this library, and then delete if necessary
    void ReleaseLibrary(NsLibraryImp const * pcLibraryImp);

    // Increment the number of times this library is being used
    void IncrementReferenceCount(NsLibraryImp const * pcLibraryImp);


    // Purpose: Add a library to those we understand, and return a pointer
    //  to the library class. In the case where the library already exists,
    //  then the existing library will be returned
    // Inputs:
    //  szDllPathName - the name of the DLL to add to the system
    // Outputs:
    //  pointer to the library or NULL if an error
    NsLibraryImp * AddLibraryToSystem(LPCSTR szDllPathName);


    // The way to instantiate a library installed on the system. If this library 
    //  cannot be loaded, then the return value will be NULL.
    // Inputs:
    //  szDllPathName - fully qualified name of the DLL to open
    // Outputs:
    //  the library, or NULL if the library cannot be opened.
    NsLibraryImp * GetLibrary(LPCSTR szDllPathName);

    // Purpose: Instantiate a library given an extension of a data file
    // Inputs:
    //  szExtension - the extension of the data file to try to open
    // Outputs:
    //  pointer to the library or NULL if an error
    NsLibraryImp * GetLibraryFromExtension(LPCSTR szExtension);

    // Purpose: Instantiate a library given a fully qualified name of a 
    //  data file that we might want to eventually open
    // Inputs:
    //  szDataFileName - fully qualifed path name of the data file we
    //                   may want to eventually open
    // Outputs:
    //  pointer to the library or NULL if an error
    NsLibraryImp * GetLibraryFromDataFileName(LPCSTR szDataFileName);

    
    
    
    // The way to get a pointer to the only Library manager
    static NsLibraryImpMgr & GetMgr()
        { 
			//ASSERT(m_pcLibraryImpMgr); 
			return *m_pcLibraryImpMgr; }


    // Tell us how many libaries (aka DLL's) have been loaded and are
    // Available for us
    // Outputs:
    //  The number of elements in the list. 0 if the list is empty
    int GetNumOfLibrariesAvailable();

    const NsLibraryList & GetLibraryList() const { return m_icLibraryList; }


private:
    // There is one and only 1 mgr. Here it is
    static NsLibraryImpMgr * m_pcLibraryImpMgr;


    // This is the list of the Libraries
    NsLibraryList m_icLibraryList;
};

static const char szAPP_REGISTRY_BASE[] = "Software\\NEUROSHARE\\PowerNap";
static const char szLIBRARY_REGISTRY_BASE[] = "Software\\NEUROSHARE\\NEUROSHARELIBRARIES";


#endif    // our standard include guards

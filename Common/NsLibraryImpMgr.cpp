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
// $Workfile: NsLibraryImpMgr.cpp $
//
// Description   : implementation of the NsLibraryImpMgr class, as well as other supporting classes
//
// Authors       : Kirk Korver
//
// $Date: 2005/03/16 16:54:31 $
//
// $History: NsLibraryImpMgr.cpp $
// 
// *****************  Version 14  *****************
// User: Kkorver      Date: 5/26/04    Time: 5:13p
// Updated in $/Neuroshare/PowerNAP
// Bug fix: wasn't properly adding all possible DLLs that have been stored
// in the registry.
// 
// *****************  Version 13  *****************
// User: Kkorver      Date: 5/19/04    Time: 1:39p
// Updated in $/Neuroshare/PowerNAP
// 
// *****************  Version 12  *****************
// User: Abranner     Date: 4/23/04    Time: 1:51p
// Updated in $/Neuroshare/PowerNAP
// Fixed warnings after switch to .NET.
// 
// *****************  Version 11  *****************
// User: Kkorver      Date: 2/19/04    Time: 4:07p
// Updated in $/Neuroshare/PowerNAP
// There is an 'ADD' button on the library details. This button will set
// the correct settings in the registry so the DLL will be remembered
// 
// *****************  Version 10  *****************
// User: Kkorver      Date: 2/18/04    Time: 1:09p
// Updated in $/Neuroshare/VC Source Code Example
// Updated so AppPath is passed in
// 
// *****************  Version 9  *****************
// User: Kkorver      Date: 10/21/03   Time: 2:43p
// Updated in $/Neuroshare/PowerNAP
// Renamed program to PowerNAP
// 
// *****************  Version 8  *****************
// User: Kkorver      Date: 9/16/03    Time: 11:38a
// Updated in $/Neuroshare/nsClassifier
// Only add an extension to the list if the extension is valid
// 
// *****************  Version 7  *****************
// User: Kkorver      Date: 9/11/03    Time: 1:57p
// Updated in $/Neuroshare/nsClassifier
// The NsLibraryElement now stores all of the extensions
// 
// *****************  Version 6  *****************
// User: Kkorver      Date: 5/12/03    Time: 11:17a
// Updated in $/Neuroshare/nsClassifier
// Added AddLibraryToSystem()
// 
// *****************  Version 5  *****************
// User: Kkorver      Date: 4/11/03    Time: 10:48a
// Updated in $/Neuroshare/nsClassifier
// Added the abillty to see how many libaries are currently loaded
// 
// *****************  Version 4  *****************
// User: Kirk         Date: 3/27/03    Time: 1:56p
// Updated in $/Neuroshare/nsClassifier
// Added IncrementReferenceCount()
// 
// *****************  Version 3  *****************
// User: Kirk         Date: 3/05/03    Time: 2:03p
// Updated in $/Neuroshare/VC Source Code Example
// AddDllToList() now accepts a LPCSTR instead of a CString
// We now look in the registry for the data files.
// 
// *****************  Version 2  *****************
// User: Kirk         Date: 3/04/03    Time: 2:25p
// Updated in $/Neuroshare/VC Source Code Example
// m_pfGetLastErrorMsg wasn't being initialized in the constructor
// m_pfGetLastErrorMsg wasn't ever being set
// 
// *****************  Version 1  *****************
// User: Kirk         Date: 3/03/03    Time: 12:19p
// Created in $/Neuroshare/VC Source Code Example
// These are the library implementation manager classes as well as all
// supporting classes
// 
//
///////////////////////////////////////////////////////////////////////////////////////////////////

// #include "StdAfx.h"
#include <atlbase.h>
#include <algorithm>

#include "NsLibraryImpMgr.h"
#include "NsLibrary.h"
#include "FileSpec.h"
#include "ns_common.h"

#include <atlstr.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

using namespace std;


// This is the one and only library manager
NsLibraryImpMgr * NsLibraryImpMgr::m_pcLibraryImpMgr = NULL;    




// Author & Date:   Kirk Korver     FEb 27, 2003
// Purpose: find the extension of the filename. By that I mean, return a 
//  pointer to the part in the filename after the last period.
// Inputs:
//  filename to search
// Outputs:
//  a pointer to the first character after the last period, or NULL on failure
LPCSTR GetExtension(LPCSTR szFileName)
{
    // Let's make sure that 
    if (!szFileName)
        return NULL;

    const char * szChar = strrchr(szFileName, '.');
    if (szChar)
    {
        ++szChar;
    }
    return szChar;
}




//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

NsLibraryImp::NsLibraryImp(LPCSTR szDllName) :
    m_pfGetLibraryInfo(0),
    m_pfOpenFile(0),
    m_pfGetFileInfo(0),
    m_pfCloseFile(0),
    m_pfGetEntityInfo(0),
    m_pfGetEventInfo(0),
    m_pfGetEventData(0),
    m_pfGetAnalogInfo(0),
    m_pfGetAnalogData(0),
    m_pfGetSegmentInfo(0),
    m_pfGetSegmentSourceInfo(0),
    m_pfGetSegmentData(0),
    m_pfGetNeuralInfo(0),
    m_pfGetNeuralData(0),
    m_pfGetIndexByTime(0),
    m_pfGetTimeByIndex(0),
    m_pfGetLastErrorMsg(0),
    m_hDLL(0)
{
    LoadDLL(szDllName);
}



NsLibraryImp::~NsLibraryImp()
{
    CleanUp();
}



// Author & Date:       Kirk Korver     08 Jan 2003
// Load the DLL into memory and set all of the function pointers
// Inputs:
//  szDllName - the name of the DLL to open
// Returns:
//  TRUE if life is good; FALSE, otherwise
bool NsLibraryImp::LoadDLL(LPCSTR szDllName)
{
    // Make it so we don't have to remember to unload prior to loading
    if (m_hDLL)
        CleanUp();

    do
    {
        m_hDLL = ::LoadLibrary(szDllName);
        if (!m_hDLL)
            break;


        // Now lets set all of our pointers


        // A macro to help with gettng the addresses
        #define SetPointers(MyName, Type, DllName)                  \
            MyName = (Type) GetProcAddress(m_hDLL, DllName);        \
            if (0 == MyName)                                        \
                break;                                              \


        SetPointers(m_pfGetLibraryInfo, NS_GETLIBRARYINFO, "ns_GetLibraryInfo")
        SetPointers(m_pfOpenFile, NS_OPENFILE, "ns_OpenFile")
        SetPointers(m_pfGetFileInfo, NS_GETFILEINFO, "ns_GetFileInfo")
        SetPointers(m_pfCloseFile, NS_CLOSEFILE, "ns_CloseFile")
        SetPointers(m_pfGetEntityInfo, NS_GETENTITYINFO, "ns_GetEntityInfo")
        SetPointers(m_pfGetEventInfo, NS_GETEVENTINFO, "ns_GetEventInfo")
        SetPointers(m_pfGetEventData, NS_GETEVENTDATA, "ns_GetEventData")
        SetPointers(m_pfGetAnalogInfo, NS_GETANALOGINFO, "ns_GetAnalogInfo")
        SetPointers(m_pfGetAnalogData, NS_GETANALOGDATA, "ns_GetAnalogData")
        SetPointers(m_pfGetSegmentInfo, NS_GETSEGMENTINFO, "ns_GetSegmentInfo")
        SetPointers(m_pfGetSegmentSourceInfo, NS_GETSEGMENTSOURCEINFO, "ns_GetSegmentSourceInfo")
        SetPointers(m_pfGetSegmentData, NS_GETSEGMENTDATA, "ns_GetSegmentData")
        SetPointers(m_pfGetNeuralInfo, NS_GETNEURALINFO, "ns_GetNeuralInfo")
        SetPointers(m_pfGetNeuralData, NS_GETNEURALDATA, "ns_GetNeuralData")
        SetPointers(m_pfGetIndexByTime, NS_GETINDEXBYTIME, "ns_GetIndexByTime")
        SetPointers(m_pfGetTimeByIndex, NS_GETTIMEBYINDEX, "ns_GetTimeByIndex")
        SetPointers(m_pfGetLastErrorMsg, NS_GETLASTERRORMSG, "ns_GetLastErrorMsg")

        #undef SetPointers
        // We don't need the macro any more


        // If we get here, then life is good
        return true;

    } while (false);

    // if we got here, then bad things have happened
    CleanUp();
    return false;

}



// Author & Date:       Kirk Korver     08 Jan 2003
void NsLibraryImp::CleanUp()
{
    if (m_hDLL)
    {
        FreeLibrary(m_hDLL);
        m_hDLL = 0;
    }
    m_pfGetLibraryInfo = 0;
    m_pfOpenFile = 0;
    m_pfGetFileInfo = 0;
    m_pfCloseFile = 0;
    m_pfGetEntityInfo = 0;
    m_pfGetEventInfo = 0;
    m_pfGetEventData = 0;
    m_pfGetAnalogInfo = 0;
    m_pfGetAnalogData = 0;
    m_pfGetSegmentInfo = 0;
    m_pfGetSegmentSourceInfo = 0;
    m_pfGetSegmentData = 0;
    m_pfGetNeuralInfo = 0;
    m_pfGetNeuralData = 0;
    m_pfGetIndexByTime = 0;
    m_pfGetTimeByIndex = 0;
    m_pfGetLastErrorMsg = 0;
}





///////////////////////////////////////////////////////////////////////////////////////
// This is the library manager

// Author & Date:   Kirk Korver     Feb 25, 2003
// Purpose: create the singleton library manager
// Inputs:
//  szAppPath - fully qualified path. No terminating slash
void NsLibraryImpMgr::CreateMgr(LPCSTR szAppPath)
{
    if (NULL == m_pcLibraryImpMgr)
    {
        m_pcLibraryImpMgr = new NsLibraryImpMgr(szAppPath);
    }
}


// Author & Date:   Kirk Korver     Feb 25, 2003
// Purpose: destroy the singleton library manager
void NsLibraryImpMgr::DestroyMgr()
{
    delete m_pcLibraryImpMgr;
    m_pcLibraryImpMgr = NULL;
}


// Author & Date:   Kirk Korver     Feb 27, 2003
// Purpose: The way to instantiate a library. If this library cannot be loaded, then
// the return value will be NULL.
// Inputs:
//  szDllPathName - fully qualified name of the DLL to open
// Outputs:
//  the library, or NULL if the library cannot be opened.
NsLibraryImp * NsLibraryImpMgr::GetLibraryFromExtension(LPCSTR szExtension)
{
    // If the input is no good, then return empty
    if (szExtension == NULL)
        return NULL;

    NsLibraryElement * pcElement = m_icLibraryList.FindExtensionMatch(szExtension);

    if (pcElement)  // If I found a match
        return pcElement->GetLibraryImp();
    else
        return NULL;
}



// Author & Date:   Kirk Korver     Feb 28, 2003
// Purpose: The way to instantiate a library. If this library cannot be loaded, then
//  the return value will be NULL.
// Inputs:
//  szDataFileName - the name of the data file we want to be able to read
// Outputs:
//  pointer to the library that will read this, or NULL if one can't be found
NsLibraryImp * NsLibraryImpMgr::GetLibraryFromDataFileName(LPCSTR szDataFileName)
{
    LPCSTR szExt = ::GetExtension(szDataFileName);
    return GetLibraryFromExtension(szExt);
}


// Author & Date:   Kirk Korver     FEb 27, 2003
// Purpose: Reduce the reference count of this library, and then delete if necessary
void NsLibraryImpMgr::ReleaseLibrary(NsLibraryImp const * pcLibraryImp)
{
    // We only need to do anything if the library is possibly valid
    if (pcLibraryImp)
    {
        NsLibraryElement * pcElement = m_icLibraryList.FindLibraryMatch(pcLibraryImp);
        // ASSERT(pcElement);

        // The reason for the test and the ASSERT is to ensure that it is found here, but 
        // don't cause a fault in the production code.
        if (pcElement)              
            pcElement->Release();
    }
}


// Author & Date:   Kirk Korver     FEb 27, 2003
// Increment the number of times this library is being used
void NsLibraryImpMgr::IncrementReferenceCount(NsLibraryImp const * pcLibraryImp)
{
    // We only need to do anything if the library is possibly valid
    if (pcLibraryImp)
    {
        NsLibraryElement * pcElement = m_icLibraryList.FindLibraryMatch(pcLibraryImp);
        // ASSERT(pcElement);

        // The reason for the test and the ASSERT is to ensure that it is found here, but 
        // don't cause a fault in the production code.
        if (pcElement)              
            pcElement->IncrementReferenceCount();
    }

}


// Purpose: Add a library to those we understand, and return a pointer
//  to the library class. In the case where the library already exists,
//  then the existing library will be returned
// Inputs:
//  szDllPathName - the name of the DLL to add to the system
// Outputs:
//  pointer to the library or NULL if an error
NsLibraryImp * NsLibraryImpMgr::AddLibraryToSystem(LPCSTR szDllPathName)
{
    NsLibraryImp * retVal;

    // If we already know about it, then use it
    retVal = GetLibrary(szDllPathName);
    if (retVal)
        return retVal;

    // Otherwise, we will have to "add it"
    m_icLibraryList.AddDllToList(szDllPathName);

    // Ok..it's in there, now lets get it
    retVal = GetLibrary(szDllPathName);
    return retVal;
}


// Author & Date:   Kirk Korver     Feb 25, 2003
// The way to instantiate a library installed on the system. If this library 
//  cannot be loaded, then the return value will be NULL.
// Inputs:
//  szDllPathName - fully qualified name of the DLL to open
// Outputs:
//  the library, or NULL if the library cannot be opened.
NsLibraryImp * NsLibraryImpMgr::GetLibrary(LPCSTR szDllPathName)
{
    // If the input is no good, then return empty
    if (szDllPathName == NULL)
        return NULL;

    NsLibraryElement * pcElement = m_icLibraryList.FindPathMatch(szDllPathName);

    if (pcElement)  // If I found a match
        return pcElement->GetLibraryImp();
    else
        return NULL;
}


// Author & Date:   Kirk Korver     Apr 11, 2003
// Tell us how many libaries (aka DLL's) have been loaded and are
// Available for us
int NsLibraryImpMgr::GetNumOfLibrariesAvailable()
{
    return m_icLibraryList.Size();
}



// Author & Date:   Kirk Korver     Feb 25, 2003
NsLibraryElement::NsLibraryElement() :
    m_pcLibraryImp(0),
    m_nLibraryImpRefCount(0)
{
}


// Author & Date:   Kirk Korver     Feb 25, 2003
NsLibraryElement::~NsLibraryElement()
{
    delete m_pcLibraryImp;
    m_pcLibraryImp = 0;
    m_nLibraryImpRefCount = 0;
}



const char * g_szLOCAL_LIB_SEARCH_PATH = "\\NeuroshareLibraries";
const char * g_szWILD_CARD = "\\*.dll";

// Author & Date:   Kirk Korver     Feb 25, 2003
// Purpose: build the list
// Inputs:
//  szAppPath - fully qualified name of the path. No terminating slashes
NsLibraryList::NsLibraryList(LPCSTR szAppPath)
{
    // Get the current working directory
    CString szDir(szAppPath);

    // First look in our current working directory
    AddListLocal(szDir);

    szDir += g_szLOCAL_LIB_SEARCH_PATH;

    // Now look in the "common" subdirectory
    AddListLocal(szDir);

    // and finally, look at the registry
    AddListRegistry();
}

// Author & Date:   Kirk Korver     Feb 25, 2003
// Purpose: cleanup
NsLibraryList::~NsLibraryList()
{

}

// Author & Date:  Kirk Korver     Sep 11, 2003
// Purpose: Add to the list of elements by searching the local directory
// Inputs:
//  szPath - the path to look for DLLs in
void NsLibraryList::AddListLocal(LPCSTR szPath)
{

    // Make a the fully qualified search path
    CString strFilesToFind(szPath);
    strFilesToFind += g_szWILD_CARD;

    CFileSpec spec;
    for (bool more = spec.First(strFilesToFind); more; more = spec.Next() ) 
    {
        if( !(spec.attrib & (_A_SUBDIR | _A_HIDDEN | _A_SYSTEM)))
        {
            // Unfortunatly, the returned name is only that of the file,
            // it doesn't include the path. Therefore, I have to manually
            // build it. :-(
            CString strFound;
            strFound.Format("%s\\%s", szPath, spec.name);

            // Got me a winner here, add it.
            AddDllToList(strFound);
        }
    }

}


// Purpose: Add this named DLL to the list of available DLLs
// Inputs:
//  strName - the name of the DLL to add
void NsLibraryList::AddDllToList(LPCSTR szName)
{
    // Well firstly, we need to open up a DLL and see what kind of data files it understands

    // Load the library directly. don't use the Library manager because we are building
    // the information that it would need for later
    NsLibraryImp icLib(szName);
    if (!icLib.IsOK())
        return;

    // And now let's learn about it
    ns_LIBRARYINFO isInfo;
    if (ns_OK != icLib.GetLibraryInfo(&isInfo, sizeof(isInfo)))
        return;


    // Ok, now I know what it does, let's save it for later
    NsLibraryElement icElement;
    icElement.m_strDllPathName = szName;
    // Add the extensions
    for (DWORD i = 0; i < isInfo.dwFileDescCount; ++i)
    {
        // Only add the extension if there is an extension
        CString szExt = isInfo.FileDesc[i].szExtension;
        szExt.TrimLeft();
        szExt.TrimRight();
        if (szExt.GetLength())
            icElement.m_vExtensions.push_back(szExt);
    }

    // Now add the elemet to the list
    m_icList.push_back(icElement);
}


// Purpose: Add to the list of elements by searching the registry
void NsLibraryList::AddListRegistry()
{
    CRegKey icKey;
    if (ERROR_SUCCESS == icKey.Open(HKEY_LOCAL_MACHINE, _T(szLIBRARY_REGISTRY_BASE), KEY_READ))
    {
        const UINT BUFF_LEN = 256;
	    TCHAR szBuffer[BUFF_LEN];

	    for (DWORD dwIndex = 0; ; ++dwIndex)
	    {
	        DWORD dwSize = ARRAY_SIZE(szBuffer);
            if (icKey.EnumKey(dwIndex, szBuffer, &dwSize) == ERROR_NO_MORE_ITEMS)
                break;  // normal exit of loop

            CRegKey icNewKey;
            if (ERROR_SUCCESS == icNewKey.Open(icKey, szBuffer, KEY_READ))
            {
                TCHAR szPathBuffer[BUFF_LEN];
                ULONG nBytes = ARRAY_SIZE(szPathBuffer);

                if (ERROR_SUCCESS == icNewKey.QueryStringValue("Path", szPathBuffer, &nBytes))
                {
                    CString strDLLName(szPathBuffer);
                    AddDllToList(strDLLName);
                }
            }
	    }
    }
}


// Author & Date:   Kirk Korver     Feb 25, 2003
// Purpose: find a matching element in the list
// Inputs:
//  szDllPathName - fully qualified name of the DLL to open
// Outputs:
//  The element in the list if found; NULL, otherwise
NsLibraryElement * NsLibraryList::FindPathMatch(LPCSTR szDllPathName)
{
    ElementList::iterator it;
    for (it = m_icList.begin(); 
         it != m_icList.end(); 
         ++it)
    {
        NsLibraryElement & rcElement = (*it);
        if (stricmp(szDllPathName, rcElement.m_strDllPathName) == 0)
            return &rcElement;
    }
    return NULL;
}


// Author & Date:   Kirk Korver     Feb 25, 2003
// Purpose: find a matching element in the list
// Inputs:
//  szDataExtension - extension of data file to match (e.g.  "nev")
// Outputs:
//  The element in the list if found; NULL, otherwise
NsLibraryElement * NsLibraryList::FindExtensionMatch(LPCSTR szDataExtension)
{
    for (ElementList::iterator it = m_icList.begin(); it != m_icList.end(); ++it)
    {
        NsLibraryElement & rcElement = (*it);

        if (it->CanReadFileType(szDataExtension))
            return &rcElement;

    }
    return NULL;
}

// Purpose: find a matching element in the list
// Inputs:
//  pcLibraryImp - pointer to the library to match up
// Outputs:
//  The element in the list if found; NULL, otherwise
NsLibraryElement * NsLibraryList::FindLibraryMatch(NsLibraryImp const * pcLibraryImp)
{
    ElementList::iterator it;
    for (it = m_icList.begin(); 
         it != m_icList.end(); 
         ++it)
    {
        NsLibraryElement & rcElement = (*it);
        if (pcLibraryImp == rcElement.m_pcLibraryImp)
            return &rcElement;
    }
    return NULL;
}


// Purpose: Tell me how many entries there are in this list
// Outputs:
//  The number of elements in the list. 0 if the list is empty
int NsLibraryList::Size()
{
    return (int)m_icList.size();
}


// Author & Date:   Kirk Korver     Feb 27, 2003
// Purpose: Get a copy of the library, and increment the reference count
// Outputs:
//  a pointer to the imp
NsLibraryImp * NsLibraryElement::GetLibraryImp()
{
    NsLibraryImp * pcRetVal = NULL;      // The return value

    if (m_pcLibraryImp)  // This only happens if the library is already in use
    {
        pcRetVal = m_pcLibraryImp;
        ++m_nLibraryImpRefCount;
    }
    else    // Ok. we have to create a new one here
    {
        pcRetVal = new NsLibraryImp(m_strDllPathName);
        if (pcRetVal->IsOK())
        {
            m_pcLibraryImp = pcRetVal;
            ++m_nLibraryImpRefCount;
        }
        else
        {
            delete pcRetVal;
            pcRetVal = NULL;
        }
    }
    
    return pcRetVal;
};


// Author & Date:       Kirk Korver     
// Reduce the reference count and delete if necessary
void NsLibraryElement::Release()
{
    --m_nLibraryImpRefCount;
    if (m_nLibraryImpRefCount == 0)
    {
        delete m_pcLibraryImp;
        m_pcLibraryImp = 0;
    }
}

// Author & Date:       Kirk Korver     Mar 27, 2003
// Purpose: Increment the reference count, cuz we have another user of us
void NsLibraryElement::IncrementReferenceCount()
{
    ++m_nLibraryImpRefCount;
}


bool NsLibraryElement::CanReadFileType(LPCSTR szExtension)
{
    if(find(m_vExtensions.begin(), m_vExtensions.end(), szExtension) == m_vExtensions.end())
    {
        return false;
    }
    else
    {
        return true;
    }
}

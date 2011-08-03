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
// $Workfile: NsLibrary.cpp $
//
// Description   : implementation of the NsLibrary class.
//
// Authors       : Kirk Korver
//
// $Date: 2005/03/16 16:54:31 $
//
// $History: NsLibrary.cpp $
// 
// *****************  Version 11  *****************
// User: Kkorver      Date: 5/19/04    Time: 1:39p
// Updated in $/Neuroshare/PowerNAP
// 
// *****************  Version 10  *****************
// User: Kkorver      Date: 5/12/03    Time: 11:14a
// Updated in $/Neuroshare/nsClassifier
// OpenFile() now accepts the DLL to use to open
// 
// *****************  Version 9  *****************
// User: Kirk         Date: 3/27/03    Time: 1:56p
// Updated in $/Neuroshare/nsClassifier
// Removed LibraryImpMgr from members
// Added Copy Constructor
// Added = operator
// 
// *****************  Version 8  *****************
// User: Kirk         Date: 3/04/03    Time: 12:29p
// Updated in $/Neuroshare/VC Source Code Example
// Removed unnecessary header include
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
// User: Kirk         Date: 1/27/03    Time: 1:08p
// Updated in $/Neuroshare/VC Source Code Example
// Added Requried Headers
// Shortened Neuroshare to Ns in names
//
///////////////////////////////////////////////////////////////////////////////////////////////////

// #include "StdAfx.h"
#include "NsLibrary.h"
#include "NsLibraryImpMgr.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif



// Author & Date:   Kirk Korver     Feb 25, 2003
// Purpose: construct a library type. In this case,
//  we want to save the library imp type
NsLibrary::NsLibrary(NsLibraryImp const * pcLibraryImp /* = NULL */) :
    m_pcLibraryImp(pcLibraryImp)
{
}


// Author & Date:   Kirk Korver     Mar 27, 2003
// Purpose: Copy constructor
NsLibrary::NsLibrary(NsLibrary const & rhs)
{
    this->m_pcLibraryImp = rhs.m_pcLibraryImp;
    NsLibraryImpMgr::GetMgr().IncrementReferenceCount(m_pcLibraryImp);
}


// Author & Date:   Kirk Korver     Mar 27, 2003
// Purpose: Equals operator, copy from the RightHandSide to us
// Inputs:
//  rhs - the right hand side of the operation
NsLibrary & NsLibrary::operator = (const NsLibrary & rhs)
{
    this->m_pcLibraryImp = rhs.m_pcLibraryImp;
    NsLibraryImpMgr::GetMgr().IncrementReferenceCount(m_pcLibraryImp);
    return *this;
}

// Author & Date:   Kirk Korver     Feb 25, 2003
// Purpose: Cause the reference count of the IMP class to be reduced.
// If the number is low enough, then the IMP class will be deleted
NsLibrary::~NsLibrary()
{
    Release();
}



// Author & Date:   Kirk Korver     Feb 28, 2003
// Purpose: Release the current LibraryImp
void NsLibrary::Release()
{
    NsLibraryImpMgr::GetMgr().ReleaseLibrary(m_pcLibraryImp);
}



// Author & Date:   Kirk Korver     Feb 28, 2003
// Purpose: Replace the libraryImp with this one.
// Inputs: 
//  pcLibraryImp - the new library imp
void NsLibrary::SetLibraryImp(NsLibraryImp const * pcLibraryImp)
{
    Release();   // cleanup the old one
    m_pcLibraryImp = pcLibraryImp;
}


// Author & Date:   Kirk Korver     09 May 2003
// Purpose: Open this data file
// Inputs:
//  pszFilename - the name of the data file to open
//  pszDllName - the name of the DLL to use to open this data file
//               when this parameter is NULL then we use the data file extension
//               to find the appropriate DLL
// Outputs:
//  hFile - handle to the file after it is open
//  returns ns_OK if all is well
ns_RESULT NsLibrary::OpenFile(const char *pszFilename, uint32 *hFile, const char * pszDllName /* = NULL */)
{
    NsLibraryImp * pcLib;

    if (pszDllName)
        pcLib = NsLibraryImpMgr::GetMgr().AddLibraryToSystem(pszDllName);
    else
        pcLib = NsLibraryImpMgr::GetMgr().GetLibraryFromDataFileName(pszFilename);

    if (pcLib)
    {
        SetLibraryImp(pcLib);
        return m_pcLibraryImp->OpenFile(pszFilename, hFile);
    }
    else
    {
        // Clear out the old one
        SetLibraryImp(NULL);

        return ns_LIBERROR;
    }
}



  


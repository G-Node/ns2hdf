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
// $Workfile: NsFile.cpp $
//
// Description   : implementation of the NeuroshareLibrary class.
//
// Authors       : Kirk Korver
//
// $Date: 2005/03/16 16:54:31 $
//
// $History: NsFile.cpp $
// 
// *****************  Version 12  *****************
// User: Kkorver      Date: 5/19/04    Time: 1:39p
// Updated in $/Neuroshare/PowerNAP
// 
// *****************  Version 11  *****************
// User: Kkorver      Date: 5/12/03    Time: 11:14a
// Updated in $/Neuroshare/nsClassifier
// Added OpenFileUsingThisDLL()
// 
// *****************  Version 10  *****************
// User: Kirk         Date: 3/27/03    Time: 1:54p
// Updated in $/Neuroshare/nsClassifier
// Mods to allow multiple data files open at same time
// 
// *****************  Version 9  *****************
// User: Kirk         Date: 3/04/03    Time: 2:25p
// Updated in $/Neuroshare/VC Source Code Example
// Fixed typo in the comments
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
// User: Kirk         Date: 2/24/03    Time: 3:25p
// Updated in $/Neuroshare/VC Source Code Example
// Removed fixed directory for the neuroshare library
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
// Added Required Headers
//
///////////////////////////////////////////////////////////////////////////////////////////////////

// #include "StdAfx.h"
#include "NsFile.h"
#include "NsLibraryImpMgr.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
NsFile::NsFile()
{
    m_hDataFile = INVALID_HANDLE;
}


NsFile::~NsFile()
{
    m_hDataFile = INVALID_HANDLE;
}

// Author & Date:       Kirk Korver     09 May 2003
// Purpose: Open a data file while explicitely naming the DLL to use
// Inputs:
//  szDataPathName - the fully qualified path name of the data file to open
//  szDllPathName - the fully qualified path name of the DLL to use to interpret this data file
// Outputs:
//  ns_OK if the file opened properly
ns_RESULT NsFile::OpenFileUsingThisDLL(LPCSTR szDataName, LPCSTR szDllPathName)
{
    return m_icLibrary.OpenFile(szDataName, &m_hDataFile, szDllPathName);
}


// Author & Date:   Kirk Korver     08 Jan 2003
// Purpose: Close the data file
ns_RESULT NsFile::CloseFile()
{ 
    if (m_hDataFile == INVALID_HANDLE)
        return ns_OK;

    // Otherwise, close it
    ns_RESULT retVal = m_icLibrary.CloseFile(m_hDataFile); 
    m_hDataFile = INVALID_HANDLE; 
    return retVal;  
}






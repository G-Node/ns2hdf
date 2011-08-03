///////////////////////////////////////////////////////////////////////////////////////////////////
//
// Copyright (C) 2005 Neuroshare Project                                                         
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
//  Cyberkinetics, Inc.,
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
// $Author: kirkkorver $
// $Date: 2005/05/18 19:17:49 $
// $Revision: 1.4 $
// $Source: /cvsroot/neuroshare/Suite/Common/Data.cpp,v $
//
///////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Data.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// Inputs:
//  dwIdxFile - the index of the file in the document's file vector
//  dwIndex - the index (or 1st index if a collection) that corresponds to this data
Data::Data(FileInfo * pcFile, DWORD dwIndex) :
    m_pcFile(pcFile),
    m_dwIndex(dwIndex)
{
}

Data::~Data()
{

}

//////////////////////////////////////////////////////////////////////
// Functions
//////////////////////////////////////////////////////////////////////


// Author & Date:   Kirk Korver     24 Feb 2004
// Purpose: find the time of a particular index here
// Inputs:
//  dwEntityID - the global entityID we are interested in
//  dwIndex - which index do we want to find out about?
// Ouputs
//  the time (in seconds) at which this index occurs
double Data::GetTimeByIndex(DWORD dwEntityID, DWORD dwIndex) const
{
    double dTimeStamp = 0;
	ns_RESULT ret = GetFile().GetTimeByIndex(dwEntityID, dwIndex, &dTimeStamp);
	//ASSERT(ret == ns_OK);
    return dTimeStamp;
}

// Author & Date:   Kirk Korver     25 Feb 2004
// Purpose: Get the file class. Needed for many fxns.
NsFile & Data::GetFile() const
{
    return m_pcFile->icFile;
}

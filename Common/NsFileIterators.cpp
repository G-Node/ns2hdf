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
// $Workfile: NsFileIterators.cpp $
//
// Description   : implementation of all of the NsFileIterators
//
// Authors       : Kirk Korver
//
// $Date: 2005/03/16 16:54:31 $
//
// $History: NsFileIterators.cpp $
// 
// *****************  Version 4  *****************
// User: Kkorver      Date: 5/19/04    Time: 1:39p
// Updated in $/Neuroshare/PowerNAP
// 
// *****************  Version 3  *****************
// User: Kirk         Date: 3/25/03    Time: 2:11p
// Updated in $/Neuroshare/nsClassifier
// Rename the NsFile iterators
// 
// *****************  Version 2  *****************
// User: Kirk         Date: 3/24/03    Time: 11:47a
// Updated in $/Neuroshare/nsClassifier
// EntityInfoByTypeIterator
// 
// *****************  Version 1  *****************
// User: Kirk         Date: 3/19/03    Time: 3:52p
// Created in $/Neuroshare/Data Editor
// First implementation of the EntityInfoIterator
// 
// $NoKeywords: $
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "NsFileIterators.h"

#include "nsAPItypes.h"
#include "NsFile.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////////////////////////

// Author & Date:   Kirk Korver     3/19/2003
// Purpose: 
// Inputs:
//  rcFile - the file that we are reading from
NsFileEntityInfoIterator::NsFileEntityInfoIterator(NsFile const & rcFile) :
    m_nIndex(0),
    m_nNumOfEntities(0),
    m_rcFile(rcFile)
{
    ns_FILEINFO Info;
    rcFile.GetFileInfo(Info);
    m_nNumOfEntities = Info.dwEntityCount;
}


// Author & Date:   Kirk Korver     3/19/2003
// Purpose: advance to the next in the list
void NsFileEntityInfoIterator::operator ++ ()
{
    ++m_nIndex;
}

// Author & Date:   Kirk Korver     3/19/2003
// Purpose: Parenthesis operator: return the current value
ns_ENTITYINFO const & NsFileEntityInfoIterator::operator () ()
{
    m_rcFile.GetEntityInfo(m_nIndex, m_isInfo);
    return m_isInfo;
}

// Author & Date:   Kirk Korver     3/19/2003
// Purpose: typecast operator: are we done yet?
// Outputs:
//  0 if we are at the end; NONZERO if there are still more
NsFileEntityInfoIterator::operator const void * () const
{
    return m_nIndex < m_nNumOfEntities ? (void *)1 : (void *)0;
}


/////////////////////////////////////////////////////////////////////////////////////////////////



// Author & Date:   Kirk Korver     Mar 24, 2003
// Purpose: constructor, save the file and the type
NsFileEntityInfoByTypeIterator::NsFileEntityInfoByTypeIterator(NsFile const & rcFile, DWORD dwType) :
    m_dwType(dwType),
    m_it(rcFile)
{
    FindFirstMatch();
}



// Advance to next
void NsFileEntityInfoByTypeIterator::operator ++ ()
{
    ++m_it; // Move of the current match
    FindFirstMatch();
}

// Parenthesis operator: return the current value
ns_ENTITYINFO const & NsFileEntityInfoByTypeIterator::operator () ()
{
    return m_it();
}

// typecast operator: are we done yet? 0 = we are done
NsFileEntityInfoByTypeIterator::operator const void * () const
{
    return m_it;
}

// Advance to the next of the type we want
void NsFileEntityInfoByTypeIterator::FindFirstMatch()
{
    for (; m_it; ++m_it)
    {
        if (m_it().dwEntityType == this->m_dwType)
        {
            return;
        }
    }
}


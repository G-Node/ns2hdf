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
// $Workfile: NsFileIterators.h $
//
// Description   : definition of all of the NsFileIterators
//
// Authors       : Kirk Korver
//
// $Date: 2005/03/16 16:54:31 $
//
// $History: NsFileIterators.h $
// 
// *****************  Version 5  *****************
// User: Kkorver      Date: 5/19/04    Time: 1:39p
// Updated in $/Neuroshare/PowerNAP
// 
// *****************  Version 4  *****************
// User: Kirk         Date: 3/27/03    Time: 1:49p
// Updated in $/Neuroshare/nsClassifier
// Added GetCurrentIndex() to the iterators
// 
// *****************  Version 3  *****************
// User: Kirk         Date: 3/25/03    Time: 2:11p
// Updated in $/Neuroshare/nsClassifier
// Rename the NsFile iterators
// 
// *****************  Version 2  *****************
// User: Kirk         Date: 3/24/03    Time: 11:47a
// Updated in $/Neuroshare/nsClassifier
// Added EntityInfoByTypeIterator
// 
// *****************  Version 1  *****************
// User: Kirk         Date: 3/19/03    Time: 3:53p
// Created in $/Neuroshare/Data Editor
// First implementation of the EntityInfoIterator
// 
// $NoKeywords: $
//
//////////////////////////////////////////////////////////////////////

#ifndef NSFILEITERATORS_H_INCLUDED
#define NSFILEITERATORS_H_INCLUDED

#include "nsAPItypes.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000



class NsFile;   // forward declaration to avoid include

class NsFileEntityInfoIterator
{
public:
    NsFileEntityInfoIterator(NsFile const & rcFile);

    void operator ++ ();                    // Advance to next
    ns_ENTITYINFO const & operator () ();   // Parenthesis operator: return the current value
    operator const void * () const;         // typecast operator: are we done yet? 0 = we are done

    DWORD GetCurrentIndex() { return m_nIndex; }

private:
    // Not implemented
    NsFileEntityInfoIterator(NsFileEntityInfoIterator const &);
    NsFileEntityInfoIterator & operator = (NsFileEntityInfoIterator const &);

    DWORD m_nIndex;             // our current position
    DWORD m_nNumOfEntities;     // the number of entites

    NsFile const & m_rcFile;    // The file that we are reading
    ns_ENTITYINFO m_isInfo;     // Storage for the info
};



class NsFileEntityInfoByTypeIterator
{
public:
    NsFileEntityInfoByTypeIterator(NsFile const & rcFile, DWORD dwType);

    void operator ++ ();                    // Advance to next
    ns_ENTITYINFO const & operator () ();   // Parenthesis operator: return the current value
    operator const void * () const;         // typecast operator: are we done yet? 0 = we are done

    DWORD GetCurrentIndex() { return m_it.GetCurrentIndex(); }

private:
    // Not implemented
    NsFileEntityInfoByTypeIterator(NsFileEntityInfoByTypeIterator const &);
    NsFileEntityInfoByTypeIterator & operator = (NsFileEntityInfoByTypeIterator const &);

    DWORD const m_dwType;       // The type we are looking for

    NsFileEntityInfoIterator m_it;    // The internal iterator we will use

    void FindFirstMatch();      // Advance to the first of the type we want
};





#endif 

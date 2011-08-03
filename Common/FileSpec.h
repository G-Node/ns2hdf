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
// $Workfile: FileSpec.h $
//
// Description   : interface for the CFileSpec class
//
// Authors       : Kirk Korver, really this was copied from MSDN at
//                 mk:@MSITStore:C:\Program%20Files\Microsoft%20Visual%20Studio\MSDN98\98VSa\1033\period96.chm::/periodic/msj/F1/D8/S1DCC.htm
//                 The original author was Paul DiLascia
//
// $Date: 2005/03/16 16:54:31 $
//
// $History: FileSpec.h $
// 
// *****************  Version 2  *****************
// User: Kkorver      Date: 5/19/04    Time: 1:39p
// Updated in $/Neuroshare/PowerNAP
// 
// *****************  Version 1  *****************
// User: Kirk         Date: 3/03/03    Time: 12:19p
// Created in $/Neuroshare/VC Source Code Example
// This is the CFileSpec class that will allow for the searching of a
// directory for a list of files matching a wildcard
// 
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef FILESPEC_H_INCLUDED
#define FILESPEC_H_INCLUDED

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <io.h>


// This is how to use this class
//
// CFileSpec spec;
// for (bool more = spec.First("*.*"); more; more=spec.Next() ) 
// {
//     if( !(spec.attrib & (_A_SUBDIR | _A_HIDDEN | _A_SYSTEM)) )
//     {
//         ... do something ...
//     }
// }




//////////////////
// Class to to navigate files that match a spec with wildcards
//
class CFileSpec : public _finddata_t 
{
    intptr_t m_hfile;            // handle from findfirst
    
public:
    CFileSpec() 
    {
        m_hfile = -1;
    }
    
    ~CFileSpec() {
        if (m_hfile>=0)
            _findclose(m_hfile);
    }
    
    bool First(const char* filespec) 
    {
        if (m_hfile>=0)
            _findclose(m_hfile);
        return (m_hfile=_findfirst((char*)filespec, this)) != -1;
    }
    
    bool Next() 
    {
        return m_hfile>=0 && _findnext(m_hfile, this)==0;
    }
};




#endif // FILESPEC_H_INCLUDED
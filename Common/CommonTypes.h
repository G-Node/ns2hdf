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
// $Workfile: CommonTypes.h $
//
// Authors       : Kirk Korver
//
// $Date: 2005/03/16 16:54:30 $
//
// $History: CommonTypes.h $
// 
// *****************  Version 2  *****************
// User: Kkorver      Date: 5/19/04    Time: 1:38p
// Updated in $/Neuroshare/PowerNAP
// 
// *****************  Version 1  *****************
// User: Kkorver      Date: 10/17/03   Time: 11:51a
// Created in $/Neuroshare/nsClassifier
// Moved this data type to its own file as so many people had defined it
// 
// $NoKeywords: $
//
//////////////////////////////////////////////////////////////////////

#ifndef COMMONTYPES_H_INCLUDED
#define COMMONTYPES_H_INCLUDED

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <vector>


//////////////////////////////////////////////////////////////////////
// Really common data types
typedef std::vector <double> WAVEFORM;


#endif  // include guards
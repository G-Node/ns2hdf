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
//
// $Workfile: ns_common.h $
//
// Description   : 
//
// Authors       : Kirk Korver
//
// $Date: 2005/03/16 16:54:31 $
//
// $History: ns_common.h $
// 
// *****************  Version 5  *****************
// User: Kkorver      Date: 6/07/04    Time: 2:12p
// Updated in $/Neuroshare/PowerNAP
// Anomaly #45
// Added   for_all   function which is for_each applied to ALL entries
// 
// *****************  Version 4  *****************
// User: Kkorver      Date: 5/19/04    Time: 1:39p
// Updated in $/Neuroshare/PowerNAP
// 
// *****************  Version 3  *****************
// User: Kkorver      Date: 10/24/03   Time: 10:42a
// Updated in $/Neuroshare/PowerNAP
// Added ARRAY_END() macro
// 
// *****************  Version 2  *****************
// User: Kkorver      Date: 10/21/03   Time: 4:31p
// Updated in $/Neuroshare/PowerNAP
// Moved ReplaceWindowControl() to the common area
// 
// *****************  Version 1  *****************
// User: Kkorver      Date: 10/17/03   Time: 9:47a
// Created in $/Neuroshare/nsClassifier
// useful utilites that don't have a better home
// 
// $NoKeywords: $
//
//////////////////////////////////////////////////////////////////////

#ifndef NS_COMMON_H_INCLUDED
#define NS_COMMON_H_INCLUDED

// Find out the size of an array
#define ARRAY_SIZE(x) (sizeof(x)/sizeof(x[0]))

// Use this for STL array functions..ONLY IF A STATIC ARRAY
// e.g.  std::max_element(x, ARRAY_END(x));
#define ARRAY_END(x) (x + ARRAY_SIZE(x))


// Simple macro that allows me to have debug-only code, that is compiler independant
#ifdef _DEBUG
    // Ok.. We want debugging output
    #define DEBUG_CODE(X)   X
#else
    #define DEBUG_CODE(X)
#endif


// Purpose: Ensure that a number is between 2 other numbers ( > or <, not =)
// Inputs:  nVal - the value we need to clip
//          nMaxValue - the maximum possible value (aka biggest positive number)
//          nMinValue - the minimum possible value (aka smallest negative number)
// Outputs: the new value of nVal adjusted so it fits within the range
template <class T>
inline T clip(T nValToClip, T nMinValue, T nMaxValue)
{
    if (nValToClip > nMaxValue)
        return nMaxValue;

    if (nValToClip < nMinValue)
        return nMinValue;

    return nValToClip;
}

// Purpose: Replace this static control with this window
// Inputs:
//  rParent - the window containing this control
//  rcNewWindow - the window that is to be created
//  nControlID - the ID of the control (from the resource editor)
// void ReplaceWindowControl(CWnd & rParent, CWnd & rcNewWindow, int nControlID);

namespace std
{
    // Author & Date:   Kirk Korver     03 Jun 2004
    // Purpose: wrap the std::for_each function to apply the functor to ALL entries
    // Usage:
    //  vector<int> v(10);
    //  int Double(int x) { return x+x; }
    //  for_all(v, Double);
    // Inputs:
    //  c - the container we care about
    //  f - the function or functor we want to apply
    template <class Container, typename Functor> inline
    Functor for_all(Container  & c, Functor & f)
    {
        return std::for_each(c.begin(), c.end(), f);
    }
};


#endif      // include guard
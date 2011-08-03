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
//  kkorver at cyberkineticsinc.com
//
// Website:
//  sourceforge.net/projects/neuroshare
//
// All other copyrights on this material are replaced by this license agreeement.
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
// $Author: acmesucks $
// $Date: 2005/03/16 16:54:30 $
// $Revision: 1.2 $
// $Source: /cvsroot/neuroshare/Suite/Common/ConstantsPNap.h,v $
//
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef CONSTANTSPNAP_H_INCLUDED
#define CONSTANTSPNAP_H_INCLUDED

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


// User Messages
enum tagUSERMESSAGES
{
    WM_USER_UPDATEUNITS = WM_USER + 1,  // update unit colors
    WM_USER_SELECTWF,                    // waveform is selected/highlight it
    WM_USER_UNSELECTWF,                  // waveform is unselected
    WM_USER_UPDATETEMPLATE              // new template
};

// User Hints
enum tagUSERHINTS
{
    HINT_USER_NONE,         // No hint
    HINT_USER_NEWDATA,      // new data
    HINT_USER_ANIMATING,    // the "animating" data has changed
};

// Masks for unit ids
enum UnitMasks
{
    UNIT_UNCLASS_MASK   = 0x0000,   // mask for unclassified units
    UNIT_NOISE_MASK     = 0x0001,   // mask for noise units
    UNIT_1_MASK         = 0x0002,   // mask for unit 1
    UNIT_2_MASK         = 0x0004,   // mask for unit 2
    UNIT_3_MASK         = 0x0008,   // mask for unit 3
    UNIT_4_MASK         = 0x0010,   // mask for unit 4
    UNIT_5_MASK         = 0x0020,   // mask for unit 5
    UNIT_6_MASK         = 0x0040,   // mask for unit 6
    UNIT_7_MASK         = 0x0080,   // mask for unit 7
    UNIT_8_MASK         = 0x0100,   // mask for unit 8
    UNIT_9_MASK         = 0x0200,   // mask for unit 9
    UNIT_10_MASK        = 0x0400,   // mask for unit 10
    UNIT_NOISE_OUT_MASK = 0x0800,   // mask for noise out units
    CONTINUOUS_MASK     = 0x1000,   // mask for continuous channel

    UNIT_ALL_MASK = UNIT_UNCLASS_MASK |
                    UNIT_NOISE_MASK |
                    UNIT_1_MASK |
                    UNIT_2_MASK |
                    UNIT_3_MASK |
                    UNIT_4_MASK |
                    UNIT_5_MASK |
                    UNIT_6_MASK |
                    UNIT_7_MASK |
                    UNIT_8_MASK |
                    UNIT_9_MASK |
                    UNIT_10_MASK |
                    UNIT_NOISE_OUT_MASK |
                    CONTINUOUS_MASK,
};

// Indeces into arrays
enum
{
    UNCLASS = 0,
    UNIT1,
    UNIT2,
    UNIT3,
    UNIT4,
    UNIT5,
    UNIT6,
    UNIT7,
    UNIT8,
    UNIT9,
    UNIT10,
    NOISE,
    NOISE_OUT,
    UNITCOUNT,
};


//////////////////////////////////////////////////////////////////////////////


// THIS IS THE MASTER LIST FOR ALL OF THE TEXTS. 
//
// If you want a sub-set of this list, create a new one below
enum
{
    UL_ALL,
    UL_UNCLASS,
    UL_UNIT1,
    UL_UNIT2,
    UL_UNIT3,
    UL_UNIT4,
    UL_UNIT5,
    UL_UNIT6,
    UL_UNIT7,
    UL_UNIT8,
    UL_UNIT9,
    UL_UNIT10,
    UL_NOISE,
    UL_NOISE_OUT,
    UL_COUNT,
};

static const char * UNITLABEL[UL_COUNT] = { "All",
                                            "Unclassified",
                                            "Unit 1",
                                            "Unit 2",
                                            "Unit 3",
                                            "Unit 4",
                                            "Unit 5",
                                            "Unit 6",
                                            "Unit 7",
                                            "Unit 8",
                                            "Unit 9",
                                            "Unit 10",
                                            "Noise",
                                            "Noise Out" };

//////////////////////////////////////////////////////////////////////////////

// Used to show all of the available "units" in the waveform dropdown list
enum 
{
    WL_ALL,
    WL_UNCLASS,
    WL_UNIT1,
    WL_UNIT2,
    WL_UNIT3,
    WL_UNIT4,
    WL_UNIT5,
    WL_UNIT6,
    WL_UNIT7,
    WL_UNIT8,
    WL_UNIT9,
    WL_UNIT10,
    WL_NOISE,
    WL_COUNT,
};
static const char * WU_LABEL[WL_COUNT] =    // this is odd, but it ensures the same wording
{
    UNITLABEL[UL_ALL],
    UNITLABEL[UL_UNCLASS],
    UNITLABEL[UL_UNIT1],
    UNITLABEL[UL_UNIT2],
    UNITLABEL[UL_UNIT3],
    UNITLABEL[UL_UNIT4],
    UNITLABEL[UL_UNIT5],
    UNITLABEL[UL_UNIT6],
    UNITLABEL[UL_UNIT7],
    UNITLABEL[UL_UNIT8],
    UNITLABEL[UL_UNIT9],
    UNITLABEL[UL_UNIT10],
    UNITLABEL[UL_NOISE],
};

//////////////////////////////////////////////////////////////////////////////

// Used for the property page manual sorting
enum 
{
    MS_UNIT1,
    MS_UNIT2,
    MS_UNIT3,
    MS_UNIT4,
    MS_UNIT5,
    MS_UNIT6,
    MS_UNIT7,
    MS_UNIT8,
    MS_UNIT9,
    MS_UNIT10,
    MS_NOISE,
    MS_NOISE_OUT,
    MS_COUNT,
};
static const char * MU_LABEL[MS_COUNT] = 
{
    UNITLABEL[UL_UNIT1],
    UNITLABEL[UL_UNIT2],
    UNITLABEL[UL_UNIT3],
    UNITLABEL[UL_UNIT4],
    UNITLABEL[UL_UNIT5],
    UNITLABEL[UL_UNIT6],
    UNITLABEL[UL_UNIT7],
    UNITLABEL[UL_UNIT8],
    UNITLABEL[UL_UNIT9],
    UNITLABEL[UL_UNIT10],
    UNITLABEL[UL_NOISE],
    UNITLABEL[UL_NOISE_OUT],
};

//////////////////////////////////////////////////////////////////////////////

typedef struct {
	COLORREF dispback;
	COLORREF dispgridmin;
	COLORREF disptext;
	COLORREF dispwave;
	COLORREF dispunit[17];  // 0 = unclassified
    COLORREF dispchansel[3];
} COLORTABLE;


/////////////////////////////////////////////////////////////////////////////
// Define global unit colors
const COLORREF	LIGHT_GREY =    RGB(192, 192, 192);
const COLORREF	MEDIUM_GREY =   RGB(160, 160, 160);
const COLORREF	DARK_GREY = 	RGB(48, 48, 48);
const COLORREF  MAGENTA = 		RGB(255, 100, 153);
const COLORREF  CYAN = 			RGB(0, 255, 255);
const COLORREF  YELLOW = 		RGB(255, 255, 0);
const COLORREF  PURPLE = 		RGB(200, 0, 255);
const COLORREF  GREEN = 		RGB(0, 255, 0);
const COLORREF  BLUE = 			RGB(0, 0, 255);
const COLORREF  RED = 			RGB(255, 0, 0);
const COLORREF  DARK_GREEN = 	RGB(57, 181, 74);
const COLORREF  ORANGE =		RGB(247, 148, 29);
const COLORREF  LT_BROWN =      RGB(166, 124, 92);
const COLORREF  BLACK=          RGB(0, 0, 0);
const COLORREF  WHITE =         RGB(254, 254, 254);
const COLORREF  LT_GREEN =      RGB(0,128,0); // used for grid


#endif // include guards

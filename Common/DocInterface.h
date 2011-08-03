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

#ifndef DOCINTERFACE_H_INCLUDED
#define DOCINTERFACE_H_INCLUDED

#include "DataSegment.h"
#include "DataEvent.h"
#include "DataAnalog.h"
#include "DataRamSegment.h"

// Forward Declares to avoid includes
class NsFile;
class PlayBack;

enum DocInterface
{
    MyUnknown,          // Give me an "unknown" interface
    DocInterface1,      // Give me a IDocInterface1
};

struct IMyUnknown
{
    // The only purpose of this interface, is to allow a way to ask for others.
    virtual void * QueryInterface(DocInterface enType) = 0;
};

struct EntityInfo
{
    DWORD dwEntityIndex;            // The index of this entity in the file
    ns_ENTITYINFO isEntityInfo;     // Information about the current entity

    //Flags for sorting information
    bool m_bRulesDefined;

    // Comparison operators
    bool operator == (EntityInfo const & rhs);
    bool operator != (EntityInfo const & rhs);
};


struct IDocInterface1
{
    virtual bool IsActiveSegment() = 0;
    virtual bool IsActiveAnalog() = 0;

    virtual NsFile * GetNsFile() = 0;
    virtual SEGMENTENTITYLIST & GetSegmentList() = 0;
	virtual EVENTENTITYLIST & GetEventList() = 0;
	virtual ANALOGENTITYLIST & GetAnalogList() = 0;
    virtual EntityInfo & GetEntity(int32 dwEntityID) = 0;   // Get the Entity associated with this entity ID
    virtual Segments & GetActiveSegment() = 0;              // Get the "segment" that is currently selected
    virtual Segments & GetSegment(int32 dwEntityID) = 0;    // Get this "segment"
    virtual Analogs & GetActiveAnalog() = 0;                // Get the active segment when it is an ANALOG
    virtual Analogs & GetAnalog(int32 dwEntityID) = 0;      // Get this "analog"
    virtual Events & GetActiveEvent() = 0;                  // Get the active segment when it is an ANALOG
    virtual Events & GetEvent(int32 dwEntityID) = 0;        // Get this "event"
    // 
    virtual void AnalogInfoByID(int32 dwEntityID, ns_ANALOGINFO &rinfo) = 0 ; 

	virtual void AddRamSegment(std::vector<DataRamSegment> vSegs, ns_SEGMENTINFO segInfo, ns_SEGSOURCEINFO srcInfo) = 0;

    virtual int32 GetActiveEntity() = 0;
    
	virtual PlayBack & GetPlayBack() = 0;
	
    virtual int GetBITUnitColor(uint32 dwUnitID) = 0;
    virtual CPen * GetBITUnitPen(uint32 dwUnitID) = 0;
	virtual COLORTABLE & GetColorTable() = 0;
    virtual CBrush & GetBackgroundBrush() = 0;
 
    // Utilities

    virtual void ShowDlgPlayback() = 0;
    virtual double GetMaxTime() = 0;
    virtual CString & GetAppPath() = 0;                     // Returns PowerNAP Directory. Data Files located here
};

#endif
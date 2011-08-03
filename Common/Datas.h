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
// $Date: 2005/05/18 19:17:50 $
// $Revision: 1.12 $
// $Source: /cvsroot/neuroshare/Suite/Common/Datas.h,v $
//
///////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef DATAS_H_INCLUDED
#define DATAS_H_INCLUDED

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <vector>

#include "nsAPItypes.h"
#include "ConstantsPNap.h"       // needed for UnitMasks
#include "DataSegment.h"
#include "DataAnalog.h"
#include "DataEvent.h"
#include "CommonTypes.h"

class DataSegment;
class DataAnalog;
class DataEvent;

// SP means SharedPointer
typedef boost::shared_ptr<DataSegment> SPDataSegment;
typedef boost::shared_ptr<DataAnalog> SPDataAnalog;
typedef boost::shared_ptr<DataEvent> SPDataEvent;

typedef std::vector <ns_SEGSOURCEINFO> VECTORSEGSOURCEINFO;
typedef std::vector <SPDataSegment> SEGMENTLIST;
typedef std::vector <SPDataAnalog> ANALOGLIST;
typedef std::vector <SPDataEvent>  EVENTLIST;

// There are only 3 choices for GetIndexByTime. Here is the list.
enum GIBT
{
    GIBT_BEFORE = ns_BEFORE,
    GIBT_CLOSEST = ns_CLOSEST,
    GIBT_AFTER = ns_AFTER,
};

class Datas
{
public:
    Datas(DWORD dwEntityID, NsFile isFile) : m_dwEntityID(dwEntityID), m_isFile(isFile) {}

    DWORD GetEntityID() const { return m_dwEntityID; }      // Get the Entity ID
    CString GetLabel() const;                               // Retrieve the entity label
	NsFile const & GetFile() const { return m_isFile; }

    // There are 3 flavors: closest, try using flag, but always give a valid number, and raw
    //
    // Choose carefully
    uint32 GetIndexByTime(double dTime) const;              // find the time closest to this
    uint32 GetIndexByTime(double dTime, GIBT nFlag) const; // Start with flag, if it fails, then use closest
    virtual ns_RESULT GetIndexByTime(double dTime, GIBT nFlag, uint32 & dwIndex) const = 0;    // most control

    // This templage function may help you do your searching if you have the right functions 
    // Defined. Look at the segments for guidance.
    template <class T>
    ns_RESULT GetIndexByTime(T & rVector, double dTime, GIBT nFlag, uint32 & nIndex) const; 


private:
    DWORD m_dwEntityID;
	NsFile m_isFile;
};

class Segments
{
public:
    SEGMENTLIST::iterator begin() { return m_vSegments.begin(); }
    SEGMENTLIST::const_iterator begin() const { return m_vSegments.begin(); }

    SEGMENTLIST::iterator end()   { return m_vSegments.end();   }
    SEGMENTLIST::const_iterator end() const  { return m_vSegments.end();   }

    DataSegment & operator [] (DWORD nIndex) { return *m_vSegments[nIndex]; }
    const DataSegment & operator [] (DWORD nIndex) const { return *m_vSegments[nIndex]; }

	void push_back(DataSegment *seg);
    void clear() { m_vSegments.clear(); }
    bool empty() const { return m_vSegments.empty(); } 
    size_t size() const { return m_vSegments.size(); }

    DataSegment & back() { return *(m_vSegments.back()); }
    const DataSegment & back() const { return *(m_vSegments.back()); }

    SEGMENTLIST::iterator erase(SEGMENTLIST::iterator it) { return m_vSegments.erase(it); }

    SEGMENTLIST & GetList() { return m_vSegments; }
    const SEGMENTLIST & GetList() const { return m_vSegments; }

    virtual void GetInfo(ns_SEGMENTINFO & rInfo) const = 0;         // Get the "segment" info
    virtual void GetInfo(VECTORSEGSOURCEINFO & rvInfo) const = 0;   // Get info about the "sources" of each segment

    void DeleteTheseUnits(UnitMasks enUnit);            // Delete every segment with this unit
    bool CompactUnits();    // remove "holes" in the unit classifications; TRUE means data changed; FALSE, not
    void ReclassifyUnits(UnitMasks enFrom, UnitMasks enTo); // reclassify the "from" units as the "to" units
    void DeleteTimeSpan(double dTimeStart, double dTimeStop);   // delete every segment between these two times

	// These are needed since SegmentsRam isn't a child of Datas like SegmentsFile.
	virtual DWORD GetEntityID() const = 0;
    virtual CString GetLabel() const = 0;
	virtual uint32 GetIndexByTime(double dTime, GIBT nFlag) const = 0;
protected:
    SEGMENTLIST m_vSegments;
};

class SegmentsFile : public Segments, Datas
{
public:
    SegmentsFile(DWORD dwEntityID, NsFile crFile) : Datas(dwEntityID, crFile) {}

    virtual void GetInfo(ns_SEGMENTINFO & rInfo) const;         // Get the "segment" info
    virtual void GetInfo(VECTORSEGSOURCEINFO & rvInfo) const;   // Get info about the "sources" of each segment

	virtual DWORD GetEntityID() const;      // Get the Entity ID
    virtual CString GetLabel() const;
	virtual uint32 GetIndexByTime(double dTime, GIBT nFlag) const;

    virtual ns_RESULT GetIndexByTime(double dTime, GIBT nFlag, uint32 & dwIndex) const;    // most control
};

class Analogs : public Datas
{
public:
    Analogs(DWORD dwEntityID, NsFile isFile) : Datas(dwEntityID, isFile) {}


    ANALOGLIST::iterator begin() { return m_vAnalogs.begin(); }
    ANALOGLIST::iterator end()   { return m_vAnalogs.end();   }
    DataAnalog & operator [] (DWORD nIndex) { return *m_vAnalogs[nIndex]; }
    void push_back(DataAnalog *seg);
    void clear() { m_vAnalogs.clear(); }
    bool empty() const { return m_vAnalogs.empty(); } 
    size_t size() const { return m_vAnalogs.size(); }
    DataAnalog & back() { return *(m_vAnalogs.back()); }
    const DataAnalog & back() const { return *(m_vAnalogs.back()); }
    ANALOGLIST::iterator erase(ANALOGLIST::iterator it) { return m_vAnalogs.erase(it); }

    ANALOGLIST & GetList() { return m_vAnalogs; }

    virtual ns_RESULT GetIndexByTime(double dTime, GIBT nFlag, uint32 & dwIndex) const;    // most control

    void GetInfo(ns_ANALOGINFO & rInfo) const;
    double GetSampleRate() const;               // Get the sampling rate in Hz


protected:
    ANALOGLIST m_vAnalogs;
};

// This class is a collection of all of the events of a single entity
class Events : public Datas
{
public:
    // Inputs:
    //  szLabel - the label of this entity
    //  dwEntityID - the EntityID of this entry in the file
    //  isFile - the file this entity belongs to
    Events(LPCSTR szLabel, DWORD dwEntityID, NsFile isFile) : 
      Datas(dwEntityID, isFile),
      m_strLabel(szLabel)
      {}

    void GetInfo(ns_EVENTINFO & rInfo) const;
#pragma message("This is bad programming. Fix as soon as Chris' work is done\n")
    CString GetLabel() const;                               // Retrieve the entity label

    void push_back(DataEvent *event);
    void clear() { m_vEvents.clear(); }
    bool empty() const { return m_vEvents.empty(); }
    void reserve(DWORD nCount) { m_vEvents.reserve(nCount); }
    size_t size() const { return m_vEvents.size(); }
    EVENTLIST::iterator begin() { return m_vEvents.begin(); }
    EVENTLIST::iterator end() { return m_vEvents.end(); }

    virtual ns_RESULT GetIndexByTime(double dTime, GIBT nFlag, uint32 & dwIndex) const;    // most control

//protected:
    EVENTLIST m_vEvents;        // The list of events. One for each index
protected:
#pragma message("Really the entity info should be stored. But I'm waiting for Chris' work to finish first\n")
    CString m_strLabel;         // This is needed to store the "label".
};


#endif // include guards

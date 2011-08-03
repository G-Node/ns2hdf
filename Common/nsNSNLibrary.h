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
// $Author: kirkkorver $
// $Date: 2005/06/17 20:23:56 $
// $Revision: 1.4 $
// $Source: /cvsroot/neuroshare/Suite/Common/nsNSNLibrary.h,v $
//
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef NSNSNLIBRARY_H_INCLUDED       // include guards
#define NSNSNLIBRARY_H_INCLUDED

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <windows.h>
#include "NsAPITypes.h"




enum InterfaceTypes
{
    IUNKNOWN,
    IWRITE_NSN,     // matches up with IWriteNSN   ***** DEPRECATED **** use IWriteNSN2
    IWRITE_NSN2,    // the second version: See IWriteNSN2
};

struct IUnKnown
{
    virtual void AddRef() = 0;
    virtual void Release() = 0;
};




enum NsnTags
{
    NSN_ENTITY_UNKNOWN      = ns_ENTITY_UNKNOWN,
    NSN_ENTITY_EVENT        = ns_ENTITY_EVENT,
    NSN_ENTITY_ANALOG       = ns_ENTITY_ANALOG,
    NSN_ENTITY_SEGMENT      = ns_ENTITY_SEGMENT,
    NSN_ENTITY_NEURALEVENT  = ns_ENTITY_NEURALEVENT,
    NSN_INFO_FILE           = 5,  // File information
};

typedef struct
{
	double dTimeStampResolution;   // Minimum timestamp resolution
	char   szAppName[64];          // Name of the application that created the file
	char   szFileComment[256];	   // Comments embedded in the source file
} ReducedFileInfo;


struct IWriteNSN : public IUnKnown
{
    virtual bool OpenFile(LPCSTR szFilename, bool bOverwrite, ReducedFileInfo * pInfo) = 0;
    virtual void CloseFile(void) = 0;

    // To use these functions, Open an entity of the type you want, add data (below), then
    // Close the entity. Repeat as required. You CANNOT have more than 1 entity type open at a time

    virtual bool BeginEntityEvent(LPCSTR szLabel, LPCSTR szDescription, uint32 nEventType) = 0;
    virtual bool AddDataEvent(uint32 nEventType, void * pData, uint32 cbLength, double dTimeStamp) = 0;
    virtual void EndEntityEvent() = 0;

    virtual bool BeginEntityAnalog(LPCSTR szLabel, ns_ANALOGINFO * pcAnalogInfo) = 0;
    virtual bool AddDataAnalog(double dStartTime, uint32 nCount, double * pData) = 0;
    virtual bool AppendDataAnalog(uint32 nCount, double * pData) = 0;   // use this if you can't add all the data with AddDataAnalog
    virtual void EndEntityAnalog() = 0;

    virtual bool BeginEntityNeuralEvent(LPCSTR szLabel, ns_NEURALINFO * pcNeuralEventInfo) = 0;
    virtual bool AddDataNeuralEvent(uint32 nCount, double * pTimeStamps) = 0;
    virtual void EndEntityNeuralEvent() = 0;

    virtual bool BeginEntitySegment(LPCSTR szLabel, uint32 dwSourceCount, double dSampleRate, LPCSTR szUnits, ns_SEGSOURCEINFO * pcSegSourceInfo) = 0;
    virtual bool AddDataSegment(double dTimeStamp, uint32 dwUnitID, uint32 nSampleCount, double * aadData) = 0;
    virtual void EndEntitySegment() = 0;
};


struct IWriteNSN2 : public IUnKnown
{
    virtual bool OpenFile(LPCSTR szFilename, bool bOverwrite, ReducedFileInfo * pInfo) = 0;
    virtual void CloseFile(void) = 0;

    // To use these functions, Open an entity of the type you want, add data (below), then
    // Close the entity. Repeat as required. You CANNOT have more than 1 entity type open at a time

    virtual bool BeginEntityEvent(LPCSTR szLabel, LPCSTR szDescription, uint32 nEventType) = 0;
    virtual bool AddDataEvent(uint32 nEventType, void * pData, uint32 cbLength, double dTimeStamp) = 0;
    virtual void EndEntityEvent() = 0;

    virtual bool BeginEntityAnalog(LPCSTR szLabel, ns_ANALOGINFO * pcAnalogInfo) = 0;
    virtual bool AddDataAnalog(double dStartTime, uint32 nCount, double * pData) = 0;
    virtual bool AppendDataAnalog(uint32 nCount, double * pData) = 0;   // use this if you can't add all the data with AddDataAnalog
    virtual void EndEntityAnalog() = 0;

    virtual bool BeginEntityNeuralEvent(LPCSTR szLabel, ns_NEURALINFO * pcNeuralEventInfo) = 0;
    virtual bool AddDataNeuralEvent(uint32 nCount, double * pTimeStamps) = 0;
    virtual void EndEntityNeuralEvent() = 0;

    virtual bool BeginEntitySegment(LPCSTR szLabel, uint32 dwSourceCount, double dSampleRate, LPCSTR szUnits, ns_SEGSOURCEINFO * pcSegSourceInfo) = 0;
    virtual bool AddDataSegment(double dTimeStamp, uint32 dwUnitID, uint32 nSampleCount, double * aadData) = 0;
    virtual void EndEntitySegment() = 0;

    virtual DWORD GetCurrentEntityID() = 0;    // At any point in time during writing, get the entity ID. Most useful after a "begin"
};


extern "C" IUnKnown * WINAPI QueryInterface(InterfaceTypes enType);



template <class T>
class InterfaceClient
{
public:
    InterfaceClient(InterfaceTypes enType) { m_pInterface = dynamic_cast<T *>(QueryInterface(enType)); }
    ~InterfaceClient() { if (m_pInterface) m_pInterface->Release(); }

    operator T * () { return m_pInterface; }

protected:
    T * m_pInterface;
};






            
#endif  // Include Guards

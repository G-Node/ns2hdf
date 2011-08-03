///////////////////////////////////////////////////////////////////////////////////////////////////
//
// Copyright (C) 2010 G-NODE
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
// Our software uses PowerNap libraries which are included into ns2hdf, 
// they are also distributed under the LGPL. PowerNap is available at:
// http://neuroshare.sourceforge.net/download.shtml
//
// Contact information:
// 
// - Cristian Tatarau
// - tatarau@bio.lmu.de
// - c.tatarau@gmx.net
// 
// - Thomas Wachtler, German Neuroinformatics Node
// - Biozentrum
//   Ludwig-Maximilians-Universitaet Muenchen
//   Grosshaderner Strasse 2
//   82152 Martinsried-Planegg
//   Germany
// - wachtler@bio.lmu.de
// - Homepage: http://www.neuroinf.de
// 
// All other copyrights on this material are replaced by this license agreeement.
//
///////////////////////////////////////////////////////////////////////////////////////////////////




// ns2hdf.cpp : Definiert den Einstiegspunkt für die Konsolenanwendung.
//

// #include "stdafx.h"
#include <atlbase.h>
#include <algorithm>
#include <stdio.h>
#include <conio.h>
#include <atlstr.h>
#include <windows.h>
#include <vector>
#include <iterator>

#include <NsLibraryImpMgr.h>
#include <ns_common.h>
#include <NsFile.h>

#include "Hdf_export.h"
using namespace std; 

/*! \mainpage 

\section intro_sec Introduction

This software converts proprietary neurophysiological files into
the open HDF5 format.

Usage: ns2hdf [sourcefile] [HDF5 file]
- sourcefile  : full path to the data file
- HDF5 file   : full path to the target HDF5 file

The Neuroshare DLLs should be stored in the same folder as this program.

\section contact_sec Contact Information
- Cristian Tatarau
- tatarau@bio.lmu.de
- c.tatarau@gmx.net

- Thomas Wachtler, German Neuroinformatics Node
- Biozentrum\n
Ludwig-Maximilians-Universitaet Muenchen\n
Grosshaderner Strasse 2\n
82152 Martinsried-Planegg\n
Germany\n
- wachtler@bio.lmu.de
- Homepage: http://www.neuroinf.de

\section copyright_sec Copyright

 Copyright (C) 2010 G-NODE

 This library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation; either
 version 2.1 of the License, or (at your option) any later version.

 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 Lesser General Public License for more details.

 A copy of the GNU Lesser General Public License can be obtained by writing to:
  Free Software Foundation, Inc.,
  59 Temple Place, Suite 330,
  Boston, MA  02111-1307
  USA

 Our software uses PowerNap libraries which are included into ns2hdf, 
 they are also distributed under the LGPL. PowerNap is available at:
 http:neuroshare.sourceforge.net/download.shtml

*/

CHdf_export hdf_export;
ns_FILEINFO  fileInfo;

void checkNullTermination(char c[], int l)
{
    int j=129;
    for (int i=0;i<l;i++)
    {
        if (c[i]==0)
            j=i;
        break;
    }
    for (int i=j;i<l;i++)
        c[i]=0;
}

// Get the fully qualifed name of the EXE running
CString GetAppPathName()
{
    char buffer[MAX_PATH];
    GetModuleFileName(NULL,buffer,MAX_PATH);

    return buffer;
}

// Get the fully qualified path to the directory where the app is running
CString GetAppPath()
{
    CString szTemp(GetAppPathName());

    szTemp = szTemp.Left(szTemp.ReverseFind('\\'));
    return szTemp;
}

void PressAKeyToContinue()
{
    int c;
    printf( "\nPress a key to continue..." );
    c = _getch();
    if (c == 0 || c == 224) _getch();
}

/// Get file info in neuroshare format
/// - parameters:
///     - NsFile const & rcLib: NsFile object
/// return:
///     - ns_FILEINFO: neuroshare file info
ns_FILEINFO readFileInfo(NsFile const & rcLib)
{
    ns_FILEINFO  fileInfo;
    if (ns_OK != rcLib.GetFileInfo(fileInfo))
    {
        printf("There was an error loading the file information!");
    }
    checkNullTermination(fileInfo.szFileType, 32);
    checkNullTermination(fileInfo.szAppName, 64);
    checkNullTermination(fileInfo.szFileComment, 256);

    return(fileInfo);
}


/// Shows some info about the selected file
/// - parameters:
///     - fileInfo: neuroshare file info
void ShowFileInfo(ns_FILEINFO fileInfo)
{

    printf("\n--- file info ---\n");
    // Let's check on the library info...we don't display this anywhere though
    // ns_LIBRARYINFO isLibInfo;
    // rcLib.GetLibraryInfo(isLibInfo);

    CString icStr;
    // Ok we read it in, now let's put the stuff on the screen
    printf("IDC_FILE_TYPE		= %s\n", fileInfo.szFileType);
    printf("IDC_ENTITY_COUNT	= %i\n", fileInfo.dwEntityCount);

    // Now let's deal with time
    printf("IDC_TIME_RESOLUTION	= %i\n", UINT(1 / fileInfo.dTimeStampResolution));
    printf("IDC_TIME_SPAN		= %i\n", UINT(fileInfo.dTimeSpan));
    printf("IDC_APPLICATION		= %i\n", fileInfo.szAppName);

    // And how about when this was created
    icStr.Format("%lu/%lu/%lu  %lu:%lu:%lu", 
        fileInfo.dwTime_Month, fileInfo.dwTime_Day, fileInfo.dwTime_Year,
        fileInfo.dwTime_Hour, fileInfo.dwTime_Min, fileInfo.dwTime_Sec);

    printf("IDC_DATE_CREATED	= %s\n", icStr);

    // Now the very nice comments
    printf("IDC_COMMENT			= \n%s\n", fileInfo.szFileComment);
}

/// Counts the number and type of entities
/// contained in the neuroshare data file.
/// Then it creates hdf5 groups for each existent entity type.
/// - parameters:
///     - NsFile const & rcLib: NsFile object
void ShowEntityInfo(NsFile const & rcLib)
{
    // Set equal to 0 because we are about to count how many we find
    UINT nUnknown = 0;
    UINT nEvents = 0;
    UINT nAnalogs = 0;
    UINT nSegments = 0;
    UINT nNeurals = 0;
    ns_FILEINFO fileInfo;
    rcLib.GetFileInfo(fileInfo);

    // Ok, we now now how many entities, let's rock'n roll
    ns_ENTITYINFO isEntityInfo;
    for (DWORD i = 0; i < fileInfo.dwEntityCount; ++i)
    {
        if (ns_OK != rcLib.GetEntityInfo(i, isEntityInfo))
        {
            printf("There was an error loading the entity information!");
        }
        else
        {
            // Life is good :-)
            switch(isEntityInfo.dwEntityType)
            {
            case ns_ENTITY_UNKNOWN:
                ++nUnknown;
                break;

            case ns_ENTITY_EVENT:
                ++nEvents;
                break;

            case ns_ENTITY_ANALOG:
                ++nAnalogs;
                break;

            case ns_ENTITY_SEGMENT:
                ++nSegments;
                break;

            case ns_ENTITY_NEURALEVENT:
                ++nNeurals;
                break;

            default:
                // This case should never happen
                _ASSERT(0);
                break;
            }
        }

    }
    // Now let's set these values on the screen
    printf("IDC_ENTITIES_UNKNOWN    = %i\n", nUnknown);
    printf("IDC_ENTITIES_EVENT      = %i\n", nEvents);
    printf("IDC_ENTITIES_ANALOG     = %i\n", nAnalogs);
    printf("IDC_ENTITIES_SEGMENT    = %i\n", nSegments);
    printf("IDC_ENTITIES_NEURAL     = %i\n", nNeurals);

    // create the HDF5 groups for each entity type with one or more elements
    if (nUnknown >0)
        hdf_export.create_root_groups( entityType::Unknown_Entity );
    if (nAnalogs >0)
        hdf_export.create_root_groups(entityType::Analog);
    if (nEvents >0)
        hdf_export.create_root_groups(entityType::Event);
    if (nSegments >0)
        hdf_export.create_root_groups(entityType::Segment);
    if (nNeurals >0)
        hdf_export.create_root_groups(entityType::Neural);
}

/// Reads text or csv event data from the specified entity.
/// - parameters:
///     - NsFile const & rcLib: NsFile object
///     - idEntity: entity index
///     - isEntityInfo: neuroshare entity info
///     - isEventInfo: neuroshare event info
void getEvent_text(NsFile const & rcLib, DWORD idEntity, 
                   ns_ENTITYINFO isEntityInfo, ns_EVENTINFO isEventInfo)
{
    uint32 cbUsed;		// number of bytes returned from nsLib
    int iTotalSize = 0; // size of all structures in byte
    int iUnitSize = 0; // size of one structure in byte

    iUnitSize= (sizeof(double)+isEventInfo.dwMaxDataLength);
    iTotalSize= iUnitSize*isEntityInfo.dwItemCount;
    BYTE *bytePtr= new BYTE[iTotalSize]; // save the data, both time stamp and values
    BYTE *arrVal= new BYTE[isEventInfo.dwMaxDataLength];

    for(UINT i = 0; i < isEntityInfo.dwItemCount; ++i)
    {
        if (ns_OK == rcLib.GetEventData(idEntity, i, 
            ((double*)(bytePtr+i*iUnitSize)) , arrVal,
            isEventInfo.dwMaxDataLength, &cbUsed))
        {
            for(UINT j=cbUsed; j<isEventInfo.dwMaxDataLength; ++j)
                *(arrVal+j)=0;
            memcpy(bytePtr+i*iUnitSize+sizeof(double), &arrVal[0], isEventInfo.dwMaxDataLength);
            // printf("    event text, item %i: %s \n", i, arrVal);
        }
        else
        {
            printf("    There was an error loading the event data! \n");
            return;
        }
    }
    printf("    write event entity %i to hdf5... ", idEntity);
    hdf_export.write_event_text(bytePtr, isEntityInfo.dwItemCount, idEntity, isEventInfo);
    delete[] bytePtr;
    delete[] arrVal;
    printf("OK\n");
}

/// Reads word event data from the specified entity.
/// - parameters:
///     - NsFile const & rcLib: NsFile object
///     - idEntity: entity index
///     - isEntityInfo: neuroshare entity info
///     - isEventInfo: neuroshare event info
void getEvent_word(NsFile const & rcLib, DWORD idEntity, 
                   ns_ENTITYINFO isEntityInfo, ns_EVENTINFO isEventInfo)
{
    uint32 cbUsed;		// data size
    eventWord *eventWordPtr;
    eventWordPtr = new eventWord[isEntityInfo.dwItemCount];
    unsigned short usDummy;
    double dDummy;
    // printf("data type WORD, max data length= %i \n", isEventInfo.dwMaxDataLength);
    // gather all paires of time stamp and data and save into a vector
    for(UINT i = 0; i < isEntityInfo.dwItemCount; ++i)
    {
        if (ns_OK == rcLib.GetEventData(idEntity, i, &dDummy, &usDummy, sizeof dDummy, &cbUsed))
        {
            eventWordPtr[i].dTimeStamp= dDummy;
            eventWordPtr[i].usData= usDummy;
            // printf("    event word, item %i: %d \n", i, usDummy);
        }
        else
        {
            printf("    There was an error loading the event data! \n");
            return;
        }
    }
    printf("    write event entity %i  to hdf5... ", idEntity);
    hdf_export.write_event_word(eventWordPtr, isEntityInfo.dwItemCount, idEntity, isEventInfo);
    delete eventWordPtr;
    printf("OK  \n");
}

/// Reads BYTE event data from the specified entity.
/// - parameters:
///     - NsFile const & rcLib: NsFile object
///     - idEntity: entity index
///     - isEntityInfo: neuroshare entity info
///     - isEventInfo: neuroshare event info
void getEvent_byte(NsFile const & rcLib, DWORD idEntity, 
                   ns_ENTITYINFO isEntityInfo, ns_EVENTINFO isEventInfo)
{
    uint32 cbUsed;		// data size
    eventByte *eventBytePtr;
    eventBytePtr = new eventByte[isEntityInfo.dwItemCount];
    BYTE usDummy;
    double dDummy;
    // printf("data type BYTE, max data length= %i \n", isEventInfo.dwMaxDataLength);
    // gather all paires of time stamp and data and save into a vector
    for(UINT i = 0; i < isEntityInfo.dwItemCount; ++i)
    {
        if (ns_OK == rcLib.GetEventData(idEntity, i, &dDummy, &usDummy, sizeof dDummy, &cbUsed))
        {
            eventBytePtr[i].dTimeStamp= dDummy;
            eventBytePtr[i].usData= usDummy;
            // printf("    event byte, item %i: %i \n", i, usDummy);
        }
        else
        {
            printf("    There was an error loading the event data! \n");
            return;
        }
    }
    printf("    write event byte %i to hdf5... ", idEntity);
    hdf_export.write_event_byte(eventBytePtr, isEntityInfo.dwItemCount, idEntity, isEventInfo);
    delete eventBytePtr;
    printf("OK  \n");
}

/// Reads DWORD event data from the specified entity.
/// - parameters:
///     - NsFile const & rcLib: NsFile object
///     - idEntity: entity index
///     - isEntityInfo: neuroshare entity info
///     - isEventInfo: neuroshare event info
void getEvent_dword(NsFile const & rcLib, DWORD idEntity, 
                    ns_ENTITYINFO isEntityInfo, ns_EVENTINFO isEventInfo)
{
    uint32 cbUsed;		// data size
    eventDword *eventDwordPtr;
    eventDwordPtr = new eventDword[isEntityInfo.dwItemCount];
    DWORD usDummy;
    double dDummy;
    // printf("data type DWORD, max data length= %i \n", isEventInfo.dwMaxDataLength);
    // gather all paires of time stamp and data and save into a vector
    for(UINT i = 0; i < isEntityInfo.dwItemCount; ++i)
    {
        if (ns_OK == rcLib.GetEventData(idEntity, i, &dDummy, &usDummy, sizeof dDummy, &cbUsed))
        {
            eventDwordPtr[i].dTimeStamp= dDummy;
            eventDwordPtr[i].usData= usDummy;
            // printf("    event word item %i: %d \n", i, usDummy);
        }
        else
        {
            printf("    There was an error loading the event data! \n");
            return;
        }
    }
    printf("    write event word %i to hdf5... ", idEntity);
    hdf_export.write_event_dword(eventDwordPtr, isEntityInfo.dwItemCount, idEntity, isEventInfo);
    delete eventDwordPtr;
    printf("OK  \n");
}

/// Processes all event entities
/// - parameters:
///     - NsFile const & rcLib: NsFile object
void ShowEvent(NsFile const & rcLib)
{
    ns_ENTITYINFO isEntityInfo;
    ns_EVENTINFO isEventInfo;
    printf("\n--- event data --- \n");

    // Now let's find the first entity of type "EVENT"
    for (DWORD idEntity = 0; idEntity < fileInfo.dwEntityCount; ++idEntity)
    {
        if (ns_OK != rcLib.GetEntityInfo(idEntity, isEntityInfo))
        {
            printf("There was an error loading the entity information! \n");
            return;
        }
        else
        {
            if (isEntityInfo.dwEntityType == ns_ENTITY_EVENT)
            {
                printf("event entity %i has %i items \n", idEntity, isEntityInfo.dwItemCount);
                // aha, found one. Let's get busy
                // Now let's see what kind of event
                if (ns_OK != rcLib.GetEventInfo(idEntity, isEventInfo) )
                {
                    printf("There was an error loading the event information! \n");
                    return;
                }
                checkNullTermination(isEventInfo.szCSVDesc, 128);
                switch ((UINT)isEventInfo.dwEventType)
                {
                case ns_EVENT_TEXT:
                    getEvent_text(rcLib, idEntity, isEntityInfo, isEventInfo);
                    break;
                case ns_EVENT_CSV:
                    getEvent_text(rcLib, idEntity, isEntityInfo, isEventInfo);
                    break;
                case ns_EVENT_BYTE:
                    getEvent_byte(rcLib, idEntity, isEntityInfo, isEventInfo);
                    break;
                case ns_EVENT_WORD:
                    getEvent_word(rcLib, idEntity, isEntityInfo, isEventInfo);
                    break;
                case ns_EVENT_DWORD:
                    getEvent_dword(rcLib, idEntity, isEntityInfo, isEventInfo);
                    break;
                default:
                    // This should never happen DLL has made a mistake
                    _ASSERT(0);
                    break;
                }
            }
        }
    }
}

/// Processes all analog entities
/// - parameters:
///     - NsFile const & rcLib: NsFile object
void ShowAnalog(NsFile const & rcLib)
{
    ns_ANALOGINFO isAnalogInfo;
    ns_ENTITYINFO isEntityInfo;
    uint32 nDummy;
    UINT nCountMax = 0;

    printf("\n--- analog data --- \n");
    // Now let's find the first entity of type "ns_ENTITY_ANALOG"
    for (DWORD idEntity = 0; idEntity < fileInfo.dwEntityCount; ++idEntity)
    {
        if (ns_OK != rcLib.GetEntityInfo(idEntity, isEntityInfo))
        {
            printf("There was an error loading the entity information!");
            return;
        }
        else
        {
            if (isEntityInfo.dwEntityType == ns_ENTITY_ANALOG)
            {
                double *dPtr= new double[isEntityInfo.dwItemCount];
                if (ns_OK != rcLib.GetAnalogInfo(idEntity, isAnalogInfo))
                {
                    printf("There was an error loading the analog information!");
                    return;
                }
                else
                {
                    checkNullTermination(isAnalogInfo.szHighFilterType, 16);
                    checkNullTermination(isAnalogInfo.szLowFilterType, 16);
                    checkNullTermination(isAnalogInfo.szProbeInfo, 128);
                    if (ns_OK != rcLib.GetAnalogData(idEntity, 0, 
                        isEntityInfo.dwItemCount,  
                        &nDummy, 
                        dPtr))
                    {
                        printf("There was an error loading the analog data!");
                        return;
                    }
                    else
                    {
                        printf("    write analog entity %i to hdf5... ", idEntity);
                        hdf_export.write_analog(dPtr, isEntityInfo.dwItemCount,
                            idEntity, isAnalogInfo);
                        printf("OK \n");
                    }
                }
                delete[] dPtr;
            }
        }
    }
}

/// Reads the segment data and writes them into the hdf5 file
/// - The segment entity allows to save data from multiple electrodes
/// so first we allocate enough memory for all channel information (i.e. ns_SEGSOURCEINFO)
/// - read the channel info
/// - allocate memory for the data. The data size is number of channels * maxSampleCount
/// Check if the allocation was successful
/// - create a segment group
/// - read all items from the current segment and write them into the 
/// previously created segment group
/// - free the reserved memory for the data
/// - done!
void getSegment(NsFile const & rcLib, DWORD idEntity, 
                ns_ENTITYINFO isEntityInfo, 
                ns_SEGMENTINFO isSegmentInfo)
{
    uint32 dwSampleCount=0;
    uint32 dwUnitID=0;
    int err=0;
    // allocate memory for the sources
    // you may have one or more sources, for example a tetrode setup will give you 4 channels
    ns_SEGSOURCEINFO *isSegmentSourceInfo= new ns_SEGSOURCEINFO[isSegmentInfo.dwSourceCount];

    for(UINT i=0; i<isSegmentInfo.dwSourceCount; ++i)
        if (ns_OK != rcLib.GetSegmentSourceInfo(idEntity, 0, *isSegmentSourceInfo))
        {
            printf("There was an error loading the segment source information!");
            return;
        }

        // time stamp
        double t;
        // allocate memory for the data
        double *dPtr = new double[isSegmentInfo.dwMaxSampleCount * isSegmentInfo.dwSourceCount];
        uint32 dataSize= isSegmentInfo.dwMaxSampleCount*sizeof(double);

        if (dPtr==0)
        {
            printf("Not enough memory!\n");
            return;
        }
        // create an empty group, later it will contain one timestamp and one data set 
        hdf_export.create_single_segment_group(idEntity, isSegmentInfo, isSegmentSourceInfo);

        for(UINT item = 0; item < isEntityInfo.dwItemCount; ++item)
        {
            err= rcLib.GetSegmentData(idEntity, item, 
                &t, 
                dPtr,
                dataSize, &dwSampleCount, &dwUnitID);
            if (ns_OK != err)
            {
                printf ("   There was an error loading the segment data! %i\n", err);
                delete[] dPtr;
                return;
            }
            else
            {
                // printf("    write segment %i item %i to hdf5... ", idEntity, item);
                hdf_export.write_segment_item(t, dPtr, idEntity, item, 
                    isSegmentInfo.dwSourceCount, isSegmentInfo.dwMaxSampleCount);
                // printf("OK\n");
            }
        }
        delete isSegmentSourceInfo;
        delete[] dPtr;
}

/// Processes all segment entities
/// - parameters:
///     - NsFile const & rcLib: NsFile object
void ShowSegment(NsFile const & rcLib)
{
    ns_ENTITYINFO isEntityInfo;
    ns_SEGMENTINFO isSegmentInfo;

    printf("\n--- segment data --- \n");

    // Now let's find the first entity of type "ns_ENTITY_SEGMENT"
    for (DWORD idEntity = 0; idEntity < fileInfo.dwEntityCount; ++idEntity)
    {
        if (ns_OK != rcLib.GetEntityInfo(idEntity, isEntityInfo))
        {
            printf("There was an error loading the entity information!");
            return;
        }
        else
        {
            if (isEntityInfo.dwEntityType == ns_ENTITY_SEGMENT)
            {
                printf("Segment Entity %i \n", idEntity);
                if (ns_OK != rcLib.GetSegmentInfo(idEntity, isSegmentInfo))
                {
                    printf("There was an error loading the segment data!");
                    return;
                }
                checkNullTermination(isSegmentInfo.szUnits, 32);
                if(isSegmentInfo.dwSourceCount>1)
                    printf("---------> SourceCount= %i\n", isSegmentInfo.dwSourceCount);
                // printf("Segment info: \n");
                // printf("source count: %i\n", isSegmentInfo.dwSourceCount);
                // printf("min sample count: %i\n", isSegmentInfo.dwMinSampleCount);
                // printf("max sample count: %i\n", isSegmentInfo.dwMaxSampleCount);
                //else
                // {
                ns_SEGSOURCEINFO isSegmentSourceInfo;
                if (ns_OK != rcLib.GetSegmentSourceInfo(idEntity, 0, isSegmentSourceInfo))
                {
                    printf("There was an error loading the segment source information!");
                    return;
                }
                else
                {
                    checkNullTermination(isSegmentSourceInfo.szLowFilterType, 16);
                    checkNullTermination(isSegmentSourceInfo.szHighFilterType, 16);
                    checkNullTermination(isSegmentSourceInfo.szProbeInfo, 128);
                    printf("    write segment %i to hdf5... ", idEntity);
                    getSegment(rcLib, idEntity, isEntityInfo, isSegmentInfo);
                    printf("OK \n");
                }
                // }
            }
        }
    }
}

/// Reads the specified neural entity and passes the
/// information to the hdf_export object which writes it into the HDF5 file.
/// - parameters:
///     - NsFile const & rcLib: NsFile object
///     - idEntity: current entity index
///     - isEntityInfo: neuroshare entity info
///     - isNeuralInfo: neuroshare neural info
void getNeural(NsFile const & rcLib, DWORD idEntity, 
               ns_ENTITYINFO isEntityInfo, 
               ns_NEURALINFO isNeuralInfo)
{
    double *dPtr= new double[isEntityInfo.dwItemCount];
    if (ns_OK != rcLib.GetNeuralData(idEntity, 0, isEntityInfo.dwItemCount, dPtr))
    {
        printf("There was an error loading the neural data!");
        return;
    }
    else
    {
        hdf_export.write_neural(dPtr, isNeuralInfo, isEntityInfo.dwItemCount, idEntity);
    }
    delete[] dPtr;
}

/// Processes all neural entities
/// - parameters:
///     - NsFile const & rcLib: NsFile object
void ShowRaster(NsFile const & rcLib)
{
    ns_NEURALINFO isNeuralInfo;
    printf("\n--- neural data --- \n");

    // Now let's find every entity of type "ns_ENTITY_NEURALEVENT"
    for (DWORD idEntity = 0; idEntity < fileInfo.dwEntityCount; ++idEntity)
    {
        ns_ENTITYINFO isEntityInfo;
        if (ns_OK != rcLib.GetEntityInfo(idEntity, isEntityInfo))
        {
            printf("There was an error loading the entity information!");
            return;
        }
        else
        {
            if (isEntityInfo.dwEntityType == ns_ENTITY_NEURALEVENT)
            {
                if (ns_OK != rcLib.GetNeuralInfo(idEntity, isNeuralInfo))
                {
                    printf("Unable to get the neural info");
                    return;
                }
                checkNullTermination(isNeuralInfo.szProbeInfo, 128);

                printf("    write neural entity %i to hdf5... ", idEntity);
                getNeural(rcLib, idEntity, isEntityInfo, isNeuralInfo);
                printf("OK\n");
            }
        }
    }
}

/// Opens the data file, reads data and saves it in the hdf5 format.\n
/// The neuroshare library usually recognises the file type and selects
/// automatically the appropriate DLL.
/// But as there seems to be an error with ".mcd" files,
/// we have to constrain the neuroshare library to open this file type
/// with "nsMCDLibrary.dll".\n
/// - parameters:
///     - path: current path
///     - file: target hdf5 file path
int main_processFile(CString path, CString file)
{
    int iOK=0;
    CString fileExtension="";
    NsFile myNsFile;
    // get the file extension
    fileExtension = file.Right(file.GetLength() - file.ReverseFind('.') -1 );
    printf("file extension= %s  \n", fileExtension);
    printf("opening file... ");
    // check if we got a mdc file
    // if so, select the appropriate DLL, else let the neuroshare class decide
    if (fileExtension == "mcd")
    {iOK= myNsFile.OpenFileUsingThisDLL(file, path + "\\"+ "nsMCDLibrary.dll");}
    else
    {iOK=  myNsFile.OpenFile(file);}

    if (iOK == 0)
    {
        printf("OK\n");
    }
    else
    {
        printf("ERROR, OpenFile returns error code %i \n", iOK);
        return(iOK);
    }
    fileInfo= readFileInfo(myNsFile);   // get file info
    ShowFileInfo(fileInfo);				// display the file info on the screen
    ShowEntityInfo(myNsFile);			// count entities
    hdf_export.write_fileInfo(&fileInfo);// write file info into hdf5 format

    // Let's make sure we have at least 1 entity
    if (fileInfo.dwEntityCount < 1)
    {
        printf("No Entities to process");
        return(-1);
    }

    // go through all entities, display info about them and 
    // save then into the hdf5 file
    ShowAnalog(myNsFile);
    ShowEvent(myNsFile);
    ShowSegment(myNsFile);
    ShowRaster(myNsFile); 

    // in all cases, we want to close the file
    myNsFile.CloseFile(); 
    return(iOK);
}

/// Displays usage information, copyright and contact data
void main_displayHelp()
{
    printf("\n=========== NeuroShare-based conversion program ================\n\n");
    printf("Usage: \nns2hdf.exe [sourcefile] [HDF5 file] \n");
    printf("sourcefile  : full path to the data file\n");
    printf("HDF5 file   : full path to the target HDF5 file\n");
    printf("The Neuroshare DLLs should be in the same folder as this program \n");

    printf("\nContact:\n");
    printf("Cristian Tatarau\n  c.tatarau@gmx.net \n  tatarau@bio.lmu.de \n");
    printf("German Neuroinformatics Node\n  wachtler@bio.lmu.de\n  http://www.neuroinf.de \n");
}

int _tmain(int argc, _TCHAR* argv[])
{
    CString path;
    CString file, resultfile;
    switch (argc)
    { 
    case 1:
        main_displayHelp();
        return(0);
        break;
    case 2:
        printf("--- second parameter missing \n");
        return(0);
        break;
    case 3:
        file= argv[1];
        resultfile= argv[2];
        break;
    }

    hdf_export.createFile(file, resultfile);

    path = GetAppPath();
    NsLibraryImpMgr::CreateMgr(path);
    printf("\n=========== NeuroShare-based conversion program ================\n\n");
    printf("current path= %s\n", path);
    printf("file = %s \n", file);
    // Warn the user if there is no way to look at any data
    if (NsLibraryImpMgr::GetMgr().GetNumOfLibrariesAvailable() == 0)
    {
        printf("There are no Neuroshare DLLs to be found.\n");
        return(0);
    }
    else
    {
        printf("%i libraries found \n", NsLibraryImpMgr::GetMgr().GetNumOfLibrariesAvailable());
    }
    // open and process the data file
    main_processFile(path, file);
    printf("\n\neverything OK, closing hdf file...");
    hdf_export.closeFile();
    printf(" OK \n");
    PressAKeyToContinue();
    return 0;
}


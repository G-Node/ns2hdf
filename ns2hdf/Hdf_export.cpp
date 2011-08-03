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
//   tatarau@bio.lmu.de
//   c.tatarau@gmx.net
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
// 
// All other copyrights on this material are replaced by this license agreeement.
//
///////////////////////////////////////////////////////////////////////////////////////////////////


#include "Hdf_export.h"


CHdf_export::CHdf_export()
{
}

CHdf_export::~CHdf_export(void)
{
}
void CHdf_export::closeFile(void)
{
    H5Fclose(this->file_id); 
}

/// Creates a HDF5 file with the specified path.
/// This function also saves the pathes to the source and target data
/// files into the appropriate private variables.
/// - parameters:
///     - sourceFile: full path tot the source data file
///     - hdfFile: full path to the target HDF5 file
int CHdf_export::createFile(CString sourceFile, CString hdfFile)
{
    CHdf_export::path_source= sourceFile;
    CHdf_export::path_hdf= hdfFile;
    /* Create a new file using default properties. */
    this->file_id = H5Fcreate(path_hdf, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);

    return(hOK);
}

/// Creates HDF5 groups for the 5 basic entity types:
/// analog, segment, event, neural, unknown data.
/// - parameters:
///     - t as entityType: the entity type for which you want to create the group
/// returns:
///     - herr_t
herr_t CHdf_export::create_root_groups(entityType t)
{
    hid_t g;
    herr_t status;
    switch (t)
    {
    case entityType::Unknown_Entity:
        g = H5Gcreate(this->file_id, "/Unknown", H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
        break;
    case entityType::Analog:
        g = H5Gcreate(this->file_id, "/Analog", H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
        break;
    case entityType::Event:
        g = H5Gcreate(this->file_id, "/Event", H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
        break;
    case entityType::Segment:
        g = H5Gcreate(this->file_id, "/Segment", H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
        break;
    case entityType::Neural:
        g = H5Gcreate(this->file_id, "/Neural", H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    }

    status = H5Gclose (g);
    return(status);
}


/// Creates a data set containing the file info as specified in the neuroshare file.
/// - parameters:
///     - fi as ns_FILEINFO*: neuroshare file info
/// return:
///     - int: Returns a non-negative value if successful; otherwise returns a negative value. 
int CHdf_export::write_fileInfo(ns_FILEINFO *fi)
{
    hid_t      s1_tid; //, dataset, space;     /* File datatype identifier */
    hid_t      string_type32, string_type64, string_type256;
    hsize_t    dim[] = {1};   /* Dataspace dimensions */
    /* Initialize the field field_type */
    string_type32 = H5Tcopy( H5T_C_S1 );
    string_type64 = H5Tcopy( H5T_C_S1 );
    string_type256 = H5Tcopy( H5T_C_S1 );

    H5Tset_size( string_type32, 32 );
    H5Tset_size( string_type64, 64 );
    H5Tset_size( string_type256, 256 );

    // Create the memory data type. 
    s1_tid = H5Tcreate (H5T_COMPOUND, sizeof(ns_FILEINFO));
    H5Tinsert(s1_tid, "szFileType", HOFFSET(ns_FILEINFO, szFileType), string_type32);
    H5Tinsert(s1_tid, "dwEntityCount", HOFFSET(ns_FILEINFO, dwEntityCount), H5T_NATIVE_UINT32);
    H5Tinsert(s1_tid, "dTimeStampResolution", HOFFSET(ns_FILEINFO, dTimeStampResolution), H5T_NATIVE_DOUBLE);
    H5Tinsert(s1_tid, "dTimeSpan", HOFFSET(ns_FILEINFO, dTimeSpan), H5T_NATIVE_DOUBLE);
    H5Tinsert(s1_tid, "szAppName", HOFFSET(ns_FILEINFO, szAppName), string_type64);
    H5Tinsert(s1_tid, "dwTime_Year", HOFFSET(ns_FILEINFO, dwTime_Year), H5T_NATIVE_UINT32);
    H5Tinsert(s1_tid, "dwTime_Month", HOFFSET(ns_FILEINFO, dwTime_Month), H5T_NATIVE_UINT32);
    H5Tinsert(s1_tid, "dwTime_DayofWeek", HOFFSET(ns_FILEINFO, dwTime_DayofWeek), H5T_NATIVE_UINT32);
    H5Tinsert(s1_tid, "dwTime_Day", HOFFSET(ns_FILEINFO, dwTime_Day), H5T_NATIVE_UINT32);
    H5Tinsert(s1_tid, "dwTime_Hour", HOFFSET(ns_FILEINFO, dwTime_Hour), H5T_NATIVE_UINT32);
    H5Tinsert(s1_tid, "dwTime_Min", HOFFSET(ns_FILEINFO, dwTime_Min), H5T_NATIVE_UINT32);
    H5Tinsert(s1_tid, "dwTime_Sec", HOFFSET(ns_FILEINFO, dwTime_Sec), H5T_NATIVE_UINT32);
    H5Tinsert(s1_tid, "dwTime_MilliSec", HOFFSET(ns_FILEINFO, dwTime_MilliSec), H5T_NATIVE_UINT32);
    H5Tinsert(s1_tid, "szFileComment", HOFFSET(ns_FILEINFO, szFileComment), string_type256);

    hOK = H5LTmake_dataset(file_id,"/File_Info",1,dim,s1_tid, fi);
    H5Tclose(s1_tid);

    return(hOK);
}


/// This function writes one analog entity into the hdf5 file.
/// - parameters:
///     - d as double*: pointer to analog data
///     - n as UINT: total number of values, needed to set the correct size of the dataset
///     - index as DWORD: global index of the current analog entity
///     - ai as ns_ANALOGINFO: neuroshare analog info
int CHdf_export::write_analog(double* d, UINT n, DWORD index, ns_ANALOGINFO ai)
{
    hid_t hSpace, hSet;
    hsize_t  dim[1] ={n}; // dim= n;
    char name[32];
    sprintf_s(name, "/Analog/Analog%i", index);

    if(n==0)
        hSpace =  H5Screate(H5S_NULL) ;
    else
        hSpace = H5Screate_simple(1, dim, NULL);
    /* Create the dataset. */
    hSet = H5Dcreate(this->file_id, name, H5T_NATIVE_DOUBLE, hSpace, 
            H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    /* Write the first dataset. */
    hOK = H5Dwrite(hSet, H5T_NATIVE_DOUBLE, H5S_ALL, H5S_ALL, H5P_DEFAULT,
        d);
    write_analog_info(hSet, ai);
    /* Terminate access to the data space. */ 
    hOK = H5Dclose(hSet);
    hOK = H5Sclose(hSpace);

    return(hOK);
}

/// Writes event data and the event attribute into the hdf5 file
/// The data type has to be specified, if it is a hdf5 compound type, you have to
/// build it first and pass the hdf5 compound type handle to this function.
/// - parameters:
///     - data: pointer to your data structures. It is a (void*) as we use different datatypes
///     - name: element name
///     - hDataType: handle of the hdf5 data type
///     - n: number of items
///     - ai: neuroshare event info for the attribute
int CHdf_export::write_event(void *data, char name[], hid_t hDataType,
                             UINT n, ns_EVENTINFO ai)
{
    hid_t hSpace, hSet;     /* Memory datatype handle */
    hsize_t  dim[1] ={n}; //   dim[2]= {n, 1};

    // hSpace = H5Screate_simple(1, dim, NULL);
    if(n==0)
        hSpace =  H5Screate(H5S_NULL) ;
    else
        hSpace = H5Screate_simple(1, dim, NULL);

    /* Create the dataset. */
    hSet = H5Dcreate(this->file_id, name, hDataType, hSpace, 
        H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    /* Write the first dataset. */
    hOK = H5Dwrite(hSet, hDataType, H5S_ALL, H5S_ALL, H5P_DEFAULT,
        data);
    write_event_info(hSet, ai);
    /* Terminate access to the data space. */ 
    hOK = H5Dclose(hSet);
    hOK = H5Sclose(hSpace);
    return(hOK);
}

/// Writes one event entity which contains byte sized data.
/// - parameters:
///     - data: pointer to the data
///     - n as UINT: total number of values, needed to set the correct size of the dataset
///     - index as DWORD: global index of the current analog entity
///     - ai as ns_EVENTINFO: neuroshare analog info
int CHdf_export::write_event_byte(eventByte *data,
                                  UINT n, DWORD index, ns_EVENTINFO ai)
{
    hid_t hDataType; /* Memory datatype handle */
    char name[32];
    sprintf_s(name, "/Event/Event%i", index);

    // Create the memory data type. 
    hDataType = H5Tcreate (H5T_COMPOUND, sizeof(eventByte));
    H5Tinsert(hDataType, "timeStamp", HOFFSET(eventByte, dTimeStamp), H5T_NATIVE_DOUBLE);
    H5Tinsert(hDataType, "data", HOFFSET(eventByte, usData), H5T_NATIVE_UINT8);

    hOK= write_event(data, name, hDataType, n, ai);
    return(hOK);
}

/// Writes one event entity which contains double word sized data.
/// - parameters:
///     - data: pointer to the data
///     - n as UINT: total number of values, needed to set the correct size of the dataset
///     - index as DWORD: global index of the current analog entity
///     - ai as ns_EVENTINFO: neuroshare analog info
int CHdf_export::write_event_dword(eventDword *data,
                                   UINT n, DWORD index, ns_EVENTINFO ai)
{
    hid_t      hDataType;  /* Memory datatype handle */
    char name[32];
    sprintf_s(name, "/Event/Event%i", index);

    // Create the memory data type. 
    hDataType = H5Tcreate (H5T_COMPOUND, sizeof(eventDword));
    H5Tinsert(hDataType, "timeStamp", HOFFSET(eventDword, dTimeStamp), H5T_NATIVE_DOUBLE);
    H5Tinsert(hDataType, "data", HOFFSET(eventDword, usData), H5T_NATIVE_UINT32);

    hOK= write_event(data, name, hDataType, n, ai);
    return(hOK);
}


/// Writes one event entity which contains word sized data.
/// - parameters:
///     - data: pointer to the data
///     - n as UINT: total number of values, needed to set the correct size of the dataset
///     - index as DWORD: global index of the current analog entity
///     - ai as ns_EVENTINFO: neuroshare analog info
int CHdf_export::write_event_word(eventWord *data,
                                  UINT n, DWORD index, ns_EVENTINFO ai)
{
    hid_t      hDataType; /* Memory datatype handle */
    char name[32];
    sprintf_s(name, "/Event/Event%i", index);

    // Create the memory data type. 
    hDataType = H5Tcreate (H5T_COMPOUND, sizeof(eventWord));
    H5Tinsert(hDataType, "timeStamp", HOFFSET(eventWord, dTimeStamp), H5T_NATIVE_DOUBLE);
    H5Tinsert(hDataType, "data", HOFFSET(eventWord, usData), H5T_NATIVE_UINT16);

    hOK= write_event(data, name, hDataType, n, ai);
    return(hOK);
}

/// Returns a hdf5 string type
/// - parameters:
///     - len as UINT: length of the string in bytes
/// return:
///     - string type as hid_t
hid_t CHdf_export::get_string_type(UINT len)
{
    hid_t      string_type;
    /* Initialize the field field_type */
    string_type = H5Tcopy( H5T_C_S1 );
    H5Tset_size( string_type, len );
    return(string_type);
}

/// Writes one event entity which contains text or csv items.
/// - parameters:
///     - data as BYTE*: pointer to data array
///     - n as UINT: total number of values, needed to set the correct size of the dataset
///     - index as DWORD: global index of the current analog entity
///     - ai as ns_EVENTINFO: neuroshare analog info
int CHdf_export::write_event_text(BYTE *data,
                                  UINT n, DWORD index, ns_EVENTINFO ai)
{
    hid_t      hDataType, string_type, hSpace, hSet;     /* Memory datatype handle */
    hsize_t     dim[2]= {n, 1};
    char name[32];
    sprintf_s(name, "/Event/Event%i", index);
    string_type= get_string_type(ai.dwMaxDataLength );

    // Create the memory data type. 
    hDataType = H5Tcreate (H5T_COMPOUND, (sizeof(double)+ai.dwMaxDataLength) ); // sizeof(data[0])
    H5Tinsert(hDataType, "timeStamp", 0, H5T_NATIVE_DOUBLE);
    H5Tinsert(hDataType, "data", sizeof(double), string_type);

    hSpace = H5Screate_simple(1, dim, NULL);
    /* Create the dataset. */
    hSet = H5Dcreate(this->file_id, name, hDataType, hSpace, 
        H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    /* Write the first dataset. */
    hOK = H5Dwrite(hSet, hDataType, H5S_ALL, H5S_ALL, H5P_DEFAULT,
        data);
    write_event_info(hSet, ai);
    // hOK = H5LTmake_dataset(file_id,name ,1,dim,hDataType,data);
    /* Terminate access to the data space. */ 
    hOK = H5Dclose(hSet);
    hOK = H5Sclose(hSpace);

    return(hOK);
}


/// Creates a group for one single segment entity. 
///
/// This segment entity will
/// contain distinct groups for each data item and for each source info.
/// Each data item contains on his part two data sets: one for the time stamp and
/// one for the multi-channel data
///
/// - parameters:
///     - index as DWORD: global index of the current analog entity
///     - info as ns_SEGMENTINFO: neuroshare segment info
///     - info_source as ns_SEGSOURCEINFO: neuroshare segment source info
int CHdf_export::create_single_segment_group(DWORD index, ns_SEGMENTINFO info, ns_SEGSOURCEINFO *info_source)
{
    hid_t h;
    char name[32];
    sprintf_s(name, "/Segment/Segment%i", index);
    h= H5Gcreate(this->file_id, name,  H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    write_segment_info(h, info);
    sprintf_s(name, "/Segment/Source_Info%i", index);
    write_segment_sourceInfo(h, name, info_source, info.dwSourceCount);
    return(h);
}

/// Creates one single segment data item.
/// Each data item contains on his part two data sets: one for the time stamp and
/// one for the multi-channel data
///
/// - parameters:
///     - t as double: time stamp
///     - data as double*: pointer to data
///     - index as DWORD: global index of the current analog entity
///     - item as UINT: current item number
int CHdf_export::write_segment_item(double t, double *data, DWORD index, UINT item, 
                                    UINT sourceCount, UINT maxSampleCount)
{
    hid_t hSpace, hSet;     /* Memory datatype handle */
    hsize_t     dim[1]= {1};
    hsize_t		dimData[2]= {maxSampleCount, sourceCount};
    char name[128]; 

    sprintf_s(name, "/Segment/Segment%i/DataItem%i", index, item);
    H5Gcreate(this->file_id, name,  H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    sprintf_s(name, "/Segment/Segment%i/DataItem%i/Time_Stamp", index, item);
    hOK = H5LTmake_dataset(this->file_id,name,1,dim,H5T_NATIVE_DOUBLE,&t);
    hSpace = H5Screate_simple(2, dimData, NULL);
    sprintf_s(name, "/Segment/Segment%i/DataItem%i/Data", index, item);
    /* Create the dataset. */
    hSet = H5Dcreate(this->file_id, name, H5T_NATIVE_DOUBLE, hSpace, 
        H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    /* Write the first dataset. */
    hOK = H5Dwrite(hSet, H5T_NATIVE_DOUBLE, H5S_ALL, H5S_ALL, H5P_DEFAULT,
        (void*)data);
    ////write_event_info(hSet, ai);
    ////// hOK = H5LTmake_dataset(file_id,name ,1,dim,hDataType,data);
    /* Terminate access to the data space. */ 
    hOK = H5Dclose(hSet);
    hOK = H5Sclose(hSpace);

    return(hOK);
}


/// Writes the neuroshare event info and anchors it
/// to the specified handle.
/// - parameters:
///     - h: handle of the element to tag
///     - info: neuroshare event info
herr_t CHdf_export::write_event_info(hid_t h, ns_EVENTINFO info)
{
    hid_t s;
    herr_t      ioOK;
    /* Initialize the string field */
    s = H5Tcopy( H5T_C_S1 );
    H5Tset_size( s, 128 );

    ioOK= write_single_attribute(h, "dwEventType", H5T_NATIVE_UINT, (void*)&info.dwEventType);
    ioOK= write_single_attribute(h, "dwMinDataLength", H5T_NATIVE_UINT, (void*)&info.dwMinDataLength); 
    ioOK= write_single_attribute(h, "dwMaxDataLength", H5T_NATIVE_UINT, (void*)&info.dwMaxDataLength); 
    ioOK= write_single_attribute(h, "szCSVDesc", s, (void*)&info.szCSVDesc);

    return(ioOK);
}

/// Writes the neuroshare segment info and anchors it
/// to the specified handle.
/// - parameters:
///     - h: handle of the element to tag
///     - info: neuroshare segment info
herr_t CHdf_export::write_segment_info(hid_t h, ns_SEGMENTINFO info)
{
    hid_t s;
    herr_t      ioOK;
    /* Initialize the string field */
    s = H5Tcopy( H5T_C_S1 );
    H5Tset_size( s, 32 );

    ioOK= write_single_attribute(h, "dwSourceCount", H5T_NATIVE_UINT, (void*)&info.dwSourceCount);
    ioOK= write_single_attribute(h, "dwMinSampleCount", H5T_NATIVE_UINT, (void*)&info.dwMinSampleCount); 
    ioOK= write_single_attribute(h, "dwMaxSampleCount", H5T_NATIVE_UINT, (void*)&info.dwMaxSampleCount); 
    ioOK= write_single_attribute(h, "dSampleRate", H5T_NATIVE_DOUBLE, (void*)&info.dSampleRate); 
    ioOK= write_single_attribute(h, "szUnits", s, (void*)&info.szUnits);

    return(ioOK);
}

/// Creates a data set containing the neuroshare segment source info.
/// - parameters:
///     - h: handle of the element to tag
///     - name[]: name of the tag
///     - info: opinter to the segment source info
herr_t CHdf_export::write_segment_sourceInfo(hid_t h, char name[], ns_SEGSOURCEINFO *info,
                                             UINT number_of_sources)
{
    hid_t      s1_tid; //, dataset, space;     /* File datatype identifier */
    hid_t      s16, s128;
    hsize_t    dim[1] = {number_of_sources};   /* Dataspace dimensions */
    //hsize_t    dim[number_of_sources] = {1};   /* Dataspace dimensions */
    /* Initialize the field field_type */
    s16 = H5Tcopy( H5T_C_S1 );
    s128 = H5Tcopy( H5T_C_S1 );

    H5Tset_size( s16, 16 );
    H5Tset_size( s128, 128 );

    // Create the memory data type. 
    s1_tid = H5Tcreate (H5T_COMPOUND, sizeof(ns_SEGSOURCEINFO));
    H5Tinsert(s1_tid, "dMinVal", HOFFSET(ns_SEGSOURCEINFO, dMinVal), H5T_NATIVE_DOUBLE);
    H5Tinsert(s1_tid, "dMaxVal", HOFFSET(ns_SEGSOURCEINFO, dMaxVal), H5T_NATIVE_DOUBLE);
    H5Tinsert(s1_tid, "dResolution", HOFFSET(ns_SEGSOURCEINFO, dResolution), H5T_NATIVE_DOUBLE);
    H5Tinsert(s1_tid, "dSubSampleShift", HOFFSET(ns_SEGSOURCEINFO, dSubSampleShift), H5T_NATIVE_DOUBLE);
    H5Tinsert(s1_tid, "dLocationX", HOFFSET(ns_SEGSOURCEINFO, dLocationX), H5T_NATIVE_DOUBLE);
    H5Tinsert(s1_tid, "dLocationY", HOFFSET(ns_SEGSOURCEINFO, dLocationY), H5T_NATIVE_DOUBLE);
    H5Tinsert(s1_tid, "dLocationZ", HOFFSET(ns_SEGSOURCEINFO, dLocationZ), H5T_NATIVE_DOUBLE);
    H5Tinsert(s1_tid, "dLocationUser", HOFFSET(ns_SEGSOURCEINFO, dLocationUser), H5T_NATIVE_DOUBLE);
    H5Tinsert(s1_tid, "dHighFreqCorner", HOFFSET(ns_SEGSOURCEINFO, dHighFreqCorner), H5T_NATIVE_DOUBLE);
    H5Tinsert(s1_tid, "dwHighFreqOrder", HOFFSET(ns_SEGSOURCEINFO, dwHighFreqOrder), H5T_NATIVE_INT32);
    H5Tinsert(s1_tid, "szHighFilterType", HOFFSET(ns_SEGSOURCEINFO, szHighFilterType), s16);
    H5Tinsert(s1_tid, "dLowFreqCorner", HOFFSET(ns_SEGSOURCEINFO, dLowFreqCorner), H5T_NATIVE_DOUBLE);
    H5Tinsert(s1_tid, "dwLowFreqOrder", HOFFSET(ns_SEGSOURCEINFO, dwLowFreqOrder), H5T_NATIVE_INT32);
    H5Tinsert(s1_tid, "szLowFilterType", HOFFSET(ns_SEGSOURCEINFO, szLowFilterType), s16);
    H5Tinsert(s1_tid, "szProbeInfo", HOFFSET(ns_SEGSOURCEINFO, szProbeInfo), s128);

    hOK = H5LTmake_dataset(this->file_id,name,1,dim,s1_tid, info);
    H5Tclose(s1_tid);
    return(hOK);
}

/// Writes the neuroshare neural info and anchors it
/// to the specified handle.
/// - parameters:
///     - h: handle of the element to tag
///     - info: neuroshare neural info
herr_t CHdf_export::write_neural_info(hid_t h, ns_NEURALINFO info)
{
    hid_t s;
    herr_t      ioOK;
    /* Initialize the string field */
    s = H5Tcopy( H5T_C_S1 );
    H5Tset_size( s, 128 );

    ioOK= write_single_attribute(h, "dwSourceEntityID", H5T_NATIVE_UINT, (void*)&info.dwSourceEntityID);
    ioOK= write_single_attribute(h, "dwSourceUnitID", H5T_NATIVE_UINT, (void*)&info.dwSourceUnitID); 
    ioOK= write_single_attribute(h, "szProbeInfo", s, (void*)&info.szProbeInfo);

    return(ioOK);
}


/// Creates an attribute and anchors it to the specified element
/// - parameters:
///     - h: handle where to anchor the tag
///     - szName[]: name of the tag
///     - hdfType: data type in hdf5 notation
///     - data: pointer to the data
herr_t CHdf_export::write_single_attribute(hid_t h, char szName[], int hdfType, void* data)
{
    hid_t hSpace, hAttribute;
    hsize_t dims;
    herr_t      status;

    /* Create the data hSpace for the attribute. */
    dims = 1;
    hSpace = H5Screate_simple(1, &dims, NULL);

    /* Create a dataset attribute. */
    hAttribute = H5Acreate (h, szName, hdfType, hSpace, 
        H5P_DEFAULT, H5P_DEFAULT);
    /* Write the attribute data. */
    status = H5Awrite(hAttribute, hdfType, data);

    /* Close the attribute. */
    status = H5Aclose(hAttribute);

    /* Close the datahSpace. */
    status = H5Sclose(hSpace);
    return(status);
}

/// Writes the neuroshare analog info and anchors it
/// to the specified handle.
/// - parameters:
///     - h: handle of the element to tag
///     - info: neuroshare analog info
herr_t CHdf_export::write_analog_info(hid_t h, ns_ANALOGINFO info)
{
    hid_t s16, s128;
    herr_t      ioOK;
    /* Initialize the string field */
    s16 = H5Tcopy( H5T_C_S1 );
    H5Tset_size( s16, 16 );
    s128 = H5Tcopy( H5T_C_S1 );
    H5Tset_size( s128, 128 );

    write_single_attribute(h, "dSampleRate", H5T_NATIVE_DOUBLE, (void*)&info.dSampleRate);
    write_single_attribute(h, "dMinVal", H5T_NATIVE_DOUBLE, (void*)&info.dMinVal); 
    write_single_attribute(h, "dMaxVal", H5T_NATIVE_DOUBLE, (void*)&info.dMaxVal); 
    write_single_attribute(h, "szUnits", s16, (void*)&info.szUnits);
    write_single_attribute(h, "dResolution", H5T_NATIVE_DOUBLE, (void*)&info.dResolution); 
    write_single_attribute(h, "dLocationX", H5T_NATIVE_DOUBLE, (void*)&info.dLocationX); 
    write_single_attribute(h, "dLocationY", H5T_NATIVE_DOUBLE, (void*)&info.dLocationY); 
    write_single_attribute(h, "dLocationZ", H5T_NATIVE_DOUBLE, (void*)&info.dLocationZ); 
    write_single_attribute(h, "dLocationUser", H5T_NATIVE_DOUBLE,  (void*)&info.dLocationUser); 
    write_single_attribute(h, "dHighFreqCorner", H5T_NATIVE_DOUBLE, (void*)&info.dHighFreqCorner); 
    write_single_attribute(h, "dwHighFreqOrder", H5T_NATIVE_INT, (void*)&info.dwHighFreqOrder); 
    write_single_attribute(h, "szHighFilterType", s16, (void*)&info.szHighFilterType);
    write_single_attribute(h, "dLowFreqCorner", H5T_NATIVE_DOUBLE, (void*)&info.dLowFreqCorner); 
    write_single_attribute(h, "dwLowFreqOrder", H5T_NATIVE_INT, (void*)&info.dwLowFreqOrder); 
    write_single_attribute(h, "szLowFilterType", s16, (void*)&info.szLowFilterType); 
    ioOK= write_single_attribute(h, "szProbeInfo", s128, (char*)&info.szProbeInfo);

    return(ioOK);
}

/// This function writes one analog entity into the hdf5 file.
/// - parameters:
///     - data: pointer to analog data
///     - info: neuroshare neural info
///     - size: data size counted in elements (not bytes)
///     - index: current item index
int CHdf_export::write_neural(double *data, ns_NEURALINFO info, UINT size, UINT index)
{
    hid_t hSpace, hSet;
    hsize_t     dim[1]= {size};
    char name[128]; 
    sprintf_s(name, "/Neural/Neural%i", index);

    hSpace = H5Screate_simple(1, dim, NULL);
    /* Create the dataset. */
    hSet = H5Dcreate(this->file_id, name, H5T_NATIVE_DOUBLE, hSpace, 
        H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    /* Write the first dataset. */
    hOK = H5Dwrite(hSet, H5T_NATIVE_DOUBLE, H5S_ALL, H5S_ALL, H5P_DEFAULT,
        data);

    write_neural_info(hSet, info);
    /* Terminate access to the data space. */ 
    hOK = H5Dclose(hSet);
    hOK = H5Sclose(hSpace);

    return(hOK);
}

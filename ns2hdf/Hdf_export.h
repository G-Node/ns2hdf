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
// 
// All other copyrights on this material are replaced by this license agreeement.
//
///////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include <atlstr.h>
#include <hdf5.h>
#include <hdf5_hl.h>
#include <NsLibraryImpMgr.h>
#include <ns_common.h>
#include <NsFile.h>
#include <vector>


enum entityType {Unknown_Entity, Analog, Event, Segment, Neural};

/// structure for word sized event data
struct eventWord {
	double dTimeStamp;
	unsigned short usData;
    } ;
/// structure for byte sized event data
struct eventByte {
	double dTimeStamp;
	BYTE usData;
    } ;
/// structure for double word sized event data
struct eventDword {
	double dTimeStamp;
	DWORD usData;
    } ;

/// This class handles write operations on the hdf5 file.
/// It creates and closes the hdf5 file, writes data and attributes into it.
class CHdf_export
	{
	public:
		CHdf_export();
		~CHdf_export(void);

        /// Closes the hdf5 file
		void CHdf_export::closeFile(void);

        /// Creates a hdf5 file with the specified name
		int createFile(CString sourceFile, CString hdfFile);
    
        /// Creates a data set containing file info
        int write_fileInfo(ns_FILEINFO *fi);

        /// Creates the hdf5 basic groups: unknown, event, neural, segment, analog
		herr_t create_root_groups(entityType t); 

        /// Writes analog data in hdf5 format
		int write_analog(double* d, UINT n, DWORD index, ns_ANALOGINFO ai);

        /// Writes word sized event data in hdf5 format
		int write_event_word(eventWord *data,
							 UINT n, DWORD index, ns_EVENTINFO ai);

        /// Writes byte sized event data in hdf5 format
		int write_event_byte(eventByte *data,
							 UINT n, DWORD index, ns_EVENTINFO ai);

        /// Writes double word sized event data in hdf5 format
		int write_event_dword(eventDword *data,
							 UINT n, DWORD index, ns_EVENTINFO ai);
        /// Writes text or csv event data in hdf5 format
		int write_event_text(BYTE *data,
							 UINT n, DWORD index, ns_EVENTINFO ai);
        /// Creates a group for segment data
		int create_single_segment_group(DWORD index, ns_SEGMENTINFO info, 
            ns_SEGSOURCEINFO *info_source);

        /// Writes one single segment data in hdf5 format
		int write_segment_item(double t, double *data, DWORD index, UINT item, 
			UINT sourceCount, UINT maxSampleCount);

        /// Writes neural data in hdf5 format
        int write_neural(double *data, ns_NEURALINFO info, UINT size, UINT index);

    private:
        /// path to source file and to the targeted hdf5 file
		CString path_source, path_hdf;

		hid_t file_id;                  /// hdf5 file handle
		herr_t hOK;                     /// function result and error output

        /// creates a string data type with specified length
        hid_t get_string_type(UINT len); 

		/// writes one single attribute variable to a dataset
		herr_t write_single_attribute(hid_t hSet, char szName[], int hdfType, void* data);

        /// Writes event data and the event attribute into the hdf5 file
        int write_event(void *data, char name[], hid_t hDataType,
							 UINT n, ns_EVENTINFO ai);

        /// Writes the event attributes
		hid_t write_event_info(hid_t dataset_id, ns_EVENTINFO eventInfo);

        /// Writes the neural attributes
        herr_t write_neural_info(hid_t h, ns_NEURALINFO info);

        /// Writes the segment attributes
		herr_t write_segment_info(hid_t h, ns_SEGMENTINFO info);

        /// Writes the sources (channels) attributes used in segment entities
        herr_t write_segment_sourceInfo(hid_t h, char name[], ns_SEGSOURCEINFO *info, UINT number_of_sources);

        /// Writes the analog attributes
		herr_t write_analog_info(hid_t hSet, ns_ANALOGINFO info);


	};

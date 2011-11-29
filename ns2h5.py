#!/usr/bin/env python

import os, sys
import h5py
import neuroshare as ns
import numpy as np
import getopt

class ProgressIndicator(object):
    def __init__(self, offset=0):
        self._cur_value = offset

    def setup(self, max_value):
        self._max_value = max_value
        self.progress (self._max_value, 0)

    def __add__(self, other):
        self._cur_value += other
        self.progress (self._max_value, self._cur_value)
        return self

    def progress (self, max_value, cur_value):
        pass

class Converter(object):
    def __init__(self, filepath, output=None, progress=None):
        if not output:
            (basefile, ext) = os.path.splitext (filepath)
            output = "%s.hdf5" % basefile
            
        nf = ns.File(filepath)
        h5 = h5py.File(output, 'w')

        self._nf = nf
        self._h5 = h5
        self._groups = {}
        self.convert_map = {1 : self.convert_event,
                            2 : self.convert_analog,
                            3 : self.convert_segment,
                            4 : self.convert_neural}
        if not progress:
            progress = ProgressIndicator()
        self._progress = progress


    def get_group_for_type(self, entity_type):
        name_map = { 1 : 'Event',
                     2 : 'Analog',
                     3 : 'Segment',
                     4 : 'Neural'}

        if not self._groups.has_key(entity_type):
            name = name_map[entity_type]
            group = self._h5.create_group(name)
            self._groups[entity_type] = group

        return self._groups[entity_type]

    def convert(self):
        progress = self._progress
        progress.setup (len(self._nf.entities))
        for entity in self._nf.entities:
            conv = self.convert_map[entity.entity_type]
            conv(entity)
            progress + 1

        self._h5.close()

    def convert_event(self, event):
        dtype = self.dtype_by_event(event)
        nitems = event.item_count
        data = np.empty([nitems], dtype)
        for n in xrange(0, event.item_count):
            data[n] = event.get_data (n)

        group = self.get_group_for_type(event.entity_type)
        dset = group.create_dataset(event.label, data=data)

    def convert_analog(self, analog):
        (data, times, ic) = analog.get_data ()
        group = self.get_group_for_type(analog.entity_type)
        d_t =  np.vstack((times, data)).T
        dset = group.create_dataset(analog.label, data=d_t)
        dset.attrs['SampleRate'] = analog.sample_rate

    def convert_segment(self, segment):
        if not segment.item_count:
            return

        group = self.get_group_for_type(segment.entity_type)
        seg_group = group.create_group(segment.label)
        
        for index in xrange(0,segment.item_count):
            (data, timestamp, samples, unit) = segment.get_data (index)
            name = '%d - %f' % (index, timestamp)
            dset = seg_group.create_dataset(name, data=data.T)
            dset.attrs['Timestamp'] = timestamp
            dset.attrs['Unit'] = unit
            dset.attrs['Index'] = index

    def convert_neural(self, neural):
        data = neural.get_data ()
        group = self._groups[neural.entity_type]
        name = "%d - %s" % (neural.id, neural.label)
        group.create_dataset(name, data=data)

    def dtype_by_event(self, event):
        type_map = { ns.EventEntity.EVENT_TEXT  : 's',
                     ns.EventEntity.EVENT_CSV   : 's',
                     ns.EventEntity.EVENT_BYTE  : 'b',
                     ns.EventEntity.EVENT_WORD  : 'h',
                     ns.EventEntity.EVENT_DWORD : 'i'}
        val_type = type_map[event.event_type]
        if event.event_type < 2:
            val_type + event.max_data_length
        return np.dtype([('timestamp','d'),('value', val_type)])


class ConsoleIndicator(ProgressIndicator):
    def __init__(self):
        super(ConsoleIndicator, self).__init__()

    def progress(self,  max_value, cur_value):
        size = 60
        prefix = "Converting"
        x = int (size*cur_value/max_value)
        sys.stdout.write("%s [%s%s] %i/%i\r" % (prefix, "#"*x, "." * (size-x),
                                                cur_value, max_value))
        sys.stdout.flush()

def main(argv):
    opts, rem = getopt.getopt(sys.argv[1:], 'o:', ['output=',
                                                   'version=',
                                                   ])
    output = None
    for opt, arg in opts:
        if opt in ("-o", "--output"):
            output = arg

    if len (rem) != 1:
        print "Wrong number of arguments"
        return -1;

    filename = rem[0]
    ci = ConsoleIndicator()
    converter = Converter(filename, output, progress=ci);
    converter.convert()
    return 0

if __name__ == "__main__":
    res = main(sys.argv[1:])
    sys.exit(res)

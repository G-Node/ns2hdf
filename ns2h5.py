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
            output = "%s.hdf5" % (basefile)

        nf = ns.File(filepath)
        h5 = h5py.File(output, 'w')

        self._nf = nf
        self._h5 = h5
        self._groups = {}
        self._groups[1] = h5.create_group('Event')
        self._groups[2] = h5.create_group('Analog')
        self._groups[3] = h5.create_group('Segment')
        self._groups[4] = h5.create_group('Neural')
        self.convert_map = {1 : self.convert_event,
                            2 : self.convert_analog,
                            3 : self.convert_segment,
                            4 : self.convert_neural}
        if not progress:
            progress = ProgressIndicator()
        self._progress = progress

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
        if nitems > 0:
            group = self._groups[event.entity_type]
            dset = group.create_dataset(event.label, data=data)

    def convert_analog(self, analog):
        (data, times, ic) = analog.get_data ()
        group = self._groups[analog.entity_type]
        d_t =  np.vstack((times, data)).T
        dset = group.create_dataset(analog.label, data=d_t)
        dset.attrs['SampleRate'] = analog.sample_rate

    def convert_segment(self, segment):
        (data, timestamp, samples, unit) = segment.get_data (0)
        #print data, timestamp, samples, unit

    def convert_neural(self, neural):
        print "FIXME: neural"

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

#!/usr/bin/env python

import sys
import struct
import subprocess
from io import BytesIO

wave_str_fmt = """=== {} ===

chunk_id = {}
chunk_size = {}
format = {}

subchunk1_id = {}
subchunk1_size = {}
audio_format = {}
num_channels = {}
sample_rate = {}
byte_rate = {}
block_align = {}
bits_per_sample = {}

subchunk2_id = {}
subchunk2_size = {}
data = {}
"""

class WaveFile(object):

    def __init__(self, path):
        self.path = path

        with open(path, "rb") as f:
            # riff chunk
            chunk = f.read(12)
            self.chunk_id = chunk[0:4]
            self.chunk_size = struct.unpack("<I", chunk[4:8])[0]
            self.format = chunk[8:12]

            # sanity checks
            if self.chunk_id != "RIFF":
                raise RuntimeError("Bad chunk id in WAVE header. Expected RIFF but found " + self.chunk_id)

            if self.format != "WAVE":
                raise RuntimeError("Bad format in WAVE header. Expected WAVE but found " + self.format)

            # fmt chunk
            chunk = f.read(24)
            self.subchunk1_id = chunk[0:4]
            self.subchunk1_size = struct.unpack("<I", chunk[4:8])[0]
            self.audio_format = struct.unpack("<H", chunk[8:10])[0]
            self.num_channels = struct.unpack("<H", chunk[10:12])[0]
            self.sample_rate = struct.unpack("<I", chunk[12:16])[0]
            self.byte_rate = struct.unpack("<I", chunk[16:20])[0]
            self.block_align = struct.unpack("<H", chunk[20:22])[0]
            self.bits_per_sample = struct.unpack("<H", chunk[22:24])[0]

            if self.audio_format != 1:
                raise RuntimeError("Only audio format 1 (PCM) is supported.")

            if self.num_channels != 2:
                raise RuntimeError("Only 2 channels are supported by the music player.")

            # fmt chunk
            chunk = f.read(8)
            self.subchunk2_id = chunk[0:4]
            self.subchunk2_size = struct.unpack("<I", chunk[4:8])[0]
            self.data = f.read(self.subchunk2_size)

            # sanity check
            chunk_size = (4
                          + (8 + self.subchunk1_size)
                          + (8 + self.subchunk2_size))
            if self.chunk_size != chunk_size:
                raise RuntimeError(("Chunk size mismatch. Expected {}, but got {}. "
                                    "Make sure there is no meta-data present in "
                                    "the data field.").format(chunk_size,
                                                              self.chunk_size))

            if self.bits_per_sample == 8:
                self.unpack_read_size = 1
                self.unpack_format = "B"
            else:
                self.unpack_read_size = 2
                self.unpack_format = "H"

    def samples(self):
        """Iterate over all data samples in the file.
        """

        reader = BytesIO(self.data)
        while True:
            sample = reader.read(self.unpack_read_size)
            if sample == '':
                break
            ch0 = struct.unpack("<" + self.unpack_format, sample)[0]
            ch1 = None
            if self.num_channels == 2:
                sample = reader.read(self.unpack_read_size)
                ch1 = struct.unpack("<" + self.unpack_format, sample)[0]
            yield ch0, ch1

    def __str__(self):
        """String representation of the WAVE file.
        """

        # output at most 16 bytes of data
        size = min(16, len(self.data))
        data = ' '.join([b.encode('hex') for b in self.data[0:size]])
        if len(self.data) > size:
            data += ' ...'

        return wave_str_fmt.format(self.path,
                                   self.chunk_id,
                                   self.chunk_size,
                                   self.format,
                                   self.subchunk1_id,
                                   self.subchunk1_size,
                                   self.audio_format,
                                   self.num_channels,
                                   self.sample_rate,
                                   self.byte_rate,
                                   self.block_align,
                                   self.bits_per_sample,
                                   self.subchunk2_id,
                                   self.subchunk2_size,
                                   data)


if __name__ == "__main__":
    path = sys.argv[1]

    if path.endswith(".mp3"):
        print "Using sox to create a wav file from given mp3 file."
        mp3path = path
        path = path[:-4] + ".wav"
        cmd = ["sox", "-b", "16", mp3path, path, "rate", "44100", "channels", "2"]
        print ' '.join(cmd)
        subprocess.check_call(cmd)

    wave_file = WaveFile(path)

    print wave_file

    # convert all samples to 12 bits, suitable for Arduino Due DAC
    path_b44 = path[:-4] + ".b44"
    print "Creating " + path_b44
    with open(path_b44, "wb") as f:
        for ch0, ch1 in wave_file.samples():
            if wave_file.bits_per_sample == 8:
                # Upconvert the sample from 8 to 12 bits by shifting
                # it 4 positions to the left.
                if ch1 is not None:
                    f.write(struct.pack("<H", (ch1 << 4) | 0x1000))
                f.write(struct.pack("<H", ch0 << 4))
            else:
                # Convert the sample to the range 0..65535 by adding
                # 0x8000 and then downconvert it from 16 to 12 bits by
                # shifting it 4 positions to the right.
                if ch1 is not None:
                    f.write(struct.pack("<H", (((ch1 + 0x8000) >> 4) & 0xfff) | 0x1000))
                f.write(struct.pack("<H", ((ch0 + 0x8000) >> 4) & 0xfff))

Music Player
============

A minimalistic stereo music player.

Hardware
--------

- Arduino Due.
- Two speakers.
- SD card or USB memory.

Setup
-----

1. Connect the left speaker to DAC0 and the right speaker to DAC1.

2. Plug in a SD card or USB memory to your PC and format it to FAT16.

3. Copy songs (.b44 files) to the root folder of the SD card or USB
   memory.

4. Unplug the SD card or USB memory from your PC and attach it to the
   Arduino Due.

Convertings songs
-----------------

All mp3-files in the folder songs can be converted to the .b12-format
with the command ``tools/convert.py <song>``. Copy the created .b12
file to a SD card or USB memory and connect it to the music player.

The format of the songs must be the expected format by the DAC
hardware as the samples are . For Arduino Due, one 12 bits sample for
each channel tagged with the channel number in the 13th bit are packed
into a 32 bits number. Use ``tools/convert.py`` to convert
uncompressed wav or mp3 files to this format.

Example output
--------------

Example shell commands to list, play, pause, resume and stop songs.

.. code:: text

   $ list
   NUMBER            NAME  LENGTH
        1       MAMMA MIA    3:45
        2           50 50    4:21
        3      IN THE C~1    2:57
   $ play
   Playing | MAMMA MIA
   Playing | 50 50
   $ play 3
   Playing | IN THE C~1
   $ pause
   Paused  | IN THE C~1
   $ play
   Playing | IN THE C~1
   $ play 2
   Playing | 50 50
   $ stop
   Stopped | 50 50
   $ play
   Playing | 50 50
   $ stop
   Stopped | 50 50

Prerequisites
-------------

.. code:: text

    sudo apt-get install sox libsox-fmt-mp3

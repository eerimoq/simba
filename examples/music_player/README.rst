MUSIC PLAYER
============

A minimalistic stereo music player.

This application needs two speakers, one connected to DAC0 and the
other to DAC1, and a SD card with songs in the root folder. The format
of the songs must be the expected format by the DAC hardware. For
Arduino Due, one 12 bits sample for each channel tagged with the
channel number in the 13th bit are packed into a 32 bits number. Use
music_player.py to convert uncompressed wav-files to this format.

Example shell commands to list, play, pause, resume and stop songs.

    $ list
    NUMBER            NAME  LENGTH
         1       MAMMA MIA    3:45
         2           50 50    4:21
         3      IN THE C~1    2:57
    $ play
    Playing 'MAMMA MIA'.
    Playing '50 50'.
    $ play 3
    Playing 'IN THE C~1'.
    $ pause
    Pausing 'IN THE C~1'.
    $ play
    Resuming 'IN THE C~1'.
    $ play 2
    Playing '50 50'.
    $ stop
    Stopping '50 50'.
    $ play 2
    Playing '50 50'.
    $ stop
    Stopping '50 50'.

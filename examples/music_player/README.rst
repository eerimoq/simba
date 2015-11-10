MUSIC PLAYER
============

A minimalistic music player.

This application needs a speaker connected to DAC1 and a SD card with
songs in the root folder. The format of the songs must be the extepced
format by the DAC hardware, often 16 bits samples.

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

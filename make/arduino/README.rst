Arduino build system
====================

Generate and release Arduino IDE build system files and folders. It's
a two step process, first generate the the files and folders to a
temporary folder. The next step is to copy the files from the
generated folder o the git repositories, commit and, push to github
and download the release archives to calculate the checksums.

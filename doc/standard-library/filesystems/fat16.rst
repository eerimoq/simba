:mod:`fat16` --- FAT16 filesystem
=================================

.. module:: fat16
   :synopsis: FAT16 filesystem.

File Allocation Table (FAT) is a computer file system architecture and
a family of industry-standard file systems utilizing it. The FAT file
system is a legacy file system which is simple and robust. It offers
good performance even in light-weight implementations, but cannot
deliver the same performance, reliability and scalability as some
modern file systems. It is, however, supported for compatibility
reasons by nearly all currently developed operating systems for
personal computers and many mobile devices and embedded systems, and
thus is a well-suited format for data exchange between computers and
devices of almost any type and age from 1981 up to the present.

Example
-------

Here is the pseudo-code for mounting a file system, performing file
operations and unmounting the file system.

All function arguments are omitted in this example.

.. code-block:: c

   /* Mount the file system. This is normally done once when the
      application starts. */
   fat16_init();
   fat16_mount();

   /* Perform file operations. */
   fat16_file_open();
   fat16_file_read();
   fat16_file_close();

   fat16_file_open();
   fat16_file_write();
   fat16_file_close();

   /* Unmount the file system when it is no long needed. Normally when
      the application stops. */
   fat16_unmount();

---------------------------------------------------

Source code: :github-blob:`src/filesystems/fat16.h`, :github-blob:`src/filesystems/fat16.c`

Test code: :github-blob:`tst/filesystems/fat16/main.c`

Test coverage: :codecov:`src/filesystems/fat16.c`

Example code: :github-blob:`examples/fat16/main.c`

---------------------------------------------------

.. doxygenfile:: filesystems/fat16.h
   :project: simba

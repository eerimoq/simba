Limitations
-----------

- No floating point support.

- Subset of libc because newlib in the toolchain seems to make
  unaligned memory accesses. At least it does not work.

  How to rebuild the toolchain with -mstrict-align (and possible
  more)?

Toolchain
---------

Download `Linaro GCC 7.2.1 with newlib`_ and `Linaro GCC 7.2.1 for
Linux`_ unpack them in ``SIMBA_ROOT/..``.

.. _Linaro GCC 7.2.1 with newlib: https://releases.linaro.org/components/toolchain/binaries/7.2-2017.11/aarch64-elf/gcc-linaro-7.2.1-2017.11-x86_64_aarch64-elf.tar.xz

.. _Linaro GCC 7.2.1 for Linux: https://releases.linaro.org/components/toolchain/binaries/7.2-2017.11/aarch64-linux-gnu/gcc-linaro-7.2.1-2017.11-x86_64_aarch64-linux-gnu.tar.xz

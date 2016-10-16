Releasing
=========

Follow these steps to create a new release:

1. Write the new version in ``VERSION.txt``. The version should hace
   the format ``<major>.<minor>.<revision>``.

   Increment ``<major>`` for non-backwards compatible changes.

   Increment ``<minor>`` for new features.

   Increment ``<revision>`` for bug fixes.

2. Write the new version in ``package.json``. This file is used by
   `PlatformIO 3` to find the current `Simba` release.

3. Run the test suites and generate the documentation.

   .. code:: text

      make test-all-boards
      make release-test

4. Generate files for Arduino.

   .. code:: text

      make arduino

5. Add the new releases to
   ``make/arduino/<family>/package_simba_<family>_index.json``. The
   sha256 sums of the zip-archives are calculated by ``make arduino``
   and written to ``simba-arduino/*.sha256``.

6. Make sure that the blink exmaple works in the Arduino IDE.

7. Commit the changes, and tag the commit with the new version.

8. Push the new commit and tag.

9. Copy the Simba Arduino releases to SourceForge.

   .. code:: text

      scp simba-arduino/simba-arduino-avr-*.zip <user>@frs.sourceforge.net:/home/frs/project/simba-arduino/avr
      scp simba-arduino/simba-arduino-sam-*.zip <user>@frs.sourceforge.net:/home/frs/project/simba-arduino/sam
      scp simba-arduino/simba-arduino-esp-*.zip <user>@frs.sourceforge.net:/home/frs/project/simba-arduino/esp
      scp make/arduino/avr/package_simba_avr_index.json <user>@frs.sourceforge.net:/home/frs/project/simba-arduino/avr
      scp make/arduino/sam/package_simba_sam_index.json <user>@frs.sourceforge.net:/home/frs/project/simba-arduino/sam
      scp make/arduino/esp/package_simba_esp_index.json <user>@frs.sourceforge.net:/home/frs/project/simba-arduino/esp

10. Download the release zip-file from Github and calculate its SHA1
   checksum. Upload the zip-file to sourceforge and add the new
   releases to ``make/platformio/manifest.json``.

   .. code:: text

      wget https://github.com/eerimoq/simba/archive/<version>.zip
      sha1sum <version>.zip
      scp <version>.zip <user>@frs.sourceforge.net:/home/frs/project/simba-platformio/

11. Commit and push.

12. Done.

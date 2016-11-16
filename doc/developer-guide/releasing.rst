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

6. Copy the Simba Arduino releases to the release repository, add,
   commit and push in the release repository.

   .. code:: text

      cp simba-arduino/simba-arduino-avr-*.zip ../simba-releases/arduino/avr
      cp simba-arduino/simba-arduino-sam-*.zip ../simba-releases/arduino/sam
      cp simba-arduino/simba-arduino-esp-*.zip ../simba-releases/arduino/esp
      cp simba-arduino/simba-arduino-esp32-*.zip ../simba-releases/arduino/esp32

7. Start a http server used to download package manifests in the Arduino IDE.

   .. code:: text

      (cd make/arduino && python -m SimpleHTTPServer)

8. Start the Arduino IDE and add these URL:s in Preferences.

   .. code:: text

      http://localhost:8000/avr/package_simba_avr_index.json
      http://localhost:8000/esp/package_simba_esp_index.json
      http://localhost:8000/esp32/package_simba_esp32_index.json
      http://localhost:8000/sam/package_simba_sam_index.json

9. Install all four packages and run the blink example for each one of
   them.
      
10. Commit the changes, and tag the commit with the new version.

11. Push the new commit and tag.

12. Copy the Simba Arduino package manifests the release repository,
   add, commit and push in the release repository.

   .. code:: text

      cp make/arduino/avr/package_simba_avr_index.json ../simba-releases/arduino/avr
      cp make/arduino/sam/package_simba_sam_index.json ../simba-releases/arduino/sam
      cp make/arduino/esp/package_simba_esp_index.json ../simba-releases/arduino/esp
      cp make/arduino/esp32/package_simba_esp32_index.json ../simba-releases/arduino/esp32

13. Download the release zip-file from Github and calculate its SHA1
    checksum. Add the zip-file to the release repository and add the
    new releases to ``make/platformio/manifest.json``.

   .. code:: text

      wget https://github.com/eerimoq/simba/archive/<version>.zip
      sha1sum <version>.zip
      cp <version>.zip ../simba-releases/platformio

14. Commit and push.

15. Done.

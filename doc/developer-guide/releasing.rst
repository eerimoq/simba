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

3. Run the test suites and generate the documentation and other files.

   .. code:: text

      make -s -j8 test-all-boards
      make -s -j8 release-test

4. Commit the generated files and tag the commit with
   ``<major>.<minor>.<revision>``.

5. Generate files for Arduino and PlatformIO releases. The generated
   archives and Arduino manifests are copied to the release
   repository.

   .. code:: text

      make -s release

6. Add, commit and push the Simba Arduino releases in the release
   repository.

   .. code:: text

      (cd ../simba-releases && \
       git add arduino/*/*.zip platformio/*.zip && \
       git commit && \
       git push origin master)

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

10. Commit and push.

11. Add, commit and push the Simba Arduino package manifests in the
    release repository.

   .. code:: text

      (cd ../simba-releases && \
       git add arduino/*/*.json && \
       git commit && \
       git push origin master)

12. Done.

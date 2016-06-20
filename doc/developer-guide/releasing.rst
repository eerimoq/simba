Releasing
=========

Follow these steps to create a new release:

1. Write the new version in ``VERSION.txt``. The version should hace
   the format ``<major>.<minor>.<revision>``.

   Increment ``<major>`` for non-backwards compatible changes.

   Increment ``<minor>`` for new features.

   Increment ``<revision>`` for bug fixes.

2. Update file headers.

   .. code:: text

      for f in $(git ls-files | xargs) ; do
          sed -i 's/<version>/<new version>/g' $f
      done

3. Run the test suites and generate the documentation.

   .. code:: text

      make release-test

4. Commit the changes, and tag the commit with the new version.

5. Generate files for Arduino and copy them to the Arduino release
   area. Also add the new release to
   ``../simba-arduino/<family>/package_simba_<family>_index.json``. The
   sh256 sums of the zip-archives are calculated by ``make arduino``
   and written to ``simba-arduino/*.sha256``.

   .. code:: text

      make arduino
      cp simba-arduino/simba-arduino-avr-*.zip ../simba-arduino/avr
      cp simba-arduino/simba-arduino-sam-*.zip ../simba-arduino/sam

6. Commit the Arduino changes in ``../simba-arduino``.

7. Push new commits and tags in both repositories.

8. Done.

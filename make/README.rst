Arduino
=======

Generate the Arduino IDE files.

.. code:: text

   cd ..
   make arduino

Copy the generated files to the release repositories.

.. code:: text

   rm -rf ../simba-arduino-avr/* && \
   cp -r simba-arduino/avr/* ../simba-arduino-avr/ && \
   rm -rf ../simba-arduino-sam/* && \
   cp -r simba-arduino/sam/* ../simba-arduino-sam/

Commit and push the changes.

Fetch the released archives and calculate the checksum.

.. code:: text

   wget https://github.com/eerimoq/simba-arduino-avr/archive/simba-arduino-avr-0.6.0.zip
   sha256sum simba-arduino-avr-0.6.0.zip
   rm simba-arduino-avr-0.6.0.zip
   wget https://github.com/eerimoq/simba-arduino-sam/archive/simba-arduino-sam-0.6.0.zip
   sha256sum simba-arduino-sam-0.6.0.zip
   rm simba-arduino-sam-0.6.0.zip

Replace the sha256 sum in package_simba_<arch>_index.json with the
generated value.

Commit and push the file.

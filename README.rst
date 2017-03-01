|gitter|_
|buildstatus|_
|codecov|_
|coverity|_
|donate|_

Simba
=====

This is the source code repository of the Simba Embedded Programming
Platform. See http://simba-os.readthedocs.org for further details.

Simba is written in C.

Don't hesitate to create issues or pull requests if you want to
improve Simba!

*“We don't make mistakes, we just have happy accidents.”*
― Bob Ross

Try it out!
===========

1. Download the `Arduino IDE`_ and install Simba using the Boards Manager.

   .. code-block:: text

      https://raw.githubusercontent.com/eerimoq/simba-releases/master/arduino/avr/package_simba_avr_index.json
      https://raw.githubusercontent.com/eerimoq/simba-releases/master/arduino/sam/package_simba_sam_index.json
      https://raw.githubusercontent.com/eerimoq/simba-releases/master/arduino/esp/package_simba_esp_index.json
      https://raw.githubusercontent.com/eerimoq/simba-releases/master/arduino/esp32/package_simba_esp32_index.json

2. Select a Simba board.
3. Open the blink example.
4. Upload!

See the `Simba installation documentation`_ for detailed step-by-step instructions.

Need help?
==========

1. Have a look at the `Simba documentation`_. It's actually pretty good. =)

2. Visit the `Simba forum`_.

3. Write an issue here on GitHub.

License
=======

Simba is licensed under the MIT License. See LICENSE for a copy of the
licence. Third party source code and libraries that Simba depends on
may have other licenses. Most third party code is placed in the
``3pp`` folder.

.. |buildstatus| image:: https://travis-ci.org/eerimoq/simba.svg
.. _buildstatus: https://travis-ci.org/eerimoq/simba

.. |gitter| image:: https://badges.gitter.im/eerimoq/simba.svg
.. _gitter: https://gitter.im/eerimoq/simba

.. |codecov| image:: https://codecov.io/gh/eerimoq/simba/branch/master/graph/badge.svg
.. _codecov: https://codecov.io/gh/eerimoq/simba

.. |coverity| image:: https://scan.coverity.com/projects/11324/badge.svg
.. _coverity: https://scan.coverity.com/projects/eerimoq-simba

.. |donate| image:: https://img.shields.io/badge/paypal-donate-yellow.svg
.. _donate: https://www.paypal.com/cgi-bin/webscr?cmd=_donations&business=X2NWV6RHV58GC&lc=SE&item_name=Simba%20OS%20and%20Build%20Framework&currency_code=USD&bn=PP%2dDonationsBF%3abtn_donateCC_LG%2egif%3aNonHosted

.. _Arduino IDE: https://www.arduino.cc/en/Main/Software
.. _Simba installation documentation: http://simba-os.readthedocs.io/en/latest/getting-started.html#arduino-arduino-ide
.. _Simba documentation: http://simba-os.readthedocs.io/en/latest
.. _Simba forum: http://forum.simbaos.org

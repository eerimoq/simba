#!/usr/bin/env python

"""Release handling.

"""

from __future__ import print_function

import sys
import os
import subprocess
import glob
import argparse
import hashlib
import json
import shutil
from collections import OrderedDict

PACKAGE_AVR_RELEASE_ENTRY_FMT = """
{{
    "name": "Simba",
    "architecture": "avr",
    "version": "{version}",
    "category": "Simba",
    "help": {{
        "online": "https://simba-os.readthedocs.org"
    }},
    "url": "https://github.com/eerimoq/simba-releases/raw/master/arduino/avr/simba-arduino-avr-{version}.zip",
    "archiveFileName": "simba-arduino-avr-{version}.zip",
    "checksum": "SHA-256:{sha256sum}",
    "size": "{size}",
    "boards": [
        {{"name" : "Arduino Mega 2560"}},
        {{"name" : "Arduino Nano"}},
        {{"name" : "Arduino Pro Micro"}},
        {{"name" : "Arduino Uno"}}
    ],
    "toolsDependencies": [
        {{
            "packager": "arduino",
            "name": "avr-gcc",
            "version": "4.9.2-atmel3.5.3-arduino2"
        }},
        {{
            "packager": "arduino",
            "name": "avrdude",
            "version": "6.0.1-arduino5"
        }}
    ]
}}
"""

PACKAGE_ESP_RELEASE_ENTRY_FMT = """
{{
    "name": "Simba",
    "architecture": "esp",
    "version": "{version}",
    "category": "Simba",
    "help": {{
        "online": "https://simba-os.readthedocs.org"
    }},
    "url": "https://github.com/eerimoq/simba-releases/raw/master/arduino/esp/simba-arduino-esp-{version}.zip",
    "archiveFileName": "simba-arduino-esp-{version}.zip",
    "checksum": "SHA-256:{sha256sum}",
    "size": "{size}",
    "boards": [
        {{"name" : "ESP-01"}},
        {{"name" : "ESP-12E"}},
        {{"name" : "NodeMCU"}},
        {{"name" : "Huzzah"}}
    ],
    "toolsDependencies": [
        {{
            "packager": "Simba",
            "version": "1.20.0-26-gb404fb9-2",
            "name": "xtensa-lx106-elf-gcc"
        }},
        {{
            "packager": "Simba",
            "version": "0.4.9",
            "name": "esptool"
        }}
    ]
}}"""

PACKAGE_ESP32_RELEASE_ENTRY_FMT = """
{{
    "name": "Simba",
    "architecture": "esp32",
    "version": "{version}",
    "category": "Simba",
    "help": {{
        "online": "https://simba-os.readthedocs.org"
    }},
    "url": "https://github.com/eerimoq/simba-releases/raw/master/arduino/esp32/simba-arduino-esp32-{version}.zip",
    "archiveFileName": "simba-arduino-esp32-{version}.zip",
    "checksum": "SHA-256:{sha256sum}",
    "size": "{size}",
    "boards": [
        {{"name" : "Nano32"}},
        {{"name" : "ESP32-DevKitC"}},
        {{"name" : "Maple-ESP32"}}
    ],
    "toolsDependencies": [
        {{
            "packager": "Simba",
            "version": "1.22.0-59",
            "name": "xtensa-esp32-elf-gcc"
        }}
    ]
}}"""

PACKAGE_SAM_RELEASE_ENTRY_FMT = """
{{
  "name": "Simba",
  "architecture": "sam",
  "version": "{version}",
  "category": "Simba",
  "help": {{
    "online": "https://simba-os.readthedocs.org"
  }},
  "url": "https://github.com/eerimoq/simba-releases/raw/master/arduino/sam/simba-arduino-sam-{version}.zip",
  "archiveFileName": "simba-arduino-sam-{version}.zip",
  "checksum": "SHA-256:{sha256sum}",
  "size": "{size}",
  "boards": [
      {{"name" : "Arduino Due"}}
  ],
  "toolsDependencies": [
    {{
      "packager": "arduino",
      "name": "arm-none-eabi-gcc",
      "version": "4.8.3-2014q1"
    }},
    {{
      "packager": "arduino",
      "name": "bossac",
      "version": "1.6.1-arduino"
    }}
  ]
}}"""

PACKAGE_RELEASE_ENTRY_FMTS = {
    'avr': PACKAGE_AVR_RELEASE_ENTRY_FMT,
    'esp': PACKAGE_ESP_RELEASE_ENTRY_FMT,
    'esp32': PACKAGE_ESP32_RELEASE_ENTRY_FMT,
    'sam': PACKAGE_SAM_RELEASE_ENTRY_FMT
}

def git_clean_dfx():
    """Clean.

    """

    command = [
        'git',
        'submodule',
        'foreach',
        '--recursive',
        'git',
        'clean',
        '-dfx']
    subprocess.check_call(command)

    command = [
        'git',
        'clean',
        '-dfx']
    subprocess.check_call(command)


def git_check_no_modified_files():
    """Make sure that there are no modified files.

    """

    command = [
        'git',
        'submodule',
        'foreach',
        '--recursive',
        'git',
        'diff',
        '--quiet',
        'HEAD']
    subprocess.check_call(command)

    command = [
        'git',
        'diff',
        '--quiet',
        'HEAD']
    subprocess.check_call(command)


def test():
    """Test.

    """

    all_boards = glob.glob("src/boards/*")
    boards = set(all_boards) - set("cygwin")

    for board in boards:
        git_clean_dfx()

        # Building one application is enough to ensure that all code
        # in src/ compiles.
        command = [
            "make",
            "APPS=examples/default-configuration",
            "BOARD=" + os.path.basename(board),
            "all"
        ]

        print(" ".join(command))

        subprocess.check_call(command)

    # Run linux tests.
    git_clean_dfx()

    command = [
        "make",
        "test"
    ]

    print(" ".join(command))

    subprocess.check_call(command)

    generate_platformio()

    # Build a PlatformIO application.
    command = [
        "platformio",
        "run",
        "-v"
    ]

    subprocess.check_call(command, cwd="examples/platformio/blink")

    generate_arduino()

    # Build an application using the Arduino builder.
    for family, board in [("avr", "nano"),
                          ("avr", "uno"),
                          ("avr", "mega2560"),
                          ("avr", "pro-micro"),
                          ("sam", "arduino_due_x_dbg"),
                          ("esp", "esp01"),
                          ("esp", "esp12e"),
                          ("esp32", "nano32"),
                          ("esp32", "esp32_devkitc"),
                          ("esp32", "maple_esp32")]:
        command = [
            "make",
            "all",
            "FAMILY=" + family,
            "BOARD=" + board
        ]

        subprocess.check_call(command, cwd="examples/arduino/blink")


def generate_docs():
    """Generate the documentation.

    """

    command = [
        "make",
        "doc"
    ]

    subprocess.check_call(command)


def build_examples():
    """Build the examples.

    """

    git_clean_dfx()

    command = [
        "make"
    ]

    subprocess.check_call(command, cwd="examples")


def generate_platformio():
    """Generate the platformio files.

    """

    command = [
        "make",
        "platformio"
    ]

    subprocess.check_call(command)


def generate_arduino():
    """Generate the arduino files.

    """

    command = [
        "make",
        "arduino"
    ]

    subprocess.check_call(command)


def package(version):
    """Create the release artifacts for Arduino IDE and PlatformIO.

    """
    
    git_clean_dfx()
    try:
        git_check_no_modified_files()
    except:
        print('Modified files under version control are not allowed '
              'when creating a release.')
        sys.exit(1)
    
    # Create the PlatformIO package.
    package_name = version + '.zip'
    command = [
        'git-archive-all',
        '--prefix', 'simba-' + version,
        package_name
    ]
    print('Creating', package_name)
    subprocess.check_call(command)

    # Calculate the SHA1 checksum of the archive.
    with open(package_name, 'rb') as f:
        sha1sum = hashlib.sha1(f.read()).hexdigest()
    print('sha1sum of {} is {}:'.format(package_name, sha1sum))
    
    # Add the release to the PlatformIO manifest.
    manifest_name = 'make/platformio/manifest.json'
    with open(manifest_name) as f:
        data = json.load(f, object_pairs_hook=OrderedDict)

    release_entry = OrderedDict([
        ('url', 'https://github.com/eerimoq/simba-releases/raw/master/platformio/{}.zip'.format(version)),
        ('sha1', sha1sum),
        ('version', version)
    ])
    data['framework-simba'].insert(0, release_entry)

    with open(manifest_name, 'w') as f:
        f.write(json.dumps(data, indent=4, separators=(',', ': ')))

    shutil.copy(package_name, '../simba-releases/platformio')

    # Create the arduino IDE release
    subprocess.check_call([
        'make/arduino/arduino.py',
        '--remove-outdir',
        '--version', version
    ])

    for family in ['avr', 'esp', 'esp32', 'sam']:
        # Add the release to the Arduino IDE manifests.
        manifest_name = 'make/arduino/{family}/package_simba_{family}_index.json'.format(
            family=family)

        # Calculate the SHA256 checksum of the archive.
        package_name = 'simba-arduino/simba-arduino-{}-{}.zip'.format(
            family, version)

        with open(package_name) as f:
            sha256sum = hashlib.sha256(f.read()).hexdigest()

        print('sha256sum of {} is {}.'.format(package_name, sha256sum))

        # Create the release entry.
        release_entry_formatted = PACKAGE_RELEASE_ENTRY_FMTS[family].format(
            version=version,
            sha256sum=sha256sum,
            size=os.stat(package_name).st_size)
        release_entry = json.loads(release_entry_formatted, object_pairs_hook=OrderedDict)
    
        # Insert the release entry into the manifest.
        with open(manifest_name) as f:
            data = json.load(f, object_pairs_hook=OrderedDict)

        data['packages'][0]['platforms'].append(release_entry)
    
        with open(manifest_name, 'w') as f:
            f.write(json.dumps(data, indent=4, separators=(',', ': ')))

        shutil.copy(package_name, '../simba-releases/arduino/' + family)
        shutil.copy(manifest_name, '../simba-releases/arduino/' + family)

    
def main():
    """Main function.

    """

    parser = argparse.ArgumentParser()
    parser.add_argument('--test', action='store_true')
    parser.add_argument('--package', action='store_true')
    parser.add_argument('--version', required=True)
    args = parser.parse_args()

    # first of all, the repository must be clean
    raw_input("ATTENTION: All files not version controlled by git will be "
              "_removed_. Press any key to continue or ctrl-c to exit.")
    print()

    git_clean_dfx()

    if args.test:
        test()
        build_examples()

        # Generate the documenation after the last "git clean -dfx"
        # execution.
        generate_docs()

    if args.package:
        package(args.version)


if __name__ == "__main__":
    main()

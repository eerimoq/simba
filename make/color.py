#!/usr/bin/env python

import sys
import re

text = sys.stdin.read()

text = re.sub('PASSED', '\033[0;32mPASSED\033[0m', text)
text = re.sub('FAILED', '\033[0;31mFAILED\033[0m', text)
text = re.sub('SKIPPED', '\033[0;33mSKIPPED\033[0m', text)

sys.stdout.write(text)

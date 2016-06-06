#!/usr/bin/env bash

set -e

# export slides to pdf and then convert them to jpg
sudo soffice --headless --convert-to pdf Simba.odp
convert -density 150 Simba.pdf -quality 85 Simba.jpg

# crop exported slides
convert Simba-0.jpg -crop 690x320+500+460 logo.jpg

#rm -f Simba.pdf
#rm Simba-*.jpg

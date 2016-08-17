#!/usr/bin/env bash

set -e

# export slides to pdf and then convert them to jpg
sudo soffice --headless --convert-to pdf Simba.odp
convert -density 150 Simba.pdf -quality 95 Simba.jpg

# crop exported slides
convert Simba-0.jpg -crop 690x320+500+460 logo.jpg
convert Simba-2.jpg -crop 1300x530+200+430 thread-scheduling.jpg
convert Simba-3.jpg -crop 1360x580+150+430 queue.jpg
convert Simba-4.jpg -crop 1120x900+210+230 packages.jpg

rm -f Simba.pdf
rm Simba-*.jpg

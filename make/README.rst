make arduino && \
rm -rf ../simba-arduino-avr/* && \
cp -r simba-arduino/avr/* ../simba-arduino-avr/ && \
(cd ../simba-arduino-avr && \
 git add . && \
 git commit -m "simba-arduino-avr-0.6.0" && \
 git tag simba-arduino-avr-0.6.0) && \
rm -rf ../simba-arduino-sam/* && \
cp -r simba-arduino/sam/* ../simba-arduino-sam/
(cd ../simba-arduino-sam && \
 git add . && \
 git commit -m "simba-arduino-sam-0.6.0" && \
 git tag simba-arduino-sam-0.6.0)

sleep 10

wget https://github.com/eerimoq/simba-arduino-avr/archive/simba-arduino-avr-0.6.0.zip && \
sha256sum simba-arduino-avr-0.6.0.zip && \
rm simba-arduino-avr-0.6.0.zip && \
wget https://github.com/eerimoq/simba-arduino-sam/archive/simba-arduino-sam-0.6.0.zip && \
sha256sum simba-arduino-sam-0.6.0.zip && \
rm simba-arduino-sam-0.6.0.zip

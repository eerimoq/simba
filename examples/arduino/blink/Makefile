BUILD_DIR ?= $(shell readlink -f ../build)

ifeq ($(FAMILY),)
    $(error "FAMILY must be given.")
endif

ifeq ($(BOARD),)
    $(error "BOARD must be given.")
endif

all:
	mkdir -p $(BUILD_DIR)
	$(ARDUINO_ROOT)/arduino-builder \
	-compile \
	-logger=machine \
	-hardware $(ARDUINO_ROOT)/hardware \
	-hardware /home/erik/workspace/simba/simba-arduino/packages \
	-tools $(ARDUINO_ROOT)/tools-builder \
	-tools $(ARDUINO_ROOT)/hardware/tools/avr \
	-tools /home/erik/.arduino15/packages \
	-built-in-libraries $(ARDUINO_ROOT)/libraries \
	-libraries /home/erik/Arduino/libraries \
	-fqbn=Simba:$(FAMILY):$(BOARD) \
	-ide-version=10610 \
	-build-path $(BUILD_DIR) \
	-warnings=none \
	-prefs=build.warn_data_percentage=75 \
	-verbose \
	$(BUILD_DIR)/../blink/main.ino

upload: all

clean:
	rm -rf $(BUILD_DIR)

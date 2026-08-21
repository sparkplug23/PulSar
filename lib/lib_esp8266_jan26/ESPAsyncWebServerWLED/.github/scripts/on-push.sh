#!/bin/bash

set -e

export ARDUINO_BUILD_DIR="$HOME/.arduino/build.tmp"

if [ ! -z "$TRAVIS_BUILD_DIR" ]; then
	export GITHUB_WORKSPACE="$TRAVIS_BUILD_DIR"
	export GITHUB_REPOSITORY="$TRAVIS_REPO_SLUG"
elif [ -z "$GITHUB_WORKSPACE" ]; then
	export GITHUB_WORKSPACE="$PWD"
	export GITHUB_REPOSITORY="me-no-dev/ESPAsyncWebServer"
fi

TARGET_PLATFORM="$1"
CHUNK_INDEX=$2
CHUNKS_CNT=$3
BUILD_PIO=0
if [ "$#" -lt 1 ]; then
	TARGET_PLATFORM="esp32"
fi
if [ "$#" -lt 3 ] || [ "$CHUNKS_CNT" -le 0 ]; then
	CHUNK_INDEX=0
	CHUNKS_CNT=1
elif [ "$CHUNK_INDEX" -gt "$CHUNKS_CNT" ]; then
	CHUNK_INDEX=$CHUNKS_CNT
elif [ "$CHUNK_INDEX" -eq "$CHUNKS_CNT" ]; then
	BUILD_PIO=1
fi

if [ "$BUILD_PIO" -eq 0 ]; then
	# ArduinoIDE Test
	source ./.github/scripts/install-arduino-ide.sh

	echo "Installing ESPAsyncWebServer ..."
	cp -rf "$GITHUB_WORKSPACE" "$ARDUINO_USR_PATH/libraries/ESPAsyncWebServer"
	echo "Installing ArduinoJson ..."
	git clone https://github.com/bblanchon/ArduinoJson "$ARDUINO_USR_PATH/libraries/ArduinoJson" > /dev/null 2>&1

	if [[ "$TARGET_PLATFORM" == "esp32" ]]; then
		echo "Installing AsyncTCP ..."
		git clone https://github.com/me-no-dev/AsyncTCP "$ARDUINO_USR_PATH/libraries/AsyncTCP" > /dev/null 2>&1
		FQBN="espressif:esp32:esp32:PSRAM=enabled,PartitionScheme=huge_app"
		source ./.github/scripts/install-arduino-core-esp32.sh
		echo "BUILDING ESP32 EXAMPLES"
	else
		echo "Installing ESPAsyncTCP ..."
		git clone https://github.com/me-no-dev/ESPAsyncTCP "$ARDUINO_USR_PATH/libraries/ESPAsyncTCP" > /dev/null 2>&1
		FQBN="esp8266com:esp8266:generic:eesz=4M1M,ip=lm2f"
		source ./.github/scripts/install-arduino-core-esp8266.sh
		echo "BUILDING ESP8266 EXAMPLES"
	fi
	.github/scripts/sketch_utils.sh chunk_build -ai $ARDUINO_IDE_PATH -au $ARDUINO_USR_PATH -t "${TARGET_PLATFORM}" -fqbn "${FQBN}" -i "${CHUNK_INDEX}" -m "${CHUNKS_CNT}" -p "$GITHUB_WORKSPACE/examples"
else
	# PlatformIO Test
	source ./.github/scripts/install-platformio.sh

	if [[ "$TARGET_PLATFORM" == "esp32" ]]; then
		BOARD="esp32dev"
		echo "BUILDING ESP32 EXAMPLES"
	else
		BOARD="esp12e"
		echo "BUILDING ESP8266 EXAMPLES"
	fi
	build_pio_sketches "$BOARD" "lib_compat_mode = strict" "$GITHUB_WORKSPACE/examples"
fi

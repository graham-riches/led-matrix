.DEFAULT_GOAL:=help
CMAKE:=cmake
TOOLCHAIN:=toolchain/Toolchain-RaspberryPi.cmake
BUILD_DIR:=build
BOOST_ROOT:=/hdd/Code/Environments/boost_1_71_0/
BOOST_INCLUDE:=/hdd/Code/Environments/boost_1_71_0/

help:                                       ## Show this help.
	@echo "Available Targets to Build"
	@echo "-------------------------------------------------------------------------------"
	@fgrep -h "##" $(MAKEFILE_LIST) | fgrep -v fgrep | sed -e 's/\\$$//' | sed -e 's/##//'

$(BUILD_DIR)/CMakeCache.txt:
	$(CMAKE) -B $(BUILD_DIR) -S . -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE=$(TOOLCHAIN) -DBoost_NO_BOOST_CMAKE=TRUE -DBOOST_ROOT=$(BOOST_ROOT) -DBoost_INCLUDE_DIR=$(BOOST_INCLUDE) -GNinja

.PHONY: build
build: $(BUILD_DIR)/CMakeCache.txt          ## Cross-compile the main binary
	ninja -C $(BUILD_DIR)

.PHONY: clean
clean:                                      ## Clean the main build
	ninja clean -C $(BUILD_DIR)
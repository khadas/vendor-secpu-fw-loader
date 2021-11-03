export V?=0
cur_dir=$(CURDIR)

export ARCH?=arm
export CROSS_COMPILE?=arm-linux-gnueabihf-
export TA_CROSS_COMPILE?=arm-linux-gnueabihf-

ifeq ($(PLATFORM_TDK_VERSION), 38)
$(info TDK_VERSION_IS_$(PLATFORM_TDK_VERSION))
export TDK_DIR ?= $(cur_dir)/../tdk_v3
else
$(info TDK_VERSION_IS_$(PLATFORM_TDK_VERSION)_NOT_DEFINED)
export TDK_DIR ?= $(cur_dir)/../tdk
endif

OUTPUT_DIR_CA = ../out/ca
OUTPUT_DIR_LB = ../out/lib

.PHONY: all
all:
	$(MAKE) -C lib O=$(OUTPUT_DIR_LB) CC=${CROSS_COMPILE}gcc
	$(MAKE) -C ca O=$(OUTPUT_DIR_CA) CC=${CROSS_COMPILE}gcc

.PHONY: clean
clean:
	$(MAKE) -C ca clean O=$(OUTPUT_DIR_CA)
	$(MAKE) -C lib clean O=$(OUTPUT_DIR_LB)


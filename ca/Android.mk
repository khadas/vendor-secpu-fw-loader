################################################################################
# Android secpu fw load makefile                                           #
################################################################################
LOCAL_PATH := $(call my-dir)

ifeq ($(PLATFORM_TDK_VERSION), 38)
CFG_TEEC_PUBLIC_PATH ?= $(BOARD_AML_VENDOR_PATH)/tdk_v3/ca_export_$(TARGET_ARCH)
else
CFG_TEEC_PUBLIC_PATH ?= $(BOARD_AML_VENDOR_PATH)/tdk/ca_export_$(TARGET_ARCH)
endif

################################################################################
# Build CA
################################################################################
include $(CLEAR_VARS)
LOCAL_CFLAGS += -DANDROID_BUILD
LOCAL_CFLAGS += -Wall

ifeq ($(shell test $(PLATFORM_SDK_VERSION) -ge 26 && echo OK), OK)
LOCAL_PROPRIETARY_MODULE := true
endif

LOCAL_SRC_FILES += tee_secpu_fw_load.c ../lib/tee_secpu_fw_loader.c

LOCAL_C_INCLUDES :=$(LOCAL_PATH)/include \
        $(LOCAL_PATH)/../lib/include \
		$(CFG_TEEC_PUBLIC_PATH)/include

LOCAL_SHARED_LIBRARIES := libteec

LOCAL_MODULE := tee_secpu_fw_load
LOCAL_MODULE_TAGS := optional
include $(BUILD_EXECUTABLE)

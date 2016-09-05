LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := retro

ifeq ($(TARGET_ARCH),arm)
ANDROID_FLAGS := -DANDROID_ARM

#arm speedups
#default to thumb because its smaller and more can fit in the cpu cache
LOCAL_ARM_MODE := thumb
#switch to arm or thumb instruction set per function based on speed(dont restrict to only arm or thumb use both)
LOCAL_CFLAGS += -mthumb-interwork

#enable/disable optimization
ifeq ($(TARGET_ARCH_ABI),armeabi-v7a)
   LOCAL_CFLAGS += -munaligned-access
   V7NEONOPTIMIZATION ?= 0
else ifeq ($(TARGET_ARCH_ABI),arm64-v8a)
#neon is a requirement for armv8 so just enable it
   LOCAL_CFLAGS += -munaligned-access
   LOCAL_ARM_NEON := true
endif

#armv7+ optimizations
ifeq ($(V7NEONOPTIMIZATION),1)
   LOCAL_ARM_NEON := true
endif
endif

ifeq ($(TARGET_ARCH),x86)
ANDROID_FLAGS :=  -DANDROID_X86
endif

ifeq ($(TARGET_ARCH),mips)
ANDROID_FLAGS := -DANDROID_MIPS -D__mips__ -D__MIPSEL__
endif

CORE_DIR := ..
WANT_UNZIP=1
WANT_LUALIB=1

include $(CORE_DIR)/Makefile.common

LOCAL_SRC_FILES    += $(SOURCES_C)

LOCAL_C_INCLUDES    = $(INCFLAGS)

COMMON_DEFINES = -O2 -ffast-math -funroll-loops -DLSB_FIRST -DINLINE=inline -D__LIBRETRO__ -DFRONTEND_SUPPORTS_RGB565 -DLUA_USE_POSIX -DOUTSIDE_SPEEX -DRANDOM_PREFIX=speex -DEXPORT= -DFIXED_POINT
LOCAL_CFLAGS   += -std=gnu99 $(COMMON_DEFINES) $(INCFLAGS) $(ANDROID_FLAGS)
LOCAL_CXXFLAGS += $(COMMON_DEFINES) $(ANDROID_FLAGS)
LOCAL_LDLIBS += -lz

include $(BUILD_SHARED_LIBRARY)

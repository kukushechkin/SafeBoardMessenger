LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := messenger

LOCAL_C_INCLUDES := include \
					../../External \
					
LOCAL_SRC_FILES := ../../source/messenger_impl.cpp \
					../../source/history_impl.cpp

LOCAL_STATIC_LIBRARIES := libstrophe libexpat
LOCAL_LDLIBS := -llog

ifeq ($(strip $(NDK_DEBUG)),1)
	LOCAL_CFLAGS += -D_DEBUG
endif

include $(BUILD_SHARED_LIBRARY)

include ../../External/libstrophe/jni/Android.mk

LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := libmessenger

LOCAL_C_INCLUDES := include \
					../../External \
					
LOCAL_SRC_FILES := ../../source/messenger_impl.cpp \
					../../source/detail/base64.cpp \
					../../source/detail/uuid.cpp \
					../../source/xmpp/xmpp_connection.cpp \
					../../source/xmpp/xmpp_context.cpp \
					../../source/xmpp/xmpp_env.cpp \
					../../source/xmpp/xmpp_logger.cpp \
					../../source/xmpp/xmpp_stanza.cpp \
					../../source/xmpp/xmpp_stanza_builder.cpp \
					../../source/xmpp/xmpp_stanza_id.cpp \
					../../source/xmpp/xmpp_stanza_parser.cpp

LOCAL_STATIC_LIBRARIES := libstrophe libexpat

ifeq ($(strip $(NDK_DEBUG)),1)
	LOCAL_CFLAGS += -D_DEBUG
endif

include $(BUILD_STATIC_LIBRARY)

COPY_TO_PREBUILT_$(TARGET_ARCH_ABI): SRC := $(LOCAL_PATH)/../../obj/local/$(TARGET_ARCH_ABI)/*.a
COPY_TO_PREBUILT_$(TARGET_ARCH_ABI): DEST := $(LOCAL_PATH)/../../lib/android/$(TARGET_ARCH_ABI)/
COPY_TO_PREBUILT_$(TARGET_ARCH_ABI): $(LOCAL_MODULE)
	$(info Copy $(SRC) to $(DEST) )
	$(shell $(call host-cp,$(SRC),$(DEST)))
  
all: COPY_TO_PREBUILT_$(TARGET_ARCH_ABI)

include ../../External/libstrophe/jni/Android.mk

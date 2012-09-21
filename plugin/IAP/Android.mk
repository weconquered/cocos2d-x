LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE    := cocos2dx_iap_static
LOCAL_MODULE_FILENAME := libcocos2dx-iap
LOCAL_SRC_FILES := libcocos2dx-iap.a
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/include \
                            $(LOCAL_PATH)/include/android 
LOCAL_CPPFLAGS := -D__STDC_LIMIT_MACROS=1
LOCAL_EXPORT_CPPFLAGS := -D__STDC_LIMIT_MACROS=1
include $(PREBUILT_STATIC_LIBRARY)

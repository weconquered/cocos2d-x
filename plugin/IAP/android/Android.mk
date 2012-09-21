LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := cocos2dx_iap_static

LOCAL_MODULE_FILENAME := libcocos2dx-iap

LOCAL_SRC_FILES := ../source/IAPProduct.cpp \
                   ../source/IAPTransaction.cpp \
                   IAP.cpp \
                   IAPCMGC.cpp \
                   IAPChinaTelecom.cpp \
                   IAPUserCenter.cpp \
                   jsoncpp/json_reader.cpp \
                   jsoncpp/json_value.cpp \
                   jsoncpp/json_writer.cpp

LOCAL_CFLAGS := 

LOCAL_EXPORT_CFLAGS := 

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../include \
                    $(LOCAL_PATH)/../include/android \
                    $(LOCAL_PATH)/jsoncpp

LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/../include \
                           $(LOCAL_PATH)/../include/android 

LOCAL_WHOLE_STATIC_LIBRARIES += cocos2dx_static

LOCAL_LDLIBS := -landroid
LOCAL_LDLIBS += -llog

include $(BUILD_STATIC_LIBRARY)

$(call import-module,cocos2dx)

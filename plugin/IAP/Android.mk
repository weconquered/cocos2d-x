LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := cocos2dx_iap_static

LOCAL_MODULE_FILENAME := libcocos2dx-iap

LOCAL_SRC_FILES := IAPProduct.cpp \
                   IAPTransaction.cpp \
                   android/IAP.cpp \
                   android/IAPCMGC.cpp \
                   android/IAPChinaTelecom.cpp \
                   android/IAPUserCenter.cpp \
                   android/jsoncpp/json_reader.cpp \
                   android/jsoncpp/json_value.cpp \
                   android/jsoncpp/json_writer.cpp

LOCAL_CFLAGS := -DCOCOS2D_JAVASCRIPT

LOCAL_EXPORT_CFLAGS := -DCOCOS2D_JAVASCRIPT

LOCAL_C_INCLUDES := $(LOCAL_PATH) \
                    $(LOCAL_PATH)/android/jsoncpp

LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH) \
                           $(LOCAL_PATH)/android 

LOCAL_WHOLE_STATIC_LIBRARIES += cocos2dx_static

LOCAL_LDLIBS := -landroid
LOCAL_LDLIBS += -llog

include $(BUILD_STATIC_LIBRARY)

$(call import-module,cocos2dx)

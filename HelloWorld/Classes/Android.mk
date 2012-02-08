LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := game_logic_shared

LOCAL_MODULE_FILENAME := libgame_logic

LOCAL_C_INCLUDES := $(LOCAL_PATH)/ \
                    $(LOCAL_PATH)/eplibs \
                    $(LOCAL_PATH)/eplibs/GestureRecognizer \
                    $(LOCAL_PATH)/eplibs/keypad \
                    $(LOCAL_PATH)/eplibs/xml \
                    $(LOCAL_PATH)/eplibs/3rd/base64 \
                    $(LOCAL_PATH)/eplibs/3rd/JSON \
                    $(LOCAL_PATH)/eplibs/3rd/md5 \
                    $(LOCAL_PATH)/eplibs/3rd/zlib 


LOCAL_SRC_FILES := AppDelegate.cpp \
                   HelloWorldScene.cpp \
                   \
                   eplibs/3rd/base64/base64.cpp \
                   eplibs/3rd/JSON/cJSON.cpp \
                   eplibs/3rd/md5/CMD5.cpp \
                   eplibs/3rd/zlib/easyzlib.c \
                   eplibs/GestureRecognizer/GestureRecognizer.cpp \
                   eplibs/GestureRecognizer/LongPressGestureRecognizer.cpp \
                   eplibs/GestureRecognizer/PanGestureRecognizer.cpp \
                   eplibs/GestureRecognizer/PinchGestureRecognizer.cpp \
                   eplibs/GestureRecognizer/RotationGestureRecognizer.cpp \
                   eplibs/GestureRecognizer/TapGestureRecognizer.cpp \
                   eplibs/keypad/EPNumberKeypadLayer.cpp \
                   eplibs/xml/EPXmlReader.cpp \
                   eplibs/xml/EPXmlValue.cpp \
                   eplibs/EPGestureRecognizerLayer.cpp \
                   eplibs/EPNotificationCenter.cpp \
                   eplibs/EPResourceManager.cpp \
                   eplibs/EPScrollListLayer.cpp \
                   eplibs/EPSpriteButton.cpp \
                   eplibs/EPTextField.cpp 
                   
                   
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)

LOCAL_STATIC_LIBRARIES := xml2_static_prebuilt

LOCAL_SHARED_LIBRARIES := cocos2dx_shared cocosdenshion_shared
LOCAL_LDLIBS := -llog 
                
include $(BUILD_SHARED_LIBRARY)


# note that the import-module calls are relative to the parent directory
# this simplifies makefiles that use this module
# it requires only one entry in NDK_MODULE_PATH - the module search path
#$(call import-module,cocos2dx/platform/third_party/android/modules/libxml2)


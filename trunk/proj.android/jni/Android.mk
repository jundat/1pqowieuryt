LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := cocos2dcpp_shared

LOCAL_MODULE_FILENAME := libcocos2dcpp




HELLOCPP_FILES := $(wildcard $(LOCAL_PATH)/hellocpp/*.cpp)
HELLOCPP_FILES := $(HELLOCPP_FILES:$(LOCAL_PATH)/%=%)

CLASSES_FILES := $(wildcard $(LOCAL_PATH)/../../Classes/*.cpp)

CLASSES_FILES += $(wildcard $(LOCAL_PATH)/../../Classes/jansson/*.cpp)
CLASSES_FILES += $(wildcard $(LOCAL_PATH)/../../Classes/jansson/*.c)

CLASSES_FILES += $(wildcard $(LOCAL_PATH)/../../Classes/NDKHelper/*.cpp)
CLASSES_FILES += $(wildcard $(LOCAL_PATH)/../../Classes/NDKHelper/*.c)


CLASSES_FILES := $(CLASSES_FILES:$(LOCAL_PATH)/%=%)

LOCAL_SRC_FILES := $(HELLOCPP_FILES)
LOCAL_SRC_FILES += $(CLASSES_FILES)


LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes \
					$(LOCAL_PATH)/../../Classes/jansson \
					$(LOCAL_PATH)/../../Classes/NDKHelper \
					

LOCAL_WHOLE_STATIC_LIBRARIES += cocos2dx_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocosdenshion_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocos_extension_static
LOCAL_WHOLE_STATIC_LIBRARIES += ezibyte_social_static

include $(BUILD_SHARED_LIBRARY)

$(call import-module,cocos2dx)
$(call import-module,extensions)
$(call import-module,CocosDenshion/android)
$(call import-module,ezibyte/ezisocial/android)
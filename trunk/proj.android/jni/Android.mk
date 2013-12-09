LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := cocos2dcpp_shared

LOCAL_MODULE_FILENAME := libcocos2dcpp

LOCAL_SRC_FILES := hellocpp/main.cpp \
           ../../Classes/AppDelegate.cpp  \
     ../../Classes/AudioManager.cpp  \
     ../../Classes/BackgroundLayer.cpp  \
     ../../Classes/Bullet.cpp  \
     ../../Classes/CCParallaxNodeExtras.cpp  \
     ../../Classes/EffectLayer.cpp  \
     ../../Classes/Enemi.cpp  \
     ../../Classes/GameObject.cpp  \
     ../../Classes/MainGameScene.cpp  \
     ../../Classes/ObjectLayer.cpp  \
     ../../Classes/Ship.cpp  \
     ../../Classes/HSJoystick.cpp  \
     ../../Classes/MenuScene.cpp  \


LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes

LOCAL_WHOLE_STATIC_LIBRARIES += cocos2dx_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocosdenshion_static

include $(BUILD_SHARED_LIBRARY)

$(call import-module,cocos2dx)
$(call import-module,CocosDenshion/android)
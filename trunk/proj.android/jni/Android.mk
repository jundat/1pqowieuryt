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
     ../../Classes/Enemy.cpp  \
     ../../Classes/GameObject.cpp  \
     ../../Classes/MainGameScene.cpp  \
     ../../Classes/ObjectLayer.cpp  \
     ../../Classes/Ship.cpp  \
     ../../Classes/MenuScene.cpp  \
     ../../Classes/Item.cpp  \
     ../../Classes/LoseDialog.cpp  \
     ../../Classes/SettingScene.cpp  \


LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes

LOCAL_WHOLE_STATIC_LIBRARIES += cocos2dx_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocosdenshion_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocos_extension_static

include $(BUILD_SHARED_LIBRARY)

$(call import-module,cocos2dx)
$(call import-module,extensions)
$(call import-module,CocosDenshion/android)
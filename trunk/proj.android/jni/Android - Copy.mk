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
	 ../../Classes/CollisionDetection.cpp  \
	 ../../Classes/ConfigLoader.cpp  \
	 ../../Classes/DataManager.cpp  \
     ../../Classes/EffectLayer.cpp  \
     ../../Classes/Enemy.cpp  \
     ../../Classes/GameClient.cpp  \
     ../../Classes/GameObject.cpp  \
	 ../../Classes/Global.cpp  \
     ../../Classes/Item.cpp  \
	 ../../Classes/LevelLoader.cpp  \
     ../../Classes/LoseDialog.cpp  \
     ../../Classes/MainGameScene.cpp  \
     ../../Classes/MenuScene.cpp  \
     ../../Classes/ObjectLayer.cpp  \
     ../../Classes/ParseClient.cpp  \
     ../../Classes/PauseDialog.cpp  \
     ../../Classes/ScoreScene.cpp  \
     ../../Classes/SettingScene.cpp  \
     ../../Classes/Ship.cpp  \


LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes

LOCAL_WHOLE_STATIC_LIBRARIES += cocos2dx_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocosdenshion_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocos_extension_static

include $(BUILD_SHARED_LIBRARY)

$(call import-module,cocos2dx)
$(call import-module,extensions)
$(call import-module,CocosDenshion/android)
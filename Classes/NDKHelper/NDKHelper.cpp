//
//  NDKHelper.cpp
//  EasyNDK-for-cocos2dx
//
//  Created by Amir Ali Jiwani on 23/02/2013.
//
//

#include "NDKHelper.h"

#define __CALLED_METHOD__           "calling_method_name"
#define __CALLED_METHOD_PARAMS__    "calling_method_params"

vector<NDKCallbackNode> NDKHelper::selectorList;

void NDKHelper::AddSelector(const char *groupName, const char *name, SEL_CallFuncND selector, CCNode* target)
{
	//CCLOG("Add selector: %s (in group %s)", name, groupName);
    NDKHelper::selectorList.push_back(NDKCallbackNode(groupName, name, selector, target));
}

void NDKHelper::RemoveAtIndex(int index)
{
    NDKHelper::selectorList[index] = NDKHelper::selectorList.back();
    NDKHelper::selectorList.pop_back();
}

void NDKHelper::RemoveSelectorsInGroup(const char *groupName)
{
    std::vector<int> markedIndices;
    
    for (unsigned int i = 0; i < NDKHelper::selectorList.size(); ++i)
    {
        if (NDKHelper::selectorList[i].getGroup().compare(groupName) == 0)
        {
            markedIndices.push_back(i);
        }
    }
    
    for (unsigned int i = 0; i < markedIndices.size(); ++i)
    {
        NDKHelper::RemoveAtIndex(markedIndices[i]);
    }
}

void NDKHelper::RemoveSelector(const char *groupName,  const char *name )
{
	std::vector<int> markedIndices;

	for (unsigned int i = 0; i < NDKHelper::selectorList.size(); ++i)
	{
		if(NDKHelper::selectorList[i].getName().compare(name) == 0)
		{
			if (NDKHelper::selectorList[i].getGroup().compare(groupName) == 0)
			{
				markedIndices.push_back(i);
			}
		}
	}

	for (unsigned int i = 0; i < markedIndices.size(); ++i)
	{
		//CCLOG("NDKHelper::Remove selector: %s (in group %s)", name, groupName);
		NDKHelper::RemoveAtIndex(markedIndices[i]);
	}
}

CCObject* NDKHelper::GetCCObjectFromJson(json_t *obj)
{
    if (obj == NULL)
        return NULL;
    
    if (json_is_object(obj))
    {
        CCDictionary *dictionary = new CCDictionary();
        //CCDictionary::create();
        
        const char *key;
        json_t *value;
        
        void *iter = json_object_iter(obj);
        while(iter)
        {
            key = json_object_iter_key(iter);
            value = json_object_iter_value(iter);
            
            dictionary->setObject(NDKHelper::GetCCObjectFromJson(value)->autorelease(), string(key));
            
            iter = json_object_iter_next(obj, iter);
        }
        
        return dictionary;
    }
    else if (json_is_array(obj))
    {
        size_t sizeArray = json_array_size(obj);
        CCArray *array = new CCArray();
        //CCArray::createWithCapacity(sizeArray);
        
        for (unsigned int i = 0; i < sizeArray; i++)
        {
            array->addObject(NDKHelper::GetCCObjectFromJson(json_array_get(obj, i))->autorelease());
        }
        
        return array;
    }
    else if (json_is_boolean(obj))
    {
        stringstream str;
        if (json_is_true(obj))
            str << true;
        else if (json_is_false(obj))
            str << false;
        
        CCString *ccString = new CCString(str.str());
        //CCString::create(str.str());
        return ccString;
    }
    else if (json_is_integer(obj))
    {
        stringstream str;
        str << json_integer_value(obj);
        
        CCString *ccString = new CCString(str.str());
        //CCString::create(str.str());
        return ccString;
    }
    else if (json_is_real(obj))
    {
        stringstream str;
        str << json_real_value(obj);
        
        CCString *ccString = new CCString(str.str());
        //CCString::create(str.str());
        return ccString;
    }
    else if (json_is_string(obj))
    {
        stringstream str;
        str << json_string_value(obj);
        
        CCString *ccString = new CCString(str.str());
        //CCString::create(str.str());
        return ccString;
    }
    
    return NULL;
}

json_t* NDKHelper::GetJsonFromCCObject(CCObject* obj)
{
    if (dynamic_cast<CCDictionary*>(obj))
    {
        CCDictionary *mainDict = (CCDictionary*)obj;
        CCArray *allKeys = mainDict->allKeys();
        json_t* jsonDict = json_object();
        
        if(allKeys == NULL ) return jsonDict;
        for (unsigned int i = 0; i < allKeys->count(); i++)
        {
            const char *key = ((CCString*)allKeys->objectAtIndex(i))->getCString();
            json_object_set_new(jsonDict,
                                key,
                                NDKHelper::GetJsonFromCCObject(mainDict->objectForKey(key)));
        }
        
        return jsonDict;
    }
    else if (dynamic_cast<CCArray*>(obj))
    {
        CCArray* mainArray = (CCArray*)obj;
        json_t* jsonArray = json_array();
        
        for (unsigned int i = 0; i < mainArray->count(); i++)
        {
            json_array_append_new(jsonArray,
                                  NDKHelper::GetJsonFromCCObject(mainArray->objectAtIndex(i)));
        }
        
        return jsonArray;
    }
    else if (dynamic_cast<CCString*>(obj))
    {
        CCString* mainString = (CCString*)obj;
        json_t* jsonString = json_string(mainString->getCString());
        
        return jsonString;
    }
    
    return NULL;
}

void NDKHelper::PrintSelectorList()
{
    for (unsigned int i = 0; i < NDKHelper::selectorList.size(); ++i)
    {
        string s = NDKHelper::selectorList[i].getGroup();
        s.append(NDKHelper::selectorList[i].getName());
        CCLOG("%s", s.c_str());
    }
}


void NDKHelper::HandleMessage(json_t *methodName, json_t* methodParams)
{
    CCLOG("Handler Message");
	if (methodName == NULL)
		return;

	const char *methodNameStr = json_string_value(methodName);

	for (unsigned int i = 0; i < NDKHelper::selectorList.size(); ++i)
	{
		if (NDKHelper::selectorList[i].getName().compare(methodNameStr) == 0)
		{
			CCObject *dataToPass = NULL;
			dataToPass = NDKHelper::GetCCObjectFromJson(methodParams);

			if (dataToPass != NULL)
				dataToPass->retain();

			SEL_CallFuncND sel = NDKHelper::selectorList[i].getSelector();
			CCNode *target = NDKHelper::selectorList[i].getTarget();

			CCLOG("Target: Reference %d", target->retainCount());

			if (target->retainCount() > 0)
			{
				//CCLOG("Run selector: %s::%s", 
				//	NDKHelper::selectorList[i].getGroup().c_str(), NDKHelper::selectorList[i].getName().c_str());
				
				CCFiniteTimeAction* action = CCSequence::create(CCCallFuncND::create(target, sel, (void*)dataToPass), NULL);
				target->runAction(action);
			}
			else
			{
				//CCLOG("NULL selector, remove: %s::%s", 
				//	NDKHelper::selectorList[i].getGroup().c_str(), NDKHelper::selectorList[i].getName().c_str());

				NDKHelper::RemoveSelector(NDKHelper::selectorList[i].getGroup().c_str(),
					NDKHelper::selectorList[i].getName().c_str());
			}

			if (dataToPass != NULL)
				dataToPass->autorelease();
			break;
		}
	}
}


#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    #include "../cocos2dx/platform/android/jni/JniHelper.h"
    #include <android/log.h>
    #include <jni.h>
    #define  LOG_TAG    "EasyNDK-for-cocos2dx"

    #define CLASS_NAME "com/cgkstudio/dbptk/classes/AndroidNDKHelper"
#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    #import "IOSNDKHelper-C-Interface.h"
#endif

extern "C"
{
    #if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    // Method for recieving NDK messages from Java, Android
    void Java_com_cgkstudio_dbptk_classes_AndroidNDKHelper_CPPNativeCallHandler(JNIEnv* env, jobject thiz, jstring json)
    {
        string jsonString = JniHelper::jstring2string(json);
        const char *jsonCharArray = jsonString.c_str();
        
        json_error_t error;
        json_t *root;
        root = json_loads(jsonCharArray, 0, &error);
        
        if (!root)
        {
            fprintf(stderr, "error: on line %d: %s\n", error.line, error.text);
            return;
        }
        
        json_t *jsonMethodName, *jsonMethodParams;
        jsonMethodName = json_object_get(root, __CALLED_METHOD__);
        jsonMethodParams = json_object_get(root, __CALLED_METHOD_PARAMS__);
        
        // Just to see on the log screen if messages are propogating properly
        //__android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, jsonCharArray);
        
        NDKHelper::HandleMessage(jsonMethodName, jsonMethodParams);
        json_decref(root);
    }
    #endif
    
    // Method for sending message from CPP to the targetted platform
    void SendMessageWithParams(string methodName, CCObject* methodParams)
    {
        CCLOG("Send Message With Params");
        if (0 == strcmp(methodName.c_str(), ""))
            return;
        
        json_t *toBeSentJson = json_object();
        json_object_set_new(toBeSentJson, __CALLED_METHOD__, json_string(methodName.c_str()));
        
        if (methodParams != NULL)
        {
            json_t* paramsJson = NDKHelper::GetJsonFromCCObject(methodParams);
            json_object_set_new(toBeSentJson, __CALLED_METHOD_PARAMS__, paramsJson);
        }
        
        #if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        JniMethodInfo t;
        
		if (JniHelper::getStaticMethodInfo(t,
                                           CLASS_NAME,
                                           "RecieveCppMessage",
                                           "(Ljava/lang/String;)V"))
		{
            char* jsonStrLocal = json_dumps(toBeSentJson, JSON_COMPACT | JSON_ENSURE_ASCII);
            string jsonStr(jsonStrLocal);
            free(jsonStrLocal);
            
            jstring stringArg1 = t.env->NewStringUTF(jsonStr.c_str());
            t.env->CallStaticVoidMethod(t.classID, t.methodID, stringArg1);
            t.env->DeleteLocalRef(stringArg1);
			t.env->DeleteLocalRef(t.classID);
		}
        #endif
        
        #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        json_t *jsonMessageName = json_string(methodName.c_str());
        
        if (methodParams != NULL)
        {
            json_t *jsonParams = NDKHelper::GetJsonFromCCObject(methodParams);
            IOSNDKHelperImpl::RecieveCPPMessage(jsonMessageName, jsonParams);
            json_decref(jsonParams);
        }
        else
        {
            IOSNDKHelperImpl::RecieveCPPMessage(jsonMessageName, NULL);
        }
        
        json_decref(jsonMessageName);
        #endif
        
        json_decref(toBeSentJson);
    }
}
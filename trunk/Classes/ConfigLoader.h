#pragma once

#ifndef __CONFIG_LOADER_H__
#define __CONFIG_LOADER_H__

#include "Global.h"
#include "cocos2d.h"

USING_NS_CC;

#define CONFIG_FILE		"config.plist"

//////////////////////////////////////////////////////////////////////////
//MACRO

#pragma region MACRO


//get value in ConfigLoader
#define CONF_INT(key)		ConfigLoader::shareConfigLoader()->GetValue(#key)->intValue()
#define CONF_FLOAT(key)		ConfigLoader::shareConfigLoader()->GetValue(#key)->floatValue()
#define CONF_STR(key)		ConfigLoader::shareConfigLoader()->GetValue(#key)->getCString()
#define CONF_CCSTR(key)		ConfigLoader::shareConfigLoader()->GetValue(#key)


//create a config variant
#define CREATE_CONF_INT(key)	static int key = CONF_INT(key)
#define CREATE_CONF_FLOAT(key)	static float key = CONF_FLOAT(key)
#define CREATE_CONF_STR(key)	static std::string key = CONF_STR(key)


#pragma endregion

//////////////////////////////////////////////////////////////////////////


class ConfigLoader
{
public:
	ConfigLoader(void);
	~ConfigLoader(void);

	static ConfigLoader* shareConfigLoader();

	CCDictionary* m_dict;

	const CCString* GetValue(const std::string key);

protected:
	static ConfigLoader* s_instance;
};


#endif //__CONFIG_LOADER_H__
#pragma once

#ifndef __CONFIG_LOADER_H__
#define __CONFIG_LOADER_H__

#include "Global.h"
#include "cocos2d.h"

USING_NS_CC;

class ConfigLoader
{
public:
	ConfigLoader(void);
	~ConfigLoader(void);

	static CCString* configeFile;
	static ConfigLoader* shareConfigLoader();

	CCDictionary* m_dict;

	const CCString* GetValue(const std::string key);

protected:
	static ConfigLoader* s_instance;
};


#endif //__CONFIG_LOADER_H__
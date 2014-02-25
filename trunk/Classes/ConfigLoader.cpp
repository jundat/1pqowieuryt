#include "ConfigLoader.h"
#include "cocos2d.h"

USING_NS_CC;

ConfigLoader* ConfigLoader::s_instance = NULL;


ConfigLoader::ConfigLoader(void)
{
	CCLOG("-----------CONFIG------------");
	m_dict = CCDictionary::createWithContentsOfFile("config.plist");

	CCDictElement* pElement = NULL;
	CCDICT_FOREACH(m_dict, pElement)
	{
		CCString* val = (CCString*)pElement->getObject();
		std::string key = std::string(pElement->getStrKey());
		CCLOG("%s : %s", key.c_str(), val->getCString());
	}
	CCLOG("--------CONFIG END-----------");
}

ConfigLoader::~ConfigLoader(void)
{
}

ConfigLoader* ConfigLoader::shareConfigLoader()
{
	if (s_instance == NULL)
	{
		s_instance = new ConfigLoader();
	}
	
	return s_instance;
}

const CCString* ConfigLoader::GetValue(const std::string key )
{
	const CCString* val = m_dict->valueForKey(key);
	if (val->length() == 0)
	{
		CCLOG("NOT A VALID KEY CONFIG: %s", key.c_str());
		return NULL;
	}

	return val;
}

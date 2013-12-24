#include "ConfigLoader.h"
#include "cocos2d.h"

USING_NS_CC;

CCString* ConfigLoader::configeFile = CCStringMake("config.plist");
ConfigLoader* ConfigLoader::s_instance = NULL;

ConfigLoader::ConfigLoader(void)
{
	CCLOG("-----------CONFIG------------");
	m_dict = CCDictionary::createWithContentsOfFile(configeFile->getCString());

	CCDictElement* pElement = NULL;
	CCDICT_FOREACH(m_dict, pElement)
	{
		CCString* v = (CCString*)pElement->getObject();
		std::string oneStrKey = pElement->getStrKey();
		CCLOG("%s : %s", oneStrKey.c_str(), v->getCString());
	}
	CCLOG("------------CONFIG-----------");

	//save to variant
	G_DESIGN_WIDTH = GetValue("G_DESIGN_WIDTH")->intValue();
	G_DESIGN_HEIGHT = GetValue("G_DESIGN_HEIGHT")->intValue();
	G_SCALE_FACTOR = GetValue("G_SCALE_FACTOR")->floatValue();

	G_PLAYER_HP = GetValue("G_PLAYER_HP")->intValue();
	G_PLAYER_DAMAGE = GetValue("G_PLAYER_DAMAGE")->intValue();
	G_MIN_PLAYER_BULLET_LEVEL = GetValue("G_MIN_PLAYER_BULLET_LEVEL")->intValue();
	G_MAX_PLAYER_BULLET_LEVEL = GetValue("G_MAX_PLAYER_BULLET_LEVEL")->intValue();
	
	G_MAX_PLAYER_BOOM = GetValue("G_MAX_PLAYER_BOOM")->intValue();

	G_MIN_ENEMY_HP = GetValue("G_MIN_ENEMY_HP")->intValue();
	G_MIN_ENEMY_DAM = GetValue("G_MIN_ENEMY_DAM")->intValue();
	G_PLAYER_ARMOR_TIME = GetValue("G_PLAYER_ARMOR_TIME")->floatValue();
	G_PLAYER_BULLET_VY = GetValue("G_PLAYER_BULLET_VY")->floatValue();

	G_PLAYER_TIME_TO_FIRE = GetValue("G_PLAYER_TIME_TO_FIRE")->floatValue();
	G_ENEMY_TIME_TO_FIRE = GetValue("G_ENEMY_TIME_TO_FIRE")->floatValue();
	
	G_DEFAULT_TIME_TO_GENERATE_ENEMY = GetValue("G_DEFAULT_TIME_TO_GENERATE_ENEMY")->floatValue();
	
	G_MIN_ENEMY_VY = GetValue("G_MIN_ENEMY_VY")->floatValue();
	G_MAX_ENEMY_VY = GetValue("G_MAX_ENEMY_VY")->floatValue();
	
	G_MIN_ENEMY_BULLET_VY = GetValue("G_MIN_ENEMY_BULLET_VY")->floatValue();
	G_BULLET_VY_FROM_ENEMY = GetValue("G_BULLET_VY_FROM_ENEMY")->floatValue();
	
	G_DEFAULT_DIFFICULTY = GetValue("G_DEFAULT_DIFFICULTY")->floatValue();

	G_VELOCITY_IN_DIFFICULTY = GetValue("G_VELOCITY_IN_DIFFICULTY")->floatValue();
	G_HP_IN_DIFFICULTY = GetValue("G_HP_IN_DIFFICULTY")->floatValue();
	G_DAMAGE_IN_DIFFICULTY = GetValue("G_DAMAGE_IN_DIFFICULTY")->floatValue();

	G_TIMEOUT_BULLET_LEVEL = GetValue("G_TIMEOUT_BULLET_LEVEL")->floatValue();

	G_ITEM_BULLET_RANDOM_PERCENT = GetValue("G_ITEM_BULLET_RANDOM_PERCENT")->floatValue();
	G_ITEM_ARMOR_RANDOM_PERCENT = GetValue("G_ITEM_ARMOR_RANDOM_PERCENT")->floatValue();
	G_ITEM_BOOM_RANDOM_PERCENT = GetValue("G_ITEM_BOOM_RANDOM_PERCENT")->floatValue();

	G_PLAYER_TIME_TO_REVIVE = GetValue("G_PLAYER_TIME_TO_REVIVE")->floatValue();
	G_PLAYER_LIFE = GetValue("G_PLAYER_LIFE")->floatValue();
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

const CCString* ConfigLoader::GetValue( char* key )
{
	const CCString* v = m_dict->valueForKey(key);
	if (v->length() == 0)
	{
		CCLOG("NOT A VALID KEY CONFIG: %s", key);
		return CCStringMake("NULL_STRING");
	}

	return v;
}

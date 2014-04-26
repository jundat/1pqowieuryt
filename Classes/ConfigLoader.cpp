#include "ConfigLoader.h"
#include "cocos2d.h"

USING_NS_CC;

//////////////////////////////////////////////////////////////////////////


ConfigLoader* ConfigLoader::s_instance = NULL;


ConfigLoader::ConfigLoader(void)
{
	//CCLOG("-----------CONFIG------------");
	m_dict = CCDictionary::createWithContentsOfFile(CONFIG_FILE);

	CCDictElement* pElement = NULL;
	CCDICT_FOREACH(m_dict, pElement)
	{
		CCString* val = (CCString*)pElement->getObject();
		std::string key = pElement->getStrKey();
		CCLOG("%s : %s", key.c_str(), val->getCString());
	}

	//SEND DATA
	G_URL_PLAYER_FB_PROFILE = std::string(GetValue(std::string("G_URL_PLAYER_FB_PROFILE"))->getCString());
	G_URL_FRIEND_LIST = std::string(GetValue(std::string("G_URL_FRIEND_LIST"))->getCString());
	G_URL_DEVICE_PROFILE = std::string(GetValue(std::string("G_URL_DEVICE_PROFILE"))->getCString());
	G_URL_SCORE = std::string(GetValue(std::string("G_URL_SCORE"))->getCString());
	G_URL_REVIVE = std::string(GetValue(std::string("G_URL_REVIVE"))->getCString());
	G_URL_GET_LAZER = std::string(GetValue(std::string("G_URL_GET_LAZER"))->getCString());
    G_URL_ITEM = std::string(GetValue(std::string("G_URL_ITEM"))->getCString());
    
    
    G_URL_GIFT_SEND_ITEM = std::string(GetValue(std::string("G_URL_GIFT_SEND_ITEM"))->getCString());
    G_URL_GIFT_GET_INBOX = std::string(GetValue(std::string("G_URL_GIFT_GET_INBOX"))->getCString());
    G_URL_GIFT_REMOVE_ITEM = std::string(GetValue(std::string("G_URL_GIFT_REMOVE_ITEM"))->getCString());
    

	G_URL_RATE = std::string(GetValue(std::string("G_URL_RATE"))->getCString());
    

	//////////////////////////////////////////////////////////////////////////

	//save to variant
	G_DESIGN_WIDTH = GetValue(std::string("G_DESIGN_WIDTH"))->intValue();
	G_DESIGN_HEIGHT = GetValue(std::string("G_DESIGN_HEIGHT"))->intValue();
	G_SCALE_FACTOR = GetValue(std::string("G_SCALE_FACTOR"))->floatValue();

	G_PLAYER_HP = GetValue(std::string("G_PLAYER_HP"))->intValue();
	G_PLAYER_DAMAGE = GetValue(std::string("G_PLAYER_DAMAGE"))->intValue();
	G_MIN_PLAYER_BULLET_LEVEL = GetValue(std::string("G_MIN_PLAYER_BULLET_LEVEL"))->intValue();
	G_MAX_PLAYER_BULLET_LEVEL = GetValue(std::string("G_MAX_PLAYER_BULLET_LEVEL"))->intValue();
	
	G_MAX_PLAYER_BOOM = GetValue(std::string("G_MAX_PLAYER_BOOM"))->intValue();

	G_MIN_ENEMY_HP = GetValue(std::string("G_MIN_ENEMY_HP"))->intValue();
	G_MIN_ENEMY_DAM = GetValue(std::string("G_MIN_ENEMY_DAM"))->intValue();
	G_PLAYER_LAZER_TIME = GetValue(std::string("G_PLAYER_LAZER_TIME"))->floatValue();
	G_PLAYER_ARMOR_TIME = GetValue(std::string("G_PLAYER_ARMOR_TIME"))->floatValue();
	G_PLAYER_BULLET_VY = GetValue(std::string("G_PLAYER_BULLET_VY"))->floatValue();

	G_PLAYER_TIME_TO_FIRE = GetValue(std::string("G_PLAYER_TIME_TO_FIRE"))->floatValue();
	G_ENEMY_TIME_TO_FIRE = GetValue(std::string("G_ENEMY_TIME_TO_FIRE"))->floatValue();
	
	G_DEFAULT_TIME_TO_GENERATE_ENEMY = GetValue(std::string("G_DEFAULT_TIME_TO_GENERATE_ENEMY"))->floatValue();
	G_TIME_TO_GENERATE_ITEM = GetValue(std::string("G_TIME_TO_GENERATE_ITEM"))->floatValue();
	
	
	G_MIN_ENEMY_VY = GetValue(std::string("G_MIN_ENEMY_VY"))->floatValue();
	G_MAX_ENEMY_VY = GetValue(std::string("G_MAX_ENEMY_VY"))->floatValue();
	
	G_MIN_ENEMY_BULLET_VY = GetValue(std::string("G_MIN_ENEMY_BULLET_VY"))->floatValue();
	G_BULLET_VY_FROM_ENEMY = GetValue(std::string("G_BULLET_VY_FROM_ENEMY"))->floatValue();
	
	G_DEFAULT_DIFFICULTY = GetValue(std::string("G_DEFAULT_DIFFICULTY"))->floatValue();

	G_VELOCITY_IN_DIFFICULTY = GetValue(std::string("G_VELOCITY_IN_DIFFICULTY"))->floatValue();
	G_HP_IN_DIFFICULTY = GetValue(std::string("G_HP_IN_DIFFICULTY"))->floatValue();
	G_DAMAGE_IN_DIFFICULTY = GetValue(std::string("G_DAMAGE_IN_DIFFICULTY"))->floatValue();

	G_TIMEOUT_BULLET_LEVEL = GetValue(std::string("G_TIMEOUT_BULLET_LEVEL"))->floatValue();

	G_ITEM_BULLET_RANDOM_PERCENT = GetValue(std::string("G_ITEM_BULLET_RANDOM_PERCENT"))->floatValue();
	G_ITEM_ARMOR_RANDOM_PERCENT = GetValue(std::string("G_ITEM_ARMOR_RANDOM_PERCENT"))->floatValue();
	G_ITEM_BOOM_RANDOM_PERCENT = GetValue(std::string("G_ITEM_BOOM_RANDOM_PERCENT"))->floatValue();

	G_PLAYER_TIME_TO_REVIVE = GetValue(std::string("G_PLAYER_TIME_TO_REVIVE"))->intValue();
	G_MAX_PLAYER_LIFE = GetValue(std::string("G_MAX_PLAYER_LIFE"))->floatValue();


	G_ENEMY_1_PERCENT = GetValue(std::string("G_ENEMY_1_PERCENT"))->floatValue();
	G_ENEMY_2_PERCENT = GetValue(std::string("G_ENEMY_2_PERCENT"))->floatValue();
	G_ENEMY_3_PERCENT = GetValue(std::string("G_ENEMY_3_PERCENT"))->floatValue();


	G_ENEMY_1_SCORE = GetValue(std::string("G_ENEMY_1_SCORE"))->intValue();
	G_ENEMY_2_SCORE = GetValue(std::string("G_ENEMY_2_SCORE"))->intValue();
	G_ENEMY_3_SCORE = GetValue(std::string("G_ENEMY_3_SCORE"))->intValue();


	G_DEFAULT_BOOM = GetValue(std::string("G_DEFAULT_BOOM"))->intValue();
	G_MAX_BOOM = GetValue(std::string("G_MAX_BOOM"))->intValue();


	G_DEFAULT_DIAMON = GetValue(std::string("G_DEFAULT_DIAMON"))->intValue();
	G_DIAMON_PER_LIFE = GetValue(std::string("G_DIAMON_PER_LIFE"))->intValue();
	G_DIAMON_TO_GET_BOOM_NOW = GetValue(std::string("G_DIAMON_TO_GET_BOOM_NOW"))->intValue();
	
	G_WAIT_TO_REVIVE = GetValue(std::string("G_WAIT_TO_REVIVE"))->floatValue();

	G_POINTS_PER_SECOND = GetValue(std::string("G_POINTS_PER_SECOND"))->intValue();

	G_TIME_TO_REFRESH_FRIENDS = GetValue(std::string("G_TIME_TO_REFRESH_FRIENDS"))->floatValue();	

	G_IS_ENCODE = GetValue(std::string("G_IS_ENCODE"))->boolValue();
	G_IS_MONEYTIZE = GetValue(std::string("G_IS_MONEYTIZE"))->boolValue();
    
    
	
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
		return CCStringMake("NULL_STRING");
	}

	return val;
}

#include "LevelLoader.h"
#include "cocos2d.h"

USING_NS_CC;

CCString* LevelLoader::s_levelFile = CCStringMake("level.csv");
LevelLoader* LevelLoader::s_instance = NULL;

LevelLoader::LevelLoader(void)
{
	m_dict = CCDictionary::create();
	m_dict->retain();

	unsigned long bufferSize = 0;
	std::string fileData  = std::string(CCString::createWithContentsOfFile(s_levelFile->getCString())->getCString());
	std::string smallStr;

	fileData = fileData.substr(0, fileData.find_last_not_of('\n'));

	int beginIndex  = fileData.find_first_of('\n'); //REMOVE 1st line
	smallStr = fileData.substr(0, beginIndex + 1); //pos, len
	fileData = fileData.substr(beginIndex + 1); //pos, end

	int score, hp;
	float velocity, genTime;

	bool isBroken = false;
	while(isBroken == false)
	{
		beginIndex = fileData.find_first_of('\n');

		if(beginIndex <= 0) {
			isBroken = true;
			beginIndex = fileData.length() - 1;
		}

		smallStr = fileData.substr(0, beginIndex + 1); //pos, len

		if (smallStr.find_first_of('\n') <= 1)
		{
			break;
		}

		fileData = fileData.substr(beginIndex + 1); //pos, end

		sscanf(smallStr.c_str(), "%d,%d,%f,%f\n", &score, &hp, &velocity, &genTime);
		LevelData* ld = LevelData::create(score, hp, velocity, genTime);
		CCLOG("%s", ld->ToString());
		m_dict->setObject(ld, score);
	}
}


LevelLoader* LevelLoader::shareLevelLoader()
{
	if (s_instance == NULL)
	{
		s_instance = new LevelLoader();
	}
	
	return s_instance;
}

LevelData* LevelLoader::GetValueLowerThan( int keyScore )
{
	return (LevelData*)m_dict->objectForKey(keyScore);
}

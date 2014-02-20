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

	int score, hp1, hp2, hp3;
	float velocity1, velocity2, velocity3, smallDelay, bigDelay;
	int arrEnemy[NUM_ENEMY_TYPE];
	

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

		sscanf(smallStr.c_str(), "%d,%d,%d,%d,%f,%f,%f,%f,%f,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\n", \
			&score, &hp1, &hp2, &hp3, &velocity1, &velocity2, &velocity3, &smallDelay, &bigDelay, \
			&arrEnemy[0], &arrEnemy[1], &arrEnemy[2], &arrEnemy[3], &arrEnemy[4], &arrEnemy[5], &arrEnemy[6], &arrEnemy[7], &arrEnemy[8], \
			&arrEnemy[9], &arrEnemy[10], &arrEnemy[11], &arrEnemy[12], &arrEnemy[13], &arrEnemy[14]);

		LevelData* ld = LevelData::create(score, hp1, hp2, hp3, velocity1, velocity2, velocity3, smallDelay, bigDelay, arrEnemy);
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

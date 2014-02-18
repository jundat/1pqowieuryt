#ifndef __CUSTOMTABELVIEWCELL_H__
#define __CUSTOMTABELVIEWCELL_H__

#include "cocos2d.h"
#include "cocos-ext.h"
USING_NS_CC;

class CustomTableViewCell : public cocos2d::extension::CCTableViewCell
{
public:
	std::string fbID;

	//get boom
	tm* m_lastTimeGetBoom;
	CCLabelTTF* m_lbGetBoomTimer;
	CCLabelTTF* m_lbGetBoom;
	CCMenuItemImage* m_itGetBoom;

	//send life
	tm* m_lastTimeSendLife;
	CCLabelTTF* m_lbSendLifeTimer;
	CCLabelTTF* m_lbSendLife;
	CCMenuItemImage* m_itSendLife;
	
	CustomTableViewCell()
	{
		//get boom
		m_lastTimeGetBoom = NULL;
		m_lbGetBoomTimer = NULL;
		m_lbGetBoom = NULL;
		m_itGetBoom = NULL;

		//send life
		m_lastTimeSendLife = NULL;
		m_lbSendLifeTimer = NULL;
		m_lbSendLife = NULL;
		m_itSendLife = NULL;
	}

	virtual void draw();
};

#endif /* __CUSTOMTABELVIEWCELL_H__ */


#ifndef __CUSTOMTABELVIEWCELL_H__
#define __CUSTOMTABELVIEWCELL_H__

#include "cocos2d.h"
#include "cocos-ext.h"
USING_NS_CC;

class CustomTableViewCell : public cocos2d::extension::CCTableViewCell
{
public:
	std::string fbID;
	
	tm* lastBoomTime;

	CCLabelTTF* lbTimer;
	CCLabelTTF* lbGetBoom;
	CCMenuItemImage* itBoom;

	CustomTableViewCell()
	{
		lastBoomTime = NULL;

		lbTimer = NULL;
		lbGetBoom = NULL;
		itBoom = NULL;
	}

	virtual void draw();
};

#endif /* __CUSTOMTABELVIEWCELL_H__ */


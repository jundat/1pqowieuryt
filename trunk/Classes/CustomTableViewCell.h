#ifndef __CUSTOMTABELVIEWCELL_H__
#define __CUSTOMTABELVIEWCELL_H__

#include "cocos2d.h"
#include "cocos-ext.h"
USING_NS_CC;

class CustomTableViewCell : public cocos2d::extension::CCTableViewCell
{
public:
	std::string fbID;
	
	int boomWaitTime;

	tm* lastBoomTime;

	CCLabelTTF* lbTimer;

	virtual void draw();
};

#endif /* __CUSTOMTABELVIEWCELL_H__ */


#ifndef __YESNO_DIALOG_PARENT_H__
#define __YESNO_DIALOG_PARENT_H__

#include "cocos2d.h"
USING_NS_CC;

class YesNoDialogParent
{
public:
	virtual void onShowDialog(){CCLOG("on show dialog");}
	virtual void onCloseDialog(){CCLOG("on close dialog");}
};


#endif //__YESNO_DIALOG_PARENT_H__
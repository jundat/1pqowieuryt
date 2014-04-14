#ifndef __CHOOSE_LANGUAGE_DIALOG_H__
#define __CHOOSE_LANGUAGE_DIALOG_H__

#include "cocos2d.h"
USING_NS_CC;

class ChooseLanguageDialog : public cocos2d::CCLayerColor
{
public:
	static cocos2d::CCScene* scene();
    virtual bool init();
    CREATE_FUNC(ChooseLanguageDialog);

public:
	void vietnamCallback(CCObject* pSender);
	void englishCallback( CCObject* pSender );
};

#endif // __CHOOSE_LANGUAGE_DIALOG_H__

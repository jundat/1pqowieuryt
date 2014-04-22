#ifndef __QUIT_DIALOG_H__
#define __QUIT_DIALOG_H__

#include "cocos2d.h"
#include "YesNoDialogParent.h"
USING_NS_CC;

class QuitDialog : public cocos2d::CCLayerColor
{
public:
    virtual bool init();
    CREATE_FUNC(QuitDialog);
    YesNoDialogParent* m_yesnoParent;
    void setYesNoDialogParent(YesNoDialogParent* parent)
    {
        m_yesnoParent = parent;
    }

public:
	void yesCallback(CCObject* pSender);
	void noCallback( CCObject* pSender );
	void keyBackClicked();
};

#endif // __QUIT_DIALOG_H__

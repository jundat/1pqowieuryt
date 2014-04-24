#ifndef __WAIT_DIALOG_H__
#define __WAIT_DIALOG_H__

#include "cocos2d.h"
#include "cocos-ext.h"
USING_NS_CC;

class WaitDialog : public cocos2d::CCLayerColor
{
public:
    virtual bool init();
    CREATE_FUNC(WaitDialog);
    
    void setTitle(std::string title);
    
public:
    CCLabelTTF* m_labelTitle;
};

#endif // __WAIT_DIALOG_H__

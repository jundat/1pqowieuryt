#include "WaitDialog.h"
#include "TextLoader.h"
#include "Global.h"

USING_NS_CC;
USING_NS_CC_EXT;

bool WaitDialog::init()
{
	if ( !CCLayerColor::initWithColor(G_DIM_COLOR) )
	{
		return false;
	}

	CCScale9Sprite* dialog = CCScale9Sprite::create("dialog.png");
	dialog->setPosition(ccp(400, 640));
	dialog->setContentSize(CCSizeMake(680, 480));
	this->addChild(dialog);

	CCSprite* sprWait = CCSprite::create("loading.png");
	sprWait->setPosition(ccp(400, 600));
	this->addChild(sprWait);
	sprWait->runAction(CCRepeatForever::create(CCRotateBy::create(1.0f, -360.0f)));
    

	m_labelTitle = CCLabelTTF::create("",
		"Roboto-Medium.ttf", 
		48, 
		CCSizeMake(600, 0), 
		kCCTextAlignmentCenter, 
		kCCVerticalTextAlignmentCenter);
	m_labelTitle->setColor(ccBLACK);
	m_labelTitle->setPosition(ccp(400, 768));
	this->addChild(m_labelTitle);

    return true;
}

void WaitDialog::setTitle(std::string title)
{
    m_labelTitle->setString(title.c_str());
}
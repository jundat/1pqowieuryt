#ifndef __LOSE_DIALOG_H__
#define __LOSE_DIALOG_H__

#include "cocos2d.h"
#include "GameClientManager.h"
#include "GameClientDelegate.h"
#include "GameClientObjects.h"
#include "TextLoader.h"
#include "WaitDialog.h"

USING_NS_CC;

class LoseDialog : public cocos2d::CCLayerColor, public GameClientDelegate
{
public:
	LoseDialog(int score, bool isBreakrecord):CCLayerColor(){
		this->m_score = score;
		this->m_isBreakRecord = isBreakrecord;
	};
    virtual bool init();
	static LoseDialog* create(int score, bool isBreakrecord) {
		LoseDialog* dig = new LoseDialog(score, isBreakrecord);
		dig->init();
		dig->autorelease();
		return dig;
	}

private:
	int m_score;

	float m_elapsedTime;
	bool m_isBreakRecord;
    bool m_isLoggedIn;
	CCLabelTTF* m_lbTimer;
	CCLabelTTF* m_lbDiamon;

	CCSprite* m_timerBar;
	CCSprite* m_timerNode;
	
	CCMenuItemImage* m_itExitButton;
	CCMenuItemImage* m_itRevive;

	//particle
	CCParticleSystem*    m_emitter;
    
    //
    void showWaitDialog(string title);
    void closeWaitDialog();
    WaitDialog* m_waitDialog;
    

public:
	void menuCallBack(CCObject* pSender);
	void reviveCallBack(CCObject* pSender);
	void update(float delta);
    
    virtual void onBuyItemCompleted(bool isSuccess, int newCoin, std::string itemType, int itemCount, std::string uniqueTag);

private:
	void breakRecord();
	void initParticle();
	void closeReviveAnimation();
};

#endif // __LOSE_DIALOG_H__

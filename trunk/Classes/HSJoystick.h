#pragma once
#include "cocos2d.h" 

USING_NS_CC;

class HSJoystick : public CCLayer
{
public:
	HSJoystick():CCLayer(){}
	virtual bool init();
	CREATE_FUNC(HSJoystick);

private:
	void ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent);
	void ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent);
	void ccTouchesCancelled(CCSet *pTouches, CCEvent *pEvent);
	void ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent);

#ifdef WIN32
	bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
	void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
	void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
	void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
#endif
	
private:
	CCPoint m_Center;
	float m_joystickRadius;
	float m_thumbRadius;

	CCSprite *m_SprThumb;
	bool m_IsPressed;
	CCPoint m_Velocity;

public:
	CCPoint GetVelocity() { return m_Velocity; }

private:
	void UpdateVelocity(CCPoint point);
	void ResetJoystick();
	bool HandleLastTouch();
};
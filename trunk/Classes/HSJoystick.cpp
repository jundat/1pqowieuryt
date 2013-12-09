#include "HSJoystick.h" 

#define JOYSTICK_OFFSET_X 5.0f
#define JOYSTICK_OFFSET_Y 5.0f


static bool isPointInCircle(CCPoint point, CCPoint center, float radius){
	float dx = (point.x - center.x);
	float dy = (point.y - center.y);
	return (radius >= sqrt((dx*dx)+(dy*dy)));
}

bool HSJoystick::init()
{
	if(!CCLayer::init())
	{
		return false;
	}

	CCSprite *bg = CCSprite::create("joystick_background.png");
	m_joystickRadius = bg->getContentSize().width/2;

	m_SprThumb = CCSprite::create("joystick_thumb.png");
	m_thumbRadius = m_SprThumb->getContentSize().width/2;

	m_Center = CCPoint(m_joystickRadius + JOYSTICK_OFFSET_X,
		m_joystickRadius + JOYSTICK_OFFSET_Y);

	bg->setPosition(m_Center);
	this->addChild(bg, 0);
	m_SprThumb->setPosition(m_Center);
	this->addChild(m_SprThumb, 1);

	m_Velocity = CCPointZero;
	
#ifdef WIN32
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
#endif

	CCDirector::sharedDirector()->getTouchDispatcher()->addStandardDelegate(this, 0);
	this->setTouchEnabled(true);
	
	return true;
}

void HSJoystick::UpdateVelocity(CCPoint point)
{
	// calculate Angle and length
	float dx = point.x - m_Center.x;
	float dy = point.y - m_Center.y;

	float distance = sqrt(dx*dx + dy*dy);
	float angle = atan2(dy,dx); // in radians

	if(distance > m_joystickRadius){
		dx = cos(angle) * m_joystickRadius;
		dy = sin(angle) * m_joystickRadius;
	}

	m_Velocity = CCPointMake(dx/m_joystickRadius, dy/m_joystickRadius);

	if(distance>m_thumbRadius)
	{
		point.x = m_Center.x + cos(angle) * m_thumbRadius;
		point.y = m_Center.y + sin(angle) * m_thumbRadius;
	}

	m_SprThumb->setPosition(point);
}

void HSJoystick::ResetJoystick()
{
	this->UpdateVelocity(m_Center);
}

bool HSJoystick::HandleLastTouch()
{
	bool wasPressed = m_IsPressed;
	if(wasPressed){
		this->ResetJoystick();
		m_IsPressed = false;
	}
	return (wasPressed ? true : false);
}

void HSJoystick::ccTouchesBegan( CCSet *pTouches, CCEvent *pEvent )
{
	CCTouch *touch = (CCTouch*)pTouches->anyObject();
	CCPoint point = this->convertToNodeSpace(touch->getLocation());

	if(isPointInCircle(point, m_Center, m_joystickRadius)){
		m_IsPressed = true;
		this->UpdateVelocity(point);
	}
}

void HSJoystick::ccTouchesMoved( CCSet *pTouches, CCEvent *pEvent )
{
	if(m_IsPressed){
		CCTouch *touch = (CCTouch*)pTouches->anyObject();
		CCPoint point =  this->convertToNodeSpace(touch->getLocation());
		this->UpdateVelocity(point);
	}
}

void HSJoystick::ccTouchesCancelled( CCSet *pTouches, CCEvent *pEvent )
{
	this->HandleLastTouch();
}

void HSJoystick::ccTouchesEnded( CCSet *pTouches, CCEvent *pEvent )
{
	this->HandleLastTouch();
}


#pragma region Test in WIN32
#ifdef WIN32
bool HSJoystick::ccTouchBegan( CCTouch *pTouch, CCEvent *pEvent )
{
	CCPoint point = this->convertToNodeSpace(pTouch->getLocation());

	if(isPointInCircle(point, m_Center, m_joystickRadius)){
		m_IsPressed = true;
		this->UpdateVelocity(point);
	}

	return true;
}

void HSJoystick::ccTouchMoved( CCTouch *pTouch, CCEvent *pEvent )
{
	if(m_IsPressed){
		CCPoint point =  this->convertToNodeSpace(pTouch->getLocation());
		this->UpdateVelocity(point);
	}
}

void HSJoystick::ccTouchCancelled( CCTouch *pTouch, CCEvent *pEvent )
{
	this->HandleLastTouch();
}

void HSJoystick::ccTouchEnded( CCTouch *pTouch, CCEvent *pEvent )
{
	this->HandleLastTouch();
}
#endif
#pragma endregion
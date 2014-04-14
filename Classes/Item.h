#ifndef __ITEM_H__
#define __ITEM_H__

#include "cocos2d.h"
#include "GameObject.h"
#include "Global.h"

USING_NS_CC;

class Item : public GameObject
{
public:
	Item(int type, float vy, CCPoint position);
	~Item();
	virtual bool init();
	static Item* create(int type, float vy, CCPoint position) {
		Item* item = new Item(type, vy, position);
		item->init();
		item->autorelease();
		return item;
	}
	virtual void update(float delta);
	CC_SYNTHESIZE(int, m_type, ItemType);
	CC_SYNTHESIZE(float, m_vx, Vx);
	CC_SYNTHESIZE(float, m_vy, Vy);
};

#endif //__ITEM_H__
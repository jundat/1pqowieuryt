#ifndef __PARALLAX_NODE_EXTRAS_H__
#define __PARALLAX_NODE_EXTRAS_H__

#include "cocos2d.h"

USING_NS_CC;

class CCParallaxNodeExtras : public CCParallaxNode {

public :
	// Need to provide a constructor 
	CCParallaxNodeExtras():CCParallaxNode(){};
	~CCParallaxNodeExtras(){};

	// just to avoid ugly later cast and also for safety
	CREATE_FUNC(CCParallaxNodeExtras);

	// Facility method (it’s expected to have it soon in COCOS2DX)    
	void incrementOffset(CCPoint offset, CCNode* node);  
} ; 

#endif //__PARALLAX_NODE_EXTRAS_H__
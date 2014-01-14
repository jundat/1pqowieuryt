#ifndef __HTTP_CLIENT_H__
#define __HTTP_CLIENT_H__

#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;

class HttpClientTest : public cocos2d::CCLayer
{
public:
    HttpClientTest();
	virtual ~HttpClientTest();
	
	CREATE_FUNC(HttpClientTest);
	
	virtual CCScene* scene() {
		CCScene *scene = CCScene::create();
		HttpClientTest *layer = HttpClientTest::create();
		scene->addChild(layer);
		return scene;
	}

    
    //Menu Callbacks
    void onMenuGetTestClicked(cocos2d::CCObject *sender);
    void onMenuPostTestClicked(cocos2d::CCObject *sender);
    void onMenuPostBinaryTestClicked(cocos2d::CCObject *sender);
    void onMenuPutTestClicked(cocos2d::CCObject *sender);
    void onMenuDeleteTestClicked(cocos2d::CCObject *sender);
    
    //Http Response Callback
    void onHttpRequestCompleted(cocos2d::extension::CCHttpClient *sender, cocos2d::extension::CCHttpResponse *response);

private:
    cocos2d::CCLabelTTF* m_labelStatusCode;
};

void runHttpClientTest();

#endif //__HTTPREQUESTHTTP_H

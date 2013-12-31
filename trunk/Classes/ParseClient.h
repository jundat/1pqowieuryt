#ifndef __CLIENT_H__
#define __CLIENT_H__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "HttpClient.h"
#include <string>

USING_NS_CC;
USING_NS_CC_EXT;

class ParseClient : public CCObject
{
public:
	static ParseClient* sharedParseClient() {
		if (_instance == NULL)
		{
			_instance = new ParseClient();
		}

		return _instance;
	}
	
	CCString* m_appId;
	CCString* m_restId;
	CCHttpRequest::HttpRequestType m_requestType;
	std::vector<std::string> m_header;

	void setRequestType(CCHttpRequest::HttpRequestType type) {
		this->m_requestType = type;
	}

	void setAppId(CCString* appId) {
		m_appId = appId;
		_calculateHeader();
	}

	void setRestId(CCString* restId) {
		m_restId = restId;
		_calculateHeader();
	}

	CCString* getAppId() {
		return CCString::create(m_appId->getCString());
	}

	CCString* getRestId() {
		return CCString::create(m_restId->getCString());
	}

	//pSelector = void onHttpRequestCompleted(CCHttpClient *sender, CCHttpResponse *response);
	void callCloudFunction(const char* funcName, const char* jsonData, SEL_HttpResponse pSelector, const char* tag = NULL);

private:
	static ParseClient* _instance;
	ParseClient() {
		m_appId = CCString::create("h6NHFlPE69MuxSjTeGqfFLiy0ME5TUEJBtFZ2RZE");
		m_restId = CCString::create("P0b6wS6boQgoZd2G5vQJTKd0JOQkgutqt3gN9Ifm");
		m_requestType = CCHttpRequest::kHttpPost;
		_calculateHeader();
	}

	void _calculateHeader() {
		CCString* s1 = CCString::createWithFormat("X-Parse-Application-Id: %s", m_appId->getCString());
		CCString* s2 = CCString::createWithFormat("X-Parse-REST-API-Key: %s", m_restId->getCString());

		m_header.push_back(std::string(s1->getCString())); //appid
		m_header.push_back(std::string(s2->getCString())); //apikey
		m_header.push_back(std::string("Content-Type: application/json"));
	}
};

#endif //__CLIENT_H__

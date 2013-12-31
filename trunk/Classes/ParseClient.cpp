#include "ParseClient.h"

ParseClient* ParseClient::_instance = NULL;

void ParseClient::callCloudFunction(const char* funcName, const char* jsonData, SEL_HttpResponse pSelector, const char* tag)
{
	CCHttpRequest* request = new CCHttpRequest();
	request->setHeaders(m_header);

	CCString* s = CCString::createWithFormat("https://api.parse.com/1/functions/%s", funcName);
	request->setUrl(s->getCString());
	
	request->setRequestType(CCHttpRequest::kHttpPost);
	request->setResponseCallback(this, pSelector);
	request->setRequestData(jsonData, strlen(jsonData));
	
	if (tag != NULL)
	{
		request->setTag(tag);
	}
	else
	{
		request->setTag("ParseClient Request");
	}

	CCHttpClient::getInstance()->send(request);
	request->release();
}

#ifndef __MY_MACRO_H__
#define __MY_MACRO_H__

#include "cocos2d.h"
USING_NS_CC;
using namespace std;


#define LIMIT_VALUE(x, minX, maxX) \
{\
	if(x < minX) { x = minX; }\
	if(x > maxX) { x = maxX; }\
}

static string MY_LIMIT_STR(string srcString, int maxlen, string addString)
{
	if ((int)srcString.length() > maxlen)
	{
		srcString = srcString.substr(0, maxlen);
		srcString.append(addString);
	}
	return srcString;
}


#define MY_RELEASE(pObj) \
do\
{\
	if((pObj) != NULL) {\
		(pObj)->release();\
		delete (pObj);\
		(pObj) = NULL;\
	}\
} while(0);


#endif //__MY_MACRO_H__

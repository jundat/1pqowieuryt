#pragma once

#ifndef __TEXT_LOADER_H__
#define __TEXT_LOADER_H__

#include "Global.h"
#include "cocos2d.h"

USING_NS_CC;
using namespace std;



#define TEXT_FILE		"language.plist"

#define LANGUAGE_ENGLISH		"English"
#define LANGUAGE_VIETNAMESE		"Vietnamese"
#define LANGUAGE_FRENCH			"French"
#define LANGUAGE_GERMAN			"German"
#define LANGUAGE_ITALIAN		"Italian"
#define LANGUAGE_SPANISH		"Spanish"
#define LANGUAGE_PORTUGUESE		"Portugese"
#define LANGUAGE_RUSSIAN		"Russian"
#define LANGUAGE_TURKISH		"Turkish"
#define LANGUAGE_KOREAN			"Korean"
#define LANGUAGE_JAPANESE		"Japanese"
#define LANGUAGE_CHINESE		"Chinese"


class TextLoader
{
public:
	TextLoader(void);
	~TextLoader(void);
	static TextLoader* shareTextLoader();

public:
	void setCurrentLanguage(string curlanguage);
	string getCurrentLanguage();
	const char* getText(const char* id);

protected:
	string m_currentLanguage;
	CCDictionary* m_allLanguageDict;
	CCDictionary* m_currentLanguageDict;

	static TextLoader* s_instance;
};


static const char* TXT(const char* id)
{
	const char* text = TextLoader::shareTextLoader()->getText(id);
	//CCLOG("TXT: %s => %s", id, text);
	return text;
}



#endif //__TEXT_LOADER_H__
#include "TextLoader.h"
#include "cocos2d.h"

USING_NS_CC;

//////////////////////////////////////////////////////////////////////////

TextLoader* TextLoader::s_instance = NULL;


TextLoader::TextLoader(void)
{
	std::string fullPath = CCFileUtils::sharedFileUtils()->fullPathForFilename(TEXT_FILE);
	m_allLanguageDict = CCDictionary::createWithContentsOfFile(fullPath.c_str());
	m_allLanguageDict->retain();
}

TextLoader::~TextLoader(void)
{
}

TextLoader* TextLoader::shareTextLoader()
{
	if (s_instance == NULL)
	{
		s_instance = new TextLoader();
		s_instance->setCurrentLanguage(LANGUAGE_ENGLISH);
	}
	
	return s_instance;
}

const char* TextLoader::getText(const char* id )
{
	const char* text = m_currentLanguageDict->valueForKey(string(id))->getCString();
	return text;
}

void TextLoader::setCurrentLanguage( string curlanguage )
{
	m_currentLanguage = string(curlanguage);
	m_currentLanguageDict = (CCDictionary*)m_allLanguageDict->objectForKey(m_currentLanguage);
	m_currentLanguageDict->retain();
}

std::string TextLoader::getCurrentLanguage()
{
	return string(m_currentLanguage);
}


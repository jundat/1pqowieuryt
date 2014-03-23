#include "TextLoader.h"
#include "cocos2d.h"

USING_NS_CC;

//////////////////////////////////////////////////////////////////////////

TextLoader* TextLoader::s_instance = NULL;


TextLoader::TextLoader(void)
{
	std::string fullPath = CCFileUtils::sharedFileUtils()->fullPathForFilename(TEXT_FILE);
	m_allLanguageDict = CCDictionary::createWithContentsOfFile(fullPath.c_str());
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

const char* TextLoader::getText(const std::string id )
{
	string text;

	text = m_currentLanguageDict->valueForKey(id)->getCString();
	
	return text.c_str();
}

void TextLoader::setCurrentLanguage( string curlanguage )
{
	m_currentLanguage = string(curlanguage);
	m_currentLanguageDict = (CCDictionary*)m_allLanguageDict->objectForKey(m_currentLanguage);
}

std::string TextLoader::getCurrentLanguage()
{
	return string(m_currentLanguage);
}


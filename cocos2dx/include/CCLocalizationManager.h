#ifndef __CCLOCALIZATIONMANAGER_H__
#define __CCLOCALIZATIONMANAGER_H__

#include "CCMutableDictionary.h"
#include "CCString.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	#define  PLATFORM_WCHAR wchar_t
#else
	#define  PLATFORM_WCHAR unsigned short
#endif

namespace   cocos2d {

class CC_DLL CCLocalizationManager : public CCObject
{
public:
	~CCLocalizationManager();
	CCLocalizationManager();

	virtual void	setLocalizationFilePath(const std::string& path);

	static CCLocalizationManager* defaultLocalizationManager()
	{
		if(myDefaultManager==0)
		{
			myDefaultManager=new CCLocalizationManager();
		}
		return myDefaultManager;
	}

	const CCString*	getLocalizedString(const std::string& key) const;

protected:

	// strings file parser methods
	
	bool	ParseStringsFile(const char*);
	CCString*	GetNextLocalizedString(char* pBuffer,unsigned long& currentpos,std::string& key,unsigned long filelen);
	bool	ExtractQuoted(PLATFORM_WCHAR*& startchar,std::string& quoted,unsigned long& currentpos,unsigned long filelen,bool isKey);
	bool	ExtractQuoted(PLATFORM_WCHAR*& startchar,unsigned short* buffer,unsigned long& currentpos,unsigned long filelen,bool isKey);
	void	ExtractComment(PLATFORM_WCHAR*& startchar,unsigned long& currentpos,unsigned long filelen);
	void	GotoNextLine(PLATFORM_WCHAR*& startchar,unsigned long& currentpos,unsigned long filelen);
	void	GotoCommentEnd(PLATFORM_WCHAR*& startchar,unsigned long& currentpos,unsigned long filelen);

	static CCLocalizationManager*	myDefaultManager;

	std::string				myLocalizationFilePath;

	CCMutableDictionary<std::string, CCString*> m_LocalizedString;
	unsigned short								m_wbuffer[2048];
};

inline const CCString*	CCLocalizedString(const CCString* key,const std::string& /*comment*/)
{
	if(key)
	{
		const CCString* value = CCLocalizationManager::defaultLocalizationManager()->getLocalizedString(key->m_sString);
		if ( value != NULL )
		{
			return value;
		}
		else
		{
			return key;
		}
	}
	return 0;
}

inline const CCString*	CCLocalizedString(const std::string& key,const std::string& /*comment*/)
{
	return CCLocalizationManager::defaultLocalizationManager()->getLocalizedString(key);
}

} // namespace   cocos2d

#endif //__CCLOCALIZATIONMANAGER_H__
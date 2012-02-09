#if!defined (CCBUNDLE_H)
#define CCBUNDLE_H

#include "cocos2d.h"
#include "CCString.h"

namespace   cocos2d {

class CC_DLL CCBundle
{
private:
		//Static fields
		static CCBundle* m_Singleton;
		
		CCBundle();

		std::map<std::string,std::vector<std::string> > myBundleMap;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
		void WriteBundleList();
#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
		std::string	GetNextLine(char* pBuffer,unsigned long& startPos,unsigned long size);
		static std::string assetsPath;
#endif

protected:

	void	RecursiveInitFileList(std::string);

public:
		~CCBundle();

		//Singleton
		static CCBundle* mainBundle()
		{
			if ( m_Singleton == NULL )
			{
				m_Singleton = new CCBundle();
			}
			return m_Singleton;
		}

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
		static std::string getAssetsPath()
		{
			return assetsPath;
		}

		static void setAssetsPath(std::string pAssetsPath)
		{
			assetsPath = pAssetsPath;
		}
#endif

		static	std::string	validateAssetPath(const std::string& path);

		//public methods
		CCString* pathForResource(std::string filename);
		CCString* pathForResource(std::string _pathname, std::string _extentionName);
};

} // namespace   cocos2d
#endif
/****************************************************************************
Copyright (c) 2010-2011 cocos2d-x.org
Copyright (c) 2011 Stephane Capo, Nicolas Blondel, Quentin Stere, Noel Leron

http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/

#include "CCBundle.h"
#include "CCGestureRecognizer.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#include <iostream>
#include <fstream>
#endif

using namespace cocos2d;

CCBundle* CCBundle::m_Singleton = 0;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
std::string CCBundle::assetsPath = "";
	#include <android/log.h>
	#include <jni.h>
	#define  LOG_TAG    "squids"
#ifdef _DEBUG
	#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)
	#define  LOGERR(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)
#else
	#define  LOGD(...) 
	#define  LOGERR(...) 
#endif // _DEBUG
#endif  // CC_PLATFORM_ANDROID


CCBundle::CCBundle()
{
	// generate map
	
	const char* StartPath=CCFileUtils::fullPathFromRelativePath("");
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	RecursiveInitFileList(StartPath);
#ifdef WRITE_BUNDLE_LIST
	WriteBundleList();
#endif
#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	CCFileData data("BundleList.txt", "rt");
	unsigned long size = data.getSize();
	char *pBuffer = (char*) data.getBuffer();
	
	if (pBuffer)
	{
		unsigned long startPos=0;

		std::string	line=GetNextLine(pBuffer,startPos,size);
		while(line != "")
		{
			size_t pos=line.find(';');
			if(pos!=string::npos)
			{
				std::string filename=line.substr(0,pos);
				std::string startDirectory=line.substr(pos+1,line.length()-(pos+1));

				std::vector<std::string> toadd;
				toadd.push_back(startDirectory);
				myBundleMap[filename]=toadd;
			}

			line=GetNextLine(pBuffer,startPos,size);
		}
	}
#endif

}

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
std::string	CCBundle::GetNextLine(char* pBuffer,unsigned long& startPos,unsigned long size)
{
	std::string result="";
	while(startPos<size)
	{
		if(pBuffer[startPos]==0xD)
		{
			startPos+=2;
			return result;
		}
		result+=pBuffer[startPos++];
	}
	return result;
}
#endif

void	CCBundle::RecursiveInitFileList(std::string	startDirectory)
{

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)

	WIN32_FIND_DATAA wfd;
	HANDLE			hFind;
	
	// Iterate through dirs

	std::vector<std::string>	subDirectoryList;	
	subDirectoryList.clear();

	std::string search=startDirectory + "*";

	hFind = ::FindFirstFileA(search.c_str(), &wfd);
	if(hFind != INVALID_HANDLE_VALUE)
	{
		do
		{
			// FIRST check if its a dir
			if(wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				// only valid directory
				if(!(wfd.cFileName[0] == '.') )
				{
					subDirectoryList.push_back(startDirectory + wfd.cFileName + "\\");
				}
			}
			else
			{
				if(!(wfd.cFileName[0] == '.') )
				{
					if(myBundleMap.find(wfd.cFileName)!=myBundleMap.end())
					{
						myBundleMap[wfd.cFileName].push_back(startDirectory);
					}
					else
					{
						std::vector<std::string> toadd;
						toadd.push_back(startDirectory);
						myBundleMap[wfd.cFileName]=toadd;
					}
				}
							
			}
		} while( ::FindNextFileA(hFind, &wfd));
		
		::FindClose(hFind);
	}

	// recurse now
	unsigned int i;
	for(i=0;i<subDirectoryList.size();i++)
	{
		RecursiveInitFileList(subDirectoryList[i]);
	}

#endif

}


CCBundle::~CCBundle()
{
}

// memory leak here probably
CCString* CCBundle::pathForResource(std::string filename)
{
	#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	LOGERR("----> CCBundle asks for: %s", filename.c_str());
	#endif

	if(myBundleMap.find(filename)!=myBundleMap.end())
	{
		filename = myBundleMap[filename][0]+filename;
		CCString* tmpstr=new CCString(filename.c_str());
		tmpstr->autorelease();

		#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
		LOGERR("CCBundle asks for CCString: %s", tmpstr->m_sString.c_str());
		#endif

		return tmpstr;
	}
	CCString* tmpstr=new CCString(filename.c_str());
	tmpstr->autorelease();
	return tmpstr;
}

// memory leak here probably
CCString* CCBundle::pathForResource(std::string _pathname, std::string _extentionName)
{
	std::string	filename=_pathname + '.' + _extentionName;

	#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	LOGD("----> CCBundle asks for: %s", filename.c_str());
	#endif

	if(myBundleMap.find(filename)!=myBundleMap.end())
	{
		filename = myBundleMap[filename][0]+filename;
		#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
		LOGD("CCBundle asks for BundleMap file: %s", filename.c_str());
		#endif

		CCString* tmpstr=new CCString(filename.c_str());
		tmpstr->autorelease();
		return tmpstr;
	}

	CCString* tmpstr=new CCString((_pathname + '.' + _extentionName).c_str());

	tmpstr->autorelease();
	return tmpstr;
}

// replace .. in paths
std::string	CCBundle::validateAssetPath(const std::string& path)
{
	std::string result=path;

	size_t pos=result.find("/..");
	while(pos!=string::npos)
	{
		size_t posslash=result.rfind("/",pos-1);
		
		if(posslash != string::npos)
		{
			result=result.substr(0,posslash)+result.substr(pos+3,result.length()-pos-3);
		}
		else
		{
			result=result.substr(pos+4,result.length()-pos-4);
		}

		pos=result.find("/..");
	}
	return result;
}

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
void CCBundle::WriteBundleList()
{
	std::ofstream fichier("BundleList.txt", ios::out | ios::trunc);
 
    if(fichier)
    {   
		std::map<std::string,std::vector<std::string> >::iterator IT = myBundleMap.begin();
		while(IT != myBundleMap.end())
		{
			fichier << IT->first << ";" << IT->second[0] << std::endl;
			IT++;
		}

		//Parse de la liste
        fichier.close();
    }
    else
	{
        std::cerr << "Impossible d'ouvrir le fichier !" << std::endl;
	}
}


#endif
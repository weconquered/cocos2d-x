//
//  EPResourceManager.h
//  EnjoyYourMoney
//
//  Created by Erawppa on 2011/8/30.
//  Copyright 2011年 __MyCompanyName__. All rights reserved.
//
#ifndef __EP_RESOURCEMANAGER_H__
#define __EP_RESOURCEMANAGER_H__

#include "cocos2d.h"

extern const char *kResourceLoadedNotif;
extern const char *kResourceLoadedTimeoutNotif;

class EPResourceManagerUnit;

class /*CC_DLL*/ EPResourceManager : public cocos2d::CCObject
{
public:
    EPResourceManager();
    ~EPResourceManager();
    
    static EPResourceManager *object(void);
    
    //
    // load/unload resources, sync
    //
    bool addPngResource(const char *pszFileName, bool plist = true, int group = -1); // ignore ".pvr"
    bool addPvrResource(const char *pszFileName, int group = -1); // ignore ".pvr.ccz"
    
    //
    // load/unload resources, ansyc, only support "Png"
    //
    bool addPngResourceAsync(const char *pszFileName, bool plist = true, int group = -1); // ignore ".pvr"
    void loadResourceAsync(void);
    void unloadResource(int group);
    void unloadAllResource(void);
    
    //
    // common
    //
    cocos2d::CCSpriteFrame *spriteFrame(const char *frameName);
    cocos2d::CCSpriteFrame *spriteFrameArg1Str(const char *format,const char *frameName);
    cocos2d::CCSpriteFrame *spriteFrameArg1Int(const char *format,int index);
private:
    //
    // internal functions
    //
    bool plistExisted(const char *plist);
    void addUnit(const char *pszFile, int group, bool plist, bool pvr);
    void textureLoadedCallback(cocos2d::CCObject* obj);
    long long getTime();
    void timeoutSelector(cocos2d::ccTime dt);
    
    //temp
    void startToLoad(cocos2d::ccTime dt);
        
    // keep all resources
    cocos2d::CCArray *m_resources;
    
    int m_asyncLoadCount;
    int m_asyncTotalCount;
    long long m_loadingTime;
};

class /*CC_DLL*/ EPResourceManagerUnit : public cocos2d::CCObject
{
public:
    EPResourceManagerUnit(const char *pszFilename, 
                          int group, 
                          bool plist,
                          bool pvr);
    ~EPResourceManagerUnit();
    
    void setState(int state,cocos2d::CCTexture2D *tex);
    void releaseResource(int group);
    
    CC_SYNTHESIZE_READONLY(int, m_group, Group)
    CC_SYNTHESIZE_READONLY(bool, m_plist, Plist)
    CC_SYNTHESIZE_READONLY(bool, m_pvr, Pvr)
    CC_SYNTHESIZE_READONLY(std::string, m_texPath, TexPath)
    CC_SYNTHESIZE_READONLY(std::string, m_plistPath, PlistPath)
    CC_SYNTHESIZE_READONLY(std::string, m_pszFilename, Filename)
    CC_PROPERTY_READONLY(int, m_state, State)

private:
    cocos2d::CCTexture2D *m_tex;
};

#endif //__EP_RESOURCEMANAGER_H__
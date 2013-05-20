#include "AppDelegate.h"
#include "MainLayerLoader.h"
#include "TimelineTestLayerLoader.h"

USING_NS_CC;
USING_NS_CC_EXT;

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    CCDirector* pDirector = CCDirector::sharedDirector();
    CCEGLView* pEGLView = CCEGLView::sharedOpenGLView();

    pDirector->setOpenGLView(pEGLView);
	
    // turn on display FPS
    pDirector->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    pDirector->setAnimationInterval(1.0 / 60);


    CCFileUtils* fileUtils = CCFileUtils::sharedFileUtils();
    std::vector<std::string> searchPaths;
    searchPaths.push_back("Published-iOS");
    fileUtils->setSearchPaths(searchPaths);
    
    std::vector<std::string> resourceOrders;
    
    CCSize frameSize = pEGLView->getFrameSize();
    CCSize resSize;
    
    CCSize designSize = CCSizeMake(480, 320);
    
    if (frameSize.height > 768)
    {// Ipadhd
        resourceOrders.push_back("resources-ipadhd");
        resSize = CCSizeMake(2048, 1536);
    }
    else if (frameSize.height > 640)
    {// Ipad
        resourceOrders.push_back("resources-ipad");
        resSize = CCSizeMake(1024, 768);
    }
    else if (frameSize.height > 320)
    {// Iphone-hd 3.5 and 4.0 inch
        resourceOrders.push_back("resources-iphonehd");
        resSize = CCSizeMake(960, 640);
    }
    else
    {// Iphone
        resourceOrders.push_back("resources-iphone");
        resSize = CCSizeMake(480, 320);
    }
    
    
    pEGLView->setDesignResolutionSize(designSize.width, designSize.height ,kResolutionFixedHeight);
    
    pDirector->setContentScaleFactor(resSize.height/designSize.height);
    fileUtils->setSearchResolutionsOrder(resourceOrders);
    
    fileUtils->loadFilenameLookupDictionaryFromFile("fileLookup.plist");
    
    // create a scene. it's an autorelease object
    CCScene *pScene = CCScene::create();
    
    /* Create an autorelease CCNodeLoaderLibrary. */
    CCNodeLoaderLibrary * ccNodeLoaderLibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
    
//    ccNodeLoaderLibrary->registerCCNodeLoader("MainLayer", MainLayerLoader::loader());
    ccNodeLoaderLibrary->registerCCNodeLoader("TimelineTestLayer", TimelineTestLayerLoader::loader());
    /* Create an autorelease CCBReader. */
    cocos2d::extension::CCBReader * ccbReader = new cocos2d::extension::CCBReader(ccNodeLoaderLibrary);
    CCBAnimationManager* manager = ccbReader->getAnimationManager();
    /* Read a ccbi file. */
//    CCNode * node = ccbReader->readNodeGraphFromFile("MainScene.ccbi");
    CCNode * node = ccbReader->readNodeGraphFromFile("TimelineTest.ccbi");
    ((TimelineTestLayer*)node)->setAnimationManganger(manager);
    ccbReader->release();
    
    pScene->addChild(node);
    
    // run
    pDirector->runWithScene(pScene);
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    CCLOG("winsize width = %f, height = %f", winSize.width, winSize.height);
    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    CCDirector::sharedDirector()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    CCDirector::sharedDirector()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
}

#include "AppDelegate.h"
#include "MainLayerLoader.h"

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

    CCSize designResolution = CCSizeMake(2048, 1536);
    pEGLView->setDesignResolutionSize(designResolution.width, designResolution.height ,kResolutionFixedHeight);
    CCFileUtils* fileUtils = CCFileUtils::sharedFileUtils();
    std::vector<std::string> searchPaths;
    searchPaths.push_back("Published-iOS");
    fileUtils->setSearchPaths(searchPaths);
    
    std::vector<std::string> resourceOrders;
    
    CCSize frameSize = pEGLView->getFrameSize();
    CCSize resSize;
    
    if (frameSize.height > 768)
    {// Ipadhd
        resourceOrders.push_back("resources-ipadhd");
        resSize = CCSizeMake(2048, 1536);
        pDirector->setContentScaleFactor(1536.0f/designResolution.height);
    }
    else if (frameSize.height > 640)
    {// Ipad
        resourceOrders.push_back("resources-ipad");
        resSize = CCSizeMake(1024, 768);
        pDirector->setContentScaleFactor(768.0f/designResolution.height);
    }
    else if (frameSize.height > 320)
    {// Iphone-hd 3.5 and 4.0 inch
        resourceOrders.push_back("resources-iphonehd");
        resSize = CCSizeMake(960, 640);
        pDirector->setContentScaleFactor(640.0f/designResolution.height);
    }
    else
    {// Iphone
        resourceOrders.push_back("resources-iphone");
        resSize = CCSizeMake(480, 320);
    }
    
    pDirector->setContentScaleFactor(resSize.height/designResolution.height);
    fileUtils->setSearchResolutionsOrder(resourceOrders);
    
    // create a scene. it's an autorelease object
    CCScene *pScene = CCScene::create();
    
    /* Create an autorelease CCNodeLoaderLibrary. */
    CCNodeLoaderLibrary * ccNodeLoaderLibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
    
    ccNodeLoaderLibrary->registerCCNodeLoader("MainLayer", MainLayerLoader::loader());
    
    /* Create an autorelease CCBReader. */
    cocos2d::extension::CCBReader * ccbReader = new cocos2d::extension::CCBReader(ccNodeLoaderLibrary);
    
    /* Read a ccbi file. */
    CCNode * node = ccbReader->readNodeGraphFromFile("MainScene.ccbi");
    
    ccbReader->release();
    
    pScene->addChild(node);
    
    // run
    pDirector->runWithScene(pScene);

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

#include "AppDelegate.h"

#include "cocos2d.h"
#include "HelloWorldScene.h"

#include "IAPCMGC.h"
#include "IAPChinaTelecom.h"
#include "IAPUserCenter.h"

using namespace cocos2d::plugin;

USING_NS_CC;

IAPProtocol* AppDelegate::s_pIAP = NULL;

IAPProtocol* AppDelegate::getIAP()
{
    return s_pIAP;
}

AppDelegate::AppDelegate()
{

}

AppDelegate::~AppDelegate()
{
}

bool AppDelegate::applicationDidFinishLaunching()
{
    CCArray* productArray = CCArray::createWithContentsOfFile("products.plist");
    
    //s_pIAP = IAPUserCenter::create(productArray, "buyudaren2test", "874126E837810FDB112C274D1D7E6FD98");
    //s_pIAP = IAPChinaTelecom::create(productArray, "90235529");
    s_pIAP = IAPCMGC::create(productArray, "HelloIAP", "CompanyName", "telephoneNumber");
    // initialize director
    CCDirector *pDirector = CCDirector::sharedDirector();
    pDirector->setOpenGLView(CCEGLView::sharedOpenGLView());

    // enable High Resource Mode(2x, such as iphone4) and maintains low resource on other devices.
    // pDirector->enableRetinaDisplay(true);

    // turn on display FPS
    pDirector->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    pDirector->setAnimationInterval(1.0 / 60);

    // create a scene. it's an autorelease object
    CCScene *pScene = HelloWorld::scene();

    // run
    pDirector->runWithScene(pScene);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground()
{
    CCDirector::sharedDirector()->pause();

    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground()
{
    CCDirector::sharedDirector()->resume();
    
    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
}

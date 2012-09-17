#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"


using namespace cocos2d;
using namespace CocosDenshion;
using namespace cocos2d::iap;

CCScene* HelloWorld::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    HelloWorld *layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    CCMenuItemImage *pCloseItem = CCMenuItemImage::create(
                                        "CloseNormal.png",
                                        "CloseSelected.png",
                                        this,
                                        menu_selector(HelloWorld::menuCloseCallback) );
    pCloseItem->setPosition( ccp(CCDirector::sharedDirector()->getWinSize().width - 20, 20) );



    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label
    CCLabelTTF* pLabel = CCLabelTTF::create("Hello World", "Thonburi", 34);

    // ask director the window size
    CCSize size = CCDirector::sharedDirector()->getWinSize();

    // position the label on the center of the screen
    pLabel->setPosition( ccp(size.width / 2, size.height - 20) );

    // add the label as a child to this layer
    this->addChild(pLabel, 1);

    // add "HelloWorld" splash screen"
    CCSprite* pSprite = CCSprite::create("HelloWorld.png");

    // position the sprite on the center of the screen
    pSprite->setPosition( ccp(size.width/2, size.height/2) );

    // add the sprite as a child to this layer
    this->addChild(pSprite, 0);
    
    CCLabelTTF* label = CCLabelTTF::create("CCCrypto", "Arial", 24);
    CCMenuItemLabel* item1 = CCMenuItemLabel::create(label,
                                                     this,
                                                     menu_selector(HelloWorld::testIAP));

    item1->setPosition( ccp(CCDirector::sharedDirector()->getWinSize().width/2, 100) );

    // create menu, it's an autorelease object
    CCMenu* pMenu = CCMenu::create(pCloseItem, item1, NULL);
    pMenu->setPosition( CCPointZero );
    this->addChild(pMenu, 1);

    return true;
}

void HelloWorld::menuCloseCallback(CCObject* pSender)
{
    CCDirector::sharedDirector()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

void HelloWorld::testIAP(cocos2d::CCObject* pSender)
{
    IAP::getInstance()->loadOneProduct("abc", 1, this);
}

void HelloWorld::onIAPLoginCompleted()
{
    CCLog("HelloWorld::onIAPLoginCompleted");
}

void HelloWorld::onIAPLoginFailed()
{
    CCLog("HelloWorld::onIAPLoginFailed");
}

void HelloWorld::onIAPRequestProductsCompleted(CCArray* productsId, CCArray* invalidProductsId/* = NULL*/)
{
    CCLog("HelloWorld::onIAPRequestProductsCompleted");
    IAP::getInstance()->purchaseOneProduct("abc", this);
}

void HelloWorld::onIAPRequestProductsFailed(IAPProductsRequestErrorCode errorCode, const char* errorMsg)
{
    CCLog("HelloWorld::onIAPRequestProductsFailed");
}

void HelloWorld::onTransactionFailed(IAPTransaction* pTransaction)
{
    CCLog("HelloWorld::onTransactionFailed");
}

void HelloWorld::onTransactionCompleted(IAPTransaction* pTransaction)
{
    CCLog("HelloWorld::onTransactionCompleted");
}

void HelloWorld::onTransactionRestored(IAPTransaction* pTransaction)
{
    CCLog("HelloWorld::onTransactionRestored");
}

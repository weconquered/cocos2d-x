#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include "AppDelegate.h"

using namespace cocos2d;
using namespace CocosDenshion;
using namespace cocos2d::plugin;

enum {
    kTag100Coins = 0,
    kTag200Coins,
    kTag300Coins,
    kTag400Coins
};

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
    
    // create menu, it's an autorelease object
    CCMenu* pMenu = CCMenu::create(pCloseItem, NULL);
    pMenu->setPosition( CCPointZero );
    this->addChild(pMenu, 1);

    for (int i = 0; i < 4; i++)
    {
        char szProductName[100] = {0};
        sprintf(szProductName, "%d00 coins", i+1);
        CCLabelTTF* label = CCLabelTTF::create(szProductName, "Arial", 24);
        CCMenuItemLabel* item = CCMenuItemLabel::create(label,
                                                         this,
                                                         menu_selector(HelloWorld::testIAP));

        item->setPosition( ccp(size.width/2, size.height / 5 * (4-i)) );
        item->setTag(kTag100Coins + i);
        pMenu->addChild(item);
    }

    AppDelegate::getIAP()->setDelegate(this);

    return true;
}

void HelloWorld::menuCloseCallback(CCObject* pSender)
{
    AppDelegate::getIAP()->notifyIAPToExit();
}

void HelloWorld::testIAP(cocos2d::CCObject* pSender)
{
    CCNode* pNode = (CCNode*) pSender;
    switch (pNode->getTag())
    {
        case kTag100Coins:
            AppDelegate::getIAP()->loadProduct("coin100");
            break;
        case kTag200Coins:
            AppDelegate::getIAP()->loadProduct("coin200");
            break;
        case kTag300Coins:
            AppDelegate::getIAP()->loadProduct("coin300");
            break;
        case kTag400Coins:
            AppDelegate::getIAP()->loadProduct("coin400");
            break;
        default:
            break;
    }
    
}

void HelloWorld::onLogonFinished(ReturnVal r)
{
    CCLog("HelloWorld::onIAPLogonFinished, r = %d", r.isSucceed);
}

void HelloWorld::onLoadProductsFinished(ReturnVal r, CCArray* productsId, CCArray* invalidProductsId/* = NULL */)
{
    CCLog("HelloWorld::onIAPLoadProductsFinished, r = %d", r.isSucceed);
    if (r.isSucceed)
    {
        if (productsId != NULL)
        {
            if (productsId->count() == 1)
            {
                CCString* pProductId = (CCString*)productsId->objectAtIndex(0);
                CCLog("HelloWorld::onLoadProductsFinished = %s", pProductId->getCString());
                AppDelegate::getIAP()->purchaseProduct(pProductId->getCString());
            }
        }
        else
        {
            // Please invoke 'purchaseSomeProduct'
        }
    }
    else
    {
        CCMessageBox("Failure", "Load Product fails");
    }
}

void HelloWorld::onTransactionFinished(ReturnVal r, IAPTransaction* pTransaction)
{
    CCLog("HelloWorld::onIAPTransactionFinished, r = %d", r.isSucceed);
    if (r.isSucceed)
    {
        CCMessageBox("Success", "Transaction succeed.");
    }
    else
    {
        CCMessageBox("Failure", "Transaction fails,");
    }
}

void HelloWorld::onNotifyGameExit()
{
    CCDirector::sharedDirector()->end();
 #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
     exit(0);
 #endif
}


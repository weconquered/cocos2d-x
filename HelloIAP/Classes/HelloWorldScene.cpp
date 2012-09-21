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

#define kProductId_100Coins "coin100"
#define kProductId_200Coins "coin200"
#define kProductId_300Coins "coin300"
#define kProductId_400Coins "coin400"

struct tagIdTagMap
{
    const char* id;
    int tag;
    /* data */
} s_idTagMap[] = {
    {kProductId_100Coins, kTag100Coins},
    {kProductId_200Coins, kTag200Coins},
    {kProductId_300Coins, kTag300Coins},
    {kProductId_400Coins, kTag400Coins}
};

static int getTagByProductId(const char* pProductId)
{
    for (int i = 0; i < sizeof(s_idTagMap)/sizeof(s_idTagMap[0]); ++i)
    {
        if (0 == strcmp(s_idTagMap[i].id, pProductId))
        {
            return s_idTagMap[i].tag;
        }
    }
    return -1;
}

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

    CCSize size = CCDirector::sharedDirector()->getWinSize();
    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    CCMenuItemImage *pCloseItem = CCMenuItemImage::create(
                                        "CloseNormal.png",
                                        "CloseSelected.png",
                                        this,
                                        menu_selector(HelloWorld::menuCloseCallback) );
    pCloseItem->setPosition( ccp(size.width - 20, 20) );


    CCLabelTTF* label = NULL;

    label = CCLabelTTF::create("Load Products", "Arial", 24);
    m_pLoadItem = CCMenuItemLabel::create(label, this, menu_selector(HelloWorld::menuCallbackLoadProducts));
    m_pLoadItem->setPosition( ccp(100, size.height/2) );


    label = CCLabelTTF::create("Reset", "Arial", 24);
    m_pResetItem = CCMenuItemLabel::create(label, this, menu_selector(HelloWorld::menuCallbackReset));
    m_pResetItem->setPosition( ccp(100, size.height/2) );
    m_pResetItem->setVisible(false);

    m_pMenu1 = CCMenu::create(pCloseItem, m_pLoadItem, m_pResetItem, NULL);
    m_pMenu1->setPosition( CCPointZero );
    this->addChild(m_pMenu1, 1);

    m_pMenu2 = CCMenu::create();
    m_pMenu2->setPosition( CCPointZero );
    this->addChild(m_pMenu2, 1);

    AppDelegate::getIAP()->setDelegate(this);

    return true;
}

void HelloWorld::menuCallbackReset(cocos2d::CCObject* pSender)
{
    m_pResetItem->setVisible(false);
    m_pLoadItem->setVisible(true);
    m_pMenu2->removeAllChildrenWithCleanup(true);
}

void HelloWorld::menuCloseCallback(CCObject* pSender)
{
    AppDelegate::getIAP()->notifyIAPToExit();
}

void HelloWorld::menuCallbackLoadProducts(cocos2d::CCObject* pSender)
{
    m_pResetItem->setVisible(true);
    m_pLoadItem->setVisible(false);

    CCArray* pProductIdArray =  CCArray::create (
                                    ccs(kProductId_100Coins),
                                    ccs(kProductId_200Coins),
                                    ccs("invalidItem"),
                                    ccs(kProductId_300Coins),
                                    ccs(kProductId_400Coins),
                                    NULL
                                );
    AppDelegate::getIAP()->loadProducts(pProductIdArray);
}

void HelloWorld::menuCallbackPurchaseProduct(cocos2d::CCObject* pSender)
{
    CCNode* pNode = (CCNode*) pSender;
    switch (pNode->getTag())
    {
        case kTag100Coins:
            AppDelegate::getIAP()->purchaseProduct(kProductId_100Coins);
            break;
        case kTag200Coins:
            AppDelegate::getIAP()->purchaseProduct(kProductId_200Coins);
            break;
        case kTag300Coins:
            AppDelegate::getIAP()->purchaseProduct(kProductId_300Coins);
            break;
        case kTag400Coins:
            AppDelegate::getIAP()->purchaseProduct(kProductId_400Coins);
            break;
        default:
            break;
    }
}

void HelloWorld::onLogonFinished(ReturnVal r)
{
    CCLog("HelloWorld::onIAPLogonFinished, r = %d", r.isSucceed);
}

void HelloWorld::onLoadProductsFinished(ReturnVal r, CCArray* pProductArray)
{
    CCLog("HelloWorld::onIAPLoadProductsFinished, r = %d", r.isSucceed);
    if (r.isSucceed)
    {
        CCSize size = CCDirector::sharedDirector()->getWinSize();
        m_pLoadItem->setVisible(false);
        m_pResetItem->setVisible(true);

        if (pProductArray != NULL && pProductArray->count() > 0)
        {
            int count = pProductArray->count();
            for (int i = 0; i < count; i++)
            {
                IAPProduct* product = (IAPProduct*)pProductArray->objectAtIndex(i);

                CCDictionary* pProductTable = product->getProductInfoTable();
                CCString* pProductName = (CCString*)pProductTable->objectForKey("productName");

                CCLabelTTF* label = CCLabelTTF::create(pProductName->getCString(), "Arial", 24);
                CCMenuItemLabel* item = CCMenuItemLabel::create(label, this, menu_selector(HelloWorld::menuCallbackPurchaseProduct));

                item->setPosition( ccp(size.width/2, size.height / (count+1) * (count-i)) );
                int tag = getTagByProductId(product->getProductId());
                if (tag != -1)
                {
                    item->setTag(tag);
                }
                
                m_pMenu2->addChild(item);

                
                CCDictElement* pElement = NULL;
                CCDICT_FOREACH(pProductTable, pElement)
                {
                    const char* key = pElement->getStrKey();
                    CCString* value = (CCString*)pElement->getObject();
                    CCLog("product info: key = %s, value = %s", key, value->getCString());
                }
            }
        }
        else
        {
            CCMessageBox("Failure", "No products");
        }
    }
    else
    {
        if (r.errorCode != kErrorPreviousRequestUncompleted)
        {
            CCMessageBox("Failure", "Load Product fails");
        }
        else
        {
            CCLog("HelloWorld::onLoadProductsFinished,kErrorPreviousRequestUncompleted");
        }
    }
    CCLog("HelloWorld::onLoadProductsFinished, end");
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


#include "HelloWorldScene.h"
#include "CCMutableArray2.h"
#include "CCArray2.h"

USING_NS_CC;

CCScene* HelloWorld::scene()
{
	// 'scene' is an autorelease object
	CCScene *scene = CCScene::node();
	
	// 'layer' is an autorelease object
	HelloWorld *layer = HelloWorld::node();

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
	CCMenuItemImage *pCloseItem = CCMenuItemImage::itemFromNormalImage(
										"CloseNormal.png",
										"CloseSelected.png",
										this,
										menu_selector(HelloWorld::menuCloseCallback) );
	pCloseItem->setPosition( ccp(CCDirector::sharedDirector()->getWinSize().width - 20, 20) );

	// create menu, it's an autorelease object
	CCMenu* pMenu = CCMenu::menuWithItems(pCloseItem, NULL);
	pMenu->setPosition( CCPointZero );
	this->addChild(pMenu, 1);

	/////////////////////////////
	// 3. add your codes below...

	// add a label shows "Hello World"
	// create and initialize a label
    CCLabelTTF* pLabel = CCLabelTTF::labelWithString("Hello World", "Arial", 24);
	// ask director the window size
	CCSize size = CCDirector::sharedDirector()->getWinSize();

	// position the label on the center of the screen
	pLabel->setPosition( ccp(size.width / 2, size.height - 50) );

	// add the label as a child to this layer
	this->addChild(pLabel, 1);

	// add "HelloWorld" splash screen"
	CCSprite* pSprite = CCSprite::spriteWithFile("HelloWorld.png");

	// position the sprite on the center of the screen
	pSprite->setPosition( ccp(size.width/2, size.height/2) );

	// add the sprite as a child to this layer
	this->addChild(pSprite, 0);
	
    
    // test smart ptr
    //SharedPtr<CCMutableArray2<SharedPtr<CCObject>>> p = CCMutableArray2<SharedPtr<CCObject>>::newMutableArray();
    CCMutableArray2<SharedPtr<CCObject>>* p = new CCMutableArray2<SharedPtr<CCObject>>();
    SharedPtr<CCObject> pObj1 = CCObject::object();
    pObj1->m_idTest = 1;
    SharedPtr<CCObject> pObj2 = CCObject::object();
    pObj2->m_idTest = 2;
    SharedPtr<CCObject> pObj3 = CCObject::object();
    pObj3->m_idTest = 3;
    SharedPtr<CCObject> pObj4 = CCObject::object();
    pObj4->m_idTest = 4;
//     p->addObject(pObj1);
//     p->addObject(pObj2);
//     p->addObject(pObj3);
//     p->addObject(pObj4);

    SharedPtr<CCArray2> p2 = CCArray2::array();
    p2->addObject(pObj1);
    p2->addObject(pObj2);
    p2->addObject(pObj3);
    p2->addObject(pObj4);
    p->addObject(p2);
    //

    CCObject* pObj;
    CCARRAY2_FOREACH(p2, pObj)
    {
        SharedPtr<CCObject> p(pObj);
        CCLog("object id = %d", pObj->m_idTest);
    }

    delete p;
	return true;
}

void HelloWorld::menuCloseCallback(CCObject* pSender)
{
	CCDirector::sharedDirector()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}

#define COCOS2D_DEBUG 1
#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include "EPScrollListLayer.h"

using namespace cocos2d;
using namespace CocosDenshion;

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
    
    createScrollH();
    
    createScrollV();
    
    createSyncScrollH();
	return true;
}

void HelloWorld::createSyncScrollH()
{
    // scroll 1
    float h = 50;
    CCSize zoneSize = CCSizeMake(400, h);
    
    CCNode* n = CCNode::node();
    n->setContentSize(zoneSize);
    n->setPosition(ccp(60,100));
    this->addChild(n);
    
    EPScrollListLayer* scrollLayer = EPScrollListLayer::nodeWithLayers(NULL, zoneSize,CCPointZero,false);
    scrollLayer->addTapSelector(this, callfuncN_selector(HelloWorld::scrollTapSelector));
    n->addChild(scrollLayer);
    
    for (int i=40 ; i<60 ; i++)
    {
        CCSprite* sp = CCSprite::spriteWithFile("Icon-Small-50.png");
        sp->setPosition(ccp(h/2,h/2));
        
        CCLayer* l = CCLayer::node();
        l->setContentSize(sp->getContentSize());
        l->setTag(i);
        l->addChild(sp);
        scrollLayer->addLayer(l);
    }
    
    //scroll 2
    CCNode* n2 = CCNode::node();
    n2->setContentSize(zoneSize);
    n2->setPosition(ccp(60,160));
    this->addChild(n2);
    
    EPScrollListLayer* scrollLayer2 = EPScrollListLayer::nodeWithLayers(NULL, zoneSize,CCPointZero,false);
    scrollLayer2->addTapSelector(this, callfuncN_selector(HelloWorld::scrollTapSelector));
    n2->addChild(scrollLayer2);
    
    for (int i=60 ; i<80 ; i++)
    {
        CCSprite* sp = CCSprite::spriteWithFile("Icon-Small-50.png");
        sp->setPosition(ccp(h/2,h/2));
        
        CCLayer* l = CCLayer::node();
        l->setContentSize(sp->getContentSize());
        l->setTag(i);
        l->addChild(sp);
        scrollLayer2->addLayer(l);
    }
    
    scrollLayer->syncScrollLayer(scrollLayer2);
    scrollLayer2->syncScrollLayer(scrollLayer);
}

void HelloWorld::createScrollH()
{    
    float h = 50;
    CCSize zoneSize = CCSizeMake(480, h);
    
    CCNode* n = CCNode::node();
    n->setContentSize(zoneSize);
    n->setPosition(ccp(0,0));
    this->addChild(n);
    
    EPScrollListLayer* scrollLayer = EPScrollListLayer::nodeWithLayers(NULL, zoneSize,CCPointZero,false);
    scrollLayer->addTapSelector(this, callfuncN_selector(HelloWorld::scrollTapSelector));
    n->addChild(scrollLayer);

    for (int i=0 ; i<20 ; i++)
    {
        CCSprite* sp = CCSprite::spriteWithFile("Icon-Small-50.png");
        sp->setPosition(ccp(h/2,h/2));
        
        CCLayer* l = CCLayer::node();
        l->setContentSize(sp->getContentSize());
        l->setTag(i);
        l->addChild(sp);
        scrollLayer->addLayer(l);
    }
}

void HelloWorld::createScrollV()
{    
    float w = 50;
    CCSize zoneSize = CCSizeMake(w,250);
    
    CCNode* n = CCNode::node();
    n->setContentSize(zoneSize);
    n->setPosition(ccp(10,60));
    this->addChild(n);
    
    EPScrollListLayer* scrollLayer = EPScrollListLayer::nodeWithLayers(NULL, zoneSize,CCPointZero,true);
    scrollLayer->addTapSelector(this, callfuncN_selector(HelloWorld::scrollTapSelector));
    n->addChild(scrollLayer);
    
    for (int i=20 ; i<40 ; i++)
    {
        CCSprite* sp = CCSprite::spriteWithFile("Icon-Small-50.png");
        sp->setPosition(ccp(w/2,w/2));
        
        CCLayer* l = CCLayer::node();
        l->setContentSize(sp->getContentSize());
        l->setTag(i);
        l->addChild(sp);
        scrollLayer->addLayer(l);
    }
}

void HelloWorld::scrollTapSelector(CCNode* sender)
{
    CCLOG("scrollTapSelector : %d", sender->getTag());
}

void HelloWorld::scrollTouchBeginSelector(CCNode* sender)
{
}

void HelloWorld::scrollTouchEndSelector(CCNode* sender)
{
}

void HelloWorld::menuCloseCallback(CCObject* pSender)
{
	CCDirector::sharedDirector()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}
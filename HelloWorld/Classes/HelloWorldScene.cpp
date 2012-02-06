#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include "EPNotificationCenter.h"
#include "EPResourceManager.h"
#include "EPXmlReader.h"
#include "EPXmlValue.h"

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
	if ( !EPGestureRecognizerLayer::init() )
	{
		return false;
	}
    
    EPNotificationCenter::sharedNotifCenter()->addObserver(this, 
                                                           callfuncO_selector(HelloWorld::completedCallback),
                                                           kResourceLoadedNotif, 
                                                           NULL);
    
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    
    CCMenuItem *item1 = CCMenuItemFont::itemFromString("loaded", 
                                                       this, 
                                                       callfuncO_selector(HelloWorld::loadedCallback));
    CCMenu *m = CCMenu::menuWithItems(item1,NULL);
    m->setPosition(ccpSub(ccp_CENTER(), ccp_IPAD(0, -130)));
    this->addChild(m);
    
	return true;
}

void HelloWorld::loadedCallback(CCObject* pSender)
{
    m_res = new EPResourceManager;    
    m_res->addPngResourceAsync("HelloWorld",false);
    m_res->addPngResourceAsync("4444_pics",true);
    m_res->loadResourceAsync();    
}

void HelloWorld::completedCallback(CCObject* pSender)
{
    CCLOG("completedCallback\n");
        
    CCSprite *sp = CCSprite::spriteWithSpriteFrameName("blocks.png");
    sp->setPosition(ccp_CENTER());
    sp->setTag(123);
    this->addChild(sp);
    
    this->addTapRecognizer(sp);
    
    this->addRotationRecognizer(sp);
    
    this->addPinchRecognizer(sp);
    
    this->addPanRecognizer(sp);
    
    this->addLongPressRecognizer(sp);
    this->setLongPressInterval(0.5f);
    
    //
    // sample.xml
    //
    EPXmlReader *reader = new EPXmlReader;
    reader->addArrayToken("bone");
    
    std::string path = CCFileUtils::fullPathFromRelativePath("sample.xml");
    reader->loadFromFile(path.c_str());
    
    EPXmlValue *root = reader->getRootDictionary();
    
    root->dump();
    
    EPXmlValue *bones = root->objectForKey("bones");
    EPXmlValue *boneArray = bones->objectForKey("bone");
    EPXmlValue *value;
    ARRAY_FOREACH(value, boneArray)
    {
        CCLOG("[name] : %s\n",value->stringForKey("name"));
        EPXmlValue *position = value->objectForKey("position");
        CCLOG("[x] : %d\n",position->integerForKey("x"));
        CCLOG("[y] : %d\n",position->integerForKey("y"));
    }
}

bool HelloWorld::gestureRecognizer(GestureRecognizer *gestureRecognizer)
{
//    if (gestureRecognizer->getRecognizerType() == kTapGestureRecognizer)
//        return true;
    return true;
}

void HelloWorld::performTap(TapGestureRecognizer *recognizer ,CCNode *node)
{
    if (node->getTag() == 123)
    {
        this->pressButton(node, callfuncN_selector(HelloWorld::pressAction));
        CCLOG("performTap");
    }
}

void HelloWorld::performLongPress(LongPressGestureRecognizer *recognizer ,CCNode *node)
{
    if (node->getTag() == 123)
    {
        CCLOG("performLongPress");
    }
}

void HelloWorld::performPan(PanGestureRecognizer *recognizer ,CCNode *node)
{
    if (node->getTag() == 123)
    {
        CCPoint pt = recognizer->getTranslationView();
        
        if (recognizer->getGestureRecognizerState() == kGestureRecognizerStateBegan)
            m_from = pt;
        else if (recognizer->getGestureRecognizerState() == kGestureRecognizerStateChanged)
        {
            CCPoint distance = ccpSub(pt, m_from);
            
            node->setPosition(ccpAdd(node->getPosition(),distance));
            m_from = pt;
        }        
    }
}

void HelloWorld::performPinch(PinchGestureRecognizer *recognizer ,CCNode *node)
{
    if (node->getTag() == 123)
    {
        CCLOG("performPinch : %f\n",recognizer->getPinchScale());
        node->setScale(recognizer->getPinchScale());
    }
}

void HelloWorld::performRotation(RotationGestureRecognizer *recognizer ,CCNode *node)
{
    if (node->getTag() == 123)
    {
        CCLOG("performRotation : %f\n",recognizer->getGestureRotation());
        node->setRotation(recognizer->getGestureRotation() + node->getRotation());
    }
}


void HelloWorld::pressAction(CCNode* node)
{
    CCLOG("pressAction");
}

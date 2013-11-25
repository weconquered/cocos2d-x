#include "HelloWorldScene.h"



enum {
    kMaxNodes = 50000,
    kNodesIncrease = 250,
    
    TEST_COUNT = 7,
};

enum {
    kTagInfoLayer = 1,
    kTagMainLayer = 2,
    kTagAutoTestMenu = 3,
    kTagMenuLayer = (kMaxNodes + 1000),
};

////////////////////////////////////////////////////////
//
// SubTest
//
////////////////////////////////////////////////////////
SubTest::~SubTest()
{
    if (_batchNode)
    {
        _batchNode->release();
        _batchNode = NULL;
    }
}

void SubTest::initWithSubTest(Node* p)
{
    _isBatchEnabled = false;
    _parent = p;
    _batchNode = NULL;
    _filename = "grossini.png";

    auto mgr = Director::getInstance()->getTextureCache();
    mgr->removeTexture(mgr->addImage(_filename));
    
    enableBatch(false);
}

void SubTest::enableBatch(bool enabled)
{
    if (_isBatchEnabled != enabled)
    {
        _isBatchEnabled = enabled;
        static_cast<HelloWorld*>(_parent)->reset();

        if (enabled)
        {// Batch
            Texture2D::setDefaultAlphaPixelFormat(Texture2D::PixelFormat::RGBA8888);
            _batchNode = SpriteBatchNode::create(_filename, 100);
            _parent->addChild(_batchNode, 0);
            _batchNode->retain();
        }
        else
        {// Non batch
            if (_batchNode)
            {
                _batchNode->removeFromParent();
                _batchNode->release();
                _batchNode = nullptr;
            }
        }
    }
    
    Texture2D::setDefaultAlphaPixelFormat(Texture2D::PixelFormat::DEFAULT);
}

Sprite* SubTest::createSpriteWithTag(int tag, bool isTrim)
{
    // create
    Texture2D::setDefaultAlphaPixelFormat(Texture2D::PixelFormat::RGBA8888);
    
    if (isTrim)
    {
        _filename = "grossini-trim.png";
    }
    else
    {
        _filename = "grossini.png";
    }
    
    Sprite* sprite = NULL;
    
    if (_isBatchEnabled)
    {
        sprite = Sprite::createWithTexture(_batchNode->getTexture());
        _batchNode->addChild(sprite, 0, tag+100);
    }
    else
    {
        sprite = Sprite::create(_filename);
        _parent->addChild(sprite, 0, tag+100);
    }
    
    Texture2D::setDefaultAlphaPixelFormat(Texture2D::PixelFormat::DEFAULT);
    
    return sprite;
}

void SubTest::removeByTag(int tag)
{
    if (_isBatchEnabled)
    {
        _batchNode->removeChildAtIndex(tag, true);
    }
    else
    {
        _parent->removeChildByTag(tag+100, true);
    }
}


Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

void performanceActions(Sprite* sprite)
{
    auto size = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();
    
    int posX = (rand() % ((int)(origin.x + size.width)));
    int posY = (rand() % ((int)(origin.y + size.height)));
    CCASSERT(posX >= origin.x && posX <= (origin.x + size.width) && posY >= origin.y && posY <= (origin.y + size.height), "Invalid postion!");
    
    sprite->setPosition(Point(posX, posY));
    
    float period = 0.5f + (rand() % 1000) / 500.0f;
    auto rot = RotateBy::create(period, 360.0f * CCRANDOM_0_1());
    auto rot_back = rot->reverse();
    auto permanentRotation = RepeatForever::create(Sequence::create(rot, rot_back, NULL));
    sprite->runAction(permanentRotation);
    
    float growDuration = 0.5f + (rand() % 1000) / 500.0f;
    auto grow = ScaleBy::create(growDuration, 0.5f, 0.5f);
    auto permanentScaleLoop = RepeatForever::create(Sequence::create(grow, grow->reverse(), NULL));
    sprite->runAction(permanentScaleLoop);
}

void doTest(Sprite* sprite)
{
    performanceActions(sprite);
}

HelloWorld::HelloWorld()
: quantityNodes(0)
, lastRenderedCount(0)
, _subTest(nullptr)
, _isTrim(false)
, _isBatch(false)
{
}


void HelloWorld::updateNodes()
{
    if( quantityNodes != lastRenderedCount )
    {
        auto infoLabel = (LabelTTF *) getChildByTag(kTagInfoLayer);
        char str[16] = {0};
        sprintf(str, "%u nodes", quantityNodes);
        infoLabel->setString(str);
        
        lastRenderedCount = quantityNodes;
    }
}

void HelloWorld::reset()
{
    if (_subTest->isBatchEnabled())
    {
        if (_subTest->getBatchNode())
        {
            _subTest->getBatchNode()->removeAllChildren();
        }
    }
    else
    {
        for (int i = 0; i < quantityNodes; ++i)
        {
            _subTest->removeByTag(i);
        }
    }
    
    quantityNodes = 0;
    lastRenderedCount = 0;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    auto winSize = Director::getInstance()->getWinSize();
    
    auto infoLabel = LabelTTF::create("0 nodes", "", 30);
    infoLabel->setColor(Color3B(0,200,20));
    infoLabel->setPosition(Point(winSize.width/2, winSize.height-90));
    addChild(infoLabel, 1, kTagInfoLayer);
    
    _subTest = new SubTest;
    _subTest->initWithSubTest(this);
    
    auto itemAdd = MenuItemFont::create("Add 250 sprites", [=](Object* sender){
        if( quantityNodes >= kMaxNodes)
            return;
        
        for( int i=0;i< kNodesIncrease;i++)
        {
            auto sprite = _subTest->createSpriteWithTag(quantityNodes, _isTrim);
            doTest(sprite);
            quantityNodes++;
        }
        
        updateNodes();
    });
    
    itemAdd->setPosition(Point(winSize.width/2, winSize.height/2));
    
    auto trim = MenuItemToggle::createWithCallback([=](Object* sender){
        int oldNodeNum = quantityNodes;
        reset();
        _isTrim = !_isTrim;
        
        for( int i=0;i< oldNodeNum;i++)
        {
            auto sprite = _subTest->createSpriteWithTag(quantityNodes, _isTrim);
            doTest(sprite);
            quantityNodes++;
        }
        
        updateNodes();
    }, MenuItemFont::create("Non-trim"), MenuItemFont::create("Trim"), nullptr);
    
    trim->setPosition(Point(winSize.width/2, winSize.height/2 - 50));
    
    auto batch = MenuItemToggle::createWithCallback([=](Object* sender){
        int oldNodeNum = quantityNodes;
        reset();
        _isBatch = !_isBatch;
        _subTest->enableBatch(_isBatch);
        
        for( int i=0;i< oldNodeNum;i++)
        {
            auto sprite = _subTest->createSpriteWithTag(quantityNodes, _isTrim);
            doTest(sprite);
            quantityNodes++;
        }
        
        updateNodes();
    }, MenuItemFont::create("Non-batch"), MenuItemFont::create("Batch"), nullptr);
    
    batch->setPosition(Point(winSize.width/2, winSize.height/2 - 100));
    
    auto resetItem = MenuItemFont::create("Reset", [=](Object* sender){
        reset();
        _isBatch = false;
        _isTrim = false;
        _subTest->enableBatch(false);
        
        trim->setSelectedIndex(0);
        batch->setSelectedIndex(0);
        
        infoLabel->setString("0 nodes");
    });
    resetItem->setPosition(Point(winSize.width/2, winSize.height/2 - 150));
    
    auto menu = Menu::create(itemAdd, trim, batch, resetItem, NULL);
    menu->setPosition(Point(0, 0));
    menu->setAnchorPoint(Point(0, 0));
    addChild(menu, 10000);
    
    return true;
}


void HelloWorld::menuCloseCallback(Object* pSender)
{
    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

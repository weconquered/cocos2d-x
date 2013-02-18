#include "TableViewTestScene.h"
#include "CustomTableViewCell.h"
#include "../ExtensionsTest.h"

USING_NS_CC;
USING_NS_CC_EXT;

TESTLAYER_CREATE_FUNC(TableViewTestLayer);
TESTLAYER_CREATE_FUNC(TableViewTestLayer2);

static NEWTESTFUNC createFunctions[] = {
    CF(TableViewTestLayer),
    CF(TableViewTestLayer2)
};

static int sceneIdx=-1;
#define MAX_LAYER (sizeof(createFunctions) / sizeof(createFunctions[0]))

static CCLayer* nextAction()
{
    sceneIdx++;
    sceneIdx = sceneIdx % MAX_LAYER;
    
    CCLayer* pLayer = (createFunctions[sceneIdx])();
    pLayer->init();
    pLayer->autorelease();
    
    return pLayer;
}

static CCLayer* backAction()
{
    sceneIdx--;
    int total = MAX_LAYER;
    if( sceneIdx < 0 )
        sceneIdx += total;
    
    CCLayer* pLayer = (createFunctions[sceneIdx])();
    pLayer->init();
    pLayer->autorelease();
    
    return pLayer;
}

static CCLayer* restartAction()
{
    CCLayer* pLayer = (createFunctions[sceneIdx])();
    pLayer->init();
    pLayer->autorelease();
    
    return pLayer;
}

void TableViewTestScene::runThisTest()
{
    CCLayer* pLayer = nextAction();
    addChild(pLayer);
    
    CCDirector::sharedDirector()->replaceScene(this);
}

//------------------------------------------------------------------
//
// TableViewTestDemo
//
//------------------------------------------------------------------

TableViewTestDemo::TableViewTestDemo(void)
{
}

TableViewTestDemo::~TableViewTestDemo(void)
{
}

std::string TableViewTestDemo::title()
{
    return "TableView Test";
}

std::string TableViewTestDemo::subtitle()
{
    return "";
}

void TableViewTestDemo::onEnter()
{
    CCLayer::onEnter();
    
    CCSize s = CCDirector::sharedDirector()->getWinSize();
    
    CCLabelTTF* label = CCLabelTTF::create(title().c_str(), "Arial", 28);
    addChild(label, 1);
    label->setPosition( ccp(s.width/2, s.height-50) );
    
    std::string strSubtitle = subtitle();
    if( ! strSubtitle.empty() )
    {
        CCLabelTTF* l = CCLabelTTF::create(strSubtitle.c_str(), "Thonburi", 16);
        addChild(l, 1);
        l->setPosition( ccp(s.width/2, s.height-80) );
    }
    
    CCMenuItemImage *item1 = CCMenuItemImage::create("Images/b1.png", "Images/b2.png", this, menu_selector(TableViewTestDemo::backCallback) );
    CCMenuItemImage *item2 = CCMenuItemImage::create("Images/r1.png","Images/r2.png", this, menu_selector(TableViewTestDemo::restartCallback) );
    CCMenuItemImage *item3 = CCMenuItemImage::create("Images/f1.png", "Images/f2.png", this, menu_selector(TableViewTestDemo::nextCallback) );
    
    CCMenu *menu = CCMenu::create(item1, item2, item3, NULL);
    
    menu->setPosition(CCPointZero);
    item1->setPosition(ccp(VisibleRect::center().x - item2->getContentSize().width*2, VisibleRect::bottom().y+item2->getContentSize().height/2));
    item2->setPosition(ccp(VisibleRect::center().x, VisibleRect::bottom().y+item2->getContentSize().height/2));
    item3->setPosition(ccp(VisibleRect::center().x + item2->getContentSize().width*2, VisibleRect::bottom().y+item2->getContentSize().height/2));
    
    addChild(menu, 1);
}

void TableViewTestDemo::restartCallback(CCObject* pSender)
{
    CCScene* s = new TableViewTestScene();
    s->addChild(restartAction());
    
    CCDirector::sharedDirector()->replaceScene(s);
    s->release();
}

void TableViewTestDemo::nextCallback(CCObject* pSender)
{
    CCScene* s = new TableViewTestScene();
    s->addChild( nextAction() );
    CCDirector::sharedDirector()->replaceScene(s);
    s->release();
}

void TableViewTestDemo::backCallback(CCObject* pSender)
{
    CCScene* s = new TableViewTestScene();
    s->addChild( backAction() );
    CCDirector::sharedDirector()->replaceScene(s);
    s->release();
} 


// on "init" you need to initialize your instance
bool TableViewTestLayer::init()
{
    if ( !CCLayerColor::initWithColor(ccc4(255, 50, 255, 255)) )
    {
        return false;
    }

	CCSize winSize = CCDirector::sharedDirector()->getWinSize();

    CCTableView* tableView = CCTableView::create(this, CCSizeMake(250, 60));
    tableView->setDirection(kCCScrollViewDirectionHorizontal);
    tableView->setPosition(ccp(20,winSize.height/2-30));
    tableView->setDelegate(this);
    this->addChild(tableView);
    tableView->reloadData();

	tableView = CCTableView::create(this, CCSizeMake(60, 250));
	tableView->setDirection(kCCScrollViewDirectionVertical);
	tableView->setPosition(ccp(winSize.width-150,winSize.height/2-120));
	tableView->setDelegate(this);
	tableView->setVerticalFillOrder(kCCTableViewFillTopDown);
	this->addChild(tableView);
	tableView->reloadData();

    return true;
}

void TableViewTestLayer::toExtensionsMainLayer(cocos2d::CCObject *sender)
{
	ExtensionsTestScene *pScene = new ExtensionsTestScene();
	pScene->runThisTest();
	pScene->release();
}

void TableViewTestLayer::tableCellTouched(CCTableView* table, CCTableViewCell* cell)
{
    CCLOG("cell touched at index: %i", cell->getIdx());
}

CCSize TableViewTestLayer::cellSizeForTable(CCTableView *table)
{
    return CCSizeMake(60, 60);
}

CCTableViewCell* TableViewTestLayer::tableCellAtIndex(CCTableView *table, unsigned int idx)
{
    CCString *string = CCString::createWithFormat("%d", idx);
    CCTableViewCell *cell = table->dequeueCell();
    if (!cell) {
        cell = new CustomTableViewCell();
        cell->autorelease();
        CCSprite *sprite = CCSprite::create("Images/Icon.png");
        sprite->setAnchorPoint(CCPointZero);
        sprite->setPosition(ccp(0, 0));
        cell->addChild(sprite);

        CCLabelTTF *label = CCLabelTTF::create(string->getCString(), "Helvetica", 20.0);
        label->setPosition(CCPointZero);
		label->setAnchorPoint(CCPointZero);
        label->setTag(123);
        cell->addChild(label);
    }
    else
    {
        CCLabelTTF *label = (CCLabelTTF*)cell->getChildByTag(123);
        label->setString(string->getCString());
    }


    return cell;
}

unsigned int TableViewTestLayer::numberOfCellsInTableView(CCTableView *table)
{
    return 20;
}

// TableViewTestLayer2

typedef struct tagTableViewCellLabel
{
    std::string title;
    std::string content;
}TableViewCellLabel;

static TableViewCellLabel s_cellLabels[] = {
    {"title01", "hello1 aaa tableview contents"},
    {"title02", "hello2 bbb tableview contents"},
    {"title03", "hello3 tableview contents sdf"},
    {"title04", "hello4 tableview contents111"},
    {"title05", "hello5 tableview contents33"},
    {"title06", "hello6 tableview contents4444"},
    {"title07", "hello7 tableview contents555"},
    {"title08", "hello8 tableview contents adf"},
    {"title09", "hello9 tableview contents6666"},
    {"title10", "hello10 tableview contents  wuicl"},
    {"title11", "hello11 tableview contents dfdsf"},
    {"title12", "hello12 tableview contents7 dk"},
    {"title13", "hello13 tableview contentssdfdsf"},
    {"title14", "hello14 tableview content dklassse1"},
    {"title15", "hello15 tableview contents888"},
    {"title16", "hello16 tableview contents oxocpa"},
    {"title17", "hello17 tableview contents couoqwoenn"},
    {"title18", "hello18 tableview contents p23jkll"},
    {"title19", "hello19 tableview contents cxjlcxvj"},
    {"title20", "hello20 tableview contents 2o3uljdlfzo"}
};

TableViewTestLayer2::~TableViewTestLayer2()
{
    std::map<std::string, CCSprite*>::iterator iter = m_mapIcon.begin();
    for (; iter != m_mapIcon.end(); ++iter) {
        iter->second->release();
    }
    m_mapIcon.clear();
    
    std::map<std::string, CCSprite*>::iterator iterBG = m_mapBg.begin();
    for (; iterBG != m_mapBg.end(); ++iterBG) {
        iterBG->second->release();
    }
    m_mapBg.clear();
}

// on "init" you need to initialize your instance
bool TableViewTestLayer2::init()
{
    if ( !CCLayerColor::initWithColor(ccc4(0, 0, 0, 255)) )
    {
        return false;
    }
    
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("res-tableview/store_main.plist");
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("res-tableview/store_main2.plist");
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("res-tableview/store_characters.plist");
    
    CCSprite* tableViewBG = CCSprite::createWithSpriteFrameName("store_main_back.png");
    tableViewBG->setAnchorPoint(ccp(0, 0));
    tableViewBG->setPosition(ccp(0, 0));
    this->addChild(tableViewBG);
    
	CCTableView* tableView = CCTableView::create(this, CCSizeMake(276, tableViewBG->getContentSize().height-50));
	tableView->setDirection(kCCScrollViewDirectionVertical);
	tableView->setPosition(ccp(17,0));
	tableView->setDelegate(this);
	tableView->setVerticalFillOrder(kCCTableViewFillTopDown);
	tableViewBG->addChild(tableView);
    

    
	tableView->reloadData();
    
    return true;
}

void TableViewTestLayer2::toExtensionsMainLayer(cocos2d::CCObject *sender)
{
	ExtensionsTestScene *pScene = new ExtensionsTestScene();
	pScene->runThisTest();
	pScene->release();
}

void TableViewTestLayer2::tableCellTouched(CCTableView* table, CCTableViewCell* cell)
{
    CCLOG("cell touched at index: %i", cell->getIdx());
}

CCSize TableViewTestLayer2::cellSizeForTable(CCTableView *table)
{
    return CCSizeMake(276, 97);
}

CCTableViewCell* TableViewTestLayer2::tableCellAtIndex(CCTableView *table, unsigned int idx)
{
    CCString *string = CCString::createWithFormat("%d", idx);

    CCLOG("idx = %d", idx);

    bool bComingSoonItem = idx >= 17 && idx <= 19;
    
    CustomTableViewCell *cell = (CustomTableViewCell*)table->dequeueCell();
    if (!cell) {
        cell = new CustomTableViewCell();
        cell->autorelease();
        
        
        std::string itemBG = bComingSoonItem ? "store_comingsoon.png" : "store_item.png";

        CCSprite *sprite = CCSprite::createWithSpriteFrameName(itemBG.c_str());
        sprite->setAnchorPoint(CCPointZero);
        sprite->setPosition(ccp(0, 0));
        sprite->retain();
        cell->setBackGround(sprite);
        
        m_mapBg[string->getCString()] = sprite;
        
        sprite = CCSprite::createWithSpriteFrameName("store_focus_cover.png");
        sprite->setAnchorPoint(ccp(0.5, 0.5));
        sprite->setPosition(ccp(276/2, 97/2-7));
        sprite->setVisible(false);
        cell->setBackGroundHighLight(sprite);
        
        
        CCSprite *icon = CCSprite::createWithSpriteFrameName(CCString::createWithFormat("store_%d.png", idx%9)->getCString());
        icon->setPosition(ccp(50, 50));
        cell->setIcon(icon);
        icon->retain();
        m_mapIcon.insert(std::map<std::string, CCSprite*>::value_type(string->getCString(), icon));
        
        CCLabelTTF *label = CCLabelTTF::create(string->getCString(), "Helvetica", 20.0);
        
        label->setPosition(ccp(0, 0));
        label->setAnchorPoint(CCPointZero);
        cell->setIndexLabel(label);
        
        CCLabelTTF *titlelabel = CCLabelTTF::create(s_cellLabels[idx%20].title.c_str(), "Helvetica", 24.0);
        titlelabel->setPosition(ccp(85, 85));
        titlelabel->setColor(ccc3(150, 0, 0));
        titlelabel->setAnchorPoint(ccp(0, 1));
        cell->setTitle(titlelabel);
        
        CCLabelTTF *contentlabel = CCLabelTTF::create(s_cellLabels[idx%20].content.c_str(), "Helvetica", 20.0, CCSizeMake(200, 80), kCCTextAlignmentLeft);
        contentlabel->setPosition(ccp(85, 60));
        contentlabel->setColor(ccc3(30, 30, 150));
        contentlabel->setAnchorPoint(ccp(0, 1));
        cell->setContent(contentlabel);

    }
    else
    {
        std::map<std::string, CCSprite*>::iterator iterBG = m_mapBg.find(string->getCString());
        if (iterBG != m_mapBg.end())
        {
            iterBG->second->removeFromParent();
            cell->setBackGround(iterBG->second);
        }
        else
        {
            std::string itemBG = bComingSoonItem ? "store_comingsoon.png" : "store_item.png";
            
            CCSprite *sprite = CCSprite::createWithSpriteFrameName(itemBG.c_str());
            sprite->setAnchorPoint(CCPointZero);
            sprite->setPosition(ccp(0, 0));
            sprite->retain();
            cell->setBackGround(sprite);
            
            m_mapBg[string->getCString()] = sprite;
        }
        
        std::map<std::string, CCSprite*>::iterator it = m_mapIcon.find(string->getCString());
        if (it != m_mapIcon.end())
        {
            it->second->removeFromParent();
            cell->setIcon(it->second);
        }
        else
        {
            CCSprite *icon = CCSprite::createWithSpriteFrameName(CCString::createWithFormat("store_%d.png", idx%9)->getCString());
            icon->setPosition(ccp(50, 50));
            cell->setIcon(icon);
            icon->retain();
            m_mapIcon.insert(std::map<std::string, CCSprite*>::value_type(string->getCString(), icon));
        }
        
        
        cell->getTitle()->setString(s_cellLabels[idx%20].title.c_str());
        cell->getContent()->setString(s_cellLabels[idx%20].content.c_str());
        
        CCLabelTTF *label = cell->getIndexLabel();
        label->setString(string->getCString());
    }
    
    cell->getIcon()->setVisible(!bComingSoonItem);
    cell->getTitle()->setVisible(!bComingSoonItem);
    cell->getContent()->setVisible(!bComingSoonItem);
    
    return cell;
}

unsigned int TableViewTestLayer2::numberOfCellsInTableView(CCTableView *table)
{
    return 20;
}

void TableViewTestLayer2::tableCellHighlight(CCTableView* table, CCTableViewCell* cell)
{
    CustomTableViewCell* p = (CustomTableViewCell*)cell;
    p->getBackGroundHighLight()->setVisible(true);
}

void TableViewTestLayer2::tableCellUnhighlight(CCTableView* table, CCTableViewCell* cell)
{
    CustomTableViewCell* p = (CustomTableViewCell*)cell;
    p->getBackGroundHighLight()->setVisible(false);
}

void TableViewTestLayer2::tableCellWillRecycle(CCTableView* table, CCTableViewCell* cell)
{
    CustomTableViewCell* p = (CustomTableViewCell*)cell;
    p->reset();
}


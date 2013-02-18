#ifndef __TABLEVIEWTESTSCENE_H__
#define __TABLEVIEWTESTSCENE_H__

#include "cocos2d.h"
#include "cocos-ext.h"

#include "../../testBasic.h"
USING_NS_CC_EXT;
class TableViewTestScene : public TestScene
{
public:
    virtual void runThisTest();
};

class TableViewTestDemo : public CCLayerColor
{
protected:
    std::string    m_strTitle;
    
public:
    TableViewTestDemo(void);
    ~TableViewTestDemo(void);
    
    virtual std::string title();
    virtual std::string subtitle();
    virtual void onEnter();
    
    void restartCallback(CCObject* pSender);
    void nextCallback(CCObject* pSender);
    void backCallback(CCObject* pSender);
};

class TableViewTestLayer : public TableViewTestDemo, public cocos2d::extension::CCTableViewDataSource, public cocos2d::extension::CCTableViewDelegate
{
public:
    virtual bool init();  
   
	void toExtensionsMainLayer(cocos2d::CCObject *sender);

    CREATE_FUNC(TableViewTestLayer);
    
    virtual void scrollViewDidScroll(cocos2d::extension::CCScrollView* view) {};
    virtual void scrollViewDidZoom(cocos2d::extension::CCScrollView* view) {}
    virtual void tableCellTouched(cocos2d::extension::CCTableView* table, cocos2d::extension::CCTableViewCell* cell);
    virtual cocos2d::CCSize cellSizeForTable(cocos2d::extension::CCTableView *table);
    virtual cocos2d::extension::CCTableViewCell* tableCellAtIndex(cocos2d::extension::CCTableView *table, unsigned int idx);
    virtual unsigned int numberOfCellsInTableView(cocos2d::extension::CCTableView *table);
};

class TableViewTestLayer2 : public TableViewTestDemo, public cocos2d::extension::CCTableViewDataSource, public cocos2d::extension::CCTableViewDelegate
{
public:
    virtual bool init();
    
	void toExtensionsMainLayer(cocos2d::CCObject *sender);
    
    CREATE_FUNC(TableViewTestLayer2);
    
    virtual void scrollViewDidScroll(cocos2d::extension::CCScrollView* view) {};
    virtual void scrollViewDidZoom(cocos2d::extension::CCScrollView* view) {}
    virtual void tableCellTouched(cocos2d::extension::CCTableView* table, cocos2d::extension::CCTableViewCell* cell);
    virtual cocos2d::CCSize cellSizeForTable(cocos2d::extension::CCTableView *table);
    virtual cocos2d::extension::CCTableViewCell* tableCellAtIndex(cocos2d::extension::CCTableView *table, unsigned int idx);
    virtual unsigned int numberOfCellsInTableView(cocos2d::extension::CCTableView *table);
    virtual void tableCellHighlight(CCTableView* table, CCTableViewCell* cell);
    virtual void tableCellUnhighlight(CCTableView* table, CCTableViewCell* cell);
    virtual void tableCellWillRecycle(CCTableView* table, CCTableViewCell* cell);
    
private:
    std::map<std::string, CCSprite*> m_mapIcon;
};

#endif // __TABLEVIEWTESTSCENE_H__

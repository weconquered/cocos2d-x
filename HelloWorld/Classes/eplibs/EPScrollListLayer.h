//
//  EPScrollListLayer.h
//  EnjoyYourMoney
//
//  Created by Erawppa on 2011/9/7.
//  Copyright 2011年 __MyCompanyName__. All rights reserved.
//
#ifndef __EPSCROLL_LIST_LAYER_H__
#define __EPSCROLL_LIST_LAYER_H__

#include "cocos2d.h"

class EPScrollListLayer : public cocos2d::CCLayer
{
public:
    EPScrollListLayer();
    
    ~EPScrollListLayer();
    
//	virtual bool init();  
    
    virtual void onExit();
    
    virtual void onEnterTransitionDidFinish();
    
    virtual void visit();
    
    static EPScrollListLayer *nodeWithLayers(cocos2d::CCArray *layers, cocos2d::CCSize size, cocos2d::CCPoint offset);    
    
//    void registerWithTouchDispatcher();
    bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    void ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    void ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    void ccTouchCancelled(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);

private:
    bool initWithLayers(cocos2d::CCArray *layers, cocos2d::CCSize size, cocos2d::CCPoint offset);
    
    void initListNode(cocos2d::CCArray *layers);
    
    void initIndicator();
    
    void moveListTo(cocos2d::CCPoint point);
    
    void listDebounceDone();
    
    cocos2d::CCRect m_tListRect;
    cocos2d::CCSize m_tMaxListContentSize;
    cocos2d::CCNode *m_pList;
    cocos2d::CCLayerColor *m_pIndicator;
    int m_nState;
    float m_fStartSwipe;
};

#endif//__EPSCROLL_LIST_LAYER_H__
//
//  EPScrollListLayer.h
//  EnjoyYourMoney
//
//  Created by Erawppa on 2011/9/7.
//  Copyright 2011年 Erawppa Co., Ltd.. All rights reserved.
//
#ifndef __EPSCROLL_LIST_LAYER_H__
#define __EPSCROLL_LIST_LAYER_H__

#include "cocos2d.h"

class EPScrollListLayer : public cocos2d::CCLayer
{
public:
    EPScrollListLayer();
    
    ~EPScrollListLayer();
    
    virtual void onExit();
    
    virtual void onEnterTransitionDidFinish();
    
    virtual void visit();
    
    static EPScrollListLayer *nodeWithLayers(cocos2d::CCArray *layers, 
                                             cocos2d::CCSize size, 
                                             cocos2d::CCPoint offset,
                                             bool vertical = true);
    
    void addTapSelector(cocos2d::CCObject *target, cocos2d::SEL_CallFuncN selector){
        m_target = target;
        m_tapSelector = selector;
    }
    
    void addTouchBeginSelector(cocos2d::CCObject *target, cocos2d::SEL_CallFuncN selector){
        m_target = target;
        m_touchBeginSelector = selector;
    }

    void addTouchEndSelector(cocos2d::CCObject *target, cocos2d::SEL_CallFuncN selector){
        m_target = target;
        m_touchEndSelector = selector;
    }

    cocos2d::CCLayer* touchForLayer(cocos2d::CCPoint touchPoint);
    void syncScrollLayer(EPScrollListLayer* scrollLayer);
    void clearSyncScrollLayer();
    void addLayer(cocos2d::CCLayer* layer);
    void removeLayer(cocos2d::CCLayer* layer);
    void insertLayer(cocos2d::CCLayer* layer,int index);
    void clear();
    
    bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    void ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    void ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    void ccTouchCancelled(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    
    void move(float distance);
    
    CC_SYNTHESIZE(bool, m_tapEnable, TapEnable)
    CC_SYNTHESIZE_READONLY(cocos2d::CCNode*, m_pList, List);

private:
    void pressedLayer(cocos2d::CCNode* sender);
    
    bool initWithLayers(cocos2d::CCArray *layers, 
                        cocos2d::CCSize size, 
                        cocos2d::CCPoint offset,
                        bool vertical);
    
    //
    // Vertical
    //
    void initVerticalListNode(cocos2d::CCArray *layers);
    void initVerticalIndicator();
    void moveVerticalListTo(cocos2d::CCPoint point);

    //
    // Horizontal
    //
    void initHorizontalListNode(cocos2d::CCArray *layers);
    void initHorizontalIndicator();
    void moveHorizontalListTo(cocos2d::CCPoint point);
    
    void listDebounceDone();
    
    cocos2d::CCRect m_tListRect;
    cocos2d::CCSize m_tMaxListContentSize;
    cocos2d::CCLayerColor *m_pIndicator;
    int m_nState;
    float m_fStartSwipe;
    float m_fUniqueStartSwipe;
    bool m_vertical;
    
    cocos2d::CCObject *m_target;
    cocos2d::SEL_CallFuncN m_tapSelector;
    cocos2d::SEL_CallFuncN m_touchBeginSelector;
    cocos2d::SEL_CallFuncN m_touchEndSelector;
    
    cocos2d::CCLayer* m_tapLayer;
    
    std::vector<EPScrollListLayer*> m_syncScrolls;
};

#endif//__EPSCROLL_LIST_LAYER_H__



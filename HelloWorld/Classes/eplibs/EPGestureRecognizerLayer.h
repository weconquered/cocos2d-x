//
//  EPGestureRecognizer.h
//  EnjoyYourMoney
//
//  Created by Erawppa on 2011/9/2.
//  Copyright 2011年 __MyCompanyName__. All rights reserved.
//
#ifndef __EPGESTURE_RECOGNIZERLAYER_H__
#define __EPGESTURE_RECOGNIZERLAYER_H__

#include "cocos2d.h"
#include "GestureRecognizer.h"
#include "TapGestureRecognizer.h"
#include "LongPressGestureRecognizer.h"
#include "PanGestureRecognizer.h"
#include "PinchGestureRecognizer.h"
#include "RotationGestureRecognizer.h"

class /*CC_DLL*/ EPGestureRecognizerLayer : public cocos2d::CCLayer
{
public:
    EPGestureRecognizerLayer();
    ~EPGestureRecognizerLayer();    
    
    static EPGestureRecognizerLayer *node();
    
    bool init();
    
	virtual void onExit();
    virtual void onEnterTransitionDidFinish();
    
    void startGestureRecognizer();
    void stopGestureRecognizer();
        
    //
    // PUBLIC FUNCTIONS
    //
    void addTapRecognizer(CCNode *node);
    void addLongPressRecognizer(CCNode *node);
    void addPanRecognizer(CCNode *node);
    void addPinchRecognizer(CCNode *node);
    void addRotationRecognizer(CCNode *node);
    
    virtual bool gestureRecognizer(GestureRecognizer *gestureRecognizer){return false;};    
    virtual void performTap(TapGestureRecognizer *recognizer ,CCNode *node){};
    virtual void performLongPress(LongPressGestureRecognizer *recognizer ,CCNode *node){};
    virtual void performPan(PanGestureRecognizer *recognizer ,CCNode *node){};
    virtual void performPinch(PinchGestureRecognizer *recognizer ,CCNode *node){};
    virtual void performRotation(RotationGestureRecognizer *recognizer ,CCNode *node){};
    
    //
    // INTERNAL EVENTS
    //
    void registerWithTouchDispatcher();    
 	void ccTouchesBegan(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);
 	void ccTouchesMoved(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);
 	void ccTouchesEnded(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);
 	void ccTouchesCancelled(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);
    
    //
    // BUTTON EFFECTS
    //
    void pressButton(CCNode *node, cocos2d::SEL_CallFuncN selector);
    bool m_bTabEnabled;
private:
    void initRecognizerArray();
    GestureRecognizer* nodeForTouch(cocos2d::CCArray *pArrays, cocos2d::CCSet *pTouches);
    void defaultTouchesCancelled(cocos2d::CCArray *pArrays, cocos2d::CCSet *pTouches);
    bool isVisibled();
    
    bool m_bTransitionDone;
    bool m_bPause;
    int m_nTouchCount;
    
    //
    // TapGestureRecognizer
    //
public:
    CC_PROPERTY(int, m_nNumberOfTapsRequired, NumberOfTapsRequired)
private:
    void tapTouchesBegan(cocos2d::CCSet *pTouches);
    void tapTouchesMoved(cocos2d::CCSet *pTouches);
    void tapTouchesEnded(cocos2d::CCSet *pTouches);
    void tapTouchesCancelled(cocos2d::CCSet *pTouches);
    cocos2d::CCArray *m_pTapRecognizers;
    cocos2d::CCArray *m_pTapNodes;

    //
    // LongPressGestureRecognizer
    //
public:
    CC_PROPERTY(int, m_nNumberOfLongPressRequired, NumberOfLongPressRequired)
    CC_PROPERTY(cocos2d::ccTime, m_fLongPressInterval, LongPressInterval);
private:
    void longPressedTimer(cocos2d::ccTime dt);
    void longPressTouchesBegan(cocos2d::CCSet *pTouches);
    void longPressTouchesMoved(cocos2d::CCSet *pTouches);
    void longPressTouchesEnded(cocos2d::CCSet *pTouches);
    void longPressTouchesCancelled(cocos2d::CCSet *pTouches);
    cocos2d::CCArray *m_pLongPressRecognizers;
    cocos2d::CCArray *m_pLongPressNodes;
    
    //
    // PanGestureRecognizer
    //
public:
    CC_PROPERTY(int, m_nNumberOfPansRequired, NumberOfPansRequired)
private:
    void panTouchesBegan(cocos2d::CCSet *pTouches);
    void panTouchesMoved(cocos2d::CCSet *pTouches);
    void panTouchesEnded(cocos2d::CCSet *pTouches);
    void panTouchesCancelled(cocos2d::CCSet *pTouches);
    cocos2d::CCArray *m_pPanRecognizers;
    cocos2d::CCArray *m_pPanNodes;
    
    //
    // PinchGestureRecognizer
    //
private:
    void pinchTouchesBegan(cocos2d::CCSet *pTouches);
    void pinchTouchesMoved(cocos2d::CCSet *pTouches);
    void pinchTouchesEnded(cocos2d::CCSet *pTouches);
    void pinchTouchesCancelled(cocos2d::CCSet *pTouches);
    cocos2d::CCArray *m_pPinchRecognizers;
    PinchGestureRecognizer *m_pPinchNode;
    
    //
    // RotationGestureRecognizer
    //
private:
    void rotationTouchesBegan(cocos2d::CCSet *pTouches);
    void rotationTouchesMoved(cocos2d::CCSet *pTouches);
    void rotationTouchesEnded(cocos2d::CCSet *pTouches);
    void rotationTouchesCancelled(cocos2d::CCSet *pTouches);
    cocos2d::CCArray *m_pRotationRecognizers;
    RotationGestureRecognizer *m_pRotationNode;
};

#endif//__EPGESTURE_RECOGNIZERLAYER_H__

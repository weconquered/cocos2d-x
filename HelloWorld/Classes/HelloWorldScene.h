#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "EPGestureRecognizerLayer.h"

class EPResourceManager;

class HelloWorld : public EPGestureRecognizerLayer
{
public:
	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();  
    
	// there's no 'id' in cpp, so we recommand to return the exactly class pointer
	static cocos2d::CCScene* scene();
	
	// a selector callback
	void loadedCallback(CCObject* pSender);
    void completedCallback(CCObject* pSender);
    void pressAction(CCNode* node);
    
	// implement the "static node()" method manually
	LAYER_NODE_FUNC(HelloWorld);
    
private:
    EPResourceManager *m_res;
    cocos2d::CCPoint m_from;
    
    bool gestureRecognizer(GestureRecognizer *gestureRecognizer);
    void performTap(TapGestureRecognizer *recognizer ,CCNode *node);
    void performLongPress(LongPressGestureRecognizer *recognizer ,CCNode *node);
    void performPan(PanGestureRecognizer *recognizer ,CCNode *node);
    void performPinch(PinchGestureRecognizer *recognizer ,CCNode *node);
    void performRotation(RotationGestureRecognizer *recognizer ,CCNode *node);
};

#endif // __HELLOWORLD_SCENE_H__

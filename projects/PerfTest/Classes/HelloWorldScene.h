#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

USING_NS_CC;

class SubTest
{
public:
    ~SubTest();
    void removeByTag(int tag);
    inline SpriteBatchNode* getBatchNode() { return _batchNode; };
    Sprite* createSpriteWithTag(int tag, bool isTrim);
    void initWithSubTest(Node* parent);
    void enableBatch(bool enabled);
    bool isBatchEnabled() { return _isBatchEnabled; };
protected:
    bool _isBatchEnabled;
    SpriteBatchNode* _batchNode;
    Node* _parent;
    const char*  _filename;
};

class HelloWorld : public cocos2d::Layer
{
public:
    HelloWorld();
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  
    
    // a selector callback
    void menuCloseCallback(Object* pSender);
    
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
    
    void updateNodes();
    void reset();
    
private:
    int quantityNodes;
    int lastRenderedCount;
    SubTest* _subTest;
    bool _isTrim;
    bool _isBatch;
};

#endif // __HELLOWORLD_SCENE_H__

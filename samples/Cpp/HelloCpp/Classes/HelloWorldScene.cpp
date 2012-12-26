#include "HelloWorldScene.h"
#include "AppMacros.h"
#include "sprite_nodes/CCSprite3D.h"
USING_NS_CC;

CCScene* HelloWorld::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    HelloWorld *layer = HelloWorld::create();

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
    
    
    
//     CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
//     CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
// 
//     /////////////////////////////
//     // 2. add a menu item with "X" image, which is clicked to quit the program
//     //    you may modify it.
// 
//     // add a "close" icon to exit the progress. it's an autorelease object
//     CCMenuItemImage *pCloseItem = CCMenuItemImage::create(
//                                         "CloseNormal.png",
//                                         "CloseSelected.png",
//                                         this,
//                                         menu_selector(HelloWorld::menuCloseCallback));
//     
// 	pCloseItem->setPosition(ccp(origin.x + visibleSize.width - pCloseItem->getContentSize().width/2 ,
//                                 origin.y + pCloseItem->getContentSize().height/2));
// 
//     // create menu, it's an autorelease object
//     CCMenu* pMenu = CCMenu::create(pCloseItem, NULL);
//     pMenu->setPosition(CCPointZero);
//     this->addChild(pMenu, 1);
// 
//     /////////////////////////////
//     // 3. add your codes below...
// 
//     // add a label shows "Hello World"
//     // create and initialize a label
//     
//     CCLabelTTF* pLabel = CCLabelTTF::create("Hello World", "Arial", TITLE_FONT_SIZE);
//     
//     // position the label on the center of the screen
//     pLabel->setPosition(ccp(origin.x + visibleSize.width/2,
//                             origin.y + visibleSize.height - pLabel->getContentSize().height));
// 
//     // add the label as a child to this layer
//     this->addChild(pLabel, 1);
// 
//     // add "HelloWorld" splash screen"
//     CCSprite* pSprite = CCSprite::create("HelloWorld.png");
// 
//     // position the sprite on the center of the screen
//     pSprite->setPosition(ccp(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
// 
//     // add the sprite as a child to this layer
//     this->addChild(pSprite, 0);
     

    CCModelMd2* pMd2 = CCModelMd2::create("hellpig.md2");
    pMd2->setSkin("hellpig.bmp");

    CCSprite3D* pSprite3D = CCSprite3D::create(pMd2);
    pSprite3D->setPosition(100, 160, 0);
    pSprite3D->setScale(50, 50, 50);
    this->addChild(pSprite3D);

    CCRotateBy* pRotateAction = CCRotateBy::create(1.0f, 360);
    CCMoveBy* pMoveAction = CCMoveBy::create(1.0f, ccp(320, 0));
    pSprite3D->runAction(CCRepeatForever::create(CCSequence::create(
            pMoveAction,
            pRotateAction,
            pMoveAction->reverse(),
            pRotateAction->reverse(),
            NULL
        )));

    return true;
}

void HelloWorld::draw()
{
    CCLayer::draw();
//     CCDirector::sharedDirector()->setDepthTest(true);
//     
//     CC_NODE_DRAW_SETUP();
// 
// 	kmMat4 matrixP;
//     kmMat4 matrixMV;
//     kmMat4 matrixMVP;
//     
//     kmGLGetMatrix(KM_GL_PROJECTION, &matrixP );
//     kmGLGetMatrix(KM_GL_MODELVIEW, &matrixMV );
//     
// 	kmVec3 translation;
// 	kmVec3Fill(&translation, 240, 150, 220);
//     
//     kmMat4 translation1;
//     kmMat4Translation(&translation1, 240, 120, 0);
//     kmMat4 translation2;
//     kmMat4Scaling(&translation2, 100, 100, 100);
//     kmMat4 out;
//     kmMat4Multiply(&out, &translation1, &translation2);
// 
//     kmMat4Multiply(&matrixMVP, &matrixP, &matrixMV);
// 	//kmMat4Multiply(&matrixMVP, &matrixMVP, &rotationAndMove);				// apply rotation and translation to the matrix
//     kmMat4Multiply(&matrixMVP, &matrixMVP, &out);
//     
//     GLuint matrixId = glGetUniformLocation(getShaderProgram()->getProgram(), kCCUniformMVPMatrix_s);
//     getShaderProgram()->setUniformLocationWithMatrix4fv(getShaderProgram()->m_uUniforms[kCCUniformMVPMatrix], matrixMVP.mat, 1);
//     
//     static unsigned int uiStartFrame = 0, uiEndFrame = 182;
//     static float fAnimSpeed = 16.5f;
//     g_MD2.animate(fAnimSpeed, uiStartFrame, uiEndFrame, true);
//     //g_MD2.render(0);
}


void HelloWorld::menuCloseCallback(CCObject* pSender)
{
    CCDirector::sharedDirector()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

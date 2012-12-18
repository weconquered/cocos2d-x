#include "HelloWorldScene.h"
#include "AppMacros.h"
USING_NS_CC;

CMd2 g_MD2;
CImage g_Skin;

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
    
    
    
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    CCMenuItemImage *pCloseItem = CCMenuItemImage::create(
                                        "CloseNormal.png",
                                        "CloseSelected.png",
                                        this,
                                        menu_selector(HelloWorld::menuCloseCallback));
    
	pCloseItem->setPosition(ccp(origin.x + visibleSize.width - pCloseItem->getContentSize().width/2 ,
                                origin.y + pCloseItem->getContentSize().height/2));

    // create menu, it's an autorelease object
    CCMenu* pMenu = CCMenu::create(pCloseItem, NULL);
    pMenu->setPosition(CCPointZero);
    this->addChild(pMenu, 1);

    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label
    
    CCLabelTTF* pLabel = CCLabelTTF::create("Hello World", "Arial", TITLE_FONT_SIZE);
    
    // position the label on the center of the screen
    pLabel->setPosition(ccp(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height - pLabel->getContentSize().height));

    // add the label as a child to this layer
    this->addChild(pLabel, 1);

    // add "HelloWorld" splash screen"
    CCSprite* pSprite = CCSprite::create("HelloWorld.png");

    // position the sprite on the center of the screen
    pSprite->setPosition(ccp(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

    // add the sprite as a child to this layer
    this->addChild(pSprite, 0);
     
    
//     model = MD2Model::create("hellpig.md2", "hellpig2.png");
//     model->retain();
    
    CCGLProgram* program = CCShaderCache::sharedShaderCache()->programForKey(kCCShader_PositionTexture);
    setShaderProgram(program);
    
    g_MD2.Load("hellpig.md2");
    g_Skin.Load("hellpig.bmp");
    g_MD2.SetSkin(g_Skin);

    return true;
}

// void HelloWorld::draw()
// {
//     CCDirector::sharedDirector()->setDepthTest(true);
//     
// 	
//     
// 	//getShaderProgram()->use();
//     CC_NODE_DRAW_SETUP();
//     
//     
// // 	kmMat4 matrixP;
// //     kmMat4 matrixMV;
// //     kmMat4 matrixMVP;
// //     
// //     kmGLGetMatrix(KM_GL_PROJECTION, &matrixP );
// //     kmGLGetMatrix(KM_GL_MODELVIEW, &matrixMV );
// //     
// // 	kmVec3 translation;
// // 	kmVec3Fill(&translation, 240, 150, 220);
// //     
// //     kmMat4 translation1;
// //     kmMat4Translation(&translation1, 150, 150, 150);
// //     
// //     kmMat4Multiply(&matrixMVP, &matrixP, &matrixMV);
// // 	//kmMat4Multiply(&matrixMVP, &matrixMVP, &rotationAndMove);				// apply rotation and translation to the matrix
// //     kmMat4Multiply(&matrixMVP, &matrixMVP, &translation1);
// //     
// //     GLuint matrixId = glGetUniformLocation(getShaderProgram()->getProgram(), "CC_MVPMatrix");
// //     getShaderProgram()->setUniformLocationWithMatrix4fv(matrixId, matrixMVP.mat, 1);
//     
// 
// //    model->draw();
//     kmGLPushMatrix();
//     kmGLLoadIdentity();
//     kmGLTranslatef(0, -0.75f, -5);
// //     kmGLLoadIdentity();
// //     kmMat4 translation1;
//     g_MD2.Render();
//     kmGLPopMatrix();
// }


void HelloWorld::menuCloseCallback(CCObject* pSender)
{
    CCDirector::sharedDirector()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

#include "HelloWorldScene.h"
#include <vector>
#include <string>
#include "support/base64.h"
using namespace std;
USING_NS_CC;

int base64Encode2( unsigned char *input, unsigned int input_len, unsigned char **output );
int base64Decode2( unsigned char *input, unsigned int input_len, unsigned char **output );

CCScene* HelloWorld::scene()
{
	// 'scene' is an autorelease object
	CCScene *scene = CCScene::node();
	
	// 'layer' is an autorelease object
	HelloWorld *layer = HelloWorld::node();

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

	/////////////////////////////
	// 2. add a menu item with "X" image, which is clicked to quit the program
	//    you may modify it.

	// add a "close" icon to exit the progress. it's an autorelease object
	CCMenuItemImage *pCloseItem = CCMenuItemImage::itemFromNormalImage(
										"CloseNormal.png",
										"CloseSelected.png",
										this,
										menu_selector(HelloWorld::menuCloseCallback) );
	pCloseItem->setPosition( ccp(CCDirector::sharedDirector()->getWinSize().width - 20, 20) );

	// create menu, it's an autorelease object
	CCMenu* pMenu = CCMenu::menuWithItems(pCloseItem, NULL);
	pMenu->setPosition( CCPointZero );
	this->addChild(pMenu, 1);

	/////////////////////////////
	// 3. add your codes below...

	// add a label shows "Hello World"
	// create and initialize a label
    CCLabelTTF* pLabel = CCLabelTTF::labelWithString("Hello World", "Arial", 24);
	// ask director the window size
	CCSize size = CCDirector::sharedDirector()->getWinSize();

	// position the label on the center of the screen
	pLabel->setPosition( ccp(size.width / 2, size.height - 50) );

	// add the label as a child to this layer
	this->addChild(pLabel, 1);

	// add "HelloWorld" splash screen"
	CCSprite* pSprite = CCSprite::spriteWithFile("HelloWorld.png");

	// position the sprite on the center of the screen
	pSprite->setPosition( ccp(size.width/2, size.height/2) );

	// add the sprite as a child to this layer
	this->addChild(pSprite, 0);
    for (int i = 0; i < 10; i++)
    {

        unsigned char szInput[] = "abcdefghijklmnopqlsdfjllasdjfl";
        szInput[i+1] = 0;
        unsigned char* pOut = NULL;
        int len = base64Encode2(szInput, strlen((char*)szInput), &pOut);

        unsigned char* pOut2 = NULL;
        int len2 = base64Decode2(pOut, len, &pOut2);

        delete[] pOut2;
        delete[] pOut;
    }

    class UnitDataToSave{
    public:
        UnitDataToSave()
        {
            memset(szName, 0, sizeof(szName));
            xLocation = 0;
            yLocation = 0;
            Life = 0;
        }
        void setValue(char* name, int x, int y, int l)
        {
            strcpy(szName, name);
            xLocation = x;
            yLocation = y;
            Life = l;
        }
        void show()
        {
            CCLog("name=%s,x=%d,y=%d,l=%d", szName, xLocation, yLocation, Life);
        }
        char szName[50];
        int xLocation;
        int yLocation;
        int Life;
    };
    
    char szName1[][10] = {{"abc"}, {"def"}, {"ghi"}, {"jkl"}, {"mno"}, {"pqs"}, {"rtu"}, {"mno"}, {"pqs"}, {"rtu"} };

    vector<UnitDataToSave> unitArray;
    for (int i = 0; i < 10; i++)
    {
        UnitDataToSave oneData;
        oneData.setValue(szName1[i], i, 10-i, i*2);
        unitArray.push_back(oneData);
    }

    string strTotal;
    vector<UnitDataToSave>::iterator it;
    for (it = unitArray.begin(); it != unitArray.end(); ++it)
    {
        unsigned char* pszEncode = (unsigned char*)&(*it);
        unsigned char* pszOut = NULL;
        int len = base64Encode2(pszEncode, sizeof(UnitDataToSave), &pszOut);
        if (len > 0 && pszOut != NULL)
        {
            strTotal.append((char*)pszOut);
            delete[] pszOut;
        }
    }

    // recover data
    UnitDataToSave* pData = new UnitDataToSave[10];
    unsigned char* pTotalOutput = NULL;
    int totalLen = base64Decode((unsigned char*)strTotal.c_str(), strTotal.length(), &pTotalOutput);
    int totalStructSize = sizeof(UnitDataToSave) * 10;
//    assert(totalLen == totalStructSize);
    memcpy(pData, pTotalOutput, totalStructSize);
    for (int  i = 0; i < 10; i++)
    {
        pData[i].show();
    }
    delete[] pData;
    
	return true;
}

void HelloWorld::menuCloseCallback(CCObject* pSender)
{
	CCDirector::sharedDirector()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}

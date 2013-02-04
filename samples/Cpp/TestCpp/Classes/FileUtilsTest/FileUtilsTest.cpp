#include "FileUtilsTest.h"


TESTLAYER_CREATE_FUNC(TestResolutionDirectories);
TESTLAYER_CREATE_FUNC(TestSearchPath);
TESTLAYER_CREATE_FUNC(TestFilenameLookup);
TESTLAYER_CREATE_FUNC(TestIsFileExist);
TESTLAYER_CREATE_FUNC(TestCreateDirectory);

static NEWTESTFUNC createFunctions[] = {
    CF(TestResolutionDirectories),
    CF(TestSearchPath),
    CF(TestFilenameLookup),
    CF(TestIsFileExist),
    CF(TestCreateDirectory)
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

void FileUtilsTestScene::runThisTest()
{
    CCLayer* pLayer = nextAction();
    addChild(pLayer);
    
    CCDirector::sharedDirector()->replaceScene(this);
}

// #pragma mark - FileUtilsDemo

void FileUtilsDemo::onEnter()
{
    CCLayer::onEnter();
    
    CCLabelTTF* label = CCLabelTTF::create(title().c_str(), "Arial", 32);
    addChild(label);
    label->setPosition(ccp(VisibleRect::center().x, VisibleRect::top().y-50));
    
    std::string subTitle = subtitle();
    if(! subTitle.empty())
    {
        CCLabelTTF* l = CCLabelTTF::create(subTitle.c_str(), "Thonburi", 16);
        addChild(l, 1);
        l->setPosition(ccp(VisibleRect::center().x, VisibleRect::top().y-80));
    }
    
    CCMenuItemImage *item1 = CCMenuItemImage::create("Images/b1.png", "Images/b2.png", this, menu_selector(FileUtilsDemo::backCallback));
    CCMenuItemImage *item2 = CCMenuItemImage::create("Images/r1.png","Images/r2.png", this, menu_selector(FileUtilsDemo::restartCallback) );
    CCMenuItemImage *item3 = CCMenuItemImage::create("Images/f1.png", "Images/f2.png", this, menu_selector(FileUtilsDemo::nextCallback) );
    
    CCMenu *menu = CCMenu::create(item1, item2, item3, NULL);
    menu->setPosition(CCPointZero);
    item1->setPosition(ccp(VisibleRect::center().x - item2->getContentSize().width*2, VisibleRect::bottom().y+item2->getContentSize().height/2));
    item2->setPosition(ccp(VisibleRect::center().x, VisibleRect::bottom().y+item2->getContentSize().height/2));
    item3->setPosition(ccp(VisibleRect::center().x + item2->getContentSize().width*2, VisibleRect::bottom().y+item2->getContentSize().height/2));
    
    addChild(menu, 1);
}

void FileUtilsDemo::backCallback(CCObject* pSender)
{
    CCScene* pScene = new FileUtilsTestScene();
    CCLayer* pLayer = backAction();
    
    pScene->addChild(pLayer);
    CCDirector::sharedDirector()->replaceScene(pScene);
    pScene->release();
}

void FileUtilsDemo::nextCallback(CCObject* pSender)
{
    CCScene* pScene = new FileUtilsTestScene();
    CCLayer* pLayer = nextAction();
    
    pScene->addChild(pLayer);
    CCDirector::sharedDirector()->replaceScene(pScene);
    pScene->release();
}

void FileUtilsDemo::restartCallback(CCObject* pSender)
{
    CCScene* pScene = new FileUtilsTestScene();
    CCLayer* pLayer = restartAction();
    
    pScene->addChild(pLayer);
    CCDirector::sharedDirector()->replaceScene(pScene);
    pScene->release();
}

string FileUtilsDemo::title()
{
    return "No title";
}

string FileUtilsDemo::subtitle()
{
    return "";
}

//#pragma mark - TestResolutionDirectories

void TestResolutionDirectories::onEnter()
{
    FileUtilsDemo::onEnter();
    CCFileUtils *sharedFileUtils = CCFileUtils::sharedFileUtils();

    string ret;
    
    sharedFileUtils->purgeCachedEntries();
    m_defaultSearchPathArray = sharedFileUtils->getSearchPaths();
    m_defaultResolutionsOrderArray = sharedFileUtils->getSearchResolutionsOrder();
    
    vector<string> searchPaths;
    searchPaths.push_back("Misc");
    sharedFileUtils->setSearchPaths(searchPaths);
    
    vector<string> resolutionsOrder;

    resolutionsOrder.push_back("resources-ipadhd");
    resolutionsOrder.push_back("resources-ipad");
    resolutionsOrder.push_back("resources-widehd");
    resolutionsOrder.push_back("resources-wide");
    resolutionsOrder.push_back("resources-hd");
    resolutionsOrder.push_back("resources-iphone");
    
    sharedFileUtils->setSearchResolutionsOrder(resolutionsOrder);
    
    for( int i=1; i<7; i++) {
        CCString *filename = CCString::createWithFormat("test%d.txt", i);
        ret = sharedFileUtils->fullPathForFilename(filename->getCString());
        CCLog("%s -> %s", filename->getCString(), ret.c_str());
    }
}

void TestResolutionDirectories::onExit()
{
    CCFileUtils *sharedFileUtils = CCFileUtils::sharedFileUtils();
    
	// reset search path
	sharedFileUtils->setSearchPaths(m_defaultSearchPathArray);
    sharedFileUtils->setSearchResolutionsOrder(m_defaultResolutionsOrderArray);
    FileUtilsDemo::onExit();
}

string TestResolutionDirectories::title()
{
    return "FileUtils: resolutions in directories";
}

string TestResolutionDirectories::subtitle()
{
    return "See the console";
}

//#pragma mark - TestSearchPath

void TestSearchPath::onEnter()
{
    FileUtilsDemo::onEnter();
    CCFileUtils *sharedFileUtils = CCFileUtils::sharedFileUtils();
    
    string ret;
    
    sharedFileUtils->purgeCachedEntries();
    m_defaultSearchPathArray = sharedFileUtils->getSearchPaths();
    m_defaultResolutionsOrderArray = sharedFileUtils->getSearchResolutionsOrder();
    
    vector<string> searchPaths;
    string writablePath = sharedFileUtils->getWriteablePath();

    createFile(writablePath+"external.txt", "Hello Cocos2d-x!");
    
    std::string strCreatedDir = writablePath+"resources-iphone";
    bool isDirCreated = sharedFileUtils->createDirectory(strCreatedDir);
    if (isDirCreated)
    {
        CCLog("create directory (%s) succeed.", strCreatedDir.c_str());
        createFile(strCreatedDir+"/external-subdir.txt", "I'm the file in sub directory");
    }
    else
    {
        CCLog("create directory (%s) fails", strCreatedDir.c_str());
    }
    
    searchPaths.push_back(writablePath);
    searchPaths.push_back("Misc/searchpath1");
    searchPaths.push_back("Misc/searchpath2");
    sharedFileUtils->setSearchPaths(searchPaths);
    
    
    vector<string> resolutionsOrder;
    
    resolutionsOrder.push_back("resources-iphone");
    resolutionsOrder.push_back("resources-ipad");
    sharedFileUtils->setSearchResolutionsOrder(resolutionsOrder);
    
    for( int i=1; i<3; i++) {
        CCString *filename = CCString::createWithFormat("file%d.txt", i);
        ret = sharedFileUtils->fullPathForFilename(filename->getCString());
        CCLog("%s -> %s", filename->getCString(), ret.c_str());
    }
    
    // Gets external.txt from writable path
    readFileContents(sharedFileUtils->fullPathForFilename("external.txt"));
    
    readFileContents(sharedFileUtils->fullPathForFilename("external-subdir.txt"));
}

void TestSearchPath::onExit()
{
	CCFileUtils *sharedFileUtils = CCFileUtils::sharedFileUtils();

	// reset search path
	sharedFileUtils->setSearchPaths(m_defaultSearchPathArray);
    sharedFileUtils->setSearchResolutionsOrder(m_defaultResolutionsOrderArray);
    FileUtilsDemo::onExit();
}

string TestSearchPath::title()
{
    return "FileUtils: search path";
}

string TestSearchPath::subtitle()
{
    return "See the console";
}

void TestSearchPath::createFile(const std::string& strFilePath, const std::string& strContent)
{
    FILE* fp = fopen(strFilePath.c_str(), "wb");
    if (fp)
    {
        fwrite(strContent.c_str(), 1, strContent.length(), fp);
        fclose(fp);
    }
}

void TestSearchPath::readFileContents(const std::string& strFilePath)
{
    FILE* fp = fopen(strFilePath.c_str(), "rb");
    if (fp)
    {
        int length = 0;
        fseek(fp, 0, SEEK_END);
        length = ftell(fp);
        fseek(fp, 0, SEEK_SET);
        char* pBuf = (char*)malloc(length+1);
        memset(pBuf, 0, length+1);
        fread(pBuf, 1, length, fp);
        CCLog("\n-------------------------begin-------------------------------\n");
        CCLog("The content of file (%s) : %s", strFilePath.c_str(), pBuf);
        CCLog("\n------------------------- end -------------------------------\n");
        fclose(fp);
        free(pBuf);
    }
}

//#pragma mark - TestFilenameLookup

void TestFilenameLookup::onEnter()
{
    FileUtilsDemo::onEnter();
		
    CCFileUtils *sharedFileUtils = CCFileUtils::sharedFileUtils();

    CCDictionary *dict = CCDictionary::create();
    dict->setObject(CCString::create("Images/grossini.png"), "grossini.bmp");
    dict->setObject(CCString::create("Images/grossini.png"), "grossini.xcf");
    
    sharedFileUtils->setFilenameLookupDictionary(dict);
    
    
    // Instead of loading carlitos.xcf, it will load grossini.png
    CCSprite *sprite = CCSprite::create("grossini.xcf");
    this->addChild(sprite);
    
    CCSize s = CCDirector::sharedDirector()->getWinSize();
    sprite->setPosition(ccp(s.width/2, s.height/2));
}

void TestFilenameLookup::onExit()
{
	
	CCFileUtils *sharedFileUtils = CCFileUtils::sharedFileUtils();
	
	// reset filename lookup
    sharedFileUtils->setFilenameLookupDictionary(CCDictionary::create());
	
    FileUtilsDemo::onExit();
}

string TestFilenameLookup::title()
{
    return "FileUtils: filename lookup";
}

string TestFilenameLookup::subtitle()
{
    return "See the console";
}

//#pragma mark - TestIsFileExist

void TestIsFileExist::onEnter()
{
    FileUtilsDemo::onEnter();
    CCSize s = CCDirector::sharedDirector()->getWinSize();
    CCFileUtils *sharedFileUtils = CCFileUtils::sharedFileUtils();
    
    CCLabelTTF* pTTF = NULL;
    bool isExist = false;
    
    isExist = sharedFileUtils->isFileExist("Images/grossini.png");
    
    pTTF = CCLabelTTF::create(isExist ? "Images/grossini.png exists" : "Images/grossini.png doesn't exist", "", 20);
    pTTF->setPosition(ccp(s.width/2, s.height/3));
    this->addChild(pTTF);
    
    isExist = sharedFileUtils->isFileExist("Images/grossini.xcf");
    pTTF = CCLabelTTF::create(isExist ? "Images/grossini.xcf exists" : "Images/grossini.xcf doesn't exist", "", 20);
    pTTF->setPosition(ccp(s.width/2, s.height/3*2));
    this->addChild(pTTF);
}

void TestIsFileExist::onExit()
{
	
	CCFileUtils *sharedFileUtils = CCFileUtils::sharedFileUtils();
	
    FileUtilsDemo::onExit();
}

string TestIsFileExist::title()
{
    return "FileUtils: check whether the file exists";
}

string TestIsFileExist::subtitle()
{
    return "";
}

//#pragma mark - TestCreateDirectory

void TestCreateDirectory::onEnter()
{
    FileUtilsDemo::onEnter();
    CCFileUtils *sharedFileUtils = CCFileUtils::sharedFileUtils();
    
    std::string writablePath = sharedFileUtils->getWriteablePath();
    bool ret = sharedFileUtils->createDirectory(writablePath+"newDir");
    CCLog("create directory return: %d", ret);
}

void TestCreateDirectory::onExit()
{
    FileUtilsDemo::onExit();
}

string TestCreateDirectory::title()
{
    return "FileUtils: test creating directory";
}

string TestCreateDirectory::subtitle()
{
    return "";
}
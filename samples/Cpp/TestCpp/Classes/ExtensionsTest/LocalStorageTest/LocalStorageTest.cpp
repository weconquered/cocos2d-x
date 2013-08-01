//
//  LocalStorageTest.cpp
//  samples
//
//  Created by James Chen on 8/1/13.
//
//

#include "LocalStorageTest.h"
#include "LocalStorage/LocalStorage.h"
#include "../ExtensionsTest.h"

USING_NS_CC;
USING_NS_CC_EXT;

LocalStorageTest::LocalStorageTest()
{
    Point visibleOrigin = EGLView::getInstance()->getVisibleOrigin();
    Size visibleSize = EGLView::getInstance()->getVisibleSize();
    
    std::string dbPath = FileUtils::getInstance()->getWritablePath();
    dbPath += "/sqlite3_test.db";
    localStorageInit(dbPath.c_str());
    
    Size winSize = Director::getInstance()->getWinSize();
    
    const int MARGIN = 40;
    const int SPACE = 35;
    
    LabelTTF *label = LabelTTF::create("LocalStorageTest Test", "Arial", 28);
    label->setPosition(Point(winSize.width / 2, winSize.height - MARGIN));
    addChild(label, 0);
    
    Menu *menuRequest = Menu::create();
    menuRequest->setPosition(Point::ZERO);
    addChild(menuRequest);
    
    Size editBoxSize = Size(visibleSize.width - 100, 40);
    
    // key
    _dbKey = EditBox::create(editBoxSize, Scale9Sprite::create("extensions/green_edit.png"));
    _dbKey->setPosition(Point(visibleOrigin.x+visibleSize.width/2, visibleOrigin.y+visibleSize.height*2/4));
    _dbKey->setFontSize(25);
    _dbKey->setFontColor(Color3B::RED);
    _dbKey->setPlaceHolder("Key:");
    _dbKey->setPlaceholderFontColor(Color3B::WHITE);
    _dbKey->setMaxLength(20);
    _dbKey->setReturnType(EditBox::KeyboardReturnType::DONE);
    _dbKey->setDelegate(this);
    addChild(_dbKey);
    
    // value
    _dbValue = EditBox::create(editBoxSize, Scale9Sprite::create("extensions/green_edit.png"));
    _dbValue->setPosition(Point(visibleOrigin.x+visibleSize.width/2, visibleOrigin.y+visibleSize.height*1/4));
    _dbValue->setFontSize(25);
    _dbValue->setFontColor(Color3B::RED);
    _dbValue->setPlaceHolder("Value:");
    _dbValue->setPlaceholderFontColor(Color3B::WHITE);
    _dbValue->setMaxLength(20);
    _dbValue->setReturnType(EditBox::KeyboardReturnType::DONE);
    _dbValue->setDelegate(this);
    addChild(_dbValue);
    
    // Add Item
    LabelTTF *labelGet = LabelTTF::create("Add Item", "Arial", 22);
    MenuItemLabel *itemGet = MenuItemLabel::create(labelGet, [this](Object* sender) {
        std::string key = _dbKey->getText();
        std::string value = _dbValue->getText();
        if (key.length() > 0 && value.length() > 0)
        {
            localStorageSetItem(key.c_str(), value.c_str());
        }
        else
        {
            MessageBox("Please enter key and value!", "Prompt");
        }
        
    });
    itemGet->setPosition(Point(winSize.width *1/ 3, winSize.height - MARGIN - SPACE));
    menuRequest->addChild(itemGet);
    
    // Remove Item
    LabelTTF *labelRemoveItem = LabelTTF::create("Remove Item", "Arial", 22);
    MenuItemLabel *removeItem = MenuItemLabel::create(labelRemoveItem, [this](Object* sender) {
        std::string key = _dbKey->getText();
        if (key.length() > 0)
        {
            if (localStorageGetItem(key.c_str()))
            {
                localStorageRemoveItem(key.c_str());
                char promptContent[100] = {0};
                snprintf(promptContent, sizeof(promptContent), "The key %s has been removed from database successfully!", key.c_str());
                MessageBox(promptContent, "Prompt");
            }
            else
            {
                MessageBox("The key to be removed doesn't exist!", "Error");
            }
        }
        else
        {
            MessageBox("Please enter key !", "Prompt");
        }
        
    });
    
    removeItem->setPosition(Point(winSize.width *2/ 3, winSize.height - MARGIN - SPACE));
    menuRequest->addChild(removeItem);
    
    
    
    // Back Menu
    MenuItemFont *itemBack = MenuItemFont::create("Back", [](Object* sender){
        ExtensionsTestScene *scene = new ExtensionsTestScene();
        scene->runThisTest();
        scene->release();
    });
    itemBack->setPosition(Point(VisibleRect::rightBottom().x - 50, VisibleRect::rightBottom().y + 25));
    Menu *menuBack = Menu::create(itemBack, NULL);
    menuBack->setPosition(Point::ZERO);
    addChild(menuBack);
}

LocalStorageTest::~LocalStorageTest()
{
    localStorageFree();
}

void LocalStorageTest::editBoxEditingDidBegin(cocos2d::extension::EditBox* editBox)
{
    log("editBox %p DidBegin !", editBox);
}

void LocalStorageTest::editBoxEditingDidEnd(cocos2d::extension::EditBox* editBox)
{
    log("editBox %p DidEnd !", editBox);
}

void LocalStorageTest::editBoxTextChanged(cocos2d::extension::EditBox* editBox, const std::string& text)
{
    log("editBox %p TextChanged, text: %s ", editBox, text.c_str());
}

void LocalStorageTest::editBoxReturn(EditBox* editBox)
{
    log("editBox %p was returned !",editBox);
    
    if (_dbKey == editBox)
    {

    }
    else if (_dbValue == editBox)
    {

    }
}

void runLocalStorageTest()
{
    Scene *scene = Scene::create();
    LocalStorageTest *layer = new LocalStorageTest();
    scene->addChild(layer);
    
    Director::getInstance()->replaceScene(scene);
    layer->release();
}


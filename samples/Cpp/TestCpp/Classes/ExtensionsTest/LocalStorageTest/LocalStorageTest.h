//
//  LocalStorageTest.h
//  samples
//
//  Created by James Chen on 8/1/13.
//
//

#ifndef __samples__LocalStorageTest__
#define __samples__LocalStorageTest__

#include "cocos2d.h"
#include "cocos-ext.h"

class LocalStorageTest : public cocos2d::Layer, public cocos2d::extension::EditBoxDelegate
{
public:
    LocalStorageTest();
    virtual ~LocalStorageTest();
    
    virtual void editBoxEditingDidBegin(cocos2d::extension::EditBox* editBox);
    virtual void editBoxEditingDidEnd(cocos2d::extension::EditBox* editBox);
    virtual void editBoxTextChanged(cocos2d::extension::EditBox* editBox, const std::string& text);
    virtual void editBoxReturn(cocos2d::extension::EditBox* editBox);
    
private:
    cocos2d::extension::EditBox* _dbKey;
    cocos2d::extension::EditBox* _dbValue;
};

void runLocalStorageTest();

#endif /* defined(__samples__LocalStorageTest__) */

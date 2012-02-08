//
//  EPSpriteButton.cpp
//  EnjoyYourMoney
//
//  Created by Erawppa on 2011/9/1.
//  Copyright 2011年 __MyCompanyName__. All rights reserved.
//

#include "EPSpriteButton.h"

using namespace cocos2d;

CCSprite *EPSpriteButton::spriteWithSpriteFrame(CCSpriteFrame *spriteFrame ,CCSpriteFrame *contentSpriteFrame)
{
    CCSprite *object = CCSprite::spriteWithSpriteFrame(spriteFrame);
    CCSprite *content = CCSprite::spriteWithSpriteFrame(contentSpriteFrame);
    content->setPosition(ccp(object->getContentSize().width/2,object->getContentSize().height/2));
    object->addChild(content);
    return object;
}

CCMenuItem *EPSpriteButton::itemWithSpriteFrame(CCSpriteFrame *spriteFrame1,
                                                CCSpriteFrame *spriteFrame2,
                                                CCSpriteFrame *contentSpriteFrame)
{
    CCSprite *sp1 = CCSprite::spriteWithSpriteFrame(spriteFrame1);
    CCSprite *sp2 = CCSprite::spriteWithSpriteFrame(spriteFrame2);
    CCSprite *content = CCSprite::spriteWithSpriteFrame(contentSpriteFrame);
    
    CCMenuItem *item = CCMenuItemSprite::itemFromNormalSprite(sp1,sp2);
    content->setPosition(ccp(item->getContentSize().width/2,item->getContentSize().height/2));
    item->addChild(content);
    
    return item;
}


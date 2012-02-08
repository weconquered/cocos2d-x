//
//  EPSpriteButton.h
//  EnjoyYourMoney
//
//  Created by Erawppa on 2011/9/1.
//  Copyright 2011年 __MyCompanyName__. All rights reserved.
//
#ifndef __EPSPRITEBUTTON_H__
#define __EPSPRITEBUTTON_H__

#include "cocos2d.h"

class /*CC_DLL*/ EPSpriteButton : public cocos2d::CCSprite
{
public:
    static cocos2d::CCSprite *spriteWithSpriteFrame(cocos2d::CCSpriteFrame *spriteFrame ,cocos2d::CCSpriteFrame *contentSpriteFrame);
    static cocos2d::CCMenuItem *itemWithSpriteFrame(cocos2d::CCSpriteFrame *spriteFrame1,
                                                    cocos2d::CCSpriteFrame *spriteFrame2,
                                                    cocos2d::CCSpriteFrame *contentSpriteFrame);
};

#endif//__EPSPRITEBUTTON_H__
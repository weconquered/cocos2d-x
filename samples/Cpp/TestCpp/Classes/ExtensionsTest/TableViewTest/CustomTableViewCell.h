#ifndef __CUSTOMTABELVIEWCELL_H__
#define __CUSTOMTABELVIEWCELL_H__

#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;

class CustomTableViewCell : public cocos2d::extension::CCTableViewCell
{
public:
    CustomTableViewCell();
	virtual void draw();
    void reset();
    
    CC_PROPERTY(CCSprite*, m_pBG, BackGround);
    CC_PROPERTY(CCSprite*, m_pBGHighLight, BackGroundHighLight);
    CC_PROPERTY(CCSprite*, m_pIcon1, Icon);
    CC_PROPERTY(CCLabelTTF*, m_pIndexLabel, IndexLabel);
    CC_PROPERTY(CCLabelTTF*, m_pTitle, Title);
    CC_PROPERTY(CCLabelTTF*, m_pContent, Content);
    
    
    
};

#endif /* __CUSTOMTABELVIEWCELL_H__ */


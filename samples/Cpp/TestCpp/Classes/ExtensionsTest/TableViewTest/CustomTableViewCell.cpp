#include "CustomTableViewCell.h"

USING_NS_CC;

CustomTableViewCell::CustomTableViewCell()
: m_pBG(NULL)
, m_pIcon1(NULL)
, m_pIndexLabel(NULL)
, m_pBGHighLight(NULL)
, m_pTitle(NULL)
, m_pContent(NULL)
{}

void CustomTableViewCell::draw()
{
	CCTableViewCell::draw();
	// draw bounding box
// 	CCPoint pos = getPosition();
// 	CCSize size = CCSizeMake(178, 200);
// 	CCPoint vertices[4]={
// 		ccp(pos.x+1, pos.y+1),
// 		ccp(pos.x+size.width-1, pos.y+1),
// 		ccp(pos.x+size.width-1, pos.y+size.height-1),
// 		ccp(pos.x+1, pos.y+size.height-1),
// 	};
// 	ccDrawColor4B(0, 0, 255, 255);
// 	ccDrawPoly(vertices, 4, true);
}

void CustomTableViewCell::reset()
{
    if (m_pIcon1)
    {
        m_pIcon1->stopAllActions();
        this->removeChild(m_pIcon1);
        m_pIcon1 = NULL;
    }
}

void CustomTableViewCell::setBackGround(CCSprite* pBG)
{
    this->removeChild(m_pBG);
    this->addChild(pBG, 0);
    m_pBG = pBG;
}

CCSprite* CustomTableViewCell::getBackGround()
{
    return m_pBG;
}

void CustomTableViewCell::setBackGroundHighLight(CCSprite* pBG)
{
    this->removeChild(m_pBGHighLight);
    this->addChild(pBG, 10);
    m_pBGHighLight = pBG;
}

CCSprite* CustomTableViewCell::getBackGroundHighLight()
{
    return m_pBGHighLight;
}

void CustomTableViewCell::setIcon(CCSprite* pIcon)
{
    this->addChild(pIcon, 1);
    pIcon->stopAllActions();
    pIcon->setRotation(0);
    pIcon->runAction(CCRepeatForever::create(CCSequence::create(CCRotateBy::create(0.2, 15), CCRotateBy::create(0.2, -15), NULL)));
    m_pIcon1 = pIcon;
}

CCSprite* CustomTableViewCell::getIcon()
{
    return m_pIcon1;
}

void CustomTableViewCell::setIndexLabel(CCLabelTTF* pIndexLabel)
{
    if (m_pIndexLabel)
    {
        this->removeChild(m_pIndexLabel);
    }
    this->addChild(pIndexLabel, 2);
    m_pIndexLabel = pIndexLabel;
}

CCLabelTTF* CustomTableViewCell::getIndexLabel()
{
    return m_pIndexLabel;
}

void CustomTableViewCell::setTitle(CCLabelTTF* pTitle)
{
    if (m_pTitle)
    {
        this->removeChild(m_pTitle);
    }
    this->addChild(pTitle, 3);
    m_pTitle = pTitle;
}

CCLabelTTF* CustomTableViewCell::getTitle()
{
    return m_pTitle;
}

void CustomTableViewCell::setContent(CCLabelTTF* pContent)
{
    if (m_pContent)
    {
        this->removeChild(m_pContent);
    }
    this->addChild(pContent, 4);
    m_pContent = pContent;
}

CCLabelTTF* CustomTableViewCell::getContent()
{
    return m_pContent;
}


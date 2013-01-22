/*
 *  CCTableViewCell.cpp
 *  SkeletonX
 *
 *  Created by Vincent on 11-11-21.
 *  Copyright 2011 GeekStudio. All rights reserved.
 *
 */

//#include "ccDrawExt.h"
#include "CCTableView.h"
#include "CCTableViewCell.h"


CCTableViewCell::CCTableViewCell(const char * mCellIdentifier)
{
	std::string tmpIdentify(mCellIdentifier);
	this->setCellIdentifier(tmpIdentify);
	
	this->setEditable(false);
	
	parentView = NULL;
	
	delBtnColor = ccRED;
	
	CCLabelTTF * labelDelete = CCLabelTTF::create("Delete", "Arial", 16);
	deleteButton = CCMenuItemLabel::create(labelDelete, this,menu_selector(CCTableViewCell::deleteButtonSelector));
	menuItem = CCMenu::createWithItem(deleteButton);
	this->addChild(menuItem,20);
	this->showDeleteButton(false);
}

CCTableViewCell::~CCTableViewCell()
{
	this->removeAllChildrenWithCleanup(true);
}

void CCTableViewCell::resetDeleteButtonPosition()
{
	if (deleteButton != NULL) {
		CCPoint tmpPos = ccp(this->getContentSize().width - deleteButton->getContentSize().width/2.0 - 8,
							 this->getContentSize().height/2);
		deleteButton->getParent()->setPosition(tmpPos);
		((CCMenuItemLabel*)deleteButton)->setColor(delBtnColor);
	}
}

void CCTableViewCell::deleteButtonSelector(CCObject * mObject)
{
	if (parentView != NULL) {
		parentView->deleteCCTableViewCell(this);
	}
}

bool CCTableViewCell::isDeleteButtonShow()
{
	return (this->menuItem->isVisible());
}

void CCTableViewCell::showDeleteButton(bool mShow)
{
	this->menuItem->setVisible(mShow);
	this->deleteButton->setVisible(mShow);
}

/*
void CCTableViewCell::onEnter()
{
	CCLay::onEnter();
}

void CCTableViewCell::onExit()
{
	CCLayerColor::onExit();
}
*/

void CCTableViewCell::draw()
{/*
	glColor4f(1, 0, 0, 1.0);
	ccDrawRect(ccp(1,1), 
					 ccp(this->getContentSize().width-1,
						 this->getContentSize().height-1));
	glColor4f(1, 1, 1, 1.0);*/
}
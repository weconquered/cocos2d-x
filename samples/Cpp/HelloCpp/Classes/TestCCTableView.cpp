/*
 *  TestCCTableView.cpp
 *  SkeletonX
 *
 *  Created by mac on 11-11-23.
 *  Copyright 2011 GeekStudio. All rights reserved.
 *
 */

//#include "DecoratedBox.h"
#include "CustomCCTableViewCell.h"
//#include "ccDrawExt.h"
#include "CCTableViewCell.h"
#include "TestCCTableView.h"

TestCCTableView::TestCCTableView(CCRect mRect)
{
	this->setTouchEnabled(true);
	this->setContentSize(mRect.size);
	this->setPosition(mRect.origin);
	rowCount = 20;
	_tableView = new CCTableView(mRect);
	_tableView->setDelegate(this);
	_tableView->setDataSource(this);
	_tableView->setPosition(ccp(0,0));
	this->addChild(_tableView);
}

TestCCTableView::~TestCCTableView()
{
}

void TestCCTableView::ccTableViewWillDisplayCellForRowAtIndexPath(CCIndexPath &mIndexPath,CCTableViewCell * mTableViewCell,CCTableView * mTableView)
{

}

void TestCCTableView::ccTableViewWillReloadCellForRowAtIndexPath(CCIndexPath &mIndexPath,CCTableViewCell * mTableViewCell,CCTableView * mTableView)
{
	char labelStr[60];
	sprintf(labelStr, "%d",mIndexPath.row);
	
	CCLabelTTF * label = (CCLabelTTF * ) mTableViewCell->getChildByTag(20);
	if (label) {
		label->setString(labelStr);
	}
}


float TestCCTableView::cellHeightForRowAtIndexPath(CCIndexPath & mIndexPath,CCTableView * mTableView)
{
//	if (mIndexPath.row == 2) {
//		return 120;
//	}else if (mIndexPath.row == 4) {
//		return 160;
//	}
	return 60;
}
void TestCCTableView::didSelectRowAtIndexPath(CCIndexPath & mIndexPath,CCTableView * mTableView)
{
	if (mIndexPath.row == rowCount -1) {
		rowCount += 2;
		mTableView->reloadData();
	}
}

unsigned int TestCCTableView::numberOfRowsInSection(unsigned int mSection,CCTableView * mTableView)
{
	return rowCount;
}

unsigned int TestCCTableView::numberOfSectionsInCCTableView(CCTableView * mTableView)
{
	return 1;
}

/*cell was set to anchor point(0.5,0.5)*/
CCTableViewCell * TestCCTableView::cellForRowAtIndexPath(CCIndexPath &mIndexPath,CCTableView * mTableView)
{
	static const char * cellIdentifier = "cell-identifier";
	
	char labelStr[60];
	sprintf(labelStr, "%d",mIndexPath.row);
	
	CustomCCTableViewCell * cell = (CustomCCTableViewCell *) mTableView->dequeueReusableCellWithIdentifier(cellIdentifier);
	if (cell == NULL) {
		printf("cell alloc row =%d\n",mIndexPath.row);
		cell = new CustomCCTableViewCell(cellIdentifier);
		cell->autorelease();
		
//		DecoratedBox * box = DecoratedBox::decoratedBoxWithFile("Icon.png", mTableView->getContentSize().width,60);
//		box->setTag(200);
//		box->setPosition(CCPointMake(mTableView->getContentSize().width/2.0,60/2.0));
//		cell->addChild(box);
		
		CCLabelTTF * label = CCLabelTTF::create(labelStr, "Arial", 16);
		label->setColor(ccc3(0, 0, 0));
		label->setPosition(ccp(40,60/2.0));
		label->setColor(ccc3(255,255,255));
		label->setTag(20);
		cell->addChild(label);
		cell->parentView = _tableView;
		//cell->setColor(ccc3(128, 128, 128)); 
	}
	else{
		CCLabelTTF * label = (CCLabelTTF * ) cell->getChildByTag(20);
		if (label) {
			label->setString(labelStr);
		}
	}
	return cell;
}

void TestCCTableView::ccTableViewCommitCellEditingStyleForRowAtIndexPath(CCTableView * mTableView,CCTableViewCellEditingStyle mCellEditStyle,CCIndexPath &mIndexPath)
{
	if (mCellEditStyle == CCTableViewCellEditingStyleDelete) {
		rowCount--;
		mTableView->reloadData();
	}
}

void TestCCTableView::draw()
{/*
	glColor4f(1, 1, 0, 1.0);
	glLineWidth(2);
	ccDrawRect(ccp(1,1), 
				   ccp(this->getContentSize().width ,
					   this->getContentSize().height));
	glLineWidth(1);*/
}
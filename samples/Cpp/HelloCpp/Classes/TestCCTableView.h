/*
 *  TestCCTableView.h
 *  SkeletonX
 *
 *  Created by mac on 11-11-23.
 *  Copyright 2011 GeekStudio. All rights reserved.
 *
 */

#include "cocos2d.h"
USING_NS_CC;

#include "CCTableView.h"


class TestCCTableView : public CCLayer,public CCTableViewDelegate,public CCTableViewDataSource
{
public:
	CCTableView * _tableView;
	TestCCTableView(CCRect mRect=CCRectZero);
	~TestCCTableView();
	unsigned int rowCount;
public:
	void ccTableViewWillDisplayCellForRowAtIndexPath(CCIndexPath &mIndexPath,CCTableViewCell * mTableViewCell,CCTableView * mTableView);
	float cellHeightForRowAtIndexPath(CCIndexPath & mIndexPath,CCTableView * mTableView);
	void didSelectRowAtIndexPath(CCIndexPath & mIndexPath,CCTableView * mTableView);
	
	unsigned int numberOfRowsInSection(unsigned int mSection,CCTableView * mTableView);
	unsigned int numberOfSectionsInCCTableView(CCTableView * mTableView);
	/*cell was set to anchor point(0.5,0.5)*/
	CCTableViewCell * cellForRowAtIndexPath(CCIndexPath &mIndexPath,CCTableView * mTableView);
	virtual void ccTableViewCommitCellEditingStyleForRowAtIndexPath(CCTableView * mTableView,CCTableViewCellEditingStyle mCellEditStyle,CCIndexPath &mIndexPath);
	virtual void ccTableViewWillReloadCellForRowAtIndexPath(CCIndexPath &mIndexPath,CCTableViewCell * mTableViewCell,CCTableView * mTableView);
	
public:
	virtual void draw();
};

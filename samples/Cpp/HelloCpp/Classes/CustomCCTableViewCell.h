/*
 *  CustomCCTableViewCell.h
 *  SkeletonX
 *
 *  Created by mac on 11-11-23.
 *  Copyright 2011 GeekStudio. All rights reserved.
 *
 */

#include "cocos2d.h"
USING_NS_CC;

#include "CCTableViewCell.h"

class CustomCCTableViewCell : public CCTableViewCell {
public:
	CustomCCTableViewCell(const char * mCellIdentifier);
	~CustomCCTableViewCell();
};
/*
 *  CCTableView.cpp
 *  SkeletonX
 *
 *  Created by Vincent on 11-11-19.
 *  Copyright 2011 GeekStudio. All rights reserved.
 *
 */

#include "CCTableViewCell.h"
#include "CCTableView.h"

CCTableView::CCTableView(CCRect mRect):CCScrollLayerExt(mRect)
{
	this->setDelegate(NULL);
	this->setDataSource(NULL);
	this->setClipsToBounds(true);
	this->setTouchEnabled(true);
	this->setShowsHorizontalScrollIndicator(true);
	this->setShowsVerticalScrollIndicator(true);
	this->setEditable(true);
	this->setLockHorizontal(true);
	this->setScrollingToIndexPath(NULL);
	this->setScrollDelegate(this);
	this->setIsScheduled(false);
	
	cellRemovedArray = new CCArray();
	cellVisibleArray = new CCArray();
	indexPathAddedArray =new CCArray();
    
    //this->schedule(schedule_selector(CCTableView::checkBoundaryOfContent));
}

CCTableView::~CCTableView()
{
    this->unschedule(schedule_selector(CCTableView::checkBoundaryOfContent));
	CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
	this->removeAllChildrenWithCleanup(true);
	cellVisibleArray->removeAllObjects();
	delete cellVisibleArray ; cellVisibleArray = NULL;
	cellRemovedArray->removeAllObjects();
	delete cellRemovedArray ; cellRemovedArray = NULL;
	indexPathAddedArray->removeAllObjects();
	delete indexPathAddedArray ; indexPathAddedArray = NULL;
}

//check duplicate in visible cell array
CCTableViewCell * CCTableView::isDuplicateInVisibleCellArray(CCIndexPath *mIndexPath)
{
	if (cellVisibleArray && cellVisibleArray->count() > 0)
	{
		for(int i=0;i<cellVisibleArray->count();i++)
		{
			CCTableViewCell* pChild = (CCTableViewCell*) cellVisibleArray->objectAtIndex(i);
			if (pChild->indexPath == *mIndexPath)
			{
				return pChild;
			}
		}
	}
	return NULL;
}

bool CCTableView::isDuplicateIndexPath(CCIndexPath & mIndexPath)
{
	if (indexPathAddedArray && indexPathAddedArray->count() > 0)
	{
		for(int i=0;i<indexPathAddedArray->count();i++)
		{
			CCIndexPath* pChild = (CCIndexPath*) indexPathAddedArray->objectAtIndex(i);
			if (*pChild == mIndexPath)
			{
				return true;
			}
		}
	}
	return false;
}

bool CCTableView::removeIndexPathFromPathAddedArray(CCIndexPath & mIndexPath)
{
	if (indexPathAddedArray && indexPathAddedArray->count() > 0)
	{
		for(int i=0;i<indexPathAddedArray->count();i++)
		{
			CCIndexPath* pChild = (CCIndexPath*) indexPathAddedArray->objectAtIndex(i);
			if (*pChild == mIndexPath)
			{
				indexPathAddedArray->removeObject(pChild);
				return true;
			}
		}
	}
	return false;
}

void CCTableView::initCCTableViewCells()
{
	if (delegate != NULL && dataSource) 
	{
		CCTableViewCell * preCellView = NULL;
		int numSection =  dataSource->numberOfSectionsInCCTableView(this);
		for ( int i=0; i< numSection; i++) {
			bool isContinue = true;
			int numRows =  dataSource->numberOfRowsInSection(i, this);
			for ( int j=0; j< numRows; j++) {
				
				CCIndexPath* mIndexPath = CCIndexPath::CCIndexPathWithSectionRow(i,j);
				
				//printf(" init index (%d,%d) retain =%d \n",mIndexPath->section,mIndexPath->row,mIndexPath->retainCount());
				
				float cellHeight = delegate->cellHeightForRowAtIndexPath(*mIndexPath, this);
				CCPoint itemPosition;
				if (preCellView == NULL) {
					itemPosition.y = contentLayer->getContentSize().height - j * cellHeight - cellHeight;
				}else {
					itemPosition = ccp(preCellView->getPosition().x, preCellView->getPosition().y - cellHeight);
				}
				
				printf("pos = %f \n",itemPosition.y);
				float deltaY = itemPosition.y;
				if ( deltaY < 0) {
					isContinue == false;
					break;
				}
				printf("content height =%f \n",contentLayer->getContentSize().height);
				CCTableViewCell * cell = dataSource->cellForRowAtIndexPath(*mIndexPath, this);
				if (cell != NULL) {
					cell->setAnchorPoint(ccp(0.5,0.5));
					cell->setContentSize(CCSizeMake(this->getContentSize().width,cellHeight));
					cell->setPosition(ccp(0,itemPosition.y));
					cell->indexPath = *mIndexPath;
					cell->resetDeleteButtonPosition();
					cell->setEditable(this->getEditable());
					this->cellVisibleArray->addObject(cell);
					this->indexPathAddedArray->addObject(mIndexPath);
					
					if (delegate)
						delegate->ccTableViewWillDisplayCellForRowAtIndexPath(*mIndexPath, cell, this);
					
					contentLayer->addChild(cell);
					
					if (delegate)
						delegate->ccTableViewDidDisplayCellForRowAtIndexPath(*mIndexPath, cell, this);
					
					preCellView = cell;
				}

				//printf(" 2 init index (%d,%d) retain =%d \n",mIndexPath->section,mIndexPath->row,mIndexPath->retainCount());
				
				if (isContinue == false) {
					break;
				}
			}
			if (!isContinue) 
				break;
		}
	}
}

CCTableViewCell* CCTableView::cellForTouch(CCTouch *touch)
{
	CCPoint touchLocation = touch->getLocation();
	
	if (cellVisibleArray && cellVisibleArray->count() > 0)
	{
		for(int i=0;i<cellVisibleArray->count();i++)
		{
			CCTableViewCell* pChild = (CCTableViewCell*) cellVisibleArray->objectAtIndex(i);
			if (pChild)
			{
				CCPoint tmpLocal = pChild->convertToNodeSpace(touchLocation);
				CCRect r = CCRectMake(pChild->getPosition().x - pChild->getContentSize().width * pChild->getAnchorPoint().x, 
									  pChild->getPosition().y - pChild->getContentSize().height * pChild->getAnchorPoint().y,
									  pChild->getContentSize().width, pChild->getContentSize().height);
				r.origin = CCPointZero;
				
				if (r.containsPoint(tmpLocal))
				{
					return pChild;
				}
			}
		}
	}
	return NULL;
}

bool CCTableView::deleteCCTableViewCell(CCTableViewCell * mCell)
{
	if (mCell != NULL) {
		int tmpCellWidth  =  mCell->getContentSize().width;
		int tmpCellHeight =  mCell->getContentSize().height;
		//int mCellSection  =  mCell->indexPath.section;
		int mCellRow	  =  mCell->indexPath.row;
		
		CCIndexPath mCellIndexPath = mCell->indexPath;
		
		this->removeIndexPathFromPathAddedArray(mCell->indexPath);
		//this->cellRemovedArray->addObject(mCell);
		contentLayer->removeChild(mCell, true);
		this->cellVisibleArray->removeObject(mCell);
		
		
		for (int i= mCellRow ; i<this->cellVisibleArray->count(); i++) 
		{
			CCTableViewCell * tmpMcell =  (CCTableViewCell * ) cellVisibleArray->objectAtIndex(i);
			CCPoint tmpPos			   =  tmpMcell->getPosition();
			tmpMcell->setPosition(ccp(tmpPos.x,tmpPos.y + tmpCellHeight));
			
			CCIndexPath tmpIndex  = tmpMcell->indexPath;
			tmpMcell->indexPath = CCIndexPath(tmpIndex.section,tmpIndex.row - 1);
		}
		
		if (dataSource) {
			dataSource->ccTableViewCommitCellEditingStyleForRowAtIndexPath(this,CCTableViewCellEditingStyleDelete, mCellIndexPath);
		}

		return true;
	}
	return false;
}

CCTableViewCell * CCTableView::dequeueReusableCellWithIdentifier(const char * mCellIdentifier)
{
	/*
	int removedCount = cellRemovedArray->count();
	std::string tmpCellIdentify(mCellIdentifier);
	if (removedCount != 0) {
		for ( int i=0; i< removedCount; i++) {
			CCTableViewCell * cell =(CCTableViewCell *) cellRemovedArray->objectAtIndex(i);
			if (cell->getCellIdentifier() == tmpCellIdentify) {
				//cell->retain();
				//printf("dequeue cell =%x retaincount=%d\n",cell,cell->retainCount());
				cell->setEditable(this->getEditable());
				cell->showDeleteButton(false);
				this->cellRemovedArray->removeObject(cell,false);
				//printf("222 dequeue cell=%x retaincount=%d\n\n",cell,cell->retainCount());
				return cell;
			}
		}
	}
	*/
	return NULL;
}

// returns nil if cell is not visible or index path is out of range
CCTableViewCell * CCTableView::cellForRowAtIndexPath(CCIndexPath &mIndexPath)
{
	for(int i=0;i<cellVisibleArray->count();i++)
	{
		CCTableViewCell* pChild = (CCTableViewCell*) cellVisibleArray->objectAtIndex(i);
		if (pChild && pChild->indexPath == mIndexPath)
		{
			return pChild;
		}
	}
	return NULL;
}

void CCTableView::scrollToIndexPath(CCIndexPath & mIndexPath)
{
	
}

void CCTableView::reloadData()
{
	if (delegate != NULL && dataSource)
	{
		CCTableViewCell * preCellView = NULL;
		int numSection =  dataSource->numberOfSectionsInCCTableView(this);
		for ( int i=0; i< numSection; i++) 
		{
			bool isContinue = true;
			int numRows =  dataSource->numberOfRowsInSection(i, this);
			int cellCount = this->cellVisibleArray->count();
			for (int j= 0; j<numRows; j++) {
				
				float firstCellY = 0;
				if (j < cellCount) {
					CCTableViewCell * cellItem = (CCTableViewCell*)this->cellVisibleArray->objectAtIndex(j);
					if (cellItem != NULL) {
						if (delegate)
							delegate->ccTableViewWillReloadCellForRowAtIndexPath(cellItem->indexPath, cellItem, this);
						preCellView = cellItem;
						if (j == 0) {
							firstCellY = cellItem->getPosition().y;
						}
						continue;
					}
				}
				
				CCIndexPath* mIndexPath = CCIndexPath::CCIndexPathWithSectionRow(i,j);
				float cellHeight = delegate->cellHeightForRowAtIndexPath(*mIndexPath, this);
				CCPoint itemPosition;
				if (preCellView == NULL) {
					itemPosition.y = contentLayer->getContentSize().height - j * cellHeight - cellHeight;
				}else {
					itemPosition = ccp(preCellView->getPosition().x, preCellView->getPosition().y - cellHeight);
				}
				
				/////////////////extent content layer's size//////////////
				float deltaY = itemPosition.y;
				CCTableViewCell * preCellItem;
				if ( deltaY < 0 && this->isDuplicateIndexPath(*mIndexPath) == false) { // content size is not enough
					CCSize contentSize = contentLayer->getContentSize();
					contentLayer->setContentSize(CCSizeMake(contentSize.width,contentSize.height - deltaY));
					
					for (int m= cellVisibleArray->count()-1; m >=0 ; m--) {
						CCTableViewCell * cellItem = (CCTableViewCell *) cellVisibleArray->objectAtIndex(m);
						CCPoint tmpPos;
						if (m == cellVisibleArray->count()-1) {
							tmpPos = ccp(cellItem->getPosition().x,firstCellY - deltaY+1);
							itemPosition = ccp(cellItem->getPosition().x, tmpPos.y - cellHeight);
						}
						else {
							tmpPos = ccp(cellItem->getPosition().x,preCellItem->getPosition().y + preCellItem->getContentSize().height);
						}
						cellItem->setPosition(tmpPos);
						preCellItem = cellItem;
					}
					//contentLayer->setPosition(ccpSub(contentLayer->getPosition(), ccp(0,deltaY)));//content layer 往下移动
					isContinue == false;
				}
				///////////////////////////////////////////////////////////////////////
				
				CCTableViewCell * cell = dataSource->cellForRowAtIndexPath(*mIndexPath, this);
				if (cell != NULL) {
					
					cell->setAnchorPoint(ccp(0.5,0.5));
					cell->setContentSize(CCSizeMake(this->getContentSize().width,cellHeight));
					cell->setPosition(ccp(0,itemPosition.y));
					cell->indexPath = *mIndexPath;
					cell->resetDeleteButtonPosition();
					cell->setEditable(this->getEditable());
					this->cellVisibleArray->addObject(cell);
					this->indexPathAddedArray->addObject(mIndexPath);
					
					if (delegate)
						delegate->ccTableViewWillDisplayCellForRowAtIndexPath(*mIndexPath, cell, this);
					
					contentLayer->addChild(cell);
					
					if (delegate)
						delegate->ccTableViewDidDisplayCellForRowAtIndexPath(*mIndexPath, cell, this);
					
					preCellView = cell;
				}
				
				if (isContinue == false) {
					break;
				}
			}
		}
	}
}

//void CCTableView::visit()
//{
//	CCScrollLayerExt::visit();
//}
//

void CCTableView::onEnter()
{
	CCScrollLayerExt::onEnter();
    this->initCCTableViewCells();
}

void CCTableView::onExit()
{
	CCScrollLayerExt::onExit();
}

/////Menu - Events
void CCTableView::registerWithTouchDispatcher()
{
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, kCCMenuHandlerPriority -1, false);
}

bool CCTableView::ccTouchBegan(CCTouch* touch, CCEvent* event)
{
	bool isInFrameRect = CCScrollLayerExt::ccTouchBegan(touch,event);
	
    if (!isInFrameRect) {
        return false;
    }
    
	beginTouch = touch;
	CCPoint touchLocation = touch->getLocation();
	
	if (!this->getIsScheduled()) {
		this->schedule(schedule_selector(CCTableView::checkBoundaryOfContent));
		this->setIsScheduled(true);
	}
	
	return true;
}

void CCTableView::ccTouchEnded(CCTouch *touch, CCEvent* event)
{
	CCScrollLayerExt::ccTouchEnded(touch,event);
	this->touchFinish(touch);
	
	if(this->getIsScheduled())
	{
		printf("did end decelerating ..\n");
		this->unschedule(schedule_selector(CCTableView::checkBoundaryOfContent));
		this->setIsScheduled(false);
	}
}

void CCTableView::ccTouchCancelled(CCTouch *touch, CCEvent* event)
{
	CCScrollLayerExt::ccTouchCancelled(touch,event);
	this->touchFinish(touch);
	
	if(this->getIsScheduled())
	{
		printf("did end decelerating ..\n");
		this->unschedule(schedule_selector(CCTableView::checkBoundaryOfContent));
		this->setIsScheduled(false);
	}
}

void CCTableView::ccTouchMoved(CCTouch* touch, CCEvent* event)
{
	CCScrollLayerExt::ccTouchMoved(touch,event);
	this->setIsTouch(true);
	CCPoint touchLocation = touch->getLocation();
    
	if (touch == beginTouch) {
		CCPoint delta = ccpSub(touchLocation, checkLocation);
		checkLocation = touchLocation;
	}
}

void CCTableView::touchFinish(CCTouch * touch)
{
	CCPoint touchLocation = touch->getLocation();
    
	if (touch == beginTouch) {
		CCPoint distance = ccpSub(beginLocation, touchLocation);
		if (fabsf(distance.x) > 64 && fabsf(distance.y) < 16 && editable) 
		{//swap gesture
			touchCell = this->cellForTouch(touch);
			if (touchCell != NULL) {
				bool isShow = touchCell->isDeleteButtonShow();
				touchCell->showDeleteButton(!isShow);
			}
		}
		else {// not swap gesture,it's just tap
			touchCell = this->cellForTouch(touch);
			if (touchCell != NULL && delegate && !this->getIsScrolling()) {
				delegate->didSelectRowAtIndexPath(touchCell->indexPath, this);
			}
		}
	}

	beginLocation = checkLocation = CCPointZero;
	this->setIsTouch(false);
}

BoundaryType CCTableView::checkBoundaryOfCell(CCTableViewCell * mCell)
{
	CCPoint itemPosition = mCell->getPosition();
	float upY	= itemPosition.y + mCell->getContentSize().height - contentLayer->getContentSize().height + contentLayer->getPosition().y;
	float downY = itemPosition.y - contentLayer->getContentSize().height + contentLayer->getPosition().y;
	
	if ((upY <=this->getContentSize().height && upY >= 0) ||
		(downY <= this->getContentSize().height && downY >= 0)) {
		return BOUNDARY_IN;
	}
	
	return BOUNDARY_OUT;
}

/*check boundary of cell*/
BoundaryType CCTableView::checkBoundaryOfCell(CCPoint &mPosition,float mCellHeight)
{
    CCPoint itemPosition = mPosition;
	float upY	= itemPosition.y + mCellHeight - contentLayer->getContentSize().height + contentLayer->getPosition().y;
	float downY = itemPosition.y - contentLayer->getContentSize().height + contentLayer->getPosition().y;
	
	if ((upY <=this->getContentSize().height && upY >= 0) ||
		(downY <= this->getContentSize().height && downY >= 0)) {
		return BOUNDARY_IN;
	}
	
	return BOUNDARY_OUT;
}

void CCTableView::checkBoundaryOfContent(float mTime)
{
	//printf("xxxx removed count =%d \n",this->cellRemovedArray->count());
	int cellVisibleCount = cellVisibleArray->count();
	if (cellVisibleArray && cellVisibleCount > 0)
	{
		if(cellVisibleCount >= 1)
		{
			CCTableViewCell * firstCell =(CCTableViewCell *) this->cellVisibleArray->objectAtIndex(0);
			CCTableViewCell * lastCell	=(CCTableViewCell *) this->cellVisibleArray->objectAtIndex(cellVisibleCount-1);

			if (cellVisibleCount == 1) {
				int allRowCountInSection = dataSource->numberOfRowsInSection(firstCell->indexPath.section, this);
				CCIndexPath tmpFirstIndex(0,0);
				CCIndexPath tmpLastIndex(0,allRowCountInSection -1);
				if (firstCell->indexPath == tmpFirstIndex) // 从完全消失后,第一次加载是第一个  
				{
					this->checkLastCell(lastCell);
				}
				else if(lastCell->indexPath == tmpLastIndex)// 从完全消失后,第一次加载是最后一个  
				{
					this->checkFirstCell(firstCell);
				}
			}
			else {// 当前超过2个以上,不必特殊加载  
				this->checkFirstCell(firstCell);
				this->checkLastCell(lastCell);
			}
		}
	}
	else {
		int allRowCountInSection = dataSource->numberOfRowsInSection(0, this);
		if (allRowCountInSection == 0) 
		{
			return;
		}
		
		CCPoint contentPos = contentLayer->getPosition();
        //printf("...posy=%f,height=%f \n",contentPos.y,contentLayer->getContentSize().height );
		if (contentPos.y - contentLayer->getContentSize().height >= 0) // 加载最后一个  
		{
           // printf("last ...posy=%f,height=%f \n",contentPos.y,contentLayer->getContentSize().height );
			CCIndexPath * lastIndexPath = CCIndexPath::CCIndexPathWithSectionRow(0,allRowCountInSection -1);
			if (lastIndexPath->row < allRowCountInSection && this->isDuplicateInVisibleCellArray(lastIndexPath) == NULL)
			{
				float cellHeight = delegate->cellHeightForRowAtIndexPath(*lastIndexPath, this);
				CCPoint tmpPos = this->getLastCellPos();
                
                BoundaryType boundary = this->checkBoundaryOfCell(tmpPos,cellHeight);
                if (boundary ==  BOUNDARY_OUT) {
                    return;
                }
                
                CCTableViewCell * cell = dataSource->cellForRowAtIndexPath(*lastIndexPath, this);
				if (cell != NULL) {
					cell->setAnchorPoint(ccp(0.5,0.5));
					cell->setContentSize(CCSizeMake(this->getContentSize().width,cellHeight));
                    cell->setPosition(this->getLastCellPos());
					cell->indexPath = *lastIndexPath;
					cell->resetDeleteButtonPosition();
					cell->setEditable(this->getEditable());
					
					if (allRowCountInSection == 1) {
                       this->cellVisibleArray->insertObject(cell,0);
                    }
					else 
                        this->cellVisibleArray->addObject(cell);
					
					if (this->isDuplicateIndexPath(*lastIndexPath) == false) {
						if (allRowCountInSection == 1) {
                            this->indexPathAddedArray->insertObject(lastIndexPath,0);
                        }
                        else
                            this->indexPathAddedArray->addObject(lastIndexPath);
					}
					
					if (delegate)
						delegate->ccTableViewWillDisplayCellForRowAtIndexPath(*lastIndexPath, cell, this);
					
					contentLayer->addChild(cell);
					
					if (delegate)
						delegate->ccTableViewDidDisplayCellForRowAtIndexPath(*lastIndexPath, cell, this);
				}
			}
		}
		else if (contentPos.y <= this->getContentSize().height)// 加载第一个  
		{
            printf("first ...\n");
			CCIndexPath * aheadIndexPath = CCIndexPath::CCIndexPathWithSectionRow(0,0);
			if (aheadIndexPath->row >=0 && aheadIndexPath->row < allRowCountInSection 
				&& this->isDuplicateInVisibleCellArray(aheadIndexPath) == NULL)
			{
				//printf("cell (%d,%d)\n",aheadIndexPath->section,aheadIndexPath->row);
				float cellHeight = delegate->cellHeightForRowAtIndexPath(*aheadIndexPath, this);
                CCPoint tmpPos = this->getLastCellPos();
                
                BoundaryType boundary = this->checkBoundaryOfCell(tmpPos,cellHeight);
                if (boundary ==  BOUNDARY_OUT) {
                    return;
                }
                
                CCTableViewCell * cell = dataSource->cellForRowAtIndexPath(*aheadIndexPath, this);
				if (cell != NULL) {
					cell->setAnchorPoint(ccp(0.5,0.5));
					cell->setContentSize(CCSizeMake(this->getContentSize().width,cellHeight));
					cell->setPosition(ccp(0,contentLayer->getContentSize().height - cellHeight));
					cell->indexPath = *aheadIndexPath;
					cell->resetDeleteButtonPosition();
					cell->setEditable(this->getEditable());
					//printf("add (%d,%d)\n",aheadIndexPath->section,aheadIndexPath->row);
					this->cellVisibleArray->insertObject(cell,0);
					
					if (this->isDuplicateIndexPath(*aheadIndexPath) == false) {
						this->indexPathAddedArray->insertObject(aheadIndexPath,0);
					}
					
					if (delegate)
						delegate->ccTableViewWillDisplayCellForRowAtIndexPath(*aheadIndexPath, cell, this);
					
					contentLayer->addChild(cell);
					
					if (delegate)
						delegate->ccTableViewDidDisplayCellForRowAtIndexPath(*aheadIndexPath, cell, this);
				}
			}
		}
	}
}


void CCTableView::checkFirstCell(CCTableViewCell * firstCell)
{
	
	BoundaryType firstType = this->checkBoundaryOfCell(firstCell);
	if (firstType == BOUNDARY_OUT) {
		
		/*
		CCTableViewCell * cell = firstCell ;
		this->cellRemovedArray->addObject(cell);
        this->setLastCellPos(cell->getPosition());
        this->removeIndexPathFromPathAddedArray(cell->indexPath);
		cell->removeFromParentAndCleanup(true);
		this->cellVisibleArray->removeObject(cell);
		 */
	}
	else {
		CCPoint itemPosition = firstCell->getPosition();
		float upY	= itemPosition.y + firstCell->getContentSize().height - contentLayer->getContentSize().height + contentLayer->getPosition().y;
		if (upY < this->getContentSize().height) 
		{
			//printf("upy (%f)\n",upY);
			CCIndexPath * aheadIndexPath = CCIndexPath::CCIndexPathWithSectionRow(firstCell->indexPath.section,firstCell->indexPath.row -1);
			int allRowCountInSection = dataSource->numberOfRowsInSection(firstCell->indexPath.section, this);
			if (aheadIndexPath->row >=0 && aheadIndexPath->row < allRowCountInSection 
				&& this->isDuplicateInVisibleCellArray(aheadIndexPath) == NULL)
			{
				printf("cell (%d,%d)\n",aheadIndexPath->section,aheadIndexPath->row);
				float cellHeight = delegate->cellHeightForRowAtIndexPath(*aheadIndexPath, this);
				CCTableViewCell * cell = dataSource->cellForRowAtIndexPath(*aheadIndexPath, this);
				if (cell != NULL) {
					cell->setAnchorPoint(ccp(0.5,0.5));
					cell->setContentSize(CCSizeMake(this->getContentSize().width,cellHeight));
					cell->setPosition(ccp(0,firstCell->getPosition().y + firstCell->getContentSize().height));
					cell->indexPath = *aheadIndexPath;
					cell->resetDeleteButtonPosition();
					cell->setEditable(this->getEditable());
					//printf("add (%d,%d)\n",aheadIndexPath->section,aheadIndexPath->row);
					this->cellVisibleArray->insertObject(cell,0);
					
					if (this->isDuplicateIndexPath(*aheadIndexPath) == false) {
						this->indexPathAddedArray->insertObject(aheadIndexPath,0);
					}
					
					if (delegate)
						delegate->ccTableViewWillDisplayCellForRowAtIndexPath(*aheadIndexPath, cell, this);
					
					contentLayer->addChild(cell);
					
					if (delegate)
						delegate->ccTableViewDidDisplayCellForRowAtIndexPath(*aheadIndexPath, cell, this);
				}
			}
		}
	}
}

void CCTableView::checkLastCell(CCTableViewCell * lastCell)
{
	BoundaryType lastType  = this->checkBoundaryOfCell(lastCell);
	// 判断最后一个  
	if (lastType == BOUNDARY_OUT) {
		/*
		CCTableViewCell * cell = lastCell ;
		this->cellRemovedArray->addObject(cell);
        this->setLastCellPos(cell->getPosition());
        printf("checkLastCell set pos =(%f,%f) indexpath(0,%d) \n",lastCellPos.x,lastCellPos.y,cell->indexPath.row);
        this->removeIndexPathFromPathAddedArray(cell->indexPath);
		cell->removeFromParentAndCleanup(true);
		this->cellVisibleArray->removeObject(cell);
		 */
	}
	else {
		CCPoint lastItemPos = lastCell->getPosition();
		//printf("content h =%f,y=%f \n",contentLayer->getContentSize().height , contentLayer->getPosition().y);
		float downY = lastItemPos.y -contentLayer->getContentSize().height + contentLayer->getPosition().y;
		if (downY > 0) 
		{
			CCIndexPath * lastIndexPath = CCIndexPath::CCIndexPathWithSectionRow(lastCell->indexPath.section,lastCell->indexPath.row +1);
			int allRowCountInSection = dataSource->numberOfRowsInSection(lastCell->indexPath.section, this);
			//printf("last index (%d,%d)\n",lastIndexPath->section,lastIndexPath->row);
			
			if (lastIndexPath->row < allRowCountInSection && this->isDuplicateInVisibleCellArray(lastIndexPath) == NULL)
			{
				float cellHeight = delegate->cellHeightForRowAtIndexPath(*lastIndexPath, this);
				CCTableViewCell * cell = dataSource->cellForRowAtIndexPath(*lastIndexPath, this);
				/////////////////extent content layer's size//////////////
				CCPoint itemPosition = ccp(0,lastCell->getPosition().y - cellHeight);
				float deltaY = itemPosition.y;
				CCTableViewCell * preCellItem;
				if ( deltaY < 0 && this->isDuplicateIndexPath(*lastIndexPath) == false) { // content size is not enough
					//printf(" ==>delta Y =%f \n",deltaY);
					CCSize contentSize = contentLayer->getContentSize();
					contentLayer->setContentSize(CCSizeMake(contentSize.width,contentSize.height - deltaY));
					contentSize = contentLayer->getContentSize();
					//printf("set content size(%f,%f)\n",contentSize.width,contentSize.height);
					for (int m= cellVisibleArray->count()-1; m >=0 ; m--) {
						CCTableViewCell * cellItem = (CCTableViewCell *) cellVisibleArray->objectAtIndex(m);
						CCPoint tmpPos;
						if (m == cellVisibleArray->count()-1) {
							tmpPos = ccp(cellItem->getPosition().x,lastCell->getPosition().y - deltaY);
							itemPosition = ccp(cellItem->getPosition().x, tmpPos.y - cellHeight);
						}
						else {
							tmpPos = ccp(cellItem->getPosition().x,preCellItem->getPosition().y + preCellItem->getContentSize().height);
						}
						cellItem->setPosition(tmpPos);
						preCellItem = cellItem;
					}
					//contentLayer->setPosition(ccpSub(contentLayer->getPosition(), ccp(0,deltaY)));// content layer 往下移动  
				}
				///////////////////////////////////////////////////////////////////////
				
				if (cell != NULL) {
                    
					cell->setAnchorPoint(ccp(0.5,0.5));
					cell->setContentSize(CCSizeMake(this->getContentSize().width,cellHeight));
					cell->setPosition(itemPosition);
					cell->indexPath = *lastIndexPath;
					cell->resetDeleteButtonPosition();
					cell->setEditable(this->getEditable());
					
					this->cellVisibleArray->addObject(cell);
					
					if (this->isDuplicateIndexPath(*lastIndexPath) == false) {
						this->indexPathAddedArray->addObject(lastIndexPath);
					}
					
					if (delegate)
						delegate->ccTableViewWillDisplayCellForRowAtIndexPath(*lastIndexPath, cell, this);
					
					contentLayer->addChild(cell);
					
					if (delegate)
						delegate->ccTableViewDidDisplayCellForRowAtIndexPath(*lastIndexPath, cell, this);
				}
			}
		}
	}
}

// CCScrollViewExt delegate  
void CCTableView::scrllViewWillBeginDecelerating(CCScrollLayerExt * mScrollView)
{
//	if(!this->getIsScheduled())
//	{
//		printf("start decelerating ..\n");
//		this->schedule(schedule_selector(CCTableView::checkBoundaryOfContent));
//		this->setIsScheduled(true);
//	}
}

void CCTableView::scrollViewDidEndDecelerating(CCScrollLayerExt * mScrollView)
{
//	if(this->getIsScheduled())
//	{
//		printf("did end decelerating ..\n");
//		this->unschedule(schedule_selector(CCTableView::checkBoundaryOfContent));
//		this->setIsScheduled(false);
//	}
}

void CCTableView::scrollViewTouchMoving(CCScrollLayerExt * mScrollView)
{
//	printf("moving end decelerating ..\n");
//	this->checkBoundaryOfContent();
}

void CCTableView::scrollViewDidEndMoving(CCScrollLayerExt * mScrollView)
{
//	if(this->getIsScheduled())
//	{
//		printf("scrollViewDidEndMoving end decelerating ..\n");
//		this->unschedule(schedule_selector(CCTableView::checkBoundaryOfContent));
//		this->setIsScheduled(false);
//	}
}
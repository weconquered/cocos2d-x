#ifndef __CCSCROLLVIEW_H__
#define __CCSCROLLVIEW_H__

#include "ccTypes.h"
#include "CCObject.h"
#include "CCZone.h"
#include "CCView.h"

namespace   cocos2d {

class CC_DLL CCScrollView : public CCView
{
private:
	bool dragSquids;
	bool firstDragging;
	bool panChanged;
	CCPoint backup;
	CCPoint desiredOffSet;
	bool right;
	bool forcedScroll;
	bool m_scrolling;
public:
    CCScrollView(void);
	virtual ~CCScrollView(void);
	
	CC_PROPERTY(CCScrollView*,m_delegate,Delegate);
	CC_PROPERTY(CCPoint,m_contentOffset,ContentOffset)
	CC_SYNTHESIZE(bool, m_showsVerticalScrollIndicator, ShowsVerticalScrollIndicator)
	CC_SYNTHESIZE(bool, m_showsHorizontalScrollIndicator, ShowsHorizontalScrollIndicator)
	CC_SYNTHESIZE(bool, m_bounces, Bounces)
	CC_SYNTHESIZE(bool, m_pagingEnabled, PagingEnabled)
	CC_SYNTHESIZE(bool, m_scrollsToTop, ScrollsToTop)
	CC_SYNTHESIZE(bool, m_delaysContentTouches, DelaysContentTouches)
	CC_SYNTHESIZE(bool, m_canCancelContentTouches, CanCancelContentTouches)
	CC_SYNTHESIZE(bool, m_dragging, Dragging)
	CC_SYNTHESIZE(bool, m_decelerating, Decelerating)
	CC_SYNTHESIZE(int,m_numPages,NumPages)
	CC_SYNTHESIZE(float,m_width,Width)
	CC_SYNTHESIZE(CCPoint,m_velocity,Velocity);

	void setContentOffset(CCPoint pt, bool _value);
	virtual void scrollViewWillBeginDecelerating(CCScrollView * scrollView);
	virtual void scrollViewDidScroll(CCScrollView *scrollView, bool _value = false);
	virtual void scrollViewWillBeginDragging(CCScrollView *scrollView);
	virtual void scrollViewDidEndDecelerating (CCScrollView *scrollView);
	virtual void scrollViewDidEndDragging(CCScrollView *scrollView, bool decelerate);

	virtual void onPan(CCGestureRecognizer* recognizer, CCNode* node);
	virtual void onEnter();
	virtual void onExit();
	virtual void update(ccTime dt);
	inline void Set_dragSquids(bool _value){dragSquids = _value;}
	inline bool CheckToScroll(){if (desiredOffSet.x == 0.0f && !m_scrolling){return true;}else{return false;}}
	inline void Set_forcedScroll(bool _value){forcedScroll = _value;}
	inline void resetContentOffSet(){m_contentOffset.x = 0.0f; m_contentOffset.y = 0.0f;}

#ifdef WIN32
	void dragWin32(float _offset);
#endif
};
}//namespace   cocos2d 

#endif // __CCSCROLLVIEW_H__

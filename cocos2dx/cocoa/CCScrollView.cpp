/****************************************************************************
Copyright (c) 2010-2011 cocos2d-x.org
Copyright (c) 2011 Stephane Capo, Nicolas Blondel, Quentin Stere, Noel Leron

http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/

#include "CCScrollView.h"
#include "CCTouchDispatcher.h"
#include "CCScheduler.h"
#include "CCGestureRecognizer.h"
#include "CCDirector.h"

namespace   cocos2d {

CCScrollView::CCScrollView() : CCView(),
m_delegate(0),
m_showsVerticalScrollIndicator(false),
m_showsHorizontalScrollIndicator(false),
m_bounces(true),
m_pagingEnabled(true),
m_scrollsToTop(true),
m_delaysContentTouches(false),
m_canCancelContentTouches(false),
m_dragging(false),
m_decelerating(false)
{
	m_contentOffset.x = 0;
	m_contentOffset.y = 0;
	dragSquids = false;
	firstDragging = true;;
	panChanged = false;
	backup.x = 0;
	backup.y = 0;
	right = false;
	forcedScroll = false;
	m_scrolling = false;
}

CCScrollView::~CCScrollView()
{
}


void CCScrollView::onEnter()
{
	// create a CCGesture recognizer for pan
	CCGestureRecognizer* recognizer = CCGestureRecognizer::CCRecognizerWithRecognizerTargetAction(new CCPanGestureRecognizer(), this,callGesture_selector(CCScrollView::onPan));
	recognizer->autorelease();
	addGestureRecognizer(recognizer);	
	recognizer->setDelegate(this);
	CCTouchDispatcher::sharedDispatcher()->addStandardDelegate(this, 0);
	CCView::onEnter();
	m_dragging=false;
	m_decelerating=false;
	
}

void CCScrollView::onExit()
{
	if(m_decelerating)
	{
		CCScheduler::sharedScheduler()->unscheduleSelector(schedule_selector(CCScrollView::update),this);
	}
	CCTouchDispatcher::sharedDispatcher()->removeDelegate(this);
	m_contentOffset.x = 0;
	m_contentOffset.y = 0;

	CCView::onExit();
}

void CCScrollView::update(ccTime dt)
{
	if(!m_dragging)
	{
		if(m_decelerating)
		{
			m_contentOffset.x-=m_velocity.x*dt;
			m_contentOffset.y-=m_velocity.y*dt;

			if(dt>1.0f)
			{
				dt=1.0f;
			}

			m_velocity.x*=0.96f*(1.0f-dt);
			m_velocity.y*=0.96f*(1.0f-dt);

			m_bounds.origin.x=m_contentOffset.x;
			m_bounds.origin.y=m_contentOffset.y;
			
			//setPosition(m_bounds.origin);

			if((fabsf(m_velocity.x)<0.01)&&(fabsf(m_velocity.y)<0.01))
			{
				m_velocity=CCPointZero;
				m_decelerating=false;
				CCScheduler::sharedScheduler()->unscheduleSelector(schedule_selector(CCScrollView::update),this);
				m_delegate->scrollViewDidEndDecelerating(this);
			}

			if ( m_contentOffset.x < 0.0f )
			{
				m_contentOffset.x = 0.0f;
				m_velocity=CCPointZero;
				m_decelerating=false;
				CCScheduler::sharedScheduler()->unscheduleSelector(schedule_selector(CCScrollView::update),this);
				m_delegate->scrollViewDidEndDecelerating(this);
			}
			else if( m_contentOffset.x > this->getContentSize().width )
			{
				m_contentOffset.x = this->getContentSize().width;
				m_velocity=CCPointZero;
				m_decelerating=false;
				CCScheduler::sharedScheduler()->unscheduleSelector(schedule_selector(CCScrollView::update),this);
				m_delegate->scrollViewDidEndDecelerating(this);
			}

			m_delegate->scrollViewDidScroll(this);
		}
		else
		{
#ifdef WIN32
			m_contentOffset.x-=m_velocity.x*dt;
			m_contentOffset.y-=m_velocity.y*dt;

			if ( m_contentOffset.x == 0.0f )
			{
				int i =0;
			}
			if(dt>1.0f)
			{
				dt=1.0f;
			}

			m_bounds.origin.x=m_contentOffset.x;
			m_bounds.origin.y=m_contentOffset.y;
			if ( m_scrolling && right && (desiredOffSet.x - m_contentOffset.x) <= 0.0f )
			{
				m_velocity=CCPointZero;
				m_decelerating=false;
				m_contentOffset.x = desiredOffSet.x;

				desiredOffSet.x = 0.0f;
				CCScheduler::sharedScheduler()->unscheduleSelector(schedule_selector(CCScrollView::update),this);
				backup.x = m_contentOffset.x;
				backup.y = m_contentOffset.y;

				m_delegate->scrollViewDidEndDecelerating(this);
				m_scrolling = false;
			}
			else if (m_scrolling && !right && (desiredOffSet.x - m_contentOffset.x) >= 0.0f )
			{
				m_velocity=CCPointZero;
				m_decelerating=false;
				m_contentOffset.x = desiredOffSet.x;

				desiredOffSet.x = 0.0f;

				backup.x = m_contentOffset.x;
				backup.y = m_contentOffset.y;

				CCScheduler::sharedScheduler()->unscheduleSelector(schedule_selector(CCScrollView::update),this);
				m_delegate->scrollViewDidEndDecelerating(this);
				m_scrolling = false;
			}
			m_delegate->scrollViewDidScroll(this, true);
#endif
#ifdef ASSORIA
			//LOGERR("X = %f // Y = %f\n", m_contentOffset.x , m_contentOffset.y);
			if (!m_dragging)
			{
				if ( m_contentOffset.x < 0.0f )
				{
					m_contentOffset.x = 0.0f;
					m_velocity=CCPointZero;
					m_decelerating=false;
					//CCScheduler::sharedScheduler()->unscheduleSelector(schedule_selector(CCScrollView::update),this);
					//m_delegate->scrollViewDidEndDecelerating(this);
				}
				else if( m_contentOffset.x > this->getContentSize().width )
				{
					m_contentOffset.x = this->getContentSize().width;
					m_velocity=CCPointZero;
					m_decelerating=false;
					//CCScheduler::sharedScheduler()->unscheduleSelector(schedule_selector(CCScrollView::update),this);
					//m_delegate->scrollViewDidEndDecelerating(this);
				}
			}
#endif
		}
	}
#ifndef WIN32
#ifdef ASSORIA
	else
	{
		//LOGERR("X = %f // Y = %f\n", m_contentOffset.x , m_contentOffset.y);
		if ( m_contentOffset.x < 0.0f )
		{
			m_contentOffset.x = 0.0f;
			m_velocity=CCPointZero;
			m_decelerating=false;
			//CCScheduler::sharedScheduler()->unscheduleSelector(schedule_selector(CCScrollView::update),this);
			//m_delegate->scrollViewDidEndDecelerating(this);
		}
		else if( m_contentOffset.x > this->getContentSize().width )
		{
			m_contentOffset.x = this->getContentSize().width;
			m_velocity=CCPointZero;
			m_decelerating=false;
			//CCScheduler::sharedScheduler()->unscheduleSelector(schedule_selector(CCScrollView::update),this);
			//m_delegate->scrollViewDidEndDecelerating(this);
		}
	}
#endif
#endif
}

void CCScrollView::setDelegate(CCScrollView* _pt)
{
	m_delegate = _pt;
}

CCScrollView* CCScrollView::getDelegate()
{
	return m_delegate;
}

void CCScrollView::scrollViewWillBeginDecelerating(CCScrollView * scrollView)
{
	//TODO
}
void CCScrollView::scrollViewDidScroll(CCScrollView *scrollView, bool _value)
{
	//TODO
}
void CCScrollView::scrollViewWillBeginDragging(CCScrollView *scrollView)
{
	//TODO
}
void CCScrollView::scrollViewDidEndDecelerating (CCScrollView *scrollView)
{
	//TODO
}
void CCScrollView::scrollViewDidEndDragging(CCScrollView *scrollView, bool decelerate)
{
	//TODO
}

void CCScrollView::onPan(CCGestureRecognizer* recognizer, CCNode* node)
{
#ifdef WIN32
	if(dragSquids)
	{
#endif
		CCPanGestureRecognizer* currentPan=(CCPanGestureRecognizer*)recognizer;
		CCDirector* director = CCDirector::sharedDirector(); 

		switch(currentPan->state)
		{
			case CCGestureRecognizerStateBegan:
			{
#ifndef WIN32
				CCScheduler::sharedScheduler()->scheduleSelector(schedule_selector(CCScrollView::update), this, 0, false);
#endif
				if(m_decelerating)
				{
					CCScheduler::sharedScheduler()->unscheduleSelector(schedule_selector(CCScrollView::update),this);
					m_decelerating=false;
					m_velocity=CCPointZero;
				}
				m_delegate->scrollViewWillBeginDragging(this);
				m_dragging=true;
				break;
			}
			case CCGestureRecognizerStateChanged:
			{
				CCPoint delta = currentPan->getTranslationInView(0); 
				//delta = director->convertVectorToGL(delta);
				
				currentPan->setTranslationInView(0,CCPointZero);
				
				m_velocity = currentPan->getVelocityInView(0); 
				//m_velocity = director->convertVectorToGL(m_velocity);
#ifdef WIN32
				panChanged = true;
#endif
				m_contentOffset.x-=delta.x;
				m_contentOffset.y-=delta.y;
				
				m_bounds.origin.x=m_contentOffset.x;
				m_bounds.origin.y=m_contentOffset.y;

				//setPosition(m_bounds.origin);
#ifndef WIN32
				CCScheduler::sharedScheduler()->scheduleSelector(schedule_selector(CCScrollView::update), this, 0, false);
#endif
				m_delegate->scrollViewDidScroll(this);

				break;
			}
			case CCGestureRecognizerStateEnded:
			case CCGestureRecognizerStateCancelled:
			{
#ifndef WIN32
				CCScheduler::sharedScheduler()->scheduleSelector(schedule_selector(CCScrollView::update), this, 0, false);
#endif
				m_velocity = currentPan->getVelocityInView(0); 
				m_dragging=false;
				m_decelerating=false;
				if(m_velocity.x!=0.0f)
				{
					m_decelerating=true;
				}
				else if(m_velocity.y!=0.0f)
				{
					m_decelerating=true;
				}
				if(m_decelerating)
				{
					CCScheduler::sharedScheduler()->scheduleSelector(schedule_selector(CCScrollView::update), this, 0, false);
				}
				m_delegate->scrollViewDidEndDragging(this,m_decelerating);
				if(m_decelerating)
				{
					m_delegate->scrollViewWillBeginDecelerating(this);
				}
				else
				{
					m_delegate->scrollViewDidEndDecelerating(this);
				}

				break;
			}
		}
#ifdef WIN32
	}
#endif
}

#ifdef WIN32
void CCScrollView::dragWin32(float _offset)
{
	if (desiredOffSet.x == 0.0f && !m_scrolling)
	{
		m_scrolling = true;
		if ( _offset < 0 )
		{
			right = true;
		}
		else
		{
			right = false;
		}
		if (firstDragging && panChanged)
		{
			m_contentOffset.x = 0;
			m_contentOffset.y = 0;
			firstDragging = false;
		}
		else if (panChanged)
		{
			m_contentOffset.x = backup.x;
			m_contentOffset.y = backup.y;
		}

		m_dragging = true;

		this->desiredOffSet.x += m_contentOffset.x + (_offset*-1);
		this->desiredOffSet.y = 0.0f;

		CCPoint delta (0.0f,0.0f); 
		
		m_velocity.x += _offset * 4.3f;
		m_velocity.y = 0.0f;

		m_contentOffset.x-=delta.x;
		m_contentOffset.y-=delta.y;

		m_bounds.origin.x=m_contentOffset.x;
		m_bounds.origin.y=m_contentOffset.y;

		m_delegate->scrollViewDidScroll(this, true);
		
		backup.x = m_contentOffset.x;
		backup.y = m_contentOffset.y;
		panChanged = false;
		m_dragging = false;
		firstDragging = false;
		m_decelerating = false;
		CCScheduler::sharedScheduler()->scheduleSelector(schedule_selector(CCScrollView::update), this, 0, false);
	}
}

#endif

CCPoint	CCScrollView::getContentOffset()
{
	return m_contentOffset;
}

void	CCScrollView::setContentOffset(CCPoint pt)
{
	m_contentOffset=pt;
	m_bounds.origin.x=m_contentOffset.x;
	m_bounds.origin.y=m_contentOffset.y;
	//setPosition(m_bounds.origin);

	if(m_delegate)
		m_delegate->scrollViewDidScroll(this);

}

void	CCScrollView::setContentOffset(CCPoint pt, bool animated)
{
	m_contentOffset=pt;

	m_bounds.origin.x=m_contentOffset.x;
	m_bounds.origin.y=m_contentOffset.y;
	//setPosition(m_bounds.origin);
	if(m_delegate)
		m_delegate->scrollViewDidScroll(this, forcedScroll);

	if (forcedScroll)
		forcedScroll = false;
}


} // namespace
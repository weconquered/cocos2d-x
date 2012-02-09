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

#include "CCView.h"
#include "CCTouchDispatcher.h"
#include "CCDirector.h"
#include "CCGestureRecognizer.h"

namespace   cocos2d {

int		CCView::myViewIDCount=1;

CCView::CCView() : CCLayer()
{
	myViewID=myViewIDCount++;
	
}

CCView::~CCView()
{
	
}

void	CCView::initWithFrame(const CCRect& aRect)
{
	m_frame=aRect;
	m_bounds.origin=CCPointZero;
	m_bounds.size=m_frame.size;
	m_center.x=m_frame.origin.x+m_bounds.size.width*0.5f;
	m_center.y=m_frame.origin.y+m_bounds.size.height*0.5f;
	setContentSize(m_frame.size);
	setPosition(m_bounds.origin);
}

void	CCView::removeFromSuperview()
{
	CCDirector::sharedDirector()->removeSubView(this);
}

CCRect	CCView::getFrame()
{
	return m_frame;
}
CCRect	CCView::getBounds()
{
	return m_bounds;
}

CCPoint	CCView::getCenter()
{
	return m_center;
}



#ifdef ASSORIA
CCAffineTransform*	CCView::getTransform()
{
	return m_transform;
}
#endif

float	CCView::getAlpha()
{
	return m_alpha;
}

ccColor3B	CCView::getBackgroundColor()
{
	return m_backgroundColor;
}

CCRect	CCView::getContentStretch()
{
	return m_contentStretch;
}

void	CCView::setFrame(CCRect rect)
{
	// TODO
	m_frame=rect;
}
void	CCView::setBounds(CCRect rect)
{
	// TODO
	m_bounds=rect;
}

void	CCView::setCenter(CCPoint pt)
{
	// TODO
	m_center=pt;
}

#ifdef ASSORIA
void	CCView::setTransform(CCAffineTransform* tr)
{
	// TODO
	m_transform=tr;
}
#endif
void	CCView::setAlpha(float a)
{
	// TODO
	m_alpha=a;
}

void	CCView::setBackgroundColor(ccColor3B	c)
{
	// TODO
	m_backgroundColor=c;
}

void	CCView::setContentStretch(CCRect rect)
{
	// TODO
	m_contentStretch=rect;
}

}
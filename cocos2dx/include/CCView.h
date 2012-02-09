#ifndef __CCVIEW_H__
#define __CCVIEW_H__

#include "ccTypes.h"
#include "CCObject.h"
#include "CCZone.h"
#include "CCLayer.h"

namespace   cocos2d {

class CC_DLL CCView : public CCLayer
{
public:
    CCView(void);
	virtual ~CCView(void);

	virtual void	initWithFrame(const CCRect& aRect);
	virtual void	removeFromSuperview();

	inline CCPoint	getLocalPos(const CCPoint& pos)
	{
		CCPoint result(pos);
		result.x-=m_bounds.origin.x;
		result.y-=m_bounds.origin.y;
		return result;
	}

	CC_PROPERTY(CCRect,m_frame,Frame)
	CC_PROPERTY(CCRect,m_bounds,Bounds)
	CC_PROPERTY(CCPoint,m_center,Center)

#ifdef ASSORIA
	CC_PROPERTY(CCAffineTransform*,m_transform,Transform)
#endif
	CC_PROPERTY(float,m_alpha,Alpha)
	CC_PROPERTY(ccColor3B,m_backgroundColor,BackgroundColor)
	CC_PROPERTY(CCRect,m_contentStretch,ContentStretch)

	int		getViewID()
	{
		return myViewID;
	}

protected:
	int				myViewID;
	static	int		myViewIDCount;

};
}//namespace   cocos2d 

#endif // __CCVIEW_H__

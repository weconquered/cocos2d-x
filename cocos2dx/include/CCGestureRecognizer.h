#ifndef __CCGESTURERECOGNIZER_H__
#define __CCGESTURERECOGNIZER_H__

#include <map>
#include <vector>
#include <string>
#include "CCObject.h"
#include "platform.h"

namespace   cocos2d {

class CCNode;
class CCGestureRecognizer;
class CCTouchDelegate;

typedef void (CCObject::*SEL_Gesture)(CCGestureRecognizer*,CCNode*);
#define callGesture_selector(_SELECTOR) (SEL_Gesture)(&_SELECTOR)

typedef enum 
{
	CCGestureRecognizerStatePossible,
	CCGestureRecognizerStateBegan,
	CCGestureRecognizerStateChanged,
	CCGestureRecognizerStateEnded,
	CCGestureRecognizerStateCancelled,

	CCGestureRecognizerStateFailed,

	CCGestureRecognizerStateRecognized = CCGestureRecognizerStateEnded

} CCGestureRecognizerState;

// base class
class CC_DLL CCGestureRecognizer : public CCObject
{
public:

	CCGestureRecognizer();
	virtual ~CCGestureRecognizer();

	void	setDelegate(CCTouchDelegate* ldelegate);

	CCTouchDelegate*	getDelegate()
	{
		return mDelegate;
	}

	void	setTarget(CCObject* ltarget)
	{
		/*if(mTarget)
		{
			mTarget->release();
		}*/
		mTarget=ltarget;
		/*if(mTarget)
		{
			mTarget->retain();
		}*/
	}
	CCObject*	getTarget()
	{
		return mTarget;
	}


	void	setNode(CCNode* lnode)
	{
		// we can't retain the node, otherwise a node will never get destroyed since it contains a
		// ref to this.  if node gets unrefed it will destroy this so all should be good
		mNode = lnode;
		
	}
	CCNode*	getNode()
	{
		return mNode;
	}
	CCGestureRecognizer*	gestureRecognizer()
	{
		return this;
	}

	static void	timeUpdate();

	static CCGestureRecognizer* CCRecognizerWithRecognizerTargetAction(CCGestureRecognizer*,CCObject* target,SEL_Gesture);

	virtual void ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent)=0;
 	virtual void ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent)=0;
 	virtual void ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent)=0;
    virtual void ccTouchesCancelled(CCSet *pTouches, CCEvent *pEvent)=0;

	virtual bool gestureRecognizershouldReceiveTouch(CCGestureRecognizer* gesturerecognizer,CCTouch * touch);

	DECLARE_KINDOF(CCGestureRecognizer,CCObject)

	// direct access
	CCGestureRecognizerState	state;


protected:

	CCTouchDelegate*	mDelegate;
	CCObject*			mTarget;
	CCNode*				mNode;
	SEL_Gesture			mSelector;

	static	cc_timeval	mLastUpdateTime;
	static	float	mApplicationTime;

};

class CC_DLL CCTapGestureRecognizer : public CCGestureRecognizer
{
public:

	CCTapGestureRecognizer();
	virtual ~CCTapGestureRecognizer();

	unsigned int numberOfTapsRequired;
	virtual void ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent);
 	virtual void ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent);
 	virtual void ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent);
    virtual void ccTouchesCancelled(CCSet *pTouches, CCEvent *pEvent);
	// TODO
	//unsigned int numberOfTouchesRequired;
	DECLARE_KINDOF(CCTapGestureRecognizer,CCGestureRecognizer)

	const CCPoint& locationInView(int viewID)
	{
		return myTapPos;
	}
protected:

	class	DetectedTap
	{
	public:
		float		myStartTime;
		float		myEndTime;
		CCPoint		myStartLocation;
		CCTouch*	myTouch;
	};

	std::vector<DetectedTap>	myTapList;

	std::vector<DetectedTap>::iterator	findTouch(CCTouch* t);

	CCPoint myTapPos;
	void	checkTapDetected();

};

class CC_DLL CCPinchGestureRecognizer : public CCGestureRecognizer
{
public:
	CCPinchGestureRecognizer();
	virtual ~CCPinchGestureRecognizer();

	virtual void ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent);
 	virtual void ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent);
 	virtual void ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent);
    virtual void ccTouchesCancelled(CCSet *pTouches, CCEvent *pEvent);

	DECLARE_KINDOF(CCPinchGestureRecognizer,CCGestureRecognizer)

	void	setScale(float sc)
	{
		myScale=sc;
		myVelocity=0.0f;
	}
	float	getScale()
	{
		return myScale;
	}
	float	getVelocity()
	{
		return myVelocity;
	}
	const CCPoint& locationInView(int viewID)
	{
		return myMiddlePos;
	}
protected:

	void checkPinchState();

	class	DetectedTouch
	{
	public:
		float		myStartTime;
		float		myCurrentTime;
		CCPoint		myStartLocation;
		CCPoint		myCurrentLocation;
	};

	std::map<CCTouch*,DetectedTouch>	myTouchList;

	float	myScale;
	float	myLastScale;
	float	myVelocity;
	float	myLastTime;

	CCPoint			myStartVector;
	CCPoint			myMiddlePos;
	
};
/*
class CC_DLL CCRotationGestureRecognizer : public CCGestureRecognizer
{
public:
	// TODO
protected:
	
};*/
/*
class CC_DLL CCSwipeGestureRecognizer : public CCGestureRecognizer
{
public:
	// TODO
protected:
	
};*/


class CC_DLL CCPanGestureRecognizer : public CCGestureRecognizer
{
public:
	CCPanGestureRecognizer();
	virtual ~CCPanGestureRecognizer();

	virtual void ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent);
 	virtual void ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent);
 	virtual void ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent);
    virtual void ccTouchesCancelled(CCSet *pTouches, CCEvent *pEvent);

	DECLARE_KINDOF(CCPanGestureRecognizer,CCGestureRecognizer)

	unsigned int maximumNumberOfTouches;
	unsigned int minimumNumberOfTouches;

	CCPoint getVelocityInView(int viewid);
	CCPoint getTranslationInView(int viewid);
	void	setTranslationInView(int viewid,const CCPoint& pt);


protected:

	bool	GetAverageTranslation(CCPoint& tr);
	void	GetAverageVelocity(CCPoint& vl);

	class	DetectedTouch
	{
	public:
		float		myStartTime;
		float		myCurrentTime;
		CCPoint		myStartLocation;
		CCPoint		myCurrentLocation;
		CCPoint		myVelocity;
	};

	std::map<CCTouch*,DetectedTouch>	myTouchList;

	CCPoint myTapPos;
	void	checkPanState();

	CCPoint	myTranslation;
	CCPoint	myVelocity;
	
};
/*
class CC_DLL CCLongPressGestureRecognizer : public CCGestureRecognizer
{
public:
	// TODO
protected:
	
};
*/

}//namespace   cocos2d 
#endif //__CCGESTURERECOGNIZER_H__
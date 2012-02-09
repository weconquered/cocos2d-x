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

#include "CCGestureRecognizer.h"
#include "CCLayer.h"
#include "CCPointExtension.h"
#include "CCDirector.h"

namespace   cocos2d {

//#define TEST_PINCH

#define TAP_OUT_DISTANCE	(15)
#define TAP_MAX_DURATION	(1.5f)
#define TAP_MAX_DELAY		(0.8f)
#define PAN_MIN_START		(8.0f)

struct cc_timeval	CCGestureRecognizer::mLastUpdateTime;
float	CCGestureRecognizer::mApplicationTime=-1.0f;

// use real time to get clic ...
void	CCGestureRecognizer::timeUpdate()
{
	struct cc_timeval now;
	if (CCTime::gettimeofdayCocos2d(&now, NULL) != 0)
	{
		mApplicationTime=-1.0f;
	}

	if(mApplicationTime<0)
	{
		mLastUpdateTime=now;
		mApplicationTime=0.0f;
	}
	else
	{
		float DeltaTime = (now.tv_sec - mLastUpdateTime.tv_sec) + (now.tv_usec - mLastUpdateTime.tv_usec) / 1000000.0f;
		DeltaTime = MAX(0, DeltaTime);
		mApplicationTime+=DeltaTime;
		mLastUpdateTime=now;
	}
}

void	CCGestureRecognizer::setDelegate(CCTouchDelegate* ldelegate)
{
	/*if(mDelegate)
	{
		mDelegate->destroy();
	}*/
	mDelegate=ldelegate;
	/*if(mDelegate)
	{
		mDelegate->keep();
	}*/
}

CCGestureRecognizer::CCGestureRecognizer() : CCObject(),
mDelegate(0),
mNode(0),
mTarget(0),
state(CCGestureRecognizerStatePossible)
{
}

CCGestureRecognizer::~CCGestureRecognizer()
{
	setDelegate(0);
	setNode(0);
	setTarget(0);
}

CCTapGestureRecognizer::CCTapGestureRecognizer() : CCGestureRecognizer(),
numberOfTapsRequired(1)
{
	myTapList.clear();
}

CCTapGestureRecognizer::~CCTapGestureRecognizer() 
{
	myTapList.clear();
}

void CCTapGestureRecognizer::ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent)
{
	if(((CCLayer*)mDelegate)->gestureRecognizerShouldBegin(this))
	{
		CCSetIterator it;
		for(it=pTouches->begin();it!=pTouches->end();it++)
		{
			CCTouch *pTouch=(CCTouch *)(*it);
			if(gestureRecognizershouldReceiveTouch(this,pTouch))
			{
				DetectedTap	newtestedtap;
				newtestedtap.myStartTime=mApplicationTime;
				newtestedtap.myEndTime=-1.0f;
				newtestedtap.myTouch=pTouch;
				newtestedtap.myStartLocation=pTouch->locationInView(pTouch->view());
				myTapList.push_back(newtestedtap);
			}
		}
	}
}

std::vector<CCTapGestureRecognizer::DetectedTap>::iterator	CCTapGestureRecognizer::findTouch(CCTouch* t)
{
	std::vector<DetectedTap>::iterator	it=myTapList.begin();
	while(it!=myTapList.end())
	{
		if((*it).myTouch==t)
		{
			if((*it).myEndTime<.0f)
			{
				break;
			}
		}
		++it;
	}
	return it;
}

void CCTapGestureRecognizer::ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent)
{
	CCSetIterator it;
	for(it=pTouches->begin();it!=pTouches->end();it++)
	{
		CCTouch *pTouch=(CCTouch *)(*it);
		std::vector<DetectedTap>::iterator	itfound=findTouch(pTouch);

		if(itfound != myTapList.end())
		{
			CCPoint currentpos=pTouch->locationInView(pTouch->view());

			DetectedTap&	testedtap=(*itfound);
			if(abs(testedtap.myStartLocation.x-currentpos.x)>TAP_OUT_DISTANCE)
			{
				myTapList.erase(itfound);
				
			}
			else if(abs(testedtap.myStartLocation.y-currentpos.y)>TAP_OUT_DISTANCE)
			{
				myTapList.erase(itfound);
				
			}
		}
	}
}
void CCTapGestureRecognizer::ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent)
{
	CCSetIterator it;
	for(it=pTouches->begin();it!=pTouches->end();it++)
	{
		CCTouch *pTouch=(CCTouch *)(*it);
		std::vector<DetectedTap>::iterator	itfound=findTouch(pTouch);

		if(itfound != myTapList.end())
		{
			DetectedTap&	testedtap=(*itfound);
			testedtap.myEndTime	=	mApplicationTime;
			// too long tap
			if((testedtap.myEndTime-testedtap.myStartTime)>TAP_MAX_DURATION)
			{
				
				myTapList.erase(itfound);
			}
		}
	}

	checkTapDetected();

}

void CCTapGestureRecognizer::ccTouchesCancelled(CCSet *pTouches, CCEvent *pEvent)
{
	CCSetIterator it;
	for(it=pTouches->begin();it!=pTouches->end();it++)
	{
		CCTouch *pTouch=(CCTouch *)(*it);
		std::vector<DetectedTap>::iterator	itfound=findTouch(pTouch);
		
		if(itfound != myTapList.end())
		{
			myTapList.erase(itfound);
		}
	}
}

void	CCTapGestureRecognizer::checkTapDetected()
{
	
	// remove too old tap
	std::vector<DetectedTap>::iterator	it=myTapList.begin();
	
	while(it!=myTapList.end())
	{
		DetectedTap&	testedtap=(*it);
		if((mApplicationTime-testedtap.myEndTime)>(numberOfTapsRequired*(TAP_MAX_DELAY + TAP_MAX_DURATION)))
		{
			it=myTapList.erase(it);
		}
		else
		{
			++it;
		}
	}

	if(myTapList.size() != numberOfTapsRequired)
	{
		return;
	}

	if(numberOfTapsRequired == 1)
	{
		DetectedTap&	testedtap=(*myTapList.begin());
		myTapPos=testedtap.myStartLocation;
		(mTarget->*mSelector)(this,mNode);
		myTapList.clear();
		return;
	}
	else if(numberOfTapsRequired == 2)
	{
		
		std::vector<DetectedTap>::iterator	ittap=myTapList.begin();
		DetectedTap&	testedtap1=(*ittap);
		ittap++;
		DetectedTap&	testedtap2=(*ittap);
		if(	(abs(testedtap1.myStartLocation.x-testedtap2.myStartLocation.x)<=TAP_OUT_DISTANCE)&&
			(abs(testedtap1.myStartLocation.y-testedtap2.myStartLocation.y)<=TAP_OUT_DISTANCE) )
		{
			if(testedtap1.myEndTime < testedtap2.myStartTime)
			{
				if((testedtap2.myStartTime - testedtap1.myEndTime)<TAP_MAX_DELAY)
				{
					myTapPos=testedtap2.myStartLocation;
					(mTarget->*mSelector)(this,mNode);
					myTapList.clear();
					return;

				}
			}
			else if(testedtap2.myEndTime < testedtap1.myStartTime)
			{
				if((testedtap1.myStartTime - testedtap2.myEndTime)<TAP_MAX_DELAY)
				{
					myTapPos=testedtap1.myStartLocation;
					(mTarget->*mSelector)(this,mNode);
					myTapList.clear();
					return;

				}
			}
		}

		// remove older tap
		if(testedtap2.myEndTime < testedtap1.myEndTime)
		{
			myTapList.erase(ittap);
		}
		else
		{
			ittap=myTapList.begin();
			myTapList.erase(ittap);
		}
	}
	else
	{
		// TODO
	}
	return;
}

CCPinchGestureRecognizer::CCPinchGestureRecognizer() : CCGestureRecognizer()
{
	myTouchList.clear();
}

CCPinchGestureRecognizer::~CCPinchGestureRecognizer()
{
	myTouchList.clear();
}

void CCPinchGestureRecognizer::ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent)
{
	if(((CCLayer*)mDelegate)->gestureRecognizerShouldBegin(this))
	{
		CCSetIterator it;
		for(it=pTouches->begin();it!=pTouches->end();it++)
		{
			CCTouch *pTouch=(CCTouch *)(*it);
			if(gestureRecognizershouldReceiveTouch(this,pTouch))
			{
				DetectedTouch	newtestedtouch;
				newtestedtouch.myStartTime=mApplicationTime;
				newtestedtouch.myCurrentTime=mApplicationTime;
				newtestedtouch.myCurrentLocation=pTouch->locationInView(pTouch->view());
				newtestedtouch.myStartLocation=newtestedtouch.myCurrentLocation;
				myTouchList[pTouch]=newtestedtouch;
			}
		}
		// always check
		checkPinchState();
	}

}
void CCPinchGestureRecognizer::ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent)
{

	bool	needCheck=false;
	CCSetIterator it;
	for(it=pTouches->begin();it!=pTouches->end();it++)
	{
		CCTouch *pTouch=(CCTouch *)(*it);
		std::map<CCTouch*,DetectedTouch>::iterator	itfound=myTouchList.find(pTouch);

		if(itfound != myTouchList.end())
		{
			DetectedTouch&	testedtouch=(*itfound).second;
			CCPoint	currentPos=pTouch->locationInView(pTouch->view());
			testedtouch.myCurrentTime=mApplicationTime;
			testedtouch.myCurrentLocation=currentPos;

			needCheck=true;
		}
	}
	if(needCheck)
	{
		checkPinchState();
	}

}
void CCPinchGestureRecognizer::ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent)
{

	bool	needCheck=false;
	CCSetIterator it;
	for(it=pTouches->begin();it!=pTouches->end();it++)
	{
		CCTouch *pTouch=(CCTouch *)(*it);
		std::map<CCTouch*,DetectedTouch>::iterator	itfound=myTouchList.find(pTouch);

		if(itfound != myTouchList.end())
		{
			myTouchList.erase(itfound);
			needCheck=true;
		}
	}
	if(needCheck)
	{
		checkPinchState();
	}

}
void CCPinchGestureRecognizer::ccTouchesCancelled(CCSet *pTouches, CCEvent *pEvent)
{
	ccTouchesEnded(pTouches, pEvent);
}

void	CCPinchGestureRecognizer::checkPinchState()
{
#ifdef TEST_PINCH
	CCGestureRecognizerState current_state=state;
	switch(current_state)
	{
	case CCGestureRecognizerStatePossible:	// waiting for a pinch to start
		{
			if(myTouchList.size()==1)
			{
				std::map<CCTouch*,DetectedTouch>::iterator	itTouch=myTouchList.begin();
				DetectedTouch& touch1=(*itTouch).second;
				
				DetectedTouch  touch2;
				touch2.myCurrentLocation.x = 480 - touch1.myCurrentLocation.x;
				touch2.myCurrentLocation.y = 320 - touch1.myCurrentLocation.y;

				if((myStartVector.x ==0.0f)&&(myStartVector.y ==0.0f))
				{
					myStartVector=ccpSub(touch2.myCurrentLocation,touch1.myCurrentLocation);
					myMiddlePos=ccpAdd(touch2.myCurrentLocation,touch1.myCurrentLocation);
					myMiddlePos=ccpMult(myMiddlePos,0.5f);
				}
				else
				{
					// check that vector lenght factor changed
					CCPoint currentVector=ccpSub(touch2.myCurrentLocation,touch1.myCurrentLocation);
					myMiddlePos=ccpAdd(touch2.myCurrentLocation,touch1.myCurrentLocation);
					myMiddlePos=ccpMult(myMiddlePos,0.5f);
					float l1_sqr=currentVector.x*currentVector.x+currentVector.y*currentVector.y;
					float l2_sqr=myStartVector.x*myStartVector.x+myStartVector.y*myStartVector.y;
					float coef=sqrtf(l1_sqr/l2_sqr);
					if((coef>1.1f)||(coef<0.9f))
					{
						state=CCGestureRecognizerStateBegan;
						myScale=coef;
						myLastScale=coef;
						myVelocity=0.0f;
						myLastTime=mApplicationTime;
					}

				}
			}
			else
			{
				myStartVector=CCPointZero;
				myScale=1.0f;
				myLastScale=1.0f;
				myVelocity=0.0f;
			}
			break;
		}
	case CCGestureRecognizerStateBegan:
	case CCGestureRecognizerStateChanged:
		{
			if(myTouchList.size()!=1)
			{
				state=CCGestureRecognizerStateEnded;
			}
			else
			{
				std::map<CCTouch*,DetectedTouch>::iterator	itTouch=myTouchList.begin();
				DetectedTouch& touch1=(*itTouch).second;
				DetectedTouch  touch2;
				touch2.myCurrentLocation.x = 480 - touch1.myCurrentLocation.x;
				touch2.myCurrentLocation.y = 320 - touch1.myCurrentLocation.y;

				CCPoint currentVector=ccpSub(touch2.myCurrentLocation,touch1.myCurrentLocation);
				myMiddlePos=ccpAdd(touch2.myCurrentLocation,touch1.myCurrentLocation);
				myMiddlePos=ccpMult(myMiddlePos,0.5f);
				float l1_sqr=currentVector.x*currentVector.x+currentVector.y*currentVector.y;
				float l2_sqr=myStartVector.x*myStartVector.x+myStartVector.y*myStartVector.y;
				float coef=sqrtf(l1_sqr/l2_sqr);
				myLastScale=myScale;
				myScale=coef;
				myStartVector=currentVector;

				float	dt=mApplicationTime-myLastTime;
				myLastTime=mApplicationTime;
				myVelocity=(myScale-myLastScale)*1.0f/dt;
				
				state=CCGestureRecognizerStateChanged;
			}
			break;
		}
	case CCGestureRecognizerStateEnded:
		{
			myStartVector=CCPointZero;
			myScale=1.0f;
			myLastScale=1.0f;
			myVelocity=0.0f;
			myLastTime=0.0f;
			state=CCGestureRecognizerStatePossible;
			break;
		}
	}

	if (((current_state != state)||(state==CCGestureRecognizerStateChanged))&&(state!=CCGestureRecognizerStatePossible))
	{
		(mTarget->*mSelector)(this,mNode);
	}
#else
	CCGestureRecognizerState current_state=state;
	switch(current_state)
	{
	case CCGestureRecognizerStatePossible:	// waiting for a pinch to start
		{
			if(myTouchList.size()==2)
			{
				std::map<CCTouch*,DetectedTouch>::iterator	itTouch=myTouchList.begin();
				DetectedTouch& touch1=(*itTouch).second;
				itTouch++;
				DetectedTouch& touch2=(*itTouch).second;

				if((myStartVector.x ==0.0f)&&(myStartVector.y ==0.0f))
				{
					myStartVector=ccpSub(touch2.myCurrentLocation,touch1.myCurrentLocation);
					myMiddlePos=ccpAdd(touch2.myCurrentLocation,touch1.myCurrentLocation);
					myMiddlePos=ccpMult(myMiddlePos,0.5f);
				}
				else
				{
					// check that vector lenght factor changed
					CCPoint currentVector=ccpSub(touch2.myCurrentLocation,touch1.myCurrentLocation);
					myMiddlePos=ccpAdd(touch2.myCurrentLocation,touch1.myCurrentLocation);
					myMiddlePos=ccpMult(myMiddlePos,0.5f);
					float l1_sqr=currentVector.x*currentVector.x+currentVector.y*currentVector.y;
					float l2_sqr=myStartVector.x*myStartVector.x+myStartVector.y*myStartVector.y;
					float coef=sqrtf(l1_sqr/l2_sqr);
					if((coef>1.1f)||(coef<0.9f))
					{
						state=CCGestureRecognizerStateBegan;
						myScale=coef;
						myLastScale=coef;
						myVelocity=0.0f;
						myLastTime=mApplicationTime;
					}

				}
			}
			else
			{
				myStartVector=CCPointZero;
				myScale=1.0f;
				myLastScale=1.0f;
				myVelocity=0.0f;
			}
			break;
		}
	case CCGestureRecognizerStateBegan:
	case CCGestureRecognizerStateChanged:
		{
			if(myTouchList.size()!=2)
			{
				state=CCGestureRecognizerStateEnded;
			}
			else
			{
				std::map<CCTouch*,DetectedTouch>::iterator	itTouch=myTouchList.begin();
				DetectedTouch& touch1=(*itTouch).second;
				itTouch++;
				DetectedTouch& touch2=(*itTouch).second;
				CCPoint currentVector=ccpSub(touch2.myCurrentLocation,touch1.myCurrentLocation);
				myMiddlePos=ccpAdd(touch2.myCurrentLocation,touch1.myCurrentLocation);
				myMiddlePos=ccpMult(myMiddlePos,0.5f);
				float l1_sqr=currentVector.x*currentVector.x+currentVector.y*currentVector.y;
				float l2_sqr=myStartVector.x*myStartVector.x+myStartVector.y*myStartVector.y;
				float coef=sqrtf(l1_sqr/l2_sqr);
				myLastScale=myScale;
				myScale=coef;
				myStartVector=currentVector;

				float	dt=mApplicationTime-myLastTime;
				myLastTime=mApplicationTime;
				myVelocity=(myScale-myLastScale)*1.0f/dt;
				
				state=CCGestureRecognizerStateChanged;
			}
			break;
		}
	case CCGestureRecognizerStateEnded:
		{
			myStartVector=CCPointZero;
			myScale=1.0f;
			myLastScale=1.0f;
			myVelocity=0.0f;
			myLastTime=0.0f;
			state=CCGestureRecognizerStatePossible;
			break;
		}
	}

	if (((current_state != state)||(state==CCGestureRecognizerStateChanged))&&(state!=CCGestureRecognizerStatePossible))
	{
		(mTarget->*mSelector)(this,mNode);
	}
#endif
}


CCPanGestureRecognizer::CCPanGestureRecognizer() : CCGestureRecognizer()
{
	maximumNumberOfTouches=1;
	minimumNumberOfTouches=1;
	myTouchList.clear();
	
}

CCPanGestureRecognizer::~CCPanGestureRecognizer()
{
	myTouchList.clear();
}

void CCPanGestureRecognizer::ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent)
{
#ifdef TEST_PINCH
	return;
#endif
	if(((CCLayer*)mDelegate)->gestureRecognizerShouldBegin(this))
	{
		CCSetIterator it;
		for(it=pTouches->begin();it!=pTouches->end();it++)
		{
			CCTouch *pTouch=(CCTouch *)(*it);
			if(gestureRecognizershouldReceiveTouch(this,pTouch))
			{
				DetectedTouch	newtestedtouch;
				newtestedtouch.myStartTime=mApplicationTime;
				newtestedtouch.myCurrentTime=mApplicationTime;
				newtestedtouch.myCurrentLocation=pTouch->locationInView(pTouch->view());
				newtestedtouch.myStartLocation=newtestedtouch.myCurrentLocation;
				newtestedtouch.myVelocity=CCPointZero;

				myTouchList[pTouch]=newtestedtouch;
			}
		}
		// always check
		checkPanState();
	}
}
void CCPanGestureRecognizer::ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent)
{
	bool	needCheck=false;
	CCSetIterator it;
	for(it=pTouches->begin();it!=pTouches->end();it++)
	{
		CCTouch *pTouch=(CCTouch *)(*it);
		std::map<CCTouch*,DetectedTouch>::iterator	itfound=myTouchList.find(pTouch);

		if(itfound != myTouchList.end())
		{
			DetectedTouch&	testedtouch=(*itfound).second;
			
			// compute instant velocity
			float	dt=mApplicationTime-testedtouch.myCurrentTime;
			CCPoint	currentPos=pTouch->locationInView(pTouch->view());
			if(dt>0.0f)
			{
				testedtouch.myVelocity=ccpSub(currentPos,testedtouch.myCurrentLocation);
				testedtouch.myVelocity=ccpMult(testedtouch.myVelocity,1.0f/dt);
			}
			testedtouch.myCurrentTime=mApplicationTime;
			if((state != CCGestureRecognizerStatePossible)||(myTouchList.size()<minimumNumberOfTouches)||(myTouchList.size()>maximumNumberOfTouches))
			{
				testedtouch.myStartLocation=testedtouch.myCurrentLocation;
			}
			testedtouch.myCurrentLocation=currentPos;

			needCheck=true;
		}
	}
	if(needCheck)
	{
		checkPanState();
	}
}
void CCPanGestureRecognizer::ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent)
{
	bool	needCheck=false;
	CCSetIterator it;
	for(it=pTouches->begin();it!=pTouches->end();it++)
	{
		CCTouch *pTouch=(CCTouch *)(*it);
		std::map<CCTouch*,DetectedTouch>::iterator	itfound=myTouchList.find(pTouch);

		if(itfound != myTouchList.end())
		{
			DetectedTouch&	testedtouch=(*itfound).second;
			if(state != CCGestureRecognizerStatePossible)
			{
				// compute instant velocity
				float	dt=mApplicationTime-testedtouch.myCurrentTime;
				CCPoint	currentPos=pTouch->locationInView(pTouch->view());
				if(dt>0.0f)
				{
					testedtouch.myVelocity=ccpSub(currentPos,testedtouch.myCurrentLocation);
					testedtouch.myVelocity=ccpMult(testedtouch.myVelocity,1.0f/dt);
					GetAverageVelocity(myVelocity);
				}
			}

			myTouchList.erase(itfound);
			needCheck=true;
		}
	}
	if(needCheck)
	{
		checkPanState();
	}

}
void CCPanGestureRecognizer::ccTouchesCancelled(CCSet *pTouches, CCEvent *pEvent)
{
	ccTouchesEnded(pTouches, pEvent);
}

void	CCPanGestureRecognizer::checkPanState()
{
	CCGestureRecognizerState current_state=state;
	switch(current_state)
	{
	case CCGestureRecognizerStatePossible:	// waiting for a pan to start
		{
			if((myTouchList.size()>=minimumNumberOfTouches)&&(myTouchList.size()<=maximumNumberOfTouches))
			{
				if(GetAverageTranslation(myTranslation))
				{
					if(	(abs(myTranslation.x)>=PAN_MIN_START)||
						(abs(myTranslation.y)>=PAN_MIN_START) )
					{
						GetAverageVelocity(myVelocity);
						state=CCGestureRecognizerStateBegan;
					}
				}
			}
			break;
		}
	case CCGestureRecognizerStateBegan:
	case CCGestureRecognizerStateChanged:
		{
			if((myTouchList.size()<minimumNumberOfTouches)||(myTouchList.size()>maximumNumberOfTouches))
			{
				state=CCGestureRecognizerStateEnded;
			}
			else
			{
				if(GetAverageTranslation(myTranslation))
				{
					GetAverageVelocity(myVelocity);
					state=CCGestureRecognizerStateChanged;
				}
				else
				{
					myVelocity=CCPointZero;
					state=CCGestureRecognizerStateEnded;
				}
			}
			break;
		}
	case CCGestureRecognizerStateEnded:
		{
			state=CCGestureRecognizerStatePossible;
			break;
		}
	}

	if (((current_state != state)||(state==CCGestureRecognizerStateChanged))&&(state!=CCGestureRecognizerStatePossible))
	{
		(mTarget->*mSelector)(this,mNode);
	}
}

bool	CCPanGestureRecognizer::GetAverageTranslation(CCPoint& tr)
{
	if(minimumNumberOfTouches == 1)
	{
		std::map<CCTouch*,DetectedTouch>::iterator	ittouch=myTouchList.begin();
		DetectedTouch&	testedtouch=(*ittouch).second;
		tr=ccpSub(testedtouch.myCurrentLocation,testedtouch.myStartLocation);
		return true;
	}
	else // TODO
	{

	}

	return false;
}
void	CCPanGestureRecognizer::GetAverageVelocity(CCPoint& vl)
{
	if(minimumNumberOfTouches == 1)
	{
		std::map<CCTouch*,DetectedTouch>::iterator	ittouch=myTouchList.begin();
		DetectedTouch&	testedtouch=(*ittouch).second;
		vl=testedtouch.myVelocity;
	}
	else // TODO
	{

	}

}

CCPoint CCPanGestureRecognizer::getVelocityInView(int viewid)
{
	return myVelocity;
}

CCPoint CCPanGestureRecognizer::getTranslationInView(int viewid)
{
	return myTranslation;
}
void	CCPanGestureRecognizer::setTranslationInView(int viewid,const CCPoint& pt)
{
	myTranslation=pt;
}


CCGestureRecognizer* CCGestureRecognizer::CCRecognizerWithRecognizerTargetAction(CCGestureRecognizer* reco,CCObject* target,SEL_Gesture eventtoThrow)
{
	reco->mSelector=eventtoThrow;
	reco->setTarget(target);

	return reco;
}




bool CCGestureRecognizer::gestureRecognizershouldReceiveTouch(CCGestureRecognizer* gesturerecognizer,CCTouch * touch)
{
	CCPoint pt = CCDirector::sharedDirector()->convertToGL(touch->locationInView(touch->view()));

	bool rslt = true;
	
	if(mNode)
	{
		rslt=mNode->isPointInArea(pt);

		if( rslt )
		{
			// still ok, now check children of parents after this node
			CCNode* node = mNode;
			CCNode* parent = mNode->getParent();
			while( node != 0 && rslt)
			{
				CCObject* child;
				bool nodeFound = false;
				CCARRAY_FOREACH(parent->getChildren(), child)
				{
					if( !nodeFound )
					{
						if( !nodeFound && node == (CCNode*)child )
							nodeFound = true;  // we need to keep track of until we hit our node, any past it have a higher z value
						continue;
					}

					if(((CCNode*)child)->isNodeInTreeTouched(pt) )
					{
						rslt = false;
						break;
					}
				}

				node = parent;
				parent = node->getParent();
			}
		}
	}

	if( rslt && mDelegate )
		rslt = ((CCLayer*)mDelegate)->gestureRecognizershouldReceiveTouch(gesturerecognizer,touch);

	return rslt;
}

#ifdef ASSORIA

- (BOOL)gestureRecognizer:(UIGestureRecognizer *)gestureRecognizer shouldRecognizeSimultaneouslyWithGestureRecognizer:(UIGestureRecognizer *)otherGestureRecognizer
{
  if( !m_delegate )
    return YES;
  return [m_delegate gestureRecognizer:gestureRecognizer shouldRecognizeSimultaneouslyWithGestureRecognizer:otherGestureRecognizer];
}

- (BOOL)gestureRecognizerShouldBegin:(UIGestureRecognizer *)gestureRecognizer
{
  if( !m_delegate )
    return YES;
  return [m_delegate gestureRecognizerShouldBegin:gestureRecognizer];
}

- (void)callback:(UIGestureRecognizer*)recognizer
{
  if( m_target )
    [m_target performSelector:m_callback withObject:recognizer withObject:m_node];
}
@end
#endif
}

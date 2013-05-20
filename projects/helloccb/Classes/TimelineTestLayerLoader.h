//
//  TimelineTestLayerLoader.h
//  helloccb
//
//  Created by James Chen on 5/19/13.
//
//

#ifndef helloccb_TimelineTestLayerLoader_h
#define helloccb_TimelineTestLayerLoader_h


#include "TimelineTestLayer.h"

class TimelineTestLayerLoader : public cocos2d::extension::CCLayerLoader {
public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(TimelineTestLayerLoader, loader);
    
protected:
    CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(TimelineTestLayer);
};

#endif


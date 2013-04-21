//
//  MainSceneLoader.h
//  helloccb
//
//  Created by James Chen on 4/21/13.
//
//

#ifndef helloccb_MainLayerLoader_h
#define helloccb_MainLayerLoader_h

#include "MainLayer.h"

class MainLayerLoader : public cocos2d::extension::CCLayerLoader {
public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(MainLayerLoader, loader);
    
protected:
    CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(MainLayer);
};

#endif

//
//  EPMacros.h
//  test2
//
//  Created by Erawppa Erawppa on 2011/12/6.
//  Copyright (c) 2011年 __MyCompanyName__. All rights reserved.
//

#ifndef __EPMacros_H__
#define __EPMacros_H__

#define EP_SAFE_DELETE(p)       if (p){delete p;p = NULL;}
#define EP_SAFE_FREE(p)         if (p){free(p);p = NULL;}

#endif//__EPMacros_H__

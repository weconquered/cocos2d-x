//
//  base64.h
//  GameControlCenter
//
//  Created by  on 2011/11/13.
//  Copyright (c) 2011年 __MyCompanyName__. All rights reserved.
//

#ifndef GameControlCenter_base64_h
#define GameControlCenter_base64_h

#include <string>

std::string base64_encode(unsigned char const* , unsigned int len);
std::string base64_decode(std::string const& s);

#endif

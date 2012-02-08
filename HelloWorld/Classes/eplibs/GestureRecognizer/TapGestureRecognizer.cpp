//
//  TapGestureRecognizer.cpp
//  EnjoyYourMoney
//
//  Created by Erawppa on 2011/9/3.
//  Copyright 2011年 __MyCompanyName__. All rights reserved.
//

#include "TapGestureRecognizer.h"

TapGestureRecognizer *TapGestureRecognizer::recognizer()
{
    TapGestureRecognizer *recognizer = new TapGestureRecognizer;
    recognizer->init();
    recognizer->autorelease();
    return recognizer;
}

eGestureRecognizerType TapGestureRecognizer::getRecognizerType()
{
    return kTapGestureRecognizer;
}

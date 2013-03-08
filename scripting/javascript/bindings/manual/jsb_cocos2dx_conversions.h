#ifndef __JSB_COCOS2DX_CONVERSIONS_H__
#define __JSB_COCOS2DX_CONVERSIONS_H__

#include "jsapi.h"
#include "cocos2d.h"

USING_NS_CC;

JSBool jsval_to_ccpoint(JSContext *cx, jsval v, CCPoint* ret);
JSBool jsval_to_ccrect(JSContext *cx, jsval v, CCRect* ret);
JSBool jsval_to_ccsize(JSContext *cx, jsval v, CCSize* ret);
JSBool jsval_to_cccolor4b(JSContext *cx, jsval v, ccColor4B* ret);
JSBool jsval_to_cccolor4f(JSContext *cx, jsval v, ccColor4F* ret);
JSBool jsval_to_cccolor3b(JSContext *cx, jsval v, ccColor3B* ret);
JSBool jsval_to_ccarray_of_CCPoint(JSContext* cx, jsval v, CCPoint **points, int *numPoints);
JSBool jsval_to_ccarray(JSContext* cx, jsval v, CCArray** ret);
JSBool jsval_to_ccdictionary(JSContext* cx, jsval v, CCDictionary** ret);
JSBool jsval_to_ccacceleration(JSContext* cx,jsval v, CCAcceleration* ret);
JSBool jsvals_variadic_to_ccarray( JSContext *cx, jsval *vp, int argc, CCArray** ret);

jsval ccpoint_to_jsval(JSContext* cx, CCPoint& v);
jsval ccrect_to_jsval(JSContext* cx, CCRect& v);
jsval ccsize_to_jsval(JSContext* cx, CCSize& v);
jsval cccolor4b_to_jsval(JSContext* cx, ccColor4B& v);
jsval cccolor4f_to_jsval(JSContext* cx, ccColor4F& v);
jsval cccolor3b_to_jsval(JSContext* cx, const ccColor3B& v);
jsval ccdictionary_to_jsval(JSContext* cx, CCDictionary *dict);
jsval ccarray_to_jsval(JSContext* cx, CCArray *arr);
jsval ccacceleration_to_jsval(JSContext* cx, CCAcceleration& v);


#endif /* __JSB_COCOS2DX_CONVERSIONS_H__ */

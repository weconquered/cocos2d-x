//
//  Created by Rohan Kuruvilla
//  Copyright (c) 2012 Zynga Inc. All rights reserved.
//


#include "jsb_basic_conversions.h"
#include "js_bindings_config.h"

// --------------------------------------
// to native
// --------------------------------------

JSBool jsval_to_opaque( JSContext *cx, jsval vp, void **r)
{
#ifdef __LP64__
	JSObject *tmp_arg;
	if( ! JS_ValueToObject( cx, vp, &tmp_arg ) )
		return JS_FALSE;

	JSB_PRECONDITION( js_IsTypedArray( tmp_arg ), "jsb: Not a TypedArray object");

	JSB_PRECONDITION( JS_GetTypedArrayByteLength( tmp_arg ) == sizeof(void*), "jsb: Invalid Typed Array lenght");
	
	int32_t* arg_array = (int32_t*)JS_GetTypedArrayData( tmp_arg );
	uint64 ret =  arg_array[0];
	ret = ret << 32;
	ret |= arg_array[1];
	
#else
	assert( sizeof(int)==4);
	int32_t ret;
	if( ! jsval_to_int32(cx, vp, &ret ) )
	  return JS_FALSE;
#endif
	*r = (void*)ret;
	return JS_TRUE;
}

JSBool jsval_to_charptr( JSContext *cx, jsval vp, const char **ret )
{
    JSString *jsstr = JS_ValueToString( cx, vp );
    JSB_PRECONDITION2( jsstr, cx, JS_FALSE, "invalid string" );

    // root it
    vp = STRING_TO_JSVAL(jsstr);

    // Not supported in SpiderMonkey v19
    //char *ptr = JS_EncodeString(cx, jsstr);
    JSStringWrapper strWrapper(jsstr);
    
    // XXX: It is converted to CCString and then back to char* to autorelease the created object.
    CCString *tmp = CCString::create(strWrapper.get());

    JSB_PRECONDITION2( tmp, cx, JS_FALSE, "Error creating string from UTF8");

    *ret = tmp->getCString();

    return JS_TRUE;
}

JSBool jsval_to_int32( JSContext *cx, jsval vp, int32_t *outval )
{
    JSBool ok = JS_TRUE;
    double dp;
    ok &= JS_ValueToNumber(cx, vp, &dp);
    JSB_PRECONDITION2(ok, cx, JS_FALSE, "Error processing arguments");
    ok &= !isnan(dp);
    JSB_PRECONDITION2(ok, cx, JS_FALSE, "Error processing arguments");

    *outval = (int32_t)dp;

    return ok;
}

JSBool jsval_to_uint32( JSContext *cx, jsval vp, uint32_t *outval )
{
    JSBool ok = JS_TRUE;
    double dp;
    ok &= JS_ValueToNumber(cx, vp, &dp);
    JSB_PRECONDITION2(ok, cx, JS_FALSE, "Error processing arguments");
    ok &= !isnan(dp);
    JSB_PRECONDITION2(ok, cx, JS_FALSE, "Error processing arguments");

    *outval = (uint32_t)dp;

    return ok;
}

JSBool jsval_to_uint16( JSContext *cx, jsval vp, uint16_t *outval )
{
    JSBool ok = JS_TRUE;
    double dp;
    ok &= JS_ValueToNumber(cx, vp, &dp);
    JSB_PRECONDITION2(ok, cx, JS_FALSE, "Error processing arguments");
    ok &= !isnan(dp);
    JSB_PRECONDITION2(ok, cx, JS_FALSE, "Error processing arguments");

    *outval = (uint16_t)dp;

    return ok;
}

// XXX: sizeof(long) == 8 in 64 bits on OS X... apparently on Windows it is 32 bits (???)
JSBool jsval_to_long( JSContext *cx, jsval vp, long *r )
{
#ifdef __LP64__
    // compatibility check
    assert( sizeof(long)==8);
    JSObject *tmp_arg;
    if( ! JS_ValueToObject( cx, vp, &tmp_arg ) )
        return JS_FALSE;

    JSB_PRECONDITION( js_IsTypedArray( tmp_arg ), "jsb: Not a TypedArray object");

    JSB_PRECONDITION( JS_GetTypedArrayByteLength( tmp_arg ) == sizeof(long), "jsb: Invalid Typed Array lenght");
    
    int32_t* arg_array = (int32_t*)JS_GetTypedArrayData( tmp_arg );
    long ret =  arg_array[0];
    ret = ret << 32;
    ret |= arg_array[1];
    
#else
    // compatibility check
    assert( sizeof(int)==4);
    long ret = JSVAL_TO_INT(vp);
#endif
    
    *r = ret;
    return JS_TRUE;
}

JSBool jsval_to_longlong( JSContext *cx, jsval vp, long long *r )
{
    JSObject *tmp_arg;
    JSBool ok = JS_ValueToObject( cx, vp, &tmp_arg );
    JSB_PRECONDITION2( ok, cx, JS_FALSE, "Error converting value to object");
    JSB_PRECONDITION2( tmp_arg && JS_IsTypedArrayObject( tmp_arg ), cx, JS_FALSE, "Not a TypedArray object");
    JSB_PRECONDITION2( JS_GetTypedArrayByteLength( tmp_arg ) == sizeof(long long), cx, JS_FALSE, "Invalid Typed Array length");
    
    uint32_t* arg_array = (uint32_t*)JS_GetArrayBufferViewData( tmp_arg );
    long long ret =  arg_array[0];
    ret = ret << 32;
    ret |= arg_array[1];
    
    *r = ret;
    return JS_TRUE;
}

JSBool jsval_to_std_string(JSContext *cx, jsval v, std::string* ret) {
    JSString *tmp = JS_ValueToString(cx, v);
    JSB_PRECONDITION2(tmp, cx, JS_FALSE, "Error processing arguments");

    JSStringWrapper str(tmp);
    *ret = str.get();
    return JS_TRUE;
}

// --------------------------------------
// to jsval
// --------------------------------------
jsval opaque_to_jsval( JSContext *cx, void *opaque )
{
#ifdef __LP64__
    uint64_t number = (uint64_t)opaque;
    JSObject *typedArray = js_CreateTypedArray(cx, js::TypedArray::TYPE_UINT32, 2);
    int32_t *buffer = (int32_t*)JS_GetTypedArrayData(typedArray);
    buffer[0] = number >> 32;
    buffer[1] = number & 0xffffffff;
    return OBJECT_TO_JSVAL(typedArray);     
#else
    assert(sizeof(int)==4);
    int32_t number = (int32_t) opaque;
    return INT_TO_JSVAL(number);
#endif
}

jsval charptr_to_jsval( JSContext *cx, const char *str)
{
    JSString *ret_obj = JS_NewStringCopyZ(cx, str);
    return STRING_TO_JSVAL(ret_obj);
}

jsval int32_to_jsval( JSContext *cx, int32_t number )
{
    return INT_TO_JSVAL(number);
}

jsval uint32_to_jsval( JSContext *cx, uint32_t number )
{
    return UINT_TO_JSVAL(number);
}

jsval long_to_jsval( JSContext *cx, long number )
{
#ifdef __LP64__
	NSCAssert( sizeof(long)==8, @"Error!");
    
	char chr[128];
	snprintf(chr, sizeof(chr)-1, "%ld", number);
	JSString *ret_obj = JS_NewStringCopyZ(cx, chr);
	return STRING_TO_JSVAL(ret_obj);
#else
	CCAssert( sizeof(int)==4, "Error!");
	return INT_TO_JSVAL(number);
#endif
}

jsval longlong_to_jsval( JSContext *cx, long long number )
{
#if JSB_REPRESENT_LONGLONG_AS_STR
	char chr[128];
	snprintf(chr, sizeof(chr)-1, "%lld", number);
	JSString *ret_obj = JS_NewStringCopyZ(cx, chr);
	return STRING_TO_JSVAL(ret_obj);
    
#else
	CCAssert( sizeof(long long)==8, "Error!");
	JSObject *typedArray = JS_NewUint32Array( cx, 2 );
	uint32_t *buffer = (uint32_t*)JS_GetArrayBufferViewData(typedArray, cx);
	buffer[0] = number >> 32;
	buffer[1] = number & 0xffffffff;
	return OBJECT_TO_JSVAL(typedArray);
#endif
}

jsval std_string_to_jsval(JSContext* cx, std::string& v) {
    JSString *str = JS_NewStringCopyZ(cx, v.c_str());
    return STRING_TO_JSVAL(str);
}

jsval c_string_to_jsval(JSContext* cx, const char* v) {
    JSString *str = JS_NewStringCopyZ(cx, v);
    return STRING_TO_JSVAL(str);
}

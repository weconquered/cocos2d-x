//
//  Created by Rohan Kuruvilla
//  Copyright (c) 2012 Zynga Inc. All rights reserved.
//
#ifndef __JSB_MANUAL_CONVERSIONS_H__
#define __JSB_MANUAL_CONVERSIONS_H__

#include "jsapi.h"

// some utility functions
// to native
JSBool jsval_to_opaque( JSContext *cx, jsval vp, void **out );
/** converts a jsval (JS string) into a char */
JSBool jsval_to_charptr( JSContext *cx, jsval vp, const char **out);
JSBool jsval_to_int32( JSContext *cx, jsval vp, int32_t *ret );
JSBool jsval_to_uint32( JSContext *cx, jsval vp, uint32_t *ret );
JSBool jsval_to_uint16( JSContext *cx, jsval vp, uint16_t *ret );
JSBool jsval_to_long( JSContext *cx, jsval vp, long *out);
JSBool jsval_to_longlong( JSContext *cx, jsval vp, long long *out); 
JSBool jsval_to_std_string(JSContext *cx, jsval v, std::string* ret);


// from native
jsval opaque_to_jsval( JSContext *cx, void* opaque);
/* Converts a char ptr into a jsval (using JS string) */
jsval charptr_to_jsval( JSContext *cx, const char *str);
jsval int32_to_jsval( JSContext *cx, int32_t l);
jsval uint32_to_jsval( JSContext *cx, uint32_t number );
jsval long_to_jsval( JSContext *cx, long number );
jsval longlong_to_jsval( JSContext *cx, long long number );
jsval std_string_to_jsval(JSContext* cx, string& v);
jsval c_string_to_jsval(JSContext* cx, const char* v);


#endif /* __JSB_MANUAL_CONVERSIONS_H__ */


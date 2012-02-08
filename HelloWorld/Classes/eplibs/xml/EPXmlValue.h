//
//  EPEPXmlValue.h
//  GameControlCenter
//
//  Created by Erawppa Erawppa on 2011/11/28.
//  Copyright (c) 2011年 __MyCompanyName__. All rights reserved.
//

#ifndef __EP_EPXmlValue_H__
#define __EP_EPXmlValue_H__

#include "cocos2d.h"
#include "cJSON.h"
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <string>
#include <map>
#include <vector>

#define ARRAY_FOREACH(s,a)                                               \
    for (int i=0 ; (s = a->objectAtIndex(i)) != NULL; i++)

class EPXmlValue;
class PlistNode;

enum
{
    kPlistStateInit = 0,
    kPlistStatePlistNode,
    kPlistStateStart,
    kPlistStateKeyElement,
    kPlistStateKeyText,
    kPlistStateValueElement,
    kPlistStateValueText,
};
typedef int eEPPlistState;

enum
{
    kEPXmlValueNone = 0,
    kEPXmlValueArray,
    kEPXmlValueDictionary,
    kEPXmlValueString,
    kEPXmlValueInteger,
    kEPXmlValueDouble,
    kEPXmlValueBoolean,
    kEPXmlValueDate,
    kEPXmlValueData
};
typedef int eEPEPXmlValueType;

typedef std::map<std::string, EPXmlValue*>  XML_DICTIONARY;
typedef std::vector<EPXmlValue*>            XML_ARRAY;
typedef std::pair<std::string, PlistNode*>  PLIST_PAIR;
typedef std::pair<std::string, std::string> STRING_PAIR;

class EPXmlValue
{
public:
    EPXmlValue();
    
    EPXmlValue(std::string key);
    
    ~EPXmlValue();
    
    bool loadJSON(const char* buffer,int length);
    bool loadJSON(const char* buffer){return loadJSON(buffer, (int)strlen(buffer));};
    bool loadZipJSON(const char* buffer,int length);
    
    EPXmlValue* copy();
    
    EPXmlValue* objectForKey(std::string key); 
    bool addObject(std::string key, EPXmlValue* object);
    bool addObject(std::string key, int value);
    bool addObject(std::string key, double value);
    bool addObject(std::string key, bool value);
    bool addObject(std::string key, const char* value);
    bool addObject(std::string key, const char* data, int length);
    bool addObject(std::string key, XML_ARRAY *value);
    bool addObject(std::string key, XML_DICTIONARY *value);
    
    bool addObject(EPXmlValue* object);
    bool addObject(int value);
    bool addObject(double value);
    bool addObject(bool value);
    bool addObject(const char* value);
    bool addObject(XML_ARRAY *value);
    bool addObject(XML_DICTIONARY *value);
    
    bool removeObjectForKey(std::string key);
    bool removeObject(EPXmlValue* object);
    
    //
    // dictionary getter & setter
    //    
    int integerForKey(std::string key);
    double doubleForKey(std::string key);
    const char* stringForKey(std::string key);
    bool booleanForKey(std::string key);
    std::string stdStringForKey(std::string key);
    const char* dataForKey(std::string key);
    
    //
    // array getter
    //
    EPXmlValue* objectAtIndex(int index); 
    int integerAtIndex(int index);
    double doubleAtIndex(int index);
    const char* stringAtIndex(int index);
    bool booleanAtIndex(int index);
    std::string stdStringAtIndex(int index);
    const char* dataAtIndex(int index);
    
    //
    // setter
    //
    void setValue(const char* value);
    void setValue(int value);
    void setValue(double value);
    void setValue(bool value);
    void setValue(const char* value,int length);
    
    void setValueForKey(const char* value,std::string key);
    void setValueForKey(int value,std::string key);
    void setValueForKey(double value,std::string key);
    void setValueForKey(bool value,std::string key);
    void setValueForKey(const char* value,int length,std::string key);
    
    void setValueAtIndex(const char* value,int index);
    void setValueAtIndex(int value,int index);
    void setValueAtIndex(double value,int index);
    void setValueAtIndex(bool value,int index);
    void setValueAtIndex(const char* value,int length,int index);
    
    void setString(std::string& value);
    void setInteger(std::string& value);
    void setDouble(std::string& value);
    void setBoolean(std::string& value);
    void setDate(std::string& value);    
    void setData(std::string& value);    
    void setArray(XML_ARRAY *array);
    void setDictionary(XML_DICTIONARY *dictionary);
    
    CC_SYNTHESIZE(xmlNodePtr, m_xmlNodePtr, XmlNodePtr)
    CC_SYNTHESIZE(xmlNodePtr, m_xmlKeyNodePtr, XmlKeyNodePtr)
    CC_SYNTHESIZE(int, m_type, Type)
    CC_SYNTHESIZE(std::string, m_key, Key)
    
    const char* toData(); //decode
    const char* toString();
    std::string toStdString();
    int toInteger();
    double toDouble();
    float toFloat();
    bool toBoolean();
    XML_ARRAY* toArray();
    XML_DICTIONARY* toDictionary();
    
    // need to free pointer by caller
    char* toJSON();
    char* toZipJSON(int *zipLength);
    
public:
    void dump(EPXmlValue *plist = NULL);
    
private:
    void deleteString(EPXmlValue* value);
    void deleteDictionary(EPXmlValue* value);
    void deleteArray(EPXmlValue* value);
    void deleteDate(EPXmlValue* value);
    
    EPXmlValue* copyObject(EPXmlValue* parent,EPXmlValue* input);
    
    void jsonAddToObject(cJSON *json,EPXmlValue* value);
    void jsonAddArrayToObject(cJSON *json,EPXmlValue* value);
    void jsonAddDictionaryToObject(cJSON *json,EPXmlValue* value);
    
    void visitJsonNode(cJSON *node,EPXmlValue* value);
    void visitJsonArray(cJSON *node,EPXmlValue* value);
    void visitJsonObject(cJSON *node,EPXmlValue* value);
    
    XML_ARRAY *m_array;
    XML_DICTIONARY *m_dictionary;
    std::string m_string;
    bool m_firstJSONode;
};


class PlistNode
{
public:
    std::string m_key;
    std::string m_value;
    xmlNodePtr m_node;
    xmlNodePtr m_keyNode;
};

#endif//__EP_EPXmlValue_H__

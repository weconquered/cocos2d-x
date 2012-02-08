//
//  EPEPXmlValue.cpp
//  GameControlCenter
//
//  Created by Erawppa Erawppa on 2011/11/28.
//  Copyright (c) 2011年 __MyCompanyName__. All rights reserved.
//

#include "EPXmlValue.h"
#include "easyzlib.h"
#include "base64.h"
#include <sstream>
#include <iomanip>
#include <algorithm>
#include "EPMacros.h"

#define EP_COMPRESS_MAX_LENGTH  10*1024

EPXmlValue::EPXmlValue()
:m_type(kEPXmlValueNone)
,m_xmlNodePtr(NULL)
,m_xmlKeyNodePtr(NULL)
,m_array(NULL)
,m_dictionary(NULL)
,m_string("")
,m_key("")
{
}

EPXmlValue::EPXmlValue(std::string key)
:m_type(kEPXmlValueNone)
,m_xmlNodePtr(NULL)
,m_xmlKeyNodePtr(NULL)
,m_array(NULL)
,m_dictionary(NULL)
,m_string("")
,m_key("")
{
    m_key = key.c_str();
}

EPXmlValue::~EPXmlValue()
{
    switch (m_type)
    {
        case kEPXmlValueArray:
            for (XML_ARRAY::iterator it = m_array->begin() ; it != m_array->end() ; ++it)
            {
                delete *it;
            } 
            delete m_array;
            break;
        case kEPXmlValueDictionary:
            for (XML_DICTIONARY::iterator it = m_dictionary->begin() ; it != m_dictionary->end() ; ++it)
            {
                delete it->second;
            }
            delete m_dictionary;
            break;
    }
}

void EPXmlValue::setValue(const char* value)
{
    if (m_xmlKeyNodePtr)
    {
        xmlNodeSetContent(m_xmlNodePtr, (const xmlChar*)value);
        xmlNodeSetName(m_xmlKeyNodePtr, (const xmlChar*)"string");
    }
    std::string v = value;
    setString(v);
}

void EPXmlValue::setValue(int value)
{
    std::stringstream st;
    st << std::setprecision(16) << value;
    std::string v = st.str();
    
    if (m_xmlKeyNodePtr)
    {
        xmlNodeSetContent(m_xmlNodePtr, (const xmlChar*)v.c_str());
        xmlNodeSetName(m_xmlKeyNodePtr, (const xmlChar*)"integer");
    }
    setInteger(v);
}

void EPXmlValue::setValue(double value)
{
    std::stringstream st;
    st << std::setprecision(16) << value;
    std::string v = st.str();
    
    if (m_xmlKeyNodePtr)
    {
        xmlNodeSetContent(m_xmlNodePtr, (const xmlChar*)v.c_str());    
        xmlNodeSetName(m_xmlKeyNodePtr, (const xmlChar*)"real");
    }
    setDouble(v);
}

void EPXmlValue::setValue(bool value)
{
    std::string v;
    if (value)
    {
        v = "true";
        if (m_xmlKeyNodePtr)
            xmlNodeSetName(m_xmlKeyNodePtr, (const xmlChar*)"true");
    }
    else
    {
        v = "false";
        if (m_xmlKeyNodePtr)
            xmlNodeSetName(m_xmlKeyNodePtr, (const xmlChar*)"false");
    }
    setBoolean(v);
}

void EPXmlValue::setValue(const char* value,int length)
{
//    std::string encode = base64_encode((const unsigned char*)value, (unsigned int)length);
//    
//    if (m_xmlKeyNodePtr)
//    {
//        xmlNodeSetContent(m_xmlNodePtr, (const xmlChar*)encode.c_str());
//        xmlNodeSetName(m_xmlKeyNodePtr, (const xmlChar*)"data");
//    }
//    setData(encode);
}

void EPXmlValue::setValueForKey(const char* value,std::string key)
{
    EPXmlValue* obj = this->objectForKey(key);
    if (!obj)
        return;
    obj->setValue(value);
}

void EPXmlValue::setValueForKey(int value,std::string key)
{
    EPXmlValue* obj = this->objectForKey(key);
    if (!obj)
        return;
    obj->setValue(value);
}

void EPXmlValue::setValueForKey(double value,std::string key)
{
    EPXmlValue* obj = this->objectForKey(key);
    if (!obj)
        return;
    obj->setValue(value);
}

void EPXmlValue::setValueForKey(bool value,std::string key)
{
    EPXmlValue* obj = this->objectForKey(key);
    if (!obj)
        return;
    obj->setValue(value);
}

void EPXmlValue::setValueForKey(const char* value,int length,std::string key)
{
    EPXmlValue* obj = this->objectForKey(key);
    if (!obj)
        return;
    obj->setValue(value, length);
}

void EPXmlValue::setValueAtIndex(const char* value,int index)
{
    EPXmlValue* obj = this->objectAtIndex(index);
    if (!obj)
        return;
    obj->setValue(value);
}

void EPXmlValue::setValueAtIndex(int value,int index)
{
    EPXmlValue* obj = this->objectAtIndex(index);
    if (!obj)
        return;
    obj->setValue(value);
}

void EPXmlValue::setValueAtIndex(double value,int index)
{
    EPXmlValue* obj = this->objectAtIndex(index);
    if (!obj)
        return;
    obj->setValue(value);
}

void EPXmlValue::setValueAtIndex(bool value,int index)
{
    EPXmlValue* obj = this->objectAtIndex(index);
    if (!obj)
        return;
    obj->setValue(value);
}

void EPXmlValue::setValueAtIndex(const char* value,int length,int index)
{
    EPXmlValue* obj = this->objectAtIndex(index);
    if (!obj)
        return;
    obj->setValue(value, length);
}

void EPXmlValue::setString(std::string& value)
{
    m_type = kEPXmlValueString;
    m_string = value.c_str();
}

void EPXmlValue::setInteger(std::string& value)
{
    m_type = kEPXmlValueInteger;
    m_string = value.c_str();
}

void EPXmlValue::setDouble(std::string& value)
{
    m_type = kEPXmlValueDouble;
    m_string = value.c_str();
}

void EPXmlValue::setBoolean(std::string& value)
{
    m_type = kEPXmlValueBoolean;
    m_string = value.c_str();
}

void EPXmlValue::setDate(std::string& value)
{
    m_type = kEPXmlValueDate;
    m_string = value.c_str();
}

void EPXmlValue::setData(std::string& value)
{
    m_type = kEPXmlValueData;
    m_string = value.c_str();
}

void EPXmlValue::setArray(XML_ARRAY *array)
{
    m_type = kEPXmlValueArray;
    m_array = array;
}

void EPXmlValue::setDictionary(XML_DICTIONARY *dictionary)
{
    m_type = kEPXmlValueDictionary;
    m_dictionary = dictionary;
}

const char* EPXmlValue::toString()
{
    return m_string.c_str();
}

const char* EPXmlValue::toData()
{
//    std::string decoded = base64_decode(m_string);
//    return decoded.c_str();
    return NULL;
}

std::string EPXmlValue::toStdString()
{
    return m_string;
}

int EPXmlValue::toInteger()
{
    return atoi(m_string.c_str());
}

double EPXmlValue::toDouble()
{
    return atof(m_string.c_str());
}

float EPXmlValue::toFloat()
{
    return (float)atof(m_string.c_str());
}

bool EPXmlValue::toBoolean()
{
    if (!strcmp(m_string.c_str(),"true"))
        return true;
    else
        return false;
}

XML_ARRAY* EPXmlValue::toArray()
{
    return m_array;
}

XML_DICTIONARY* EPXmlValue::toDictionary()
{
    return m_dictionary;
}

EPXmlValue* EPXmlValue::objectAtIndex(int index)
{
    if (m_type == kEPXmlValueArray)
    {
        
        XML_ARRAY::iterator it;
        int i=0;
        for (i=0,it = m_array->begin() ; it != m_array->end() ; ++it,i++)
        {
            if (i == index)
            {
                return (EPXmlValue*)*it;
            }
        }
    }
    
    return NULL;
}

EPXmlValue* EPXmlValue::objectForKey(std::string key)
{
    if (m_type == kEPXmlValueDictionary)
    {
        XML_DICTIONARY::iterator it = m_dictionary->find(key);
        if (it != m_dictionary->end())
            return it->second;
    }
    
    return NULL;
}

bool EPXmlValue::addObject(std::string key, EPXmlValue* object)
{
    if (m_type != kEPXmlValueDictionary || !m_dictionary)
        return false;
    
    m_dictionary->insert(XML_DICTIONARY::value_type(key,object));
    return true;
}

bool EPXmlValue::addObject(std::string key, int value)
{
    EPXmlValue *obj = new EPXmlValue(key);
    obj->setValue(value);
    if (addObject(key, obj))
        return true;
    
    EP_SAFE_DELETE(obj)
    return false;
}

bool EPXmlValue::addObject(std::string key, double value)
{
    EPXmlValue *obj = new EPXmlValue(key);
    obj->setValue(value);
    if (addObject(key, obj))
        return true;
    
    EP_SAFE_DELETE(obj)
    return false;
}

bool EPXmlValue::addObject(std::string key, bool value)
{
    EPXmlValue *obj = new EPXmlValue(key);
    obj->setValue(value);
    if (addObject(key, obj))
        return true;
    
    EP_SAFE_DELETE(obj)
    return false;
}

bool EPXmlValue::addObject(std::string key, const char* value)
{
    if (!value)
        return false;
    
    EPXmlValue *obj = new EPXmlValue(key);
    obj->setValue(value);
    if (addObject(key, obj))
        return true;
    
    EP_SAFE_DELETE(obj)
    return false;
}

bool EPXmlValue::addObject(std::string key, const char* data, int length)
{
    if (!data)
        return false;

    EPXmlValue *obj = new EPXmlValue(key);
    obj->setValue(data,length);
    if (addObject(key, obj))
        return true;
    
    EP_SAFE_DELETE(obj)
    return false;
}

bool EPXmlValue::addObject(std::string key, XML_ARRAY *value)
{
    EPXmlValue *obj = new EPXmlValue(key);
    obj->setArray(value);
    if (addObject(key, obj))
        return true;
    
    EP_SAFE_DELETE(obj)
    return false;
}

bool EPXmlValue::addObject(std::string key, XML_DICTIONARY *value)
{
    EPXmlValue *obj = new EPXmlValue(key);
    obj->setDictionary(value);
    if (addObject(key, obj))
        return true;
    
    EP_SAFE_DELETE(obj)
    return false;
}

bool EPXmlValue::addObject(EPXmlValue* object)
{
    if (m_type != kEPXmlValueArray || !m_array)
        return false;
    
    m_array->push_back(object);
    return true;
}

bool EPXmlValue::addObject(int value)
{
    EPXmlValue *obj = new EPXmlValue;
    obj->setValue(value);
    if (addObject(obj))
        return true;
    
    EP_SAFE_DELETE(obj)
    return false;
}

bool EPXmlValue::addObject(double value)
{
    EPXmlValue *obj = new EPXmlValue;
    obj->setValue(value);
    if (addObject(obj))
        return true;
    
    EP_SAFE_DELETE(obj)
    return false;
}

bool EPXmlValue::addObject(bool value)
{
    EPXmlValue *obj = new EPXmlValue;
    obj->setValue(value);
    if (addObject(obj))
        return true;
    
    EP_SAFE_DELETE(obj)
    return false;
}

bool EPXmlValue::addObject(const char* value)
{
    EPXmlValue *obj = new EPXmlValue;
    obj->setValue(value);
    if (addObject(obj))
        return true;
    
    EP_SAFE_DELETE(obj)
    return false;
}

bool EPXmlValue::addObject(XML_ARRAY *value)
{
    EPXmlValue *obj = new EPXmlValue;
    obj->setArray(value);
    if (addObject(obj))
        return true;
    
    EP_SAFE_DELETE(obj)
    return false;
}

bool EPXmlValue::addObject(XML_DICTIONARY *value)
{
    EPXmlValue *obj = new EPXmlValue;
    obj->setDictionary(value);
    if (addObject(obj))
        return true;
    
    EP_SAFE_DELETE(obj)
    return false;
}

bool EPXmlValue::removeObjectForKey(std::string key)
{
    if (m_type != kEPXmlValueDictionary || !m_dictionary)
        return false;
    
    XML_DICTIONARY::iterator it = m_dictionary->find(key);
    if (it == m_dictionary->end())
        return false;
    
    EP_SAFE_DELETE(it->second)
    
    m_dictionary->erase(it);
    return true;
}

bool EPXmlValue::removeObject(EPXmlValue* object)
{
    if (m_type != kEPXmlValueArray || !m_array)
        return false;
    
    XML_ARRAY::iterator it = find(m_array->begin(),m_array->end(),object);
    if (it == m_array->end())
        return false;
    
    EP_SAFE_DELETE((*it))
    m_array->erase(it);
    return true;
}

void EPXmlValue::jsonAddArrayToObject(cJSON *json,EPXmlValue* value)
{
    XML_ARRAY::iterator it;
    cJSON *arrayJson = cJSON_CreateArray();
    cJSON_AddItemToObject(json, value->getKey().c_str(), arrayJson);
    
    for (it = value->toArray()->begin() ; it != value->toArray()->end() ; ++it)
    {
        jsonAddToObject(arrayJson,(EPXmlValue*)*it);
    }    
}

void EPXmlValue::jsonAddDictionaryToObject(cJSON *json,EPXmlValue* value)
{
    XML_DICTIONARY::iterator it;
    cJSON *dictJson = cJSON_CreateObject();
    cJSON_AddItemToObject(json, value->getKey().c_str(), dictJson);
    
    for (it = value->toDictionary()->begin() ; it != value->toDictionary()->end() ; ++it)
    {
        jsonAddToObject(dictJson,it->second);
    }
}

void EPXmlValue::jsonAddToObject(cJSON *json,EPXmlValue* value)
{
    if (value->getType() == kEPXmlValueString)
        cJSON_AddStringToObject(json,value->getKey().c_str(),value->toString());
    else if (value->getType() == kEPXmlValueData)
        cJSON_AddStringToObject(json,value->getKey().c_str(),value->toString());
    else if (value->getType() == kEPXmlValueInteger)
        cJSON_AddNumberToObject(json,value->getKey().c_str(),value->toInteger());
    else if (value->getType() == kEPXmlValueDouble)
        cJSON_AddNumberToObject(json,value->getKey().c_str(),value->toDouble());
    else if (value->getType() == kEPXmlValueBoolean)
    {
        if (value->toBoolean())
            cJSON_AddTrueToObject(json,value->getKey().c_str());
        else
            cJSON_AddFalseToObject(json,value->getKey().c_str());
    }
    else if (value->getType() == kEPXmlValueDictionary)
        jsonAddDictionaryToObject(json, value);
    else if (value->getType() == kEPXmlValueArray)
        jsonAddArrayToObject(json, value);
    else if (value->getType() == kEPXmlValueDate)
        cJSON_AddStringToObject(json,value->getKey().c_str(),value->toString());
}

char* EPXmlValue::toJSON()
{
    cJSON *root = cJSON_CreateObject();
    jsonAddToObject(root,this);
    char *jsonOutput = cJSON_Print(root);
    cJSON_Delete(root);
    return jsonOutput;
}

char* EPXmlValue::toZipJSON(int *zipLength)
{
    *zipLength = 0;
    
    char *json = toJSON();
    
    char* compressData = (char*)malloc(EP_COMPRESS_MAX_LENGTH);
    memset(compressData,0,EP_COMPRESS_MAX_LENGTH);
    
    long destLen = EP_COMPRESS_MAX_LENGTH;
    
    int ret = ezcompress( (unsigned char*)compressData, &destLen, (const unsigned char*)json, (long)strlen(json));
    
    EP_SAFE_FREE(json)
    
    if (!ret)
    {
        *zipLength = (int)destLen;
        return compressData;
    }
    else
    {
        EP_SAFE_FREE(compressData)
        return NULL;
    }
}

void EPXmlValue::visitJsonArray(cJSON *node,EPXmlValue* value)
{
    if (!m_firstJSONode)
    {
        if (m_type == kEPXmlValueNone)
        {
            m_key = node->string;
            setArray(new XML_ARRAY);
            value = this;
        }    
        else
        {
            EPXmlValue *newValue = new EPXmlValue((node->string)?node->string:"");
            newValue->setArray(new XML_ARRAY);
            
            if (value->getType() == kEPXmlValueDictionary)
            {
                value->toDictionary()->insert(XML_DICTIONARY::value_type(node->string,newValue));
                value = newValue;
            }
            else
            {
                value->toArray()->push_back(newValue);
                value = newValue;
            }
        }
    }
    else
        m_firstJSONode = false;
    
    cJSON *child = node->child;
	while (child)
	{
		visitJsonNode(child,value);
		child=child->next;
	}
}

void EPXmlValue::visitJsonObject(cJSON *node,EPXmlValue* value)
{
    std::string key = (node->string)?node->string:"";
    
    if (!m_firstJSONode)
    {
        if (m_type == kEPXmlValueNone)
        {
            m_key = key.c_str();
            setDictionary(new XML_DICTIONARY);
            value = this;
        }    
        else
        {
            EPXmlValue *newValue = new EPXmlValue(key.c_str());
            newValue->setDictionary(new XML_DICTIONARY);
            
            if (value->getType() == kEPXmlValueDictionary)
            {
                value->toDictionary()->insert(XML_DICTIONARY::value_type(key.c_str(),newValue));
                value = newValue;
            }
            else
            {
                value->toArray()->push_back(newValue);
                value = newValue;
            }
        }
    }
    else
        m_firstJSONode = false;
    
    cJSON *child = node->child;
	while (child)
	{
		visitJsonNode(child,value);
		child=child->next;
	}
}

void EPXmlValue::visitJsonNode(cJSON *node,EPXmlValue* value)
{
    switch ((node->type)&255)
	{
		case cJSON_NULL:
            break;
        case cJSON_False:
        case cJSON_True:
        case cJSON_Number:
        case cJSON_String:
        {
            std::string key;
            if (node->string)
            {
                //dictionary
                key = node->string;
            }
            else
            {
                //array
                std::stringstream st;
                st << std::setprecision(16) << value->toArray()->size();
                key = "item " + st.str();
            }
            
            EPXmlValue *val = new EPXmlValue(key);
            if (((node->type) & 255) == cJSON_False)
            {
                val->setValue(false);
            }
            if (((node->type) & 255) == cJSON_True)
            {
                val->setValue(true);
            }
            if (((node->type) & 255) == cJSON_Number)
            {
                val->setValue(node->valuedouble);
            }
            if (((node->type) & 255) == cJSON_String)
            {
                val->setValue(node->valuestring);
            }
            
            if (value->getType() == kEPXmlValueDictionary)
            {
                value->toDictionary()->insert(XML_DICTIONARY::value_type(key.c_str(),val));
            }
            else
            {
                value->toArray()->push_back(val);
            }
            break;
        }
		case cJSON_Array:
            visitJsonArray(node,value);
            break;
		case cJSON_Object:
            visitJsonObject(node,value);
            break;
	}
}

bool EPXmlValue::loadZipJSON(const char* buffer,int length)
{
    bool ret = false;
    ezbuffer input,output;
    input.pBuf = (unsigned char*)buffer;
    input.nLen = length;
    
    if (!ezuncompress(output, input))
    {
        ret = loadJSON((char*)output.pBuf);
    }
    return ret;
}

bool EPXmlValue::loadJSON(const char* buffer,int length)
{
    if (!buffer)
        return false;
    
    cJSON *root = cJSON_Parse(buffer);
    if (!root)
        return false;
    
    m_firstJSONode = true;
    visitJsonNode(root,NULL);
    
    cJSON_Delete(root);
    return true;
}

int EPXmlValue::integerForKey(std::string key)
{
    EPXmlValue* value = this->objectForKey(key);
    if (value)
        return value->toInteger();
    return 0;
}

double EPXmlValue::doubleForKey(std::string key)
{
    EPXmlValue* value = this->objectForKey(key);
    if (value)
        return value->toDouble();
    return 0;
}

const char* EPXmlValue::stringForKey(std::string key)
{
    EPXmlValue* value = this->objectForKey(key);
    if (value)
        return value->toString();
    return NULL;
}

bool EPXmlValue::booleanForKey(std::string key)
{
    EPXmlValue* value = this->objectForKey(key);
    if (value)
        return value->toBoolean();
    return false;
}

std::string EPXmlValue::stdStringForKey(std::string key)
{
    EPXmlValue* value = this->objectForKey(key);
    if (value)
        return value->toStdString();
    return "";
}

const char* EPXmlValue::dataForKey(std::string key)
{
    EPXmlValue* value = this->objectForKey(key);
    if (value)
        return value->toData();
    return NULL;
}

int EPXmlValue::integerAtIndex(int index)
{
    EPXmlValue* value = this->objectAtIndex(index);
    if (value)
        return value->toInteger();
    return 0;
}

double EPXmlValue::doubleAtIndex(int index)
{
    EPXmlValue* value = this->objectAtIndex(index);
    if (value)
        return value->toDouble();
    return 0;
}

const char* EPXmlValue::stringAtIndex(int index)
{
    EPXmlValue* value = this->objectAtIndex(index);
    if (value)
        return value->toString();
    return NULL;
}

const char* EPXmlValue::dataAtIndex(int index)
{
    EPXmlValue* value = this->objectAtIndex(index);
    if (value)
        return value->toData();
    return NULL;
}

bool EPXmlValue::booleanAtIndex(int index)
{
    EPXmlValue* value = this->objectAtIndex(index);
    if (value)
        return value->toBoolean();
    return false;
}

std::string EPXmlValue::stdStringAtIndex(int index)
{
    EPXmlValue* value = this->objectAtIndex(index);
    if (value)
        return value->toStdString();
    return "";
}

EPXmlValue* EPXmlValue::copy()
{
    EPXmlValue *value = copyObject(NULL,this);
    return value;
}

EPXmlValue* EPXmlValue::copyObject(EPXmlValue* parent,EPXmlValue* input)
{
    EPXmlValue *value = new EPXmlValue(input->getKey());
    
    if (parent)
    {
        if (parent->getType() == kEPXmlValueDictionary)
        {
            parent->toDictionary()->insert(XML_DICTIONARY::value_type(input->getKey(),value));
        }
        else
        {
            parent->toArray()->push_back(value);
        }
    }
    
    if (input->getType() == kEPXmlValueDictionary)
    {
        value->setDictionary(new XML_DICTIONARY);
        
        XML_DICTIONARY::iterator it;
        for (it = input->toDictionary()->begin() ; it != input->toDictionary()->end() ; ++it)
        {
            copyObject(value, it->second);
        }
    }
    else if (input->getType() == kEPXmlValueArray)
    {
        value->setArray(new XML_ARRAY);
        
        XML_ARRAY::iterator it;
        for (it = input->toArray()->begin() ; it != input->toArray()->end() ; ++it)
        {
            copyObject(value, *it);
        }  
    }
    else
    {
        value->m_type = input->m_type;
        value->m_string = input->m_string.c_str();
    }
    return value;
}

void EPXmlValue::dump(EPXmlValue *plist)
{
    char* buf = NULL;
    
    if (plist)
        buf = plist->toJSON();
    else
        buf = this->toJSON();
    
    printf("EPXmlValue dump : \n%s\n",buf);
    
    EP_SAFE_FREE(buf)
}


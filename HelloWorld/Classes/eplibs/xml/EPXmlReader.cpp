//
//  EPXmlReader.cpp
//  t1
//
//  Created by Erawppa on 2011/9/21.
//  Copyright 2011年 __MyCompanyName__. All rights reserved.
//

#include "EPXmlReader.h"
#include "EPMacros.h"
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>

#define MAX_XML_LENGTH 256
#define XMLNODE_FOREACH(n)                  \
    for (n = node ; n ; n = n->next)

#define ELEMENTNODE_FOREACH(n)              \
    for (n = node ; n ; n = n->next)        \
    if(n->type == XML_ELEMENT_NODE)

using namespace std;

enum
{
    kEPXmlNodeNone = 0,
    kEPXmlNodeResult,
    kEPXmlNodeContent
};
typedef int kEPXmlNodeType;

EPXmlReader::EPXmlReader()
:m_doc(NULL)
,m_root(NULL)
,m_rootDictionary(NULL)
{
}

EPXmlReader::~EPXmlReader()
{
    EP_SAFE_DELETE(m_rootDictionary)
    
    if (m_doc)
    {
        xmlFreeDoc(m_doc);
    }

    std::vector<std::string*>::iterator it;
    for (it = m_arrayTable.begin() ; it != m_arrayTable.end() ; ++it)
    {
        std::string *token = (std::string*)*it;
        delete token;
    }
}

bool EPXmlReader::loadFromFile(const char* file)
{
    FILE *fp = fopen(file, "rb");
    if (fp <= 0)
        return false;
    
    fseek(fp, 0, SEEK_END);
    int size = ftell(fp);
    char *buffer = new char[size];
    if (!buffer)
        return false;
    
    fseek(fp, 0, SEEK_SET);
    fread(buffer, 1, size, fp);
    fclose(fp);
    
    m_doc = xmlReadMemory(buffer,size, "utf-8", NULL, XML_PARSE_NOBLANKS);
    if (!m_doc)
    {
        delete buffer;
        return false;
    }
    
    xmlParsing();    
    delete buffer;
    return true;
}

bool EPXmlReader::loadFromBuffer(const char* buffer)
{
    m_doc = xmlReadMemory(buffer,(int)strlen(buffer), "utf-8", NULL, XML_PARSE_NOBLANKS);
    if (!m_doc)
    {
        return false;
    }
    
    xmlParsing();
    return true;
}

void EPXmlReader::addArrayToken(const char* token)
{
    std::string *arrayToken = new std::string(token);
    m_arrayTable.push_back(arrayToken);
}

void EPXmlReader::xmlParsing()
{
    if (m_doc)
    {
        m_root = xmlDocGetRootElement(m_doc);
//        nodeParsing(NULL,m_root, "");
        
        PlistNode* node = new PlistNode;
        STRING_PAIR pair("","");
        nodeParsing(NULL,m_root, &pair);
        
        EP_SAFE_DELETE(node)
        
        m_resultDictionary = m_rootDictionary->objectForKey("result");
        m_contentDictionary = m_rootDictionary->objectForKey("content");
    }
}

void EPXmlReader::nodeParsing(EPXmlValue* data,xmlNodePtr node,STRING_PAIR* pair)
{
    xmlNodePtr curNode;
    
    XMLNODE_FOREACH(curNode)
    {       
        if(curNode->type == XML_ELEMENT_NODE) 
        {
//            CCLOG("XML_ELEMENT_NODE : %s\n",curNode->name);        
            
            bool createDictionary = false;
            if (pair->second.empty() && !pair->first.empty())
                createDictionary = true;
            
            if (createDictionary)
            {                
                EPXmlValue *newValue = new EPXmlValue(pair->first);
                EPXmlValue *array = NULL;
                
                std::vector<std::string*>::iterator it;
                for (it = m_arrayTable.begin() ; it != m_arrayTable.end() ; ++it)
                {
                    std::string *token = (std::string*)*it;
                    if (!pair->first.compare(token->c_str()))
                    {
                        array = data->objectForKey(token->c_str());
                        if (!array)
                        {
                            EPXmlValue *newArray = new EPXmlValue(pair->first);
                            newArray->setArray(new XML_ARRAY);
                            
                            if (data->getType() == kEPXmlValueDictionary)
                                data->toDictionary()->insert(XML_DICTIONARY::value_type(pair->first,newArray));
                            else
                                data->toArray()->push_back(newArray);
                            
                            array = newArray;
                        }
                        break;
                    }
                }
                
                newValue->setDictionary(new XML_DICTIONARY);
                
                if (array)
                {
                    array->toArray()->push_back(newValue);
                }
                else if (data)
                {
                    if (data->getType() == kEPXmlValueDictionary)
                        data->toDictionary()->insert(XML_DICTIONARY::value_type(pair->first,newValue));
                    else
                        data->toArray()->push_back(newValue);
                }
                else
                    m_rootDictionary = newValue;
                
                data = newValue;
                
//                CCLOG("dictionary , %s , %ld\n",pair->first.c_str(),(ssize_t)data);
            }
            
            pair->first = (char*)curNode->name;
        }        
        else if (curNode->type == XML_TEXT_NODE)
        {
//            CCLOG("XML_TEXT_NODE : %s, : %s\n",curNode->name,curNode->content);
            
            if (curNode->content)
            {
                pair->second = (char*)curNode->content;

                EPXmlValue *newValue = new EPXmlValue(pair->first);
                newValue->setString(pair->second);
                
                if (data->getType() == kEPXmlValueDictionary)
                {
                    data->toDictionary()->insert(XML_DICTIONARY::value_type(pair->first,newValue));
                }
                else
                {
                    data->toArray()->push_back(newValue);
                }

//                CCLOG("string pair (%s,%s) , %ld\n",pair->first.c_str(),pair->second.c_str(), (ssize_t)data);
            }
            
            pair->first.clear();
            pair->second.clear();
        }
        
        if(curNode->type == XML_ELEMENT_NODE)
            nodeParsing(data,curNode->xmlChildrenNode, pair);
    }
    
}

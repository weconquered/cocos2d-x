//
//  EPXmlReader.h
//  t1
//
//  Created by Erawppa on 2011/9/21.
//  Copyright 2011年 __MyCompanyName__. All rights reserved.
//

#ifndef __EP_XML_PARSER_H__
#define __EP_XML_PARSER_H__

#include "cocos2d.h"
#include "EPXmlValue.h"
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <string>
#include <map>
#include <vector>


class EPXmlReader
{
public:
    EPXmlReader();
    
    ~EPXmlReader();
    
    bool loadFromFile(const char* file);
    bool loadFromBuffer(const char* buffer);
    void addArrayToken(const char* token);
    CC_SYNTHESIZE_READONLY(EPXmlValue*, m_rootDictionary, RootDictionary)
    CC_SYNTHESIZE_READONLY(EPXmlValue*, m_resultDictionary, ResultDictionary)
    CC_SYNTHESIZE_READONLY(EPXmlValue*, m_contentDictionary, ContentDictionary)
    
private:
    void xmlParsing();
    void nodeParsing(EPXmlValue* data,xmlNodePtr node,STRING_PAIR* pair);
    
    xmlDocPtr m_doc;
    xmlNodePtr m_root;
    std::vector<std::string*> m_arrayTable;
};

#endif//__EP_XML_PARSER_H__
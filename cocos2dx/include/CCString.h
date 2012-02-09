/****************************************************************************
Copyright (c) 2010 cocos2d-x.org

http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/
#ifndef __CCSTRING_H__
#define __CCSTRING_H__

#include <string>
#include <stdlib.h>
#include <cctype>
#include <iostream>
#include <algorithm>
#include <vector>
#include "CCObject.h"

namespace cocos2d {

    struct	str_tolower
    { 
        char operator()(char c) const 
        {
            return std::tolower(static_cast<unsigned char>(c));
        } 
    };

    struct str_toupper
    { 
        char operator()(char c) const 
        {
            return std::toupper(static_cast<unsigned char>(c));
        } 
    };

    int CC_DLL sprintfw(unsigned short* _Dest,const unsigned short* _format,...);

	class CC_DLL CCString : public CCObject
	{
        DECLARE_KINDOF(CCString,CCObject);
    protected: 
        bool		mIsWC;
        unsigned short*		mWBuffer;
    public:
		std::string m_sString;
	public:
		CCString()
			: m_sString("")
            , mWBuffer(0)
            , mIsWC(false)
		{}

        const unsigned short* getWBuffer() const
        {
            if(mIsWC)
            {
                return mWBuffer;
            }
            return 0;
        }

        CCString(const char * str):
            mWBuffer(0)
        {
            m_sString = str;
            mIsWC=false;
        }
        CCString(const unsigned short * str):
            mWBuffer(0)
        {
            int l=0;

            const unsigned short * lstr=str;

            while(*lstr != 0)
            {
                m_sString+=(char)(*lstr++);
                l++;
            }
            l++;

            mWBuffer=new unsigned short[l];
            memcpy(mWBuffer,str,l*sizeof(unsigned short));

            mIsWC=true;
        }


		virtual ~CCString()
        { 
            m_sString.clear(); 
            if(mWBuffer)
            {
                delete [] mWBuffer;
            }
        }

        unsigned short*	toWBuffer()
        {
            if(mWBuffer)
            {
                delete [] mWBuffer;
            }
            int l=m_sString.length();
            l++;
            mWBuffer=new unsigned short[l];

            char* read=(char*)m_sString.c_str();
            unsigned short* write=mWBuffer;
            while(*read)
            {
                *write++=(unsigned short)(*read++);
            }
            mWBuffer[l-1]=0;

            mIsWC=true;

            return mWBuffer;
        }

		int toInt()
		{
			return atoi(m_sString.c_str());
		}
		unsigned int toUInt()
		{
			return (unsigned int)atoi(m_sString.c_str());
		}
		float toFloat()
		{
			return (float)atof(m_sString.c_str());
		}
		std::string toStdString()
		{
			return m_sString;
		}

		bool isEmpty()
		{
			return m_sString.empty();
		}

        virtual bool isEqual(const CCObject* pObject)
        {
            bool bRet = false;
            const CCString* pStr = dynamic_cast<const CCString*>(pObject);
            if (pStr != NULL)
            {
                if (0 == m_sString.compare(pStr->m_sString))
                {
                    bRet = true;
                }
            }
            return bRet;
        }

        static std::string capitalizedString(std::string &s)
        {
            unsigned int size = s.size();

            for(unsigned int i = 1; i < size; i++)
            {
                //La lettre precédente est un espace 
                if (s.at(i-1) == ' ')
                {
                    char c = s[i];
                    char buffer[10];
                    sprintf(buffer,"%s",c);
                    std::string tmp = buffer;
                    std::transform(tmp.begin(),tmp.end(),tmp.begin(), str_toupper());
                    s.replace(i,1,tmp.c_str());
                }
            }
            return s;
        }

        static std::vector<std::string> split(const std::string &s, char delim)
        {
            std::vector<std::string> elems;
            std::string ss = s;
            int start=0;
            int pos=0;
            while(!s.empty())
            {
                pos = ss.find_first_of(delim, pos+1);
                elems.push_back(ss.substr(start, pos-start));
                start=pos+1;
                if(pos==-1)
                    break;
            }

            return elems;
        }

        static void stringByReplacingOccurrencesOfString(std::string &_string, const std::string occurence, const std::string value)
        {
            std::vector<int> position;

            if(_string.find(occurence) != std::string::npos)
            {
                position.push_back(_string.find(occurence));
            }
            else
            {
                //No occurence found
                return;
            }

            //Add all occurence position
            while(_string.find(occurence, position[position.size()-1]+1) != std::string::npos)
            {
                position.push_back(_string.find(occurence, position[position.size()-1]+1));
            }

            //Replace all occurence by value
            std::vector<int>::iterator it = position.begin();

            while (it != position.end())
            {
                _string.replace(*it,occurence.size(), value.c_str());
                it++;
            }

        }

        CCString* stringByReplacingOccurrencesOfString(const std::string occurence, const std::string value)
        {
            CCString* copy = NULL;
            std::vector<int> position;

            if(m_sString.find(occurence) != std::string::npos)
            {
                copy = new CCString (m_sString.c_str());
                position.push_back(copy->m_sString.find(occurence));
            }
            else
            {
                //No occurence found
                return this;
            }

            //Add all occurence position
            while(copy->m_sString.find(occurence, position[position.size()-1]+1) != std::string::npos)
            {
                position.push_back(copy->m_sString.find(occurence, position[position.size()-1]+1));
            }

            //Replace all occurence by value
            std::vector<int>::iterator it = position.begin();

            while (it != position.end())
            {
                copy->m_sString.replace(*it,occurence.size(), value.c_str());
                it++;
            }
            return copy;
        }
        static std::vector<std::string> split(const std::string &s,const std::string & delim)
        {
            std::vector<std::string> elems;
            std::string ss = s;
            int start=0;
            int pos=0;
            while(!s.empty())
            {
                pos = ss.find_first_of(delim, pos+1);
                elems.push_back(ss.substr(start, pos-start));
                start=pos+1;
                if(pos==-1)
                    break;
            }

            return elems;
        }

        bool	hasPrefix(const std::string& test)
        {
            if(m_sString.substr(0,test.length()) == test)
            {
                return true;
            }
            return false;
        }

        unsigned int length()
        {
            return m_sString.length();
        }

        bool operator <  (const CCString& r) const { return m_sString<r.m_sString; }
	};
}// namespace cocos2d
#endif //__CCSTRING_H__
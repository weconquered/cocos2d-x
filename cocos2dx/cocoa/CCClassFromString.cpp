/****************************************************************************
Copyright (c) 2010-2011 cocos2d-x.org
Copyright (c) 2011 Stephane Capo, Nicolas Blondel, Quentin Stere, Noel Leron

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

#include "CCObject.h"
#include "CCString.h"

using namespace cocos2d;

static void printcharw(unsigned short **str, int c)
{
	**str = (unsigned short)c;
	++(*str);
}

#define PAD_RIGHT 1
#define PAD_ZERO 2

static int printsw(unsigned short **out, const unsigned short *string, int width, int pad)
{
	int pc = 0, padchar = ' ';

	if (width > 0) {
		int len = 0;
		const unsigned short *ptr;
		for (ptr = string; *ptr; ++ptr) ++len;
		if (len >= width) width = 0;
		else width -= len;
		if (pad & PAD_ZERO) padchar = '0';
	}
	if (!(pad & PAD_RIGHT)) {
		for ( ; width > 0; --width) {
			printcharw (out, padchar);
			++pc;
		}
	}
	for ( ; *string ; ++string) {
		printcharw (out, *string);
		++pc;
	}
	for ( ; width > 0; --width) {
		printcharw (out, padchar);
		++pc;
	}

	return pc;
}

/* the following should be enough for 32 bit int */
#define PRINT_BUF_LEN 12

static int printiw(unsigned short **out, int i, int b, int sg, int width, int pad, int letbase)
{
	unsigned short print_buf[PRINT_BUF_LEN];
	unsigned short *s;
	int t, neg = 0, pc = 0;
	unsigned int u = i;

	if (i == 0) {
		print_buf[0] = '0';
		print_buf[1] = '\0';
		return printsw (out, print_buf, width, pad);
	}

	if (sg && b == 10 && i < 0) {
		neg = 1;
		u = -i;
	}

	s = print_buf + PRINT_BUF_LEN-1;
	*s = '\0';

	while (u) {
		t = u % b;
		if( t >= 10 )
			t += letbase - '0' - 10;
		*--s = t + '0';
		u /= b;
	}

	if (neg) {
		if( width && (pad & PAD_ZERO) ) {
			printcharw (out, '-');
			++pc;
			--width;
		}
		else {
			*--s = '-';
		}
	}

	return pc + printsw (out, s, width, pad);
}


static int printw(unsigned short **out,const unsigned short *format, va_list args )
{
	int width, pad;
	int pc = 0;
	unsigned short scr[2];

	for (; *format != 0; ++format) {
		if (*format == '%') {
			++format;
			width = pad = 0;
			if (*format == '\0') break;
			if (*format == '%') goto out;
			if (*format == '-') {
				++format;
				pad = PAD_RIGHT;
			}
			while (*format == '0') {
				++format;
				pad |= PAD_ZERO;
			}
			for ( ; *format >= '0' && *format <= '9'; ++format) {
				width *= 10;
				width += *format - '0';
			}
			if( *format == 's' ) {
				unsigned short *s = (unsigned short *)va_arg( args, int );
				if(s)
				{
					pc += printsw (out, s, width, pad);
				}
				continue;
			}
			if( (*format == 'd')||(*format == 'i') ) {
				pc += printiw (out, va_arg( args, int ), 10, 1, width, pad, 'a');
				continue;
			}
			if( *format == 'x' ) {
				pc += printiw (out, va_arg( args, int ), 16, 0, width, pad, 'a');
				continue;
			}
			if( *format == 'X' ) {
				pc += printiw (out, va_arg( args, int ), 16, 0, width, pad, 'A');
				continue;
			}
			if( *format == 'u' ) {
				pc += printiw (out, va_arg( args, int ), 10, 0, width, pad, 'a');
				continue;
			}
			if( *format == 'c' ) {
				/* char are converted to int then pushed on the stack */
				scr[0] = (unsigned short)va_arg( args, int );
				scr[1] = '\0';
				pc += printsw (out, scr, width, pad);
				continue;
			}
		}
		else {
		out:
			printcharw(out, *format);
			++pc;
		}
	}
	if (out) **out = '\0';
	va_end( args );
	return pc;
}


int cocos2d::sprintfw(unsigned short* _Dest,const unsigned short* _format,...)
{

	va_list args;
	va_start( args, _format );
	return printw( &_Dest, _format, args );

}

CC_DLL CCClassFromStringFactory	cocos2d::globalCCClassFactory;	


CCClassFromStringFactory::~CCClassFromStringFactory()
{
}

CCClassFromStringFactory::CCClassFromStringFactory()
{
}

void CCClassFromStringFactory::addClass(const char* classname,ClassCreator create)
{
	std::string classnamestring=classname;
	myCreatorMap[classnamestring]=create;
}

CCObject*	CCClassFromStringFactory::createInstance(const std::string& className)
{
	std::map<std::string,ClassCreator>::iterator	itfound=myCreatorMap.find(className);
	if(itfound != myCreatorMap.end())
	{
		return (*(*itfound).second)();
	}
	CCLOG("Class :%s not registered in factory\n",className.c_str());
	return 0;
}

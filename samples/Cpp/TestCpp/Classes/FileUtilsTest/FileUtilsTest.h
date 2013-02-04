#ifndef __FILEUTILSTEST_H__
#define __FILEUTILSTEST_H__

#include "../testBasic.h"
USING_NS_CC;
using namespace std;

class FileUtilsTestScene : public TestScene
{
public:
    virtual void runThisTest();
};

class FileUtilsDemo : public CCLayer
{
public:
    virtual void onEnter();
    virtual string title();
    virtual string subtitle();
    void backCallback(CCObject* pSender);
    void nextCallback(CCObject* pSender);
    void restartCallback(CCObject* pSender);
};

class TestResolutionDirectories : public FileUtilsDemo
{
public:
    virtual void onEnter();
    virtual void onExit();
    virtual string title();
    virtual string subtitle();
private:
    vector<string> m_defaultSearchPathArray;
    vector<string> m_defaultResolutionsOrderArray;
};

class TestSearchPath : public FileUtilsDemo
{
public:
    virtual void onEnter();
    virtual void onExit();
    virtual string title();
    virtual string subtitle();
private:
    void createFile(const std::string& strFilePath, const std::string& strContent);
    void readFileContents(const std::string& strFilePath);
    vector<string> m_defaultSearchPathArray;
    vector<string> m_defaultResolutionsOrderArray;
};

class TestFilenameLookup : public FileUtilsDemo
{
public:
    virtual void onEnter();
    virtual void onExit();
    virtual string title();
    virtual string subtitle();
};

class TestIsFileExist : public FileUtilsDemo
{
public:
    virtual void onEnter();
    virtual void onExit();
    virtual string title();
    virtual string subtitle();
};

class TestCreateDirectory : public FileUtilsDemo
{
public:
    virtual void onEnter();
    virtual void onExit();
    virtual string title();
    virtual string subtitle();
};

#endif /* __FILEUTILSTEST_H__ */

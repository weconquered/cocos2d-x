

#ifndef  _RUNTIME_CONFIG_H_
#define  _RUNTIME_CONFIG_H_


class  RuntimeConfig
{
public:
	static RuntimeConfig* getInstance();
	void setSearchPath();
	void waitConnect();
private:
    RuntimeConfig();
    virtual ~RuntimeConfig();

};

#endif // _RUNTIME_CONFIG_H_


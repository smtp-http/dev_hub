#ifndef __HTTP_H__
#define __HTTP_H__

#include <string>
#include "connbase.h"

using namespace std;
using namespace lux;
class HttpPoster : public IUpdateSink
{
public:
	static void UpdateInfo();
protected:
	string m_url;
};

class SolderHttpPoster : public HttpPoster
{
public:
	SolderHttpPoster(string url);
	//virtual void UpdateInfo(const char *buf,int len);

};

class BalanceHttpPoster : public HttpPoster
{
public:
	BalanceHttpPoster(string url);
	//virtual void UpdateInfo(const char *buf,int len);

};

class GlueDispenserHttpPoster : public HttpPoster
{
public:
	GlueDispenserHttpPoster(string url);
	//virtual void UpdateInfo(const char *buf,int len);

};

#endif


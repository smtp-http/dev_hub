#ifndef __FUNCTION_MANAGER_H__
#define __FUNCTION_MANAGER_H__

#include "config.h"


class FunctionManager : public lux::IConfigUser {
public:
	FunctionManager();
	~FunctionManager();
	static FunctionManager& GetInstance();
	virtual void Update(const Json::Value &value);
};


#endif

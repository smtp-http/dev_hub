#ifndef __SEQUENCE_H__ //sequence
#define __SEQUENCE_H__

#include <string>
#include "connbase.h"

namespace lux {

class SolderCtrl;



class SequenceState 
{
public:
	virtual int RecvDownStream(const std::string msg) = 0;
	virtual int RecvUpstream(const char *data,int len) = 0;
protected:
	void ChangeState(SequenceState *s);
	

	SolderCtrl *m_sequenceEquipmen;
};

class SequenceIdleState : public SequenceState
{
public:
	static SequenceIdleState & Instance(SequenceType type,SolderCtrl *equip);
	virtual int RecvDownStream(const std::string msg);
	virtual int RecvUpstream(const char *data,int len);
	
};


class SequenceUpLoadState : public SequenceState
{
public:
	static SequenceUpLoadState & Instance(SequenceType type,SolderCtrl *equip);
	virtual int RecvDownStream(const std::string msg);
	virtual int RecvUpstream(const char *data,int len);
};

class SequenceUpLoadRepState : public SequenceState
{
public:
	static SequenceUpLoadRepState & Instance(SequenceType type,SolderCtrl *equip);
	virtual int RecvDownStream(const std::string msg);
	virtual int RecvUpstream(const char *data,int len);
};

class SequenceUpLoadRepA0State : public SequenceState
{
public:
	static SequenceUpLoadRepA0State & Instance(SequenceType type,SolderCtrl *equip);
	virtual int RecvDownStream(const std::string msg);
	virtual int RecvUpstream(const char *data,int len);
};



class SequenceDownLoadState : public SequenceState
{
public:
	static SequenceDownLoadState & Instance(SequenceType type,SolderCtrl *equip);
	virtual int RecvDownStream(const std::string msg);
	virtual int RecvUpstream(const char *data,int len);
};


};


#endif

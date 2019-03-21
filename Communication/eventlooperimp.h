#ifndef __LUXEVENTLOOPERIMP_H__
#define __LUXEVENTLOOPERIMP_H__

#include <list>
//#include <event.h>
#include <event2/event.h>
#include "eventlooper.h"

using namespace std;

namespace lux{

struct TimerElement{
	TimerID m_id;
	ITimerUserSink *m_sink;
	TimerFlag m_flag;
	struct timeval m_tmInterval;
	struct event *m_event;
};

class EventLooperImp : public EventLooper
{
	struct event_base *m_evBase;
	typedef std::list<TimerElement*> TimerList;
	TimerList m_timers;
	TimerID m_nextID;
public:
	EventLooperImp();
	~EventLooperImp();
public:
	event_base* CurrentEvBase() const;
	
	virtual void RunEventLoop();

	virtual void StopEventLoop(long timeAfter);
	
	virtual TimerID ScheduleTimer(struct timeval *tv, TimerFlag flag, ITimerUserSink *sink);
	
	virtual void CancelTimer(TimerID tid);

	void OnTimeout(TimerElement *te);
private:
	TimerID GetNextID();
};

};
#endif //__LUXEVENTLOOPERIMP_H__

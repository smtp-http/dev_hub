#include <iostream>
#include <algorithm>
#include <functional>
#include "eventlooperimp.h"

using namespace lux;

EventLooper::EventLooper()
{
}

EventLooper& EventLooper::GetInstance()
{
	static EventLooperImp s_inst;
	return s_inst;
}
	
void EventLooper::RunEventLoop()
{
}

void EventLooper::StopEventLoop(long timeAfter)
{
}

TimerID EventLooper::ScheduleTimer(struct timeval *tv, TimerFlag flag, ITimerUserSink *sink)
{
	return (TimerID)0;
}

void EventLooper::CancelTimer(TimerID tid)
{
}
	
static void on_timeout(int fd, short events, void * args)
{
	EventLooperImp &el = (EventLooperImp&)EventLooper::GetInstance();
	el.OnTimeout((TimerElement*)args);
}

EventLooperImp::EventLooperImp()
	: m_evBase(event_base_new())
	, m_nextID(1)
{
}

EventLooperImp::~EventLooperImp()
{
	event_base_free(m_evBase);
	m_evBase = NULL;
}

event_base* EventLooperImp::CurrentEvBase() const
{
	return m_evBase;
}

void EventLooperImp::RunEventLoop()
{
	if (m_evBase){
		event_base_dispatch(m_evBase);
	}
}

void EventLooperImp::StopEventLoop(long timeAfter)
{
	if (m_evBase){
		struct timeval tv = {timeAfter/1000, (timeAfter%1000)*1000};
		event_base_loopexit(m_evBase, &tv);
	}
}

class TimerCompLt
{
public:
	TimerCompLt() {}
	bool operator() (const TimerElement *te, const TimerElement *te2){
		return te->m_id < te2->m_id;
	}
};

TimerID EventLooperImp::ScheduleTimer(struct timeval *tv, TimerFlag flag, ITimerUserSink *sink)
{
	if (!tv || !sink)
		return -1;

	short ef = 0;
	if (TF_FIRE_PERIODICALLY == flag){
		ef = EV_PERSIST;
	}

	TimerElement *te = new TimerElement();
	te->m_id = GetNextID();
	te->m_flag = flag;
	te->m_sink = sink;
	te->m_tmInterval = *tv;

	struct event *ev = event_new(m_evBase, -1, ef, on_timeout, te);
	if (!ev){
		delete te;
		return -1;
	}
	te->m_event = ev;
	if (event_add(ev, tv) != 0)
		return -1;

	TimerList::iterator it = m_timers.begin();
	if (it == m_timers.end()){
		m_timers.push_back(te);
	}else{
		for(; it != m_timers.end(); it++){
			if ((*it)->m_id >= te->m_id){
				m_timers.insert(it, te);
				break;
			}
		}
	}

	//std::cout << "&&&& ScheduleTimer: " << te->m_id << std::endl;

	return (TimerID)te->m_id;
}

class TimerCompEq
{
	TimerID m_tid;
public:
	TimerCompEq(TimerID tid) : m_tid(tid) {}
	bool operator() (const TimerElement *te){
		return m_tid == te->m_id;
	}
};

void EventLooperImp::CancelTimer(TimerID tid)
{
	//auto cmp = [tid](const TimerElement &te) { return te.m_id == tid };
	//std::pointer_to_binary_function<const TimerElement&, TimerID, bool> cmp(IsTheTimerElement);
	//std::cout << "&&&& CancelTimer: "  << tid << std::endl;
	TimerCompEq te(tid);
	TimerList::iterator it = std::find_if(m_timers.begin(), m_timers.end(), te);
	if (it != m_timers.end()){
		//printf("&--\n");
		event_del((*it)->m_event);
		event_free((*it)->m_event);
		m_timers.erase(it);
	}
}


void EventLooperImp::OnTimeout(TimerElement *te)
{
	if (te){
		te->m_sink->OnTimer(te->m_id);
		if (te->m_flag == TF_FIRE_ONCE){
			m_timers.erase(std::find(m_timers.begin(), m_timers.end(), te));
		}
	}
}

TimerID EventLooperImp::GetNextID()
{
	return m_nextID++;
}

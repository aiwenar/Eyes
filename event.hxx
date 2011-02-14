#ifndef _eyes_event_hxx
#define _eyes_event_hxx

#include <QtCore/QVector>

#include "defines.hxx"

enum _ev_type
{
  none,
  plask,
	quit,
	pet,
};

class Event
{
  public:
		_ev_type	type;
};

class EventQueue
{
  public:
            EventQueue ();
					 ~EventQueue ();
    void    push  ( Event * ev );
    Event * poll ();
	  bool		poll ( Event * ev );
  private:
		uint32	size;
    QVector	<Event*> queue;
};

#endif //eyes_event_hxx

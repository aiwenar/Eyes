#include "event.hxx"

EventQueue::EventQueue ()
{
	size = 0;
}

EventQueue::~EventQueue ()
{
	queue.clear ();
	delete &queue, &size;
}

void EventQueue::push ( Event * ev )
{
  queue.push_back ( ev );
	size ++;
}

Event * EventQueue::poll ()
{
  if ( not ( size > 0 ) )
    return ( nil );
  Event * ev;
  ev = queue.at ( 0 );
	queue.pop_front ();
	size --;
	return ( ev );
}

bool EventQueue::poll ( Event * ev )
{
	if ( not ( size > 0 ) )
	  return ( false );
  ev = queue.at ( 0 );
	queue.pop_front ();
	size --;
	return ( true );
}
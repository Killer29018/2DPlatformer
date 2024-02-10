#pragma once

#include <list>

#include "Events.hpp"

class EventObserver
{
  public:
    virtual ~EventObserver(){};

    virtual void receiveEvent(const Event* event) = 0;
};

class EventDispatcher
{
  public:
    virtual ~EventDispatcher(){};

    virtual void attach(EventObserver* observer);
    virtual void detach(EventObserver* observer);

  protected:
    std::list<EventObserver*> m_Observers;

  protected:
    virtual void notify(const Event* event);
};

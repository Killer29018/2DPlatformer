#include "EventHandler.hpp"

#include <algorithm>

void EventDispatcher::attach(EventObserver* observer) { m_Observers.push_back(observer); }

void EventDispatcher::detach(EventObserver* observer)
{
    auto pos = std::find(m_Observers.begin(), m_Observers.end(), observer);
    m_Observers.erase(pos);
}

void EventDispatcher::notify(const Event* event)
{
    for (auto it = m_Observers.begin(); it != m_Observers.end(); it++)
    {
        (*it)->receiveEvent(event);
    }
}

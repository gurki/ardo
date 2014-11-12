#include "NotifciationCenter.h"


map<string, NotificationCenter::NotifiableRefSet> NotificationCenter::observers;


////////////////////////////////////////////////////////////////////////////////
void NotificationCenter::registerObserver(
    const string& event,
    const shared_ptr<Notifiable>& observer)
{
    observers.at(event).insert(observer);
}


////////////////////////////////////////////////////////////////////////////////
void NotificationCenter::unregisterObserver(
    const string& event,
    const shared_ptr<Notifiable>& observer)
{
    observers.at(event).erase(observer);
}


////////////////////////////////////////////////////////////////////////////////
void NotificationCenter::notify(
   const string& event)
{
    for (const auto& observer : observers[event]) {
        observer->notify(event);
    }
}
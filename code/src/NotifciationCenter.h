#ifndef NOTIFICATION_CENTER_H
#define NOTIFICATION_CENTER_H

#include <memory>
#include <vector>
#include <set>
#include <map>
#include <string>

using namespace std;


////////////////////////////////////////////////////////////////////////////////
class Notifiable
{
    public:
    
//        Notifiable() {};

        virtual void notify(
            const string& event
        ) = 0;
};


////////////////////////////////////////////////////////////////////////////////
class NotificationCenter
{
    public:
        
        static void registerObserver(
            const string& event,
            const shared_ptr<Notifiable>& observer
        );
        
        static void unregisterObserver(
           const string& event,
           const shared_ptr<Notifiable>& observer
        );
        
        static void notify(
            const string& event
        );
        
        
    private:
        
        typedef shared_ptr<Notifiable> NotifiableRef;
        typedef set<NotifiableRef> NotifiableRefSet;
        static map<string, NotifiableRefSet> observers;
    
        NotificationCenter() {};
        ~NotificationCenter() {};
};


#endif
#include "observer.hpp"

#include <map>
#include <set>
#include <list>
#include <random>

using namespace util;

struct ISubject::impl {
    std::map<unsigned long, std::function<void()>> observer_list;
};

std::set<unsigned long> observer_ids;
std::mt19937 rng(static_cast<unsigned long>(0));
unsigned long get_next_id(){
    bool found = false;
    unsigned long next_id;
    unsigned int count = 0;
    while(!found && count < 5000){
        next_id = rng();
        if(observer_ids.find(next_id) == observer_ids.end()){
            observer_ids.insert(next_id);
            return next_id;
        }
        count++;
    }
    return 0;
}

IObserver::IObserver()
{
    observer_id = get_next_id();
}

unsigned long IObserver::getObserverID() const
{
    return this->observer_id;
}

ISubject::ISubject():
    pImpl(std::make_unique<ISubject::impl>())
{}

ISubject::~ISubject() = default;

void ISubject::subscribe(const IObserver *const observer, const std::function<void()> notify)
{
    if(observer != nullptr){
        unsigned long id = observer->getObserverID();
        pImpl->observer_list[id] = notify;
    }
}
bool ISubject::unsubscribe(const IObserver *const observer)
{
    if(observer != nullptr){
        return pImpl->observer_list.erase(observer->getObserverID()) == 1;
    }
}

void ISubject::notifyAll()
{
    std::list<unsigned long> remove_list;
    for(auto it = pImpl->observer_list.cbegin(); it != pImpl->observer_list.cend(); ++it){
        try{
            it->second();
        }
        catch(std::exception exception){
            remove_list.push_back(it->first);
        }
    }

    // remove entries where notification failed
    for(auto invalid_observer: remove_list){
        pImpl->observer_list.erase(invalid_observer);
    }
}

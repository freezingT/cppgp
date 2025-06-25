#pragma once

#include <memory>
#include <functional>

namespace gp::util {

/**
 * Interface to allow observation of a ISubject class.
 * This class can subscribe at an ISubject and the provided function
 * gets called by ISubject at certain events.
 */
class IObserver {
public:
    /**
     * \brief default constructor.
     */
    IObserver();

    /**
     * \brief Get unique object id.
     * \return the object id
     */
    unsigned long getObserverID() const;

private:
    unsigned long observer_id;
};


/**
 * Interface to allow notification of subscribed IObserver classes.
 * It provides public functions for IObserver classes to subscribe
 * and unsubscribe. Further, this class provides a protected member
 * function notifyAll that notifies all subscribed IObserver classes.
 */
class ISubject {
public:
    ISubject();
    ~ISubject();

    /**
     * \brief
     */
    void subscribe(const IObserver* const observer, const std::function<void()> notify);

    /**
     *
     */
    bool unsubscribe(const IObserver* const observer);

protected:
    /**
     *
     */
    void notifyAll();

private:
    struct impl;
    std::unique_ptr<impl> pImpl;
};

} // namespace gp::util

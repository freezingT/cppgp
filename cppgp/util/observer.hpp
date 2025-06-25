#pragma once

#include <memory>
#include <functional>

namespace gp::util {

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
    void notifyAll();

private:
    struct impl;
    std::unique_ptr<impl> pImpl;
};

} // namespace gp::util

#include <cppgp/util/observer.hpp>

#include <gtest/gtest.h>

int status1, status2;

class ObsClass1 : public util::IObserver {
public:
    void getNotified(){status1++;};
};

class ObsClass2 : public util::IObserver {
public:
    void getNotified(){status2++;};
};

class Client : public util::ISubject {
public:
    void doNotify() {this->notifyAll();}
};


TEST(observer, observer_observation_id){
    ObsClass1 o1, o2;
    ObsClass2 o3;
    EXPECT_NE(o1.getObserverID(), o2.getObserverID());
    EXPECT_NE(o1.getObserverID(), o3.getObserverID());
    EXPECT_NE(o2.getObserverID(), o3.getObserverID());
}


TEST(observer, observer_notify_subscribe){
    status1 = 0;
    status2 = 0;

    ObsClass1 o1;
    Client c;
    ASSERT_EQ(status1, 0);
    auto tmp = std::bind(&ObsClass1::getNotified, &o1);
    c.subscribe(&o1, tmp);
    ASSERT_EQ(status1, 0);

    c.doNotify();
    ASSERT_EQ(status1, 1);
    ASSERT_EQ(status2, 0);
    c.doNotify();
    ASSERT_EQ(status1, 2);

    c.unsubscribe(&o1);
    c.doNotify();
    ASSERT_EQ(status1, 2);
    ASSERT_EQ(status2, 0);
}

TEST(observer, observer_notify_subscribe2){
    status1 = 0;
    status2 = 0;

    ObsClass1 o1, o2;
    ObsClass2 o3;
    Client c;
    ASSERT_EQ(status1, 0);
    ASSERT_EQ(status2, 0);
    c.subscribe(&o1, std::bind(&ObsClass1::getNotified, &o1));
    c.subscribe(&o2, std::bind(&ObsClass1::getNotified, &o2));
    c.subscribe(&o3, std::bind(&ObsClass2::getNotified, &o3));
    ASSERT_EQ(status1, 0);
    ASSERT_EQ(status2, 0);

    c.doNotify();
    ASSERT_EQ(status1, 2);
    ASSERT_EQ(status2, 1);

    c.unsubscribe(&o1);
    c.doNotify();
    ASSERT_EQ(status1, 3);
    ASSERT_EQ(status2, 2);

    c.unsubscribe(&o3);
    c.doNotify();
    ASSERT_EQ(status1, 4);
    ASSERT_EQ(status2, 2);
}

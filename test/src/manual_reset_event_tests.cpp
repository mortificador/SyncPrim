#include "manual_reset_event.h"
#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include <thread>
#include <atomic>

namespace {

    TEST(manual_reset_event_test, basic_wait)
    {
        sync_prim::manual_reset_event manual_reset;

        int checker = 1;
        std::thread t1([&]() {
            manual_reset.wait_one();
            checker = 4;
        });
        std::thread t2([&]() {
            std::this_thread::sleep_for(std::chrono::seconds(1));
            checker = 3;
            manual_reset.set();
        });

        t1.join();
        t2.join();
        EXPECT_EQ(4, checker);
    }

    TEST(manual_reset_event_test, multiple_threads_waiting)
    {
        sync_prim::manual_reset_event manual_reset;

        std::atomic<int> count{0};
        std::thread t1([&]() {
            manual_reset.wait_one();
            count++;
        });
        std::thread t2([&]() {
            manual_reset.wait_one();
            count++;
        });
        std::thread t3([&]() {
            manual_reset.wait_one();
            count++;
        });
        std::thread t4([&]() {
            std::this_thread::sleep_for(std::chrono::seconds(1));
            manual_reset.set();
        });

        t1.join();
        t2.join();
        t3.join();
        t4.join();
        EXPECT_EQ(3, count.load());
    }

    TEST(manual_reset_event_test, initially_set)
    {
        sync_prim::manual_reset_event manual_reset{true};

        std::thread t1([&]() {
            manual_reset.wait_one();
        });

        t1.join();
    }

    TEST(manual_reset_event_test, set_reset)
    {
        sync_prim::manual_reset_event manual_reset;

        std::thread t1([&]() {
            manual_reset.wait_one();
        });

        std::thread t2([&]() {
            std::this_thread::sleep_for(std::chrono::seconds(1));
            manual_reset.set();
        });

        t1.join();
        t2.join();

        manual_reset.reset();
        int check = 1;
        t1 = std::thread([&]() {
            manual_reset.wait_one();
            check = 2;
        });

        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        ASSERT_EQ(1, check);

        manual_reset.set();
        std::this_thread::sleep_for(std::chrono::milliseconds(500));

        t1.join();
        ASSERT_EQ(2, check);
    }

    TEST(manual_reset_event_test, wait_for)
    {
        sync_prim::manual_reset_event manual_reset;

        EXPECT_EQ(false, manual_reset.wait_one(std::chrono::milliseconds(30)));
    }
}
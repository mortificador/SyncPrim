#include "count_down_event.h"
#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include <thread>
#include <atomic>

namespace {

    TEST(count_down_event, basic_wait) {
        sync_prim::count_down_event cde(2);

        std::atomic<int> awakes_send{0};
        std::thread t1([&]() {
            std::this_thread::sleep_for(std::chrono::seconds(1));
            if(cde.signal())
            {
                    awakes_send++;
            }
            if(cde.signal())
            {
                    awakes_send++;
            }
        });
        std::thread t2([&]() {
            std::this_thread::sleep_for(std::chrono::seconds(1));
            if(cde.signal())
            {
                    awakes_send++;
            }
        });

        std::thread t3([&]() {
            cde.wait();
        });

        t3.join();
        t1.join();
        t2.join();

        EXPECT_EQ(1, awakes_send.load());
    }


    TEST(count_down_event, multiple_wait) {
        const int count = 50000;
        sync_prim::count_down_event cde(count);

        std::atomic<int> awakes_send{0};

        std::thread t1([&]() {
            std::this_thread::sleep_for(std::chrono::seconds(1));
            for(int i = 0; i < count; i++) {
                if (cde.signal()) {
                    awakes_send++;
                }
            }
        });
        std::thread t2([&]() {
            std::this_thread::sleep_for(std::chrono::seconds(1));
            for(int i = 0; i < count; i++) {
                if (cde.signal()) {
                    awakes_send++;
                }
            }
        });

        std::thread t3([&]() {
            cde.wait();
        });

        t3.join();
        t1.join();
        t2.join();

        EXPECT_EQ(1, awakes_send.load());
    }

    TEST(count_down_event, try_add) {
        sync_prim::count_down_event cde{1};

        cde.signal();

        EXPECT_EQ(false, cde.try_add());

        cde.add();

        EXPECT_EQ(true, cde.try_add());
    }

    TEST(count_down_event, try_add_count) {
        sync_prim::count_down_event cde{1};

        EXPECT_EQ(true, cde.signal());
        EXPECT_EQ(false, cde.try_add());

        cde.add();

        EXPECT_EQ(true, cde.try_add(9));

        for(int i = 0; i < 9; i++)
        {
            EXPECT_EQ(false, cde.signal());
        }

        EXPECT_EQ(true, cde.signal());
    }

    TEST(count_down_event, wait_timeout) {
        sync_prim::count_down_event cde{1};

        EXPECT_EQ(false,
                cde.wait(std::chrono::milliseconds(30)));
    }
}
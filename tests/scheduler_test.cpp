#include <lib/Scheduler.cpp>
#include <gtest/gtest.h>
#include <sstream>

TEST(MyAny_test, test) {
    int a = 10;
    MyAny any(10);
    ASSERT_EQ(any.Get_Value<int>(), 10);

}

TEST(Scheduler_Test, discriminant_test) {
    float a = 1;
    float b = -2;
    float c = 0;

    TTaskScheduler scheduler;

    auto id1 = scheduler.add([](float a, float c) { return -4 * a * c; }, a, c);

    auto id2 = scheduler.add([](float b, float v) { return b * b + v; }, b, scheduler.getFutureResult<float>(id1));

    auto id3 = scheduler.add([](float b, float d) { return -b + std::sqrt(d); }, b,
                             scheduler.getFutureResult<float>(id2));

    auto id4 = scheduler.add([](float b, float d) { return -b - std::sqrt(d); }, b,
                             scheduler.getFutureResult<float>(id2));

    auto id5 = scheduler.add([](float a, float v) { return v / (2 * a); }, a, scheduler.getFutureResult<float>(id3));

    auto id6 = scheduler.add([](float a, float v) { return v / (2 * a); }, a, scheduler.getFutureResult<float>(id4));

    scheduler.executeAll();

    ASSERT_EQ(scheduler.getResult<float>(id5), 2);
    ASSERT_EQ(scheduler.getResult<float>(id6), 0);
}

TEST(Scheduler_Test, string_test) {
    std::string a = "12";
    std::string b = "34";

    TTaskScheduler scheduler;
    auto id1 = scheduler.add([](std::string a, std::string b) { return a + b; }, a, b);

    scheduler.executeAll();

    ASSERT_EQ(scheduler.getResult<std::string>(id1), "1234");
}


TEST(Scheduler_Test, sqrt_test) {
    float a = 14;
    float b = 13;

    TTaskScheduler scheduler;
    auto id1 = scheduler.add([](float a, float b) { return sqrt(a) * sqrt(b); }, a, b);

    scheduler.executeAll();

    ASSERT_NEAR(scheduler.getResult<float>(id1), 12, 13);
}

TEST(Scheduler_Test, big_test) {
    float a = 1;
    float b = -2;
    float c = 0;
    float d = 120;

    TTaskScheduler scheduler;

    auto id1 = scheduler.add([](float a, float c) { return a * c; }, a, c);

    auto id2 = scheduler.add([](float b, float v) { return b * b + v; }, b, scheduler.getFutureResult<float>(id1));

    auto id3 = scheduler.add([](float b, float d) { return -b + d; }, b,
                             scheduler.getFutureResult<float>(id2));

    auto id4 = scheduler.add([](float b, float d) { return -b - d; }, b,
                             scheduler.getFutureResult<float>(id2));

    auto id5 = scheduler.add([](float a, float v) { return a * v * 10; }, a, scheduler.getFutureResult<float>(id3));

    auto id6 = scheduler.add([](float a, float v) { return a * v * 0.8; }, a, scheduler.getFutureResult<float>(id4));
    auto id7 = scheduler.add([](float a, float v) { return a * v; }, d, scheduler.getFutureResult<float>(id4));
    auto id8 = scheduler.add([](float a, float v) { return a * v; }, d, scheduler.getFutureResult<float>(id5));

    scheduler.executeAll();

    ASSERT_EQ(scheduler.getResult<float>(id7), -240);
    ASSERT_EQ(scheduler.getResult<float>(id8), 7200);
}

TEST(Scheduler_Test, big_value_test) {
    long long a = 99999;
    long long b = 99999;

    TTaskScheduler scheduler;

    auto id1 = scheduler.add([](long long a, long long c) { return a * c; }, a, b);

    scheduler.executeAll();

    ASSERT_EQ(scheduler.getResult<long long>(id1), 9999800001);
}

TEST(Scheduler_Test, bool_test) {
    bool a = true;
    bool b = true;

    TTaskScheduler scheduler;

    auto id1 = scheduler.add([](bool a, bool b) { return a ^ b; }, a, b);

    scheduler.executeAll();

    ASSERT_EQ(scheduler.getResult<bool>(id1), false);
}

TEST(Scheduler_Test, pow_test) {
    TTaskScheduler scheduler;

    auto id1 = scheduler.add([](int a) { return a * 3; }, 3);
    auto id2 = scheduler.add([](int a) { return a * 3; }, scheduler.getFutureResult<int>(id1));
    auto id3 = scheduler.add([](int a) { return a * 3; }, scheduler.getFutureResult<int>(id2));


    scheduler.executeAll();

    ASSERT_EQ(scheduler.getResult<int>(id3), 81);
}
#include <iostream>
#include <lib/Scheduler.cpp>


int main() {
    std::string a = "12";
    std::string b = "34";
    std::cout << a + b << '\n';

    TTaskScheduler scheduler;
    auto id1 = scheduler.add([](const std::string& a, const std::string& b) { return a + b; }, a, b);

    scheduler.executeAll();

    std::cout << "x1 = " << scheduler.getResult<std::string>(id1) << std::endl;

}

#include <iostream>
#include <vector>
#include <any>
#include <string>


class MyAny {
private:
    struct Base {
        virtual Base *get_copy() = 0;

        virtual ~Base() {}

        virtual void *get_value() = 0;
    };

    template<typename T>
    struct Derived : public Base {
        T value;

        Derived(const T& value) : value(value) {}

        Base *get_copy() override {
            return new Derived<T>(value);
        }

        void *get_value() override {
            T *ptr = new T(value);
            return static_cast<void *>(ptr);
        }

    };

    Base *storage = nullptr;
public:
    MyAny() : storage(nullptr) {};

    template<typename U>
    explicit MyAny(const U& value):storage(new Derived<U>(value)) {}

    ~MyAny() {
        delete storage;
    }

    MyAny(const MyAny& other) : storage(other.storage->get_copy()) {}

    template<typename U>
    MyAny& operator=(const U& value) {
        delete storage;
        storage = new Derived<U>(value);
        return *this;
    }

    template<typename T>
    T Get_Value() {
        T return_object = *(reinterpret_cast<T *>(storage->get_value()));
        return return_object;
    }
};


class TTaskScheduler {
public:
    template<typename Function, typename first_arg, typename second_arg>
    MyAny& add(const Function& function, const first_arg& arg1, const second_arg& arg2) {
        MyAny node(function(arg1, arg2));
        results.push_back(node);
        return results.back();
    }

    template<typename Function, typename first_arg>
    MyAny& add(const Function& function, const first_arg& arg1) {
        MyAny node(function(arg1));
        results.push_back(node);
        return results.back();
    }

    template< typename first_arg>
    MyAny& add(const first_arg& arg1) {
        MyAny node(arg1);
        results.push_back(node);
        return results.back();
    }


    template<typename T>
    T getFutureResult(MyAny& id) {
        return id.Get_Value<T>();
    };

    void executeAll() {
    }

    template<typename T>
    T getResult(MyAny& id) {
        return id.Get_Value<T>();
    }

private:
    std::vector<MyAny> results;
};
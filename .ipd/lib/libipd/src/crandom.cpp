#include "libipd_random.h"

#include <chrono>
#include <random>

using namespace std;

static auto construct_engine()
{
    random_device rd;
    auto time = static_cast<random_device::result_type>(
            chrono::high_resolution_clock::now()
                    .time_since_epoch().count());
    return mt19937_64(rd() ^ time);
}

class Engine
{
public:
    static Engine& instance()
    {
        static Engine it;
        return it;
    }

    template <class T>
    static T random(T limit)
    {
        uniform_int_distribution<T> distribution{0, limit - 1};
        return distribution(instance().engine_);
    }

private:
    Engine() : engine_{construct_engine()} { }

    mt19937_64 engine_;
};

unsigned long random_ulong(unsigned long limit)
{
    return Engine::random(limit);
}

int random_int(int limit)
{
    return Engine::random(limit);
}


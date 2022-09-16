// Copyright 2021, 2022 by Mark Melton
//

#include <gtest/gtest.h>
#include "coro/strand/strand.h"
#include "coro/strand/real_scheduler.h"
#include "coro/strand/virtual_scheduler.h"

using namespace coro;

TEST(CoroStrand, RealScheduler)
{
    RealScheduler s;
    std::vector<int> results;
    
    s.on_loop([&results]() -> Strand {
	for (auto i = 0; i < 10; i += 2) {
	    results.push_back(i);
	    co_yield Yield::Resume{};
	}
	co_return Yield::Finished{};
    });
    
    s.on_loop([&results]() -> Strand {
	for (auto i = 1; i < 10; i += 2) {
	    results.push_back(i);
	    co_yield Yield::Resume{};
	}
	co_return Yield::Finished{};
    });

    s.run();

    std::vector<int> expected{0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    EXPECT_EQ(results, expected);
}

TEST(CoroStrand, VirtualScheduler)
{
    VirtualScheduler s;
    std::vector<int> results;
    
    s.on_loop([&results]() -> Strand {
	for (auto i = 0; i < 10; i += 2) {
	    results.push_back(i);
	    co_yield Yield::Resume{};
	}
	co_return Yield::Finished{};
    });
    
    s.on_loop([&results]() -> Strand {
	for (auto i = 1; i < 10; i += 2) {
	    results.push_back(i);
	    co_yield Yield::Resume{};
	}
	co_return Yield::Finished{};
    });

    s.run();

    std::vector<int> expected{0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    EXPECT_EQ(results, expected);
}

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}




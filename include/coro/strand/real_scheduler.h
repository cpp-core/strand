// Copyright (C) 2021, 2022 by Mark Melton
//

#pragma once
#include "coro/strand/scheduler.h"

namespace coro {

class RealScheduler : public Scheduler {
public:
    RealScheduler(chron::TimePoint start = chron::TimePoint{},
		  chron::TimePoint end = chron::TimePoint::max(),
		  chron::nanos resolution = chron::millis{1})
	: Scheduler(Mode::RealTime, start, end, resolution) {
    }

    virtual ~RealScheduler() {
    }

    virtual bool run_group(Strands& strands) override;
};

}; // coro


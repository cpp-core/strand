// Copyright (C) 2017, 2018, 2019, 2021, 2022 by Mark Melton
//

#pragma once
#include "coro/strand/strand.h"
#include "coro/strand/scheduler.h"

namespace coro
{

class TimeMarker {
public:
    TimeMarker(chron::TimePoint tp, Scheduler& scheduler)
	: tp_(tp)
	, scheduler_(scheduler) {
    }
	
    bool await_ready() const noexcept {
	return scheduler_.fast_forward(tp_);
    }
    
    void await_suspend(Strand::Handle coro) noexcept {
	coro.promise().state_ = Yield::ResumeAt{tp_};
    }
	
    void await_resume() const noexcept {
    }
	
private:
    chron::TimePoint tp_;
    Scheduler& scheduler_;
};

}; // coro

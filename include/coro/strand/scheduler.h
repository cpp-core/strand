// Copyright (C) 2021, 2022 by Mark Melton
//

#pragma once
#include <queue>
#include "coro/strand/strand.h"
#include "core/chrono/lowres_clock.h"
#include "core/chrono/timepoint.h"

namespace coro {

class Scheduler {
public:
    using Mode = chron::LowResClock::Mode;
    
    struct StrandPriority {
	bool operator()(const Strand *l, const Strand *r) const {
	    return l->next_runtime() > r->next_runtime();
	};
    };
    using RunQueue = std::priority_queue<Strand*, std::vector<Strand*>, StrandPriority>;
    
    Scheduler(Mode mode,
	      chron::TimePoint start,
	      chron::TimePoint end,
	      chron::nanos resolution)
	: clock_(mode, resolution) 
	, start_(start)
	, end_(end) {
	if (mode == Mode::Virtual)
	    clock().virtual_now(start);
    }

    virtual ~Scheduler() {
    }

    bool run();
    Strand *active() { return active_task_; }
    void unsuspend(Strand *s) { tasks().push(s); }
    
    void stop() { set_done(); }
    bool done() const { return done_; }
    bool fast_forward(chron::TimePoint tp);

    const std::exception_ptr& eptr() const { return exception_ptr_; }

    const chron::LowResClock& clock() const { return clock_; }
    chron::LowResClock& clock() { return clock_; }
    
    chron::TimePoint virtual_now() const { return clock_.virtual_now(); }
    chron::TimePoint now() const { return clock_.now(); }
    
    Strand::Profiles profiles() const;

    template<class F>
    void on_setup(F&& functor) {
	setup_.emplace_back(std::move(functor));
    }

    template<class F>
    void on_tear_down(F&& functor) {
	tear_down_.emplace_back(std::move(functor));
    }

    auto& on_loop(Yield::Code state, Strand&& strand) {
	loop_.emplace_back(state, std::move(strand));
	return loop_.back();
    }
    
    auto& on_loop(Strand&& strand) {
	return on_loop(Yield::Resume{}, std::forward<Strand>(strand));
    }

    template<class L> requires StrandLambda<L>
    auto& on_loop(Yield::Code state, L&& lambda) {
	return on_loop(state, construct_strand(std::forward<L>(lambda)));
    }
    
    template<class L> requires StrandLambda<L>
    auto& at_time(chron::TimePoint tp, L&& lambda) {
	return on_loop(Yield::ResumeAt{tp}, std::forward<L>(lambda));
    }

    template<class L> requires StrandLambda<L>
    auto& after_duration(chron::nanos duration, L&& lambda) {
	return on_loop(Yield::ResumeAfter{duration}, std::forward<L>(lambda));
    }

    template<class L> requires StrandLambda<L>
    auto& after_duration_wallclock(chron::nanos duration, L&& lambda) {
	return on_loop(Yield::ResumeAfterReal{duration}, std::forward<L>(lambda));
    }

    template<class L> requires StrandLambda<L>
    auto& on_loop(L&& lambda) {
	return on_loop(construct_strand(std::forward<L>(lambda)));
    }

protected:
    virtual bool run_group(Strands& strands) = 0;
    
    const RunQueue& tasks() const { return tasks_; }
    RunQueue& tasks() { return tasks_; }

    Strand *active_task() const { return active_task_; }
    void active_task(Strand *s) { active_task_ = s; }
    
    void set_done() { done_ = true; }
    void set_eptr(std::exception_ptr ptr) { exception_ptr_ = ptr; }
    
    RunQueue tasks_;
    Strand *active_task_{nullptr};

private:
    using SetupFunctor = std::function<void()>;
    using SetupFunctors = std::vector<SetupFunctor>;
    
    using TearDownFunctor = std::function<void()>;
    using TearDownFunctors = std::vector<TearDownFunctor>;
    
    using Lambda = std::function<Strand()>;
    using LambdaPtr = std::unique_ptr<Lambda>;
    using LambdaPtrs = std::vector<LambdaPtr>;

    template<class L>
    Strand construct_strand(L&& lambda) {
	saved_.emplace_back(std::make_unique<Lambda>(std::forward<L>(lambda)));
	return (*saved_.back())();
    }

    bool done_{false};
    std::exception_ptr exception_ptr_;
    LambdaPtrs saved_;
    Strands loop_;
    SetupFunctors setup_;
    TearDownFunctors tear_down_;
    chron::LowResClock clock_;
    chron::TimePoint start_, end_;
};

using SchedulerPtr = std::unique_ptr<Scheduler>;

}; // coro


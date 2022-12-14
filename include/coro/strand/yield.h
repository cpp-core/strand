// Copyright (C) 2021, 2022 by Mark Melton
//

#pragma once
#include <variant>
#include "core/chrono/chrono.h"
#include "core/mp/list.h"

namespace coro
{

struct Yield {
    struct Exception { std::exception_ptr eptr; };
    struct Finished {};
    struct Resume {};
    struct ResumeAfter { chron::nanos duration; };
    struct ResumeAfterReal { chron::nanos duration; };
    struct ResumeAt { chron::TimePoint tp; };
    struct Shutdown {};
    struct Suspend {};
    struct Terminate {};
    using Code = std::variant<Exception,
			      Finished,
			      Resume,
			      ResumeAfter,
			      ResumeAfterReal,
			      ResumeAt,
			      Shutdown,
			      Suspend,
			      Terminate>;

    using CoReturnTypes = std::tuple<Exception, Finished, Shutdown, Terminate>;
    using CoYieldTypes = std::tuple<ResumeAfter, ResumeAfterReal, ResumeAt, Resume, Suspend>;
};

std::ostream& operator<<(std::ostream& os, const Yield::Code& yield);

}; // coro

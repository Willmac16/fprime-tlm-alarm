#undef TRACEPOINT_PROVIDER
#define TRACEPOINT_PROVIDER fprime_profiler

#undef TRACEPOINT_INCLUDE
#define TRACEPOINT_INCLUDE "./fprime_profiler_tp.h"

#if !defined(_FPRIME_PROFILER_TP_H) || defined(TRACEPOINT_HEADER_MULTI_READ)
#define _FPRIME_PROFILER_TP_H

#include <lttng/tracepoint.h>

TRACEPOINT_EVENT(
    fprime_profiler,
    entry,
    TP_ARGS(
        const char*, component_name_arg,
        unsigned int, context_arg
    ),
    TP_FIELDS(
        ctf_string(component_name, component_name_arg)
        ctf_integer(unsigned int, context, context_arg)
    )
)

TRACEPOINT_EVENT(
    fprime_profiler,
    exit,
    TP_ARGS(
        const char*, component_name_arg,
        unsigned int, context_arg
    ),
    TP_FIELDS(
        ctf_string(component_name, component_name_arg)
        ctf_integer(unsigned int, context, context_arg)
    )
)

#endif /* _FPRIME_PROFILER_TP_H */

#include <lttng/tracepoint-event.h>

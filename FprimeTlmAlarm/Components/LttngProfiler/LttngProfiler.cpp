#include "FprimeTlmAlarm/Components/LttngProfiler/LttngProfiler.hpp"
#include "FpTypes.hpp"

#define TRACEPOINT_DEFINE
#include "fprime_profiler_tp.h"

namespace FprimeTlmAlarm {

    // ----------------------------------------------------------------------
    // Component construction and destruction
    // ----------------------------------------------------------------------

    LttngProfiler::LttngProfiler(const char* const compName) :
        LttngProfilerComponentBase(compName)
    {
    }

    LttngProfiler::~LttngProfiler() {
    }

    // ----------------------------------------------------------------------
    // Handler implementations for user-defined typed input ports
    // ----------------------------------------------------------------------

    void LttngProfiler::schedIn_handler(FwIndexType portNum, U32 context) {
        // Emit LTTNG entry event
        tracepoint(fprime_profiler, entry, this->getObjName(), context);

        // Forward schedule signal to driven component
        if (this->isConnected_schedOut_OutputPort(0)) {
            this->schedOut_out(0, context);
        }

        // Emit LTTNG exit event
        tracepoint(fprime_profiler, exit, this->getObjName(), context);
    }

}

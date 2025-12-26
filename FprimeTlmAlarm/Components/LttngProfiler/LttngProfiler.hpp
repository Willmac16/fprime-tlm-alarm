#ifndef FprimeTlmAlarm_LttngProfiler_HPP
#define FprimeTlmAlarm_LttngProfiler_HPP

#include "FprimeTlmAlarm/Components/LttngProfiler/LttngProfilerComponentAc.hpp"

namespace FprimeTlmAlarm {

    class LttngProfiler : public LttngProfilerComponentBase {
      public:
        // ----------------------------------------------------------------------
        // Component construction and destruction
        // ----------------------------------------------------------------------

        //! Construct LttngProfiler object
        LttngProfiler(const char* const compName);

        //! Destroy LttngProfiler object
        ~LttngProfiler();

      PRIVATE:
        // ----------------------------------------------------------------------
        // Handler implementations for user-defined typed input ports
        // ----------------------------------------------------------------------

        //! Handler implementation for schedIn
        //!
        //! Receives schedule signal from rate group, emits LTTNG entry event,
        //! forwards to driven component, then emits LTTNG exit event
        void schedIn_handler(FwIndexType portNum, U32 context) override;
    };

}

#endif

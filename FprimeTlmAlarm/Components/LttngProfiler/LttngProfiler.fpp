module FprimeTlmAlarm {

    @ LTTNG profiling shim component that emits entry/exit events
    @ Sits between a rate group and its driven component
    passive component LttngProfiler {

        # ----------------------------------------------------------------------
        # General Ports
        # ----------------------------------------------------------------------

        @ Schedule input port from rate group
        sync input port schedIn: Svc.Sched

        @ Schedule output port to driven component
        output port schedOut: Svc.Sched

    }

}

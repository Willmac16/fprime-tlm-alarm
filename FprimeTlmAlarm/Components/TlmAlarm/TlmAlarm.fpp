module FprimeTlmAlarm {

    @ Comparison operator types for monitors
    enum ComparisonOp {
        GREATER_THAN = 0         @< >
        GREATER_THAN_OR_EQUAL = 1 @< >=
        LESS_THAN = 2            @< <
        LESS_THAN_OR_EQUAL = 3   @< <=
        EQUAL = 4                @< ==
        NOT_EQUAL = 5            @< !=
    }

    @ Monitor Tlm Mnemonics Onboard
    queued component TlmAlarm {
        # RX Tlm from the system (Likely a TlmSplitter)
        async input port TlmRecv: Fw.Tlm

        @ Example port: receiving calls from the rate group
        sync input port run: Svc.Sched

        ##############################################################################
        #### Monitor Response Port                                                   #
        ##############################################################################

        @ Output port called when a monitor triggers
        output port ResponseOut: [10] ResponseOut

        ###############################################################################
        # Standard AC Ports: Required for Channels, Events, Commands, and Parameters  #
        ###############################################################################
        @ Port for requesting the current time
        time get port timeCaller

        @ Enables command handling
        import Fw.Command

        @ Enables event handling
        import Fw.Event

        @ Enables telemetry channels handling
        import Fw.Channel

        @ Port to return the value of a parameter
        param get port prmGetOut

        @Port to set the value of a parameter
        param set port prmSetOut

        ###############################################################################
        # Events                                                                      #
        ###############################################################################

        @ Monitor triggered alarm condition
        event MonitorTriggered(
            monitorId: U32 @< Monitor ID that triggered
            channelId: U32 @< Telemetry channel ID
        ) \
        severity warning high \
        format "Monitor {} triggered for channel {}"

        @ Monitor successfully configured
        event MonitorConfigured(
            monitorId: U32 @< Monitor ID
            channelId: U32 @< Telemetry channel ID
        ) \
        severity activity high \
        format "Monitor {} configured for channel {}"

        @ Monitor enabled
        event MonitorEnabled(
            monitorId: U32 @< Monitor ID
        ) \
        severity activity low \
        format "Monitor {} enabled"

        @ Monitor disabled
        event MonitorDisabled(
            monitorId: U32 @< Monitor ID
        ) \
        severity activity low \
        format "Monitor {} disabled"

        @ Monitor threshold updated
        event ThresholdUpdated(
            monitorId: U32 @< Monitor ID
            threshold: F64  @< New threshold value
        ) \
        severity activity low \
        format "Monitor {} threshold updated to {}"

        @ Monitor configuration failed - max monitors reached
        event ConfigurationFailed(
            reason: string size 80 @< Failure reason
        ) \
        severity warning high \
        format "Monitor configuration failed: {}"

        ###############################################################################
        # Telemetry Channels                                                         #
        ###############################################################################

        @ Number of active monitors
        telemetry NumActiveMonitors: U32 \
        format "{}"

        @ Total monitor triggers
        telemetry TotalTriggers: U32 \
        format "{}"

    }
}
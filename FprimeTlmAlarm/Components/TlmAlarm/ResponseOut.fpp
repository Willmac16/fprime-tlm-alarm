module FprimeTlmAlarm {
    @ Port for notifying when a monitor triggers
    @ Sends the monitor ID that triggered
    port ResponseOut(
        monitorId: U32 @< The ID of the monitor that triggered
    )
}

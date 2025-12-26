// ======================================================================
// \title  TlmAlarm.cpp
// \author wmac
// \brief  cpp file for TlmAlarm component implementation class
// ======================================================================

#include "FprimeTlmAlarm/Components/TlmAlarm/TlmAlarm.hpp"
#include <Fw/Types/Assert.hpp>

namespace FprimeTlmAlarm {

// ----------------------------------------------------------------------
// Component construction and destruction
// ----------------------------------------------------------------------

TlmAlarm::TlmAlarm(const char* const compName) : TlmAlarmComponentBase(compName), m_numMonitors(0) {
    // Initialize all monitors
    for (U32 i = 0; i < MAX_MONITORS; i++) {
        m_monitors[i] = MonitorConfig();
    }
}

TlmAlarm::~TlmAlarm() {}

// ----------------------------------------------------------------------
// Handler implementations for typed input ports
// ----------------------------------------------------------------------

void TlmAlarm::TlmRecv_handler(FwIndexType portNum, FwChanIdType id, Fw::Time& timeTag, Fw::TlmBuffer& val) {
    // Find if there's a monitor configured for this channel
    MonitorConfig* config = findMonitorByChannel(id);

    if (config == nullptr || !config->enabled) {
        // No monitor for this channel or monitor is disabled
        return;
    }

    // Evaluate the monitor condition
    bool triggered = evaluateMonitor(*config, val);

    if (triggered) {
        // Increment consecutive trigger count
        config->currentCount++;

        // Check if we've reached the debounce threshold
        if (config->currentCount >= config->debounceCount) {
            // Trigger the monitor response
            if (isConnected_ResponseOut_OutputPort(0)) {
                ResponseOut_out(0, config->monitorId);
            }

            // Log event
            log_WARNING_HI_MonitorTriggered(config->monitorId, id);

            // Reset counter after triggering
            config->currentCount = 0;
        }
    } else {
        // Condition not met, reset counter
        config->currentCount = 0;
    }
}

void TlmAlarm::run_handler(FwIndexType portNum, U32 context) {
    // This handler is called periodically by rate group
    // Could be used for periodic monitor health checks or statistics
}

// ----------------------------------------------------------------------
// Public configuration methods
// ----------------------------------------------------------------------

bool TlmAlarm::configureMonitor(FwChanIdType channelId,
                                U32 monitorId,
                                TlmDataType dataType,
                                ComparisonOp comparisonOp,
                                F64 threshold,
                                U32 debounceCount,
                                bool usesMagnitude) {
    // Check if we have room for another monitor
    if (m_numMonitors >= MAX_MONITORS) {
        return false;
    }

    // Check if monitor ID already exists
    if (findMonitorById(monitorId) != nullptr) {
        return false;
    }

    // Create new monitor configuration
    MonitorConfig& config = m_monitors[m_numMonitors];
    config.channelId = channelId;
    config.monitorId = monitorId;
    config.dataType = dataType;
    config.comparisonOp = comparisonOp;
    config.debounceCount = debounceCount;
    config.currentCount = 0;
    config.enabled = true;
    config.usesMagnitude = usesMagnitude;

    // Set threshold value
    setThresholdValue(config.threshold, dataType, threshold);

    m_numMonitors++;

    log_ACTIVITY_HI_MonitorConfigured(monitorId, channelId);

    return true;
}

bool TlmAlarm::enableMonitor(U32 monitorId) {
    MonitorConfig* config = findMonitorById(monitorId);
    if (config == nullptr) {
        return false;
    }

    config->enabled = true;
    config->currentCount = 0;  // Reset counter when enabling

    log_ACTIVITY_LO_MonitorEnabled(monitorId);

    return true;
}

bool TlmAlarm::disableMonitor(U32 monitorId) {
    MonitorConfig* config = findMonitorById(monitorId);
    if (config == nullptr) {
        return false;
    }

    config->enabled = false;
    config->currentCount = 0;  // Reset counter when disabling

    log_ACTIVITY_LO_MonitorDisabled(monitorId);

    return true;
}

bool TlmAlarm::updateThreshold(U32 monitorId, F64 threshold) {
    MonitorConfig* config = findMonitorById(monitorId);
    if (config == nullptr) {
        return false;
    }

    setThresholdValue(config->threshold, config->dataType, threshold);
    config->currentCount = 0;  // Reset counter when threshold changes

    log_ACTIVITY_LO_ThresholdUpdated(monitorId, threshold);

    return true;
}

// ----------------------------------------------------------------------
// Private helper methods
// ----------------------------------------------------------------------

bool TlmAlarm::evaluateMonitor(MonitorConfig& config, Fw::TlmBuffer& buffer) {
    // Create a copy of the buffer for deserialization
    Fw::TlmBuffer localBuffer = buffer;
    Fw::SerializeStatus status;

    switch (config.dataType) {
        case TlmDataType::U8_TYPE: {
            U8 value;
            status = localBuffer.deserialize(value);
            if (status != Fw::FW_SERIALIZE_OK) {
                return false;
            }
            return compareNumeric(config.comparisonOp, value, config.threshold.u8_val);
        }

        case TlmDataType::U16_TYPE: {
            U16 value;
            status = localBuffer.deserialize(value);
            if (status != Fw::FW_SERIALIZE_OK) {
                return false;
            }
            return compareNumeric(config.comparisonOp, value, config.threshold.u16_val);
        }

        case TlmDataType::U32_TYPE: {
            U32 value;
            status = localBuffer.deserialize(value);
            if (status != Fw::FW_SERIALIZE_OK) {
                return false;
            }
            return compareNumeric(config.comparisonOp, value, config.threshold.u32_val);
        }

        case TlmDataType::U64_TYPE: {
            U64 value;
            status = localBuffer.deserialize(value);
            if (status != Fw::FW_SERIALIZE_OK) {
                return false;
            }
            return compareNumeric(config.comparisonOp, value, config.threshold.u64_val);
        }

        case TlmDataType::I8_TYPE: {
            I8 value;
            status = localBuffer.deserialize(value);
            if (status != Fw::FW_SERIALIZE_OK) {
                return false;
            }
            return compareNumeric(config.comparisonOp, value, config.threshold.i8_val);
        }

        case TlmDataType::I16_TYPE: {
            I16 value;
            status = localBuffer.deserialize(value);
            if (status != Fw::FW_SERIALIZE_OK) {
                return false;
            }
            return compareNumeric(config.comparisonOp, value, config.threshold.i16_val);
        }

        case TlmDataType::I32_TYPE: {
            I32 value;
            status = localBuffer.deserialize(value);
            if (status != Fw::FW_SERIALIZE_OK) {
                return false;
            }
            return compareNumeric(config.comparisonOp, value, config.threshold.i32_val);
        }

        case TlmDataType::I64_TYPE: {
            I64 value;
            status = localBuffer.deserialize(value);
            if (status != Fw::FW_SERIALIZE_OK) {
                return false;
            }
            return compareNumeric(config.comparisonOp, value, config.threshold.i64_val);
        }

        case TlmDataType::F32_TYPE: {
            F32 value;
            status = localBuffer.deserialize(value);
            if (status != Fw::FW_SERIALIZE_OK) {
                return false;
            }
            return compareNumeric(config.comparisonOp, value, config.threshold.f32_val);
        }

        case TlmDataType::F64_TYPE: {
            F64 value;
            status = localBuffer.deserialize(value);
            if (status != Fw::FW_SERIALIZE_OK) {
                return false;
            }
            return compareNumeric(config.comparisonOp, value, config.threshold.f64_val);
        }

        case TlmDataType::VEC_F32_TYPE: {
            VectorF32 value;
            status = localBuffer.deserialize(value.x);
            if (status != Fw::FW_SERIALIZE_OK) {
                return false;
            }
            status = localBuffer.deserialize(value.y);
            if (status != Fw::FW_SERIALIZE_OK) {
                return false;
            }
            status = localBuffer.deserialize(value.z);
            if (status != Fw::FW_SERIALIZE_OK) {
                return false;
            }
            return compareVector(config.comparisonOp, value, config.threshold.vec_f32_val, config.usesMagnitude);
        }

        case TlmDataType::VEC_F64_TYPE: {
            VectorF64 value;
            status = localBuffer.deserialize(value.x);
            if (status != Fw::FW_SERIALIZE_OK) {
                return false;
            }
            status = localBuffer.deserialize(value.y);
            if (status != Fw::FW_SERIALIZE_OK) {
                return false;
            }
            status = localBuffer.deserialize(value.z);
            if (status != Fw::FW_SERIALIZE_OK) {
                return false;
            }
            return compareVector(config.comparisonOp, value, config.threshold.vec_f64_val, config.usesMagnitude);
        }

        case TlmDataType::QUAT_F32_TYPE: {
            QuaternionF32 value;
            status = localBuffer.deserialize(value.w);
            if (status != Fw::FW_SERIALIZE_OK) {
                return false;
            }
            status = localBuffer.deserialize(value.x);
            if (status != Fw::FW_SERIALIZE_OK) {
                return false;
            }
            status = localBuffer.deserialize(value.y);
            if (status != Fw::FW_SERIALIZE_OK) {
                return false;
            }
            status = localBuffer.deserialize(value.z);
            if (status != Fw::FW_SERIALIZE_OK) {
                return false;
            }
            return compareQuaternion(config.comparisonOp, value, config.threshold.quat_f32_val, config.usesMagnitude);
        }

        case TlmDataType::QUAT_F64_TYPE: {
            QuaternionF64 value;
            status = localBuffer.deserialize(value.w);
            if (status != Fw::FW_SERIALIZE_OK) {
                return false;
            }
            status = localBuffer.deserialize(value.x);
            if (status != Fw::FW_SERIALIZE_OK) {
                return false;
            }
            status = localBuffer.deserialize(value.y);
            if (status != Fw::FW_SERIALIZE_OK) {
                return false;
            }
            status = localBuffer.deserialize(value.z);
            if (status != Fw::FW_SERIALIZE_OK) {
                return false;
            }
            return compareQuaternion(config.comparisonOp, value, config.threshold.quat_f64_val, config.usesMagnitude);
        }

        default:
            return false;
    }
}

void TlmAlarm::setThresholdValue(ThresholdValue& dest, TlmDataType type, F64 value) {
    switch (type) {
        case TlmDataType::U8_TYPE:
            dest.u8_val = static_cast<U8>(value);
            break;
        case TlmDataType::U16_TYPE:
            dest.u16_val = static_cast<U16>(value);
            break;
        case TlmDataType::U32_TYPE:
            dest.u32_val = static_cast<U32>(value);
            break;
        case TlmDataType::U64_TYPE:
            dest.u64_val = static_cast<U64>(value);
            break;
        case TlmDataType::I8_TYPE:
            dest.i8_val = static_cast<I8>(value);
            break;
        case TlmDataType::I16_TYPE:
            dest.i16_val = static_cast<I16>(value);
            break;
        case TlmDataType::I32_TYPE:
            dest.i32_val = static_cast<I32>(value);
            break;
        case TlmDataType::I64_TYPE:
            dest.i64_val = static_cast<I64>(value);
            break;
        case TlmDataType::F32_TYPE:
            dest.f32_val = static_cast<F32>(value);
            break;
        case TlmDataType::F64_TYPE:
            dest.f64_val = value;
            break;
        case TlmDataType::VEC_F32_TYPE:
            // For vectors, store magnitude threshold (or could be set component-wise)
            dest.vec_f32_val.x = static_cast<F32>(value);
            dest.vec_f32_val.y = static_cast<F32>(value);
            dest.vec_f32_val.z = static_cast<F32>(value);
            break;
        case TlmDataType::VEC_F64_TYPE:
            dest.vec_f64_val.x = value;
            dest.vec_f64_val.y = value;
            dest.vec_f64_val.z = value;
            break;
        case TlmDataType::QUAT_F32_TYPE:
            dest.quat_f32_val.w = static_cast<F32>(value);
            dest.quat_f32_val.x = static_cast<F32>(value);
            dest.quat_f32_val.y = static_cast<F32>(value);
            dest.quat_f32_val.z = static_cast<F32>(value);
            break;
        case TlmDataType::QUAT_F64_TYPE:
            dest.quat_f64_val.w = value;
            dest.quat_f64_val.x = value;
            dest.quat_f64_val.y = value;
            dest.quat_f64_val.z = value;
            break;
    }
}

MonitorConfig* TlmAlarm::findMonitorByChannel(FwChanIdType channelId) {
    for (U32 i = 0; i < m_numMonitors; i++) {
        if (m_monitors[i].channelId == channelId) {
            return &m_monitors[i];
        }
    }
    return nullptr;
}

MonitorConfig* TlmAlarm::findMonitorById(U32 monitorId) {
    for (U32 i = 0; i < m_numMonitors; i++) {
        if (m_monitors[i].monitorId == monitorId) {
            return &m_monitors[i];
        }
    }
    return nullptr;
}

}  // namespace FprimeTlmAlarm

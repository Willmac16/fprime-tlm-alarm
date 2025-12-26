// ======================================================================
// \title  TlmAlarm.hpp
// \author wmac
// \brief  hpp file for TlmAlarm component implementation class
// ======================================================================

#ifndef FprimeTlmAlarm_TlmAlarm_HPP
#define FprimeTlmAlarm_TlmAlarm_HPP

#include "FprimeTlmAlarm/Components/TlmAlarm/TlmAlarmComponentAc.hpp"
#include <Fw/Types/SerialBuffer.hpp>
#include <cmath>

namespace FprimeTlmAlarm {

//! Maximum number of monitors that can be configured
static constexpr U32 MAX_MONITORS = 256;

//! Telemetry data types supported for monitoring
enum class TlmDataType : U8 {
    U8_TYPE = 0,
    U16_TYPE = 1,
    U32_TYPE = 2,
    U64_TYPE = 3,
    I8_TYPE = 4,
    I16_TYPE = 5,
    I32_TYPE = 6,
    I64_TYPE = 7,
    F32_TYPE = 8,
    F64_TYPE = 9,
    VEC_F32_TYPE = 10,  // 3-element float vector
    VEC_F64_TYPE = 11,  // 3-element double vector
    QUAT_F32_TYPE = 12, // 4-element float quaternion
    QUAT_F64_TYPE = 13  // 4-element double quaternion
};

//! Vector structure for F32
struct VectorF32 {
    F32 x;
    F32 y;
    F32 z;

    F32 magnitude() const {
        return std::sqrt(x * x + y * y + z * z);
    }
};

//! Vector structure for F64
struct VectorF64 {
    F64 x;
    F64 y;
    F64 z;

    F64 magnitude() const {
        return std::sqrt(x * x + y * y + z * z);
    }
};

//! Quaternion structure for F32
struct QuaternionF32 {
    F32 w;
    F32 x;
    F32 y;
    F32 z;

    F32 magnitude() const {
        return std::sqrt(w * w + x * x + y * y + z * z);
    }
};

//! Quaternion structure for F64
struct QuaternionF64 {
    F64 w;
    F64 x;
    F64 y;
    F64 z;

    F64 magnitude() const {
        return std::sqrt(w * w + x * x + y * y + z * z);
    }
};

//! Union to hold different threshold types
union ThresholdValue {
    U8 u8_val;
    U16 u16_val;
    U32 u32_val;
    U64 u64_val;
    I8 i8_val;
    I16 i16_val;
    I32 i32_val;
    I64 i64_val;
    F32 f32_val;
    F64 f64_val;
    VectorF32 vec_f32_val;
    VectorF64 vec_f64_val;
    QuaternionF32 quat_f32_val;
    QuaternionF64 quat_f64_val;

    ThresholdValue() : u64_val(0) {}
};

//! Monitor configuration structure
struct MonitorConfig {
    FwChanIdType channelId;              //!< Telemetry channel ID to monitor
    U32 monitorId;                       //!< Unique monitor ID
    TlmDataType dataType;                //!< Data type of the channel
    ComparisonOp comparisonOp;           //!< Comparison operator to use
    ThresholdValue threshold;            //!< Threshold value for comparison
    U32 debounceCount;                   //!< Number of consecutive triggers needed
    U32 currentCount;                    //!< Current consecutive trigger count
    bool enabled;                        //!< Whether this monitor is enabled
    bool usesMagnitude;                  //!< For vectors/quats: compare magnitude instead of components

    MonitorConfig()
        : channelId(0),
          monitorId(0),
          dataType(TlmDataType::U32_TYPE),
          comparisonOp(ComparisonOp::GREATER_THAN),
          debounceCount(1),
          currentCount(0),
          enabled(false),
          usesMagnitude(false) {}
};

class TlmAlarm final : public TlmAlarmComponentBase {
  public:
    // ----------------------------------------------------------------------
    // Component construction and destruction
    // ----------------------------------------------------------------------

    //! Construct TlmAlarm object
    TlmAlarm(const char* const compName  //!< The component name
    );

    //! Destroy TlmAlarm object
    ~TlmAlarm();

    // ----------------------------------------------------------------------
    // Public methods for monitor configuration
    // ----------------------------------------------------------------------

    //! Configure a monitor for a telemetry channel
    //! \param channelId: Telemetry channel ID to monitor
    //! \param monitorId: Unique monitor ID
    //! \param dataType: Data type of the channel
    //! \param comparisonOp: Comparison operator to use
    //! \param threshold: Threshold value (as F64, will be cast to appropriate type)
    //! \param debounceCount: Number of consecutive triggers needed
    //! \param usesMagnitude: For vectors/quats, compare magnitude instead of components
    //! \return true if configuration successful, false otherwise
    bool configureMonitor(FwChanIdType channelId,
                         U32 monitorId,
                         TlmDataType dataType,
                         ComparisonOp comparisonOp,
                         F64 threshold,
                         U32 debounceCount = 1,
                         bool usesMagnitude = false);

    //! Enable a monitor
    //! \param monitorId: Monitor ID to enable
    //! \return true if successful, false if monitor not found
    bool enableMonitor(U32 monitorId);

    //! Disable a monitor
    //! \param monitorId: Monitor ID to disable
    //! \return true if successful, false if monitor not found
    bool disableMonitor(U32 monitorId);

    //! Update threshold for a monitor at runtime
    //! \param monitorId: Monitor ID
    //! \param threshold: New threshold value
    //! \return true if successful, false if monitor not found
    bool updateThreshold(U32 monitorId, F64 threshold);

  private:
    // ----------------------------------------------------------------------
    // Handler implementations for typed input ports
    // ----------------------------------------------------------------------

    //! Handler implementation for TlmRecv
    void TlmRecv_handler(FwIndexType portNum,  //!< The port number
                         FwChanIdType id,      //!< Telemetry Channel ID
                         Fw::Time& timeTag,    //!< Time Tag
                         Fw::TlmBuffer& val    //!< Buffer containing serialized telemetry value
                         ) override;

    //! Handler implementation for run
    //!
    //! Example port: receiving calls from the rate group
    void run_handler(FwIndexType portNum,  //!< The port number
                     U32 context           //!< The call order
                     ) override;

    // ----------------------------------------------------------------------
    // Private helper methods
    // ----------------------------------------------------------------------

    //! Deserialize and compare a value against monitor threshold
    //! \param config: Monitor configuration
    //! \param buffer: Telemetry buffer containing serialized value
    //! \return true if comparison condition is met, false otherwise
    bool evaluateMonitor(MonitorConfig& config, Fw::TlmBuffer& buffer);

    //! Template method for numeric comparison
    template <typename T>
    bool compareNumeric(ComparisonOp op, T value, T threshold);

    //! Template method for vector comparison (component-wise or magnitude)
    template <typename VecType>
    bool compareVector(ComparisonOp op, const VecType& value, const VecType& threshold, bool usesMagnitude);

    //! Template method for quaternion comparison (component-wise or magnitude)
    template <typename QuatType>
    bool compareQuaternion(ComparisonOp op, const QuatType& value, const QuatType& threshold, bool usesMagnitude);

    //! Set threshold value from F64
    void setThresholdValue(ThresholdValue& dest, TlmDataType type, F64 value);

    //! Find monitor configuration by channel ID
    //! \return pointer to config if found, nullptr otherwise
    MonitorConfig* findMonitorByChannel(FwChanIdType channelId);

    //! Find monitor configuration by monitor ID
    //! \return pointer to config if found, nullptr otherwise
    MonitorConfig* findMonitorById(U32 monitorId);

    // Member variables
  private:
    MonitorConfig m_monitors[MAX_MONITORS];  //!< Array of monitor configurations
    U32 m_numMonitors;                       //!< Number of active monitors
};

// ----------------------------------------------------------------------
// Template implementations
// ----------------------------------------------------------------------

template <typename T>
bool TlmAlarm::compareNumeric(ComparisonOp op, T value, T threshold) {
    switch (op) {
        case ComparisonOp::GREATER_THAN:
            return value > threshold;
        case ComparisonOp::GREATER_THAN_OR_EQUAL:
            return value >= threshold;
        case ComparisonOp::LESS_THAN:
            return value < threshold;
        case ComparisonOp::LESS_THAN_OR_EQUAL:
            return value <= threshold;
        case ComparisonOp::EQUAL:
            return value == threshold;
        case ComparisonOp::NOT_EQUAL:
            return value != threshold;
        default:
            return false;
    }
}

template <typename VecType>
bool TlmAlarm::compareVector(ComparisonOp op, const VecType& value, const VecType& threshold, bool usesMagnitude) {
    if (usesMagnitude) {
        // Compare magnitudes
        auto valMag = value.magnitude();
        auto thrMag = threshold.magnitude();
        return compareNumeric(op, valMag, thrMag);
    } else {
        // Component-wise comparison - all components must satisfy the condition
        bool xResult = compareNumeric(op, value.x, threshold.x);
        bool yResult = compareNumeric(op, value.y, threshold.y);
        bool zResult = compareNumeric(op, value.z, threshold.z);
        return xResult && yResult && zResult;
    }
}

template <typename QuatType>
bool TlmAlarm::compareQuaternion(ComparisonOp op,
                                const QuatType& value,
                                const QuatType& threshold,
                                bool usesMagnitude) {
    if (usesMagnitude) {
        // Compare magnitudes
        auto valMag = value.magnitude();
        auto thrMag = threshold.magnitude();
        return compareNumeric(op, valMag, thrMag);
    } else {
        // Component-wise comparison - all components must satisfy the condition
        bool wResult = compareNumeric(op, value.w, threshold.w);
        bool xResult = compareNumeric(op, value.x, threshold.x);
        bool yResult = compareNumeric(op, value.y, threshold.y);
        bool zResult = compareNumeric(op, value.z, threshold.z);
        return wResult && xResult && yResult && zResult;
    }
}

}  // namespace FprimeTlmAlarm

#endif

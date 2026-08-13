#pragma once

#include <array>
#include <cstdint>

#include "mode.h"

namespace spotykach {

enum class EngineType : uint8_t {
    Tape = 0,
    AudreyII,
    Oscillator,
    Reverb,
    Microcosm,
    Plaits,
    Rings,
    Elements,
    Benjolin,
    Count
};

constexpr bool engine_is_effect(const EngineType type)
{
    return type == EngineType::Reverb
           || type == EngineType::Microcosm
           || type == EngineType::Rings;
}

struct DeckEngineConfig {
    EngineType type { EngineType::Tape };
};

struct EngineControls {
    float sos { 0.5f };
    float mix { 0.5f };
    float mod_freq { 0.5f };
    float mod_amt { 0.0f };
    float size { 0.5f };
    float pitch { 0.5f };
    float pos { 0.5f };
    float env { 0.5f };
    float flux_mix { 0.0f };
    float flux_intensity { 0.5f };
    float grit_mix { 0.0f };
    float grit_intensity { 0.5f };
    float feedback { 0.0f };
    Mode mode { Mode::Reel };
    bool reverse { false };
};

constexpr const char* engine_type_name(const EngineType type)
{
    switch(type)
    {
        case EngineType::Tape: return "Tape";
        case EngineType::AudreyII: return "AudreyII";
        case EngineType::Oscillator: return "Oscillator";
        case EngineType::Reverb: return "Reverb";
        case EngineType::Microcosm: return "Microcosm";
        case EngineType::Plaits: return "Plaits";
        case EngineType::Rings: return "Rings";
        case EngineType::Elements: return "Elements";
        case EngineType::Benjolin: return "Benjolin";
        default: return "Unknown";
    }
}

} // namespace spotykach

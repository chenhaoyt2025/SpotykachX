#pragma once

#include "daisysp.h"
#include "Effects/overdrive.h"
#include "Effects/sampleratereducer.h"
#include "Filters/svf.h"
#include "Noise/grainlet.h"
#include "PhysicalModeling/modalvoice.h"
#include "Synthesis/zoscillator.h"
#include "Utility/delayline.h"

#include "../../lib/DaisySP/DaisySP-LGPL/Source/Effects/reverbsc.h"

#include "core/engine.h"
#include "engine/audrey2/FeedbackSynthEngine.h"

namespace spotykach {

class DeckEngine {
  public:
    DeckEngine() = default;
    ~DeckEngine() = default;

    void init(float sample_rate);
    void set_controls(const EngineControls& controls) { controls_ = controls; }
    void prepare(EngineType type);
    void process(EngineType type, bool fx_only, float in0, float in1, float& out0, float& out1);

  private:
    void prepare_audrey();
    void prepare_oscillator();
    void prepare_reverb();
    void prepare_microcosm();
    void prepare_plaits();
    void prepare_rings();
    void prepare_elements();
    void prepare_benjolin();

    void process_audrey(float in0, float in1, float& out0, float& out1);
    void process_oscillator(float in0, float in1, float& out0, float& out1);
    void process_reverb(float in0, float in1, float& out0, float& out1);
    void process_microcosm(float in0, float in1, float& out0, float& out1);
    void process_plaits(float in0, float in1, float& out0, float& out1);
    void process_rings(float in0, float in1, float& out0, float& out1);
    void process_elements(float in0, float in1, float& out0, float& out1);
    void process_benjolin(float in0, float in1, float& out0, float& out1);

    float sample_rate_ { 48000.0f };
    EngineControls controls_ {};

    infrasonic::FeedbackSynth::Engine audrey_;

    daisysp::Oscillator osc_a_;
    daisysp::Oscillator osc_b_;
    daisysp::Svf osc_filter_;

    daisysp::ReverbSc verb_;

    static constexpr size_t kMicrocosmDelaySamples = 144000;
    daisysp::DelayLine<float, kMicrocosmDelaySamples> micro_delay_;
    daisysp::SampleRateReducer micro_reducer_;
    daisysp::Overdrive micro_drive_;

    daisysp::GrainletOscillator plaits_;

    daisysp::Resonator rings_;

    daisysp::ModalVoice elements_;
    daisysp::ZOscillator elements_zosc_;

    daisysp::Oscillator benjolin_osc_a_;
    daisysp::Oscillator benjolin_osc_b_;
    daisysp::Svf benjolin_filter_;

    float micro_feedback_ { 0.0f };
    float micro_grain_pos_ { 0.0f };
    float micro_grain_len_ { 0.0f };
    float micro_grain_period_ { 0.0f };
    float micro_center_ { 0.0f };
    float micro_speed_ { 1.0f };
    float micro_mix_ { 0.5f };
    float micro_drive_amt_ { 0.0f };
    float micro_reducer_amt_ { 1.0f };
    float micro_spread_ { 0.0f };
    float micro_jitter_ { 0.0f };
    float micro_prev_in_ { 0.0f };
    uint32_t micro_rng_ { 0x12345678u };

    bool benjolin_prev_clock_ { false };
    bool benjolin_prev_data_ { false };
    uint8_t benjolin_shift_reg_ { 0x5Au };
    float benjolin_rungler_ { 0.0f };
};

} // namespace spotykach

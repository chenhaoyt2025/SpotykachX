#include "engine/deck.engine.h"

#include <cmath>

#include "Utility/dsp.h"

using namespace daisysp;
using namespace spotykach;

namespace {

float norm_to_note(const float norm, const float lo = 24.0f, const float hi = 84.0f)
{
    return fmap(norm, lo, hi);
}

float norm_to_freq(const float norm, const float lo = 30.0f, const float hi = 1800.0f)
{
    return fmap(norm, lo, hi, Mapping::EXP);
}

float centered(const float norm)
{
    return (norm * 2.0f) - 1.0f;
}

float mix2(const float dry, const float wet, const float mix)
{
    return (dry * (1.0f - mix)) + (wet * mix);
}

float rand01(uint32_t& rng)
{
    rng = rng * 1664525u + 1013904223u;
    return static_cast<float>(rng >> 8) * (1.0f / static_cast<float>(0x01000000));
}

float clamp01(const float v)
{
    return std::clamp(v, 0.0f, 1.0f);
}

float hann(const float phase)
{
    return 0.5f - 0.5f * cosf(TWOPI_F * phase);
}

} // namespace

void DeckEngine::init(float sample_rate)
{
    sample_rate_ = sample_rate;

    audrey_.Init(sample_rate);

    osc_a_.Init(sample_rate);
    osc_b_.Init(sample_rate);
    osc_a_.SetAmp(0.5f);
    osc_b_.SetAmp(0.5f);
    osc_a_.SetWaveform(Oscillator::WAVE_SAW);
    osc_b_.SetWaveform(Oscillator::WAVE_TRI);

    osc_filter_.Init(sample_rate);
    osc_filter_.SetRes(0.1f);

    verb_.Init(sample_rate);

    micro_delay_.Init();
    micro_reducer_.Init();
    micro_drive_.Init();

    plaits_.Init(sample_rate);

    rings_.Init(0.25f, 2, sample_rate);

    elements_.Init(sample_rate);
    elements_zosc_.Init(sample_rate);

    benjolin_osc_a_.Init(sample_rate);
    benjolin_osc_b_.Init(sample_rate);
    benjolin_osc_a_.SetAmp(1.0f);
    benjolin_osc_b_.SetAmp(1.0f);
    benjolin_osc_a_.SetWaveform(Oscillator::WAVE_SQUARE);
    benjolin_osc_b_.SetWaveform(Oscillator::WAVE_SQUARE);
    benjolin_filter_.Init(sample_rate);
    benjolin_filter_.SetRes(0.3f);
}

void DeckEngine::prepare(EngineType type)
{
    switch(type)
    {
        case EngineType::AudreyII: prepare_audrey(); break;
        case EngineType::Oscillator: prepare_oscillator(); break;
        case EngineType::Reverb: prepare_reverb(); break;
        case EngineType::Microcosm: prepare_microcosm(); break;
        case EngineType::Plaits: prepare_plaits(); break;
        case EngineType::Rings: prepare_rings(); break;
        case EngineType::Elements: prepare_elements(); break;
        case EngineType::Benjolin: prepare_benjolin(); break;
        case EngineType::Tape:
        default: break;
    }
}

void DeckEngine::process(EngineType type, float in0, float in1, float& out0, float& out1)
{
    prepare(type);

    float engine_out0 = in0;
    float engine_out1 = in1;

    switch(type)
    {
        case EngineType::AudreyII: process_audrey(in0, in1, engine_out0, engine_out1); break;
        case EngineType::Oscillator: process_oscillator(in0, in1, engine_out0, engine_out1); break;
        case EngineType::Reverb: process_reverb(in0, in1, engine_out0, engine_out1); break;
        case EngineType::Microcosm: process_microcosm(in0, in1, engine_out0, engine_out1); break;
        case EngineType::Plaits: process_plaits(in0, in1, engine_out0, engine_out1); break;
        case EngineType::Rings: process_rings(in0, in1, engine_out0, engine_out1); break;
        case EngineType::Elements: process_elements(in0, in1, engine_out0, engine_out1); break;
        case EngineType::Benjolin: process_benjolin(in0, in1, engine_out0, engine_out1); break;
        case EngineType::Tape:
        default:
            break;
    }

    out0 = engine_out0;
    out1 = engine_out1;
}

void DeckEngine::prepare_audrey()
{
    audrey_.SetStringPitch(norm_to_note(controls_.pitch, 28.0f, 82.0f));
    audrey_.SetFeedbackGain(fmap(controls_.feedback, -48.0f, 10.0f));
    audrey_.SetFeedbackDelay(fmap(controls_.size, 0.001f, 0.3f, Mapping::EXP));
    audrey_.SetFeedbackLPFCutoff(norm_to_freq(controls_.env, 300.0f, 18000.0f));
    audrey_.SetFeedbackHPFCutoff(norm_to_freq(controls_.mod_freq, 10.0f, 1600.0f));
    audrey_.SetReverbMix(controls_.mix);
    audrey_.SetReverbFeedback(fmap(controls_.mod_amt, 0.2f, 0.98f));
    audrey_.SetEchoDelaySendAmount(controls_.flux_mix);
    audrey_.SetEchoDelayTime(fmap(controls_.pos, 0.05f, 5.0f, Mapping::EXP));
    audrey_.SetEchoDelayFeedback(fmap(controls_.flux_intensity, 0.0f, 1.4f));
    audrey_.SetOutputLevel(fmap(controls_.sos, 0.0f, 1.0f));
}

void DeckEngine::prepare_oscillator()
{
    auto base_freq = norm_to_freq(controls_.pitch, 35.0f, 1400.0f);
    auto detune = fmap(controls_.size, 0.0f, 12.0f);
    osc_a_.SetFreq(base_freq);
    osc_b_.SetFreq(base_freq * powf(2.0f, centered(detune * 0.0833333f) * 0.25f));
    osc_filter_.SetFreq(norm_to_freq(controls_.env, 120.0f, 12000.0f));
    osc_filter_.SetRes(fmap(controls_.feedback, 0.05f, 0.95f));
}

void DeckEngine::prepare_reverb()
{
    verb_.SetFeedback(fmap(controls_.feedback, 0.15f, 0.98f));
    verb_.SetLpFreq(norm_to_freq(controls_.env, 220.0f, 16000.0f));
}

void DeckEngine::prepare_microcosm()
{
    micro_mix_ = clamp01(controls_.mix);
    micro_feedback_ = fmap(controls_.feedback, 0.0f, 0.985f);
    micro_grain_period_ = fmap(controls_.mod_freq, 80.0f, 2200.0f, Mapping::EXP);
    micro_grain_len_ = fmap(controls_.size, 16.0f, 12000.0f, Mapping::EXP);
    micro_center_ = fmap(controls_.pos, 32.0f, static_cast<float>(kMicrocosmDelaySamples - 2), Mapping::EXP);
    micro_speed_ = powf(2.0f, centered(controls_.mod_amt) * 2.0f);
    micro_drive_amt_ = fmap(controls_.env, 0.0f, 1.0f);
    micro_reducer_amt_ = clamp01(controls_.grit_intensity);
    micro_spread_ = fmap(controls_.flux_mix, 0.0f, 0.45f);
    micro_jitter_ = clamp01(controls_.flux_intensity);
    micro_drive_.SetDrive(fmap(controls_.grit_mix, 0.0f, 1.0f));
    micro_reducer_.SetFreq(fmap(controls_.mod_amt, 1.0f, 0.15f));
}

void DeckEngine::prepare_plaits()
{
    auto freq = norm_to_freq(controls_.pitch, 30.0f, 1800.0f);
    plaits_.SetFreq(freq);
    plaits_.SetFormantFreq(norm_to_freq(controls_.mod_freq, 40.0f, 4200.0f));
    plaits_.SetShape(controls_.size * 3.0f);
    plaits_.SetBleed(clamp01(controls_.mod_amt));
}

void DeckEngine::prepare_rings()
{
    rings_.SetFreq(norm_to_freq(controls_.mod_freq, 40.0f, 1500.0f));
    rings_.SetStructure(clamp01(controls_.size));
    rings_.SetBrightness(clamp01(controls_.env));
    rings_.SetDamping(clamp01(controls_.feedback));
}

void DeckEngine::prepare_elements()
{
    elements_.SetFreq(norm_to_freq(controls_.pitch, 35.0f, 1400.0f));
    elements_.SetAccent(clamp01(controls_.mod_amt));
    elements_.SetStructure(clamp01(controls_.size));
    elements_.SetBrightness(clamp01(controls_.env));
    elements_.SetDamping(clamp01(controls_.feedback));
    elements_.SetSustain(controls_.flux_mix > 0.45f);
    elements_zosc_.SetFreq(norm_to_freq(controls_.mod_freq, 35.0f, 1800.0f));
    elements_zosc_.SetFormantFreq(norm_to_freq(controls_.pos, 40.0f, 2000.0f));
    elements_zosc_.SetShape(controls_.flux_intensity * 2.5f);
    elements_zosc_.SetMode(centered(controls_.grit_intensity));
}

void DeckEngine::prepare_benjolin()
{
    auto rungler_pitch = centered(benjolin_rungler_) * fmap(controls_.mod_amt, 0.0f, 36.0f);
    auto osc_a_freq = norm_to_freq(controls_.pitch, 15.0f, 1200.0f);
    auto osc_b_freq = norm_to_freq(controls_.mod_freq, 8.0f, 900.0f);
    osc_a_freq *= powf(2.0f, rungler_pitch / 12.0f);
    osc_b_freq *= powf(2.0f, centered(controls_.size) * 2.0f);

    benjolin_osc_a_.SetFreq(osc_a_freq);
    benjolin_osc_b_.SetFreq(osc_b_freq);
    benjolin_filter_.SetFreq(norm_to_freq(controls_.env, 80.0f, 14000.0f));
    benjolin_filter_.SetRes(fmap(controls_.feedback, 0.1f, 0.98f));
}

void DeckEngine::process_audrey(float in0, float in1, float& out0, float& out1)
{
    audrey_.Process(in0, in1, out0, out1);
}

void DeckEngine::process_oscillator(float in0, float in1, float& out0, float& out1)
{
    auto fm = centered(controls_.pos) * 120.0f * (fabsf(in0) + fabsf(in1));
    osc_a_.PhaseAdd(fm / sample_rate_);
    osc_b_.PhaseAdd(-fm / sample_rate_);

    auto a = osc_a_.Process();
    auto b = osc_b_.Process();
    auto dry = (a + b) * 0.35f;
    auto l = dry + (in0 * controls_.flux_mix);
    auto r = ((a * 0.6f) - (b * 0.4f)) + (in1 * controls_.flux_mix);
    osc_filter_.Process(l + r);
    out0 = mix2(l, osc_filter_.Low(), controls_.mix) * fmap(controls_.sos, 0.2f, 1.0f);
    out1 = mix2(r, osc_filter_.Band(), controls_.mix) * fmap(controls_.sos, 0.2f, 1.0f);
}

void DeckEngine::process_reverb(float in0, float in1, float& out0, float& out1)
{
    float wet_l = 0.0f;
    float wet_r = 0.0f;
    verb_.Process(in0, in1, &wet_l, &wet_r);
    out0 = mix2(in0, wet_l, controls_.mix) * fmap(controls_.sos, 0.1f, 1.0f);
    out1 = mix2(in1, wet_r, controls_.mix) * fmap(controls_.sos, 0.1f, 1.0f);
}

void DeckEngine::process_microcosm(float in0, float in1, float& out0, float& out1)
{
    auto mono = (in0 + in1) * 0.5f;
    auto excite = mono + (micro_prev_in_ - mono) * controls_.flux_mix;
    auto write = excite + (micro_feedback_ * micro_prev_in_ * controls_.env);
    micro_delay_.Write(write);

    auto grain_period = std::max(32.0f, micro_grain_period_);
    auto grain_len = std::clamp(micro_grain_len_, 16.0f, grain_period * 2.5f);

    if(micro_grain_pos_ <= 0.0f || micro_grain_pos_ >= grain_len)
    {
        micro_grain_pos_ = 0.0f;
        auto jitter = (rand01(micro_rng_) - 0.5f) * 2.0f * micro_jitter_ * grain_len;
        micro_center_ = std::clamp(micro_center_ + jitter, grain_len + 2.0f, static_cast<float>(kMicrocosmDelaySamples - 4));
        micro_reducer_.SetFreq(fmap(controls_.grit_intensity, 1.0f, 0.05f));
    }

    auto phase = micro_grain_pos_ / grain_len;
    auto win = hann(phase);
    auto pitch = micro_speed_;
    auto left_delay = micro_center_ + (micro_grain_pos_ * pitch);
    auto right_delay = std::clamp(left_delay + (micro_spread_ * grain_len), 2.0f, static_cast<float>(kMicrocosmDelaySamples - 2));
    auto left = micro_delay_.ReadHermite(std::clamp(left_delay, 1.0f, static_cast<float>(kMicrocosmDelaySamples - 2))) * win;
    auto right = micro_delay_.ReadHermite(right_delay) * win;

    left = micro_drive_.Process(left);
    right = micro_drive_.Process(right);
    left = micro_reducer_.Process(left);
    right = micro_reducer_.Process(right);

    auto wet0 = left;
    auto wet1 = right;
    auto dry0 = in0;
    auto dry1 = in1;

    out0 = mix2(dry0, wet0, micro_mix_) * fmap(controls_.sos, 0.2f, 1.0f);
    out1 = mix2(dry1, wet1, micro_mix_) * fmap(controls_.sos, 0.2f, 1.0f);

    micro_grain_pos_ += std::max(0.25f, pitch);
    micro_prev_in_ = mono;
}

void DeckEngine::process_plaits(float in0, float in1, float& out0, float& out1)
{
    auto mono = (in0 + in1) * 0.5f;
    auto exciter = mono + (in0 - in1) * controls_.flux_mix;
    auto voice = plaits_.Process();
    auto spread = centered(controls_.pos) * 0.15f;

    out0 = mix2(exciter, voice, controls_.mix) * fmap(controls_.sos, 0.2f, 1.0f);
    out1 = mix2(exciter + spread, voice, controls_.mix) * fmap(controls_.sos, 0.2f, 1.0f);
}

void DeckEngine::process_rings(float in0, float in1, float& out0, float& out1)
{
    auto mono = (in0 + in1) * 0.5f;
    auto excite = mono + ((in0 - in1) * controls_.flux_mix) + (fabsf(mono) * controls_.flux_intensity * 0.5f);
    auto wet = rings_.Process(excite);
    auto spread = centered(controls_.pos) * 0.2f;

    out0 = mix2(in0, wet, controls_.mix) * fmap(controls_.sos, 0.2f, 1.0f);
    out1 = mix2(in1 + spread, wet, controls_.mix) * fmap(controls_.sos, 0.2f, 1.0f);
}

void DeckEngine::process_elements(float in0, float in1, float& out0, float& out1)
{
    auto mono = (in0 + in1) * 0.5f;
    auto trig = fabsf(mono - micro_prev_in_) > 0.08f;
    auto voice = elements_.Process(trig);
    auto exciter = elements_zosc_.Process() * 0.35f;
    auto spread = centered(controls_.pos) * 0.18f;
    auto wet = voice + exciter;

    out0 = mix2(in0, wet, controls_.mix) * fmap(controls_.sos, 0.2f, 1.0f);
    out1 = mix2(in1 + spread, wet, controls_.mix) * fmap(controls_.sos, 0.2f, 1.0f);
    micro_prev_in_ = mono;
}

void DeckEngine::process_benjolin(float in0, float in1, float& out0, float& out1)
{
    auto osc_a = benjolin_osc_a_.Process();
    auto osc_b = benjolin_osc_b_.Process();

    auto data_high = osc_a > osc_b;
    auto clock_high = osc_b > 0.0f;
    if(clock_high && !benjolin_prev_clock_)
    {
        benjolin_shift_reg_ = static_cast<uint8_t>((benjolin_shift_reg_ << 1) | (data_high ? 1 : 0));
        auto rungler_bits = static_cast<float>(benjolin_shift_reg_ & 0x07u)
                            + static_cast<float>((benjolin_shift_reg_ >> 5) & 0x07u);
        benjolin_rungler_ = rungler_bits / 14.0f;
    }
    benjolin_prev_clock_ = clock_high;
    benjolin_prev_data_ = data_high;

    auto rungler_cv = centered(benjolin_rungler_);
    auto xor_wave = data_high != clock_high ? 1.0f : -1.0f;
    auto rungler_square = benjolin_rungler_ > 0.5f ? 1.0f : -1.0f;
    auto ext = (in0 + in1) * 0.5f;

    auto raw = (osc_a * 0.45f)
               + (osc_b * 0.25f)
               + (xor_wave * fmap(controls_.flux_mix, 0.0f, 0.45f))
               + (rungler_square * fmap(controls_.flux_intensity, 0.0f, 0.4f))
               + (ext * fmap(controls_.grit_mix, 0.0f, 0.5f));

    benjolin_filter_.Process(raw + (rungler_cv * fmap(controls_.pos, 0.0f, 1.4f)));
    auto wet0 = mix2(benjolin_filter_.Low(), benjolin_filter_.Band(), controls_.mix);
    auto wet1 = mix2(benjolin_filter_.Band(), benjolin_filter_.High(), controls_.mix);

    wet0 += rungler_cv * fmap(controls_.grit_intensity, 0.0f, 0.35f);
    wet1 -= rungler_cv * fmap(controls_.grit_intensity, 0.0f, 0.25f);

    out0 = wet0 * fmap(controls_.sos, 0.15f, 1.0f);
    out1 = wet1 * fmap(controls_.sos, 0.15f, 1.0f);
}

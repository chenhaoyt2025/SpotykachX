#include "engine/audrey2/FeedbackSynthEngine.h"
#include "engine/audrey2/DSPUtils.h"
#include "engine/audrey2/sdram_alloc.h"

using namespace infrasonic;
using namespace infrasonic::FeedbackSynth;
using namespace daisysp;

void Engine::Init(const float sample_rate) {
  using ED = AudreyEchoDelay<kMaxEchoDelaySamp>;

  echo_delay_[0] = EchoDelayPtr(SDRAM::allocate<ED>());
  echo_delay_[1] = EchoDelayPtr(SDRAM::allocate<ED>());
  verb_ = VerbPtr(SDRAM::allocate<Chorus>());

  sample_rate_ = sample_rate;
  fb_delay_smooth_coef_ = audrey_onepole_coef(0.2f, sample_rate);

  noise_.Init();
  noise_.SetAmp(0.0f);

  for (unsigned int i = 0; i < 2; i++) {

    strings_[i].Init(sample_rate);
    strings_[i].SetBrightness(0.98f);
    strings_[i].SetFreq(mtof(40.0f));
    strings_[i].SetDamping(0.4f);

    fb_delayline_[i].Init();

    echo_delay_[i]->Init(sample_rate);
    echo_delay_[i]->SetDelayTime(5.0f, true);
    echo_delay_[i]->SetFeedback(0.5f);
    echo_delay_[i]->SetLagTime(0.5f);

    overdrive_[i].Init();
    overdrive_[i].SetDrive(0.4f);
  }

  verb_->Init(sample_rate);
  verb_->SetFeedback(0.35f);
  verb_->SetLfoDepth(0.25f, 0.32f);
  verb_->SetLfoFreq(0.18f, 0.23f);
  verb_->SetDelayMs(18.0f, 27.0f);

  fb_lpf_.Init(sample_rate);
  fb_lpf_.SetQ(0.9f);
  fb_lpf_.SetCutoff(18000.0f);

  fb_hpf_.Init(sample_rate);
  fb_hpf_.SetQ(0.9f);
  fb_hpf_.SetCutoff(60.f);
}

void Engine::SetStringPitch(const float nn) {
  const auto freq = mtof(nn);
  strings_[0].SetFreq(freq);
  strings_[1].SetFreq(freq);
}

void Engine::SetFeedbackGain(const float gain_db) {
  fb_gain_ = audrey_dbfs2lin(gain_db);
}

void Engine::SetFeedbackDelay(const float delay_s) {
  fb_delay_samp_target_ =
      DSY_CLAMP(delay_s * sample_rate_, 1.0f,
                static_cast<float>(kMaxFeedbackDelaySamp - 1));
}

void Engine::SetFeedbackLPFCutoff(const float cutoff_hz) {
  fb_lpf_.SetCutoff(cutoff_hz);
}

void Engine::SetFeedbackHPFCutoff(const float cutoff_hz) {
  fb_hpf_.SetCutoff(cutoff_hz);
}

void Engine::SetEchoDelayTime(const float echo_time) {
  echo_delay_[0]->SetDelayTime(echo_time);
  echo_delay_[1]->SetDelayTime(echo_time);
}

void Engine::SetEchoDelayFeedback(const float echo_fb) {
  echo_delay_[0]->SetFeedback(echo_fb);
  echo_delay_[1]->SetFeedback(echo_fb);
}

void Engine::SetEchoDelaySendAmount(const float echo_send) {
  echo_send_ = echo_send;
}

void Engine::SetReverbMix(const float mix) {
  verb_mix_ = mix;
}

void Engine::SetReverbFeedback(const float feedback) {
  verb_->SetFeedback(feedback * 0.7f);
  verb_->SetLfoDepth(fmap(feedback, 0.1f, 0.85f), fmap(feedback, 0.15f, 0.9f));
}

void Engine::SetOutputLevel(const float level) {
  output_level_ = level;
}

void Engine::Process(float inL, float inR, float &outL, float &outR) {
  // --- Update audio-rate-smoothed control params ---

  fonepole(fb_delay_samp_, fb_delay_samp_target_, fb_delay_smooth_coef_);

  // --- Process Samples ---

  float l_in, r_in, sampL, sampR, echoL, echoR, verbL, verbR;
  const float noise_samp = noise_.Process();

  // ---> Feedback Loop

  // Get noise + feedback output
  l_in = fb_delayline_[0].Read(fb_delay_samp_) + noise_samp;
  r_in = fb_delayline_[1].Read(daisysp::fmax(1.0f, fb_delay_samp_ - 4.f)) + noise_samp;

  // feedback部分经过KarplusString，输入信号绕过string，二者在失真前相加
  float ksL = strings_[0].Process(l_in);
  float ksR = strings_[1].Process(r_in);
  sampL = overdrive_[0].Process(ksL + inL);
  sampR = overdrive_[1].Process(ksR + inR);

  // Distort + Clip
  sampL = overdrive_[0].Process(sampL);
  sampR = overdrive_[1].Process(sampR);

  // Filter in feedback loop
  fb_lpf_.ProcessStereo(sampL, sampR);
  fb_hpf_.ProcessStereo(sampL, sampR);

  // ---> Reverb

  verb_->Process((sampL + sampR) * 0.5f);
  verbL = verb_->GetLeft();
  verbR = verb_->GetRight();

  //       (sampL * (1.0f - verb_mix_)) + verbL * verb_mix_;
  //       sampL - sampL * verb_mix + verbL * verb_mix_;
  sampL -= (sampL - verbL) * verb_mix_;
  sampR -= (sampR - verbR) * verb_mix_;

  // ---> Resonator feedback

  // Write back into delay with attenuation
  fb_delayline_[0].Write(sampL * fb_gain_);
  fb_delayline_[1].Write(sampR * fb_gain_);

  // ---> Echo Delay

  echoL = echo_delay_[0]->Process(sampL * echo_send_);
  echoR = echo_delay_[1]->Process(sampR * echo_send_);

  sampL = 0.5f * (sampL + echoL);
  sampR = 0.5f * (sampR + echoR);

  // ---> Output
  outL = sampL * output_level_;
  outR = sampR * output_level_;
}

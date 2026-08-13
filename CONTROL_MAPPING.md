# SpotykachX Control Mapping

## English

### Engine selection and routing

- Only Deck A can change engine. Deck B always remains Tape.
- Hold `Spot` and touch `Trig A` to cycle: `Tape`, `AudreyII`, `Oscillator`, `Reverb`, `Microcosm`, `Plaits`, `Rings`, `Elements`, `Benjolin`.
- In a Deck A non-Tape engine, `Alt + Play A` selects the external Deck A input.
- In a Deck A non-Tape engine, `Alt + Rev A` selects the complete Deck B output.
- The internal chain is `Deck B Tape + Deck B Flux/Grit -> Deck A engine -> Deck A Flux/Grit`.
- `SOS/MIX A` uses the engine's original shared Mix/output-level mapping; it is not a final dry/wet control after Deck A Flux/Grit.
- The global Crossfade still mixes the Deck A and Deck B branches after this processing.
- Changing engines does not clear the Deck A Tape buffer. Returning to Tape restores the saved Tape controls.

### Engine colors and ring

Tape uses the original Reel, Slice, and Drift ring display. A non-Tape engine uses the following identity color. The ring area follows `PITCH`: pitch for self-generating engines and wet/dry for effect engines.

| Engine | Color | Type | `PITCH` / ring area |
|---|---|---|---|
| AudreyII | Green `#00FF00` | Self-generating | Pitch |
| Oscillator | Yellow `#FFDE21` | Self-generating | Pitch |
| Reverb | Blue `#0000FF` | Effect | Wet/dry |
| Microcosm | Pink `#FF00FF` | Effect | Wet/dry |
| Plaits | Cyan `#00B4D8` | Self-generating | Pitch |
| Rings | White `#FFFFFF` | Effect | Wet/dry |
| Elements | Purple `#C850FF` | Self-generating | Pitch |
| Benjolin | Yellow-green `#B6FF00` | Self-generating | Pitch |

Original colors remain: Reel orange `#F7941D`, Slice blue `#0064FF`, Drift purple `#C850FF`, internal clock green, external clock pink, MIDI clock cyan, Flux red-pink, Grit soft yellow, and Grit harsh orange.

### Original Tape controls

Deck B always uses these controls. Deck A uses them while its engine is Tape.

| Control | Normal | With `Alt` | With held Tap or FX touch |
|---|---|---|---|
| `SOS/MIX` | I/O Mix | Feedback | Tap + Deck A: Key Interval; Flux/Grit touch: FX Mix |
| `MOD FREQ` | Mod Speed | - | Tap + Deck A: Tempo; Tap + Deck B: Pan Speed |
| `MOD AMT` | Mod Amount | - | Tap + Deck A: Click Mix; Tap + Deck B: Pan Range |
| `SIZE` | Size | Drift: Window Size | Slice + held Tap: beat-fitted slice length |
| `PITCH` | Speed/Pitch | Quantized pitch while adjusting | Flux/Grit touch: FX Intensity; storage selection: slot |
| `POS` | Start Position | Cue Start Offset | Flux touch: Flux Feedback |
| `ENV` | Envelope Shape | Drift: Envelope Size | - |
| `CROSSFADE` | Global Deck A/B Crossfade | - | - |

Touch controls:

- `Play`: forward play. `Rev`: reverse play.
- `Alt + Play`: external recording. `Alt + Rev`: internal recording.
- Hold `Flux` or `Grit` for a momentary effect; `Alt + Flux/Grit` toggles the effect lock.
- `Trig`: trigger. `Alt + Trig`: arm/disarm the sequence. Holding an armed sequence control clears it.
- In Internal clock mode, repeated Tap presses set tempo. `Tap + Alt` cycles Internal, external clock, and MIDI clock sources.
- After Tap is held long enough, `Play/Rev` enters the original SD-card load/save workflow.

Original three-position modes:

- Reel: linear tape-style playback.
- Slice: clock-oriented sliced/polyphonic playback.
- Drift: spread/granular-style playback.
- Global route switch: Generative Stereo, Double Mono, or Stereo.

### Deck A non-Tape parameters

`SOS/MIX A` supplies each engine's shared Mix/output-level value. The mappings below list the parameters actually read by each engine. Original Deck A Flux and Grit remain after the engine.

| Engine | `PITCH` | `MOD FREQ` | `MOD AMT` | `SIZE` | `POS` | `ENV` | `Alt + SOS` |
|---|---|---|---|---|---|---|---|
| AudreyII | String pitch | Feedback HPF | Reverb feedback | Feedback delay | Echo delay | Feedback LPF | Feedback gain |
| Oscillator | Base frequency | Unused | Unused | Oscillator detune | Input FM depth | Filter cutoff | Filter resonance |
| Reverb | Wet/dry | Unused | Unused | Unused | Unused | Reverb low-pass | Reverb feedback |
| Microcosm | Wet/dry | Grain period | Playback speed | Grain length | Delay window | Feedback/drive factor | Feedback |
| Plaits | Carrier frequency | Formant frequency | Bleed | Shape | Stereo offset | Unused | Unused |
| Rings | Wet/dry | Resonator frequency | Unused | Structure | Stereo offset | Brightness | Damping |
| Elements | Frequency | Exciter frequency | Accent | Structure | Exciter formant | Brightness | Damping |
| Benjolin | Oscillator A frequency | Oscillator B/rungler clock | Rungler pitch depth | Oscillator B offset | Rungler filter modulation | Filter cutoff | Resonance |

FX-touch parameters used inside the engine:

- AudreyII: `Flux Mix` = echo send; `Flux Intensity` = echo feedback.
- Oscillator: `Flux Mix` = external input amount.
- Microcosm: `Flux Mix` = stereo spread/history blend; `Flux Intensity` = grain jitter; `Grit Mix` = drive; `Grit Intensity` = sample-rate reduction.
- Plaits: `Flux Mix` = input/exciter blend.
- Rings: `Flux Mix` and `Flux Intensity` shape the excitation.
- Elements: `Flux Mix` enables sustain; `Flux Intensity` changes exciter shape; `Grit Intensity` changes exciter mode.
- Benjolin: `Flux Mix` adds XOR, `Flux Intensity` adds rungler square, `Grit Mix` adds external input, and `Grit Intensity` adds direct rungler modulation.

### Reserved input

The first 4051 channel 7 and `sr2 bit7` are not assigned. No new function should use them without an explicit design decision. The second 4051 channel 7 is the Crossfade control.

## 中文

### 引擎切换与路由

- 只有 Deck A 可以切换引擎，Deck B 始终保持 Tape。
- 按住 `Spot` 再触摸 `Trig A`，依次切换：`Tape`、`AudreyII`、`Oscillator`、`Reverb`、`Microcosm`、`Plaits`、`Rings`、`Elements`、`Benjolin`。
- Deck A 非 Tape 模式中，`Alt + Play A` 选择 Deck A 外部输入。
- Deck A 非 Tape 模式中，`Alt + Rev A` 选择 Deck B 的完整输出。
- 内部信号链为 `Deck B Tape + Deck B Flux/Grit -> Deck A 引擎 -> Deck A Flux/Grit`。
- `SOS/MIX A` 使用各引擎原来的 Mix/输出电平共享映射，不是位于 Deck A Flux/Grit 之后的最终干湿控制。
- 全局 Crossfade 仍在处理之后混合 Deck A 与 Deck B 两条支路。
- 切换引擎不会清空 Deck A Tape 缓冲；切回 Tape 时恢复保存的 Tape 控制值。

### 引擎颜色与光环

Tape 使用原版 Reel、Slice、Drift 光环。非 Tape 引擎使用下表颜色；光环面积跟随 `PITCH`：自发声引擎表示音高，效果引擎表示干湿比。

| 引擎 | 颜色 | 类型 | `PITCH` / 光环面积 |
|---|---|---|---|
| AudreyII | 绿色 `#00FF00` | 自发声 | 音高 |
| Oscillator | 黄色 `#FFDE21` | 自发声 | 音高 |
| Reverb | 蓝色 `#0000FF` | 效果 | 干湿比 |
| Microcosm | 粉色 `#FF00FF` | 效果 | 干湿比 |
| Plaits | 青色 `#00B4D8` | 自发声 | 音高 |
| Rings | 白色 `#FFFFFF` | 效果 | 干湿比 |
| Elements | 紫色 `#C850FF` | 自发声 | 音高 |
| Benjolin | 黄绿色 `#B6FF00` | 自发声 | 音高 |

原版颜色保持不变：Reel 橙色 `#F7941D`、Slice 蓝色 `#0064FF`、Drift 紫色 `#C850FF`、内部时钟绿色、外部时钟粉色、MIDI 时钟青色、Flux 红粉色、Grit soft 黄色、Grit harsh 橙色。

### 原版 Tape 控制

Deck B 始终使用这些控制。Deck A 在 Tape 引擎时也使用这些控制。

| 控制 | 正常 | 按住 `Alt` | 按住 Tap 或触摸 FX |
|---|---|---|---|
| `SOS/MIX` | I/O Mix | Feedback | Tap + Deck A：Key Interval；触摸 Flux/Grit：FX Mix |
| `MOD FREQ` | Mod Speed | - | Tap + Deck A：Tempo；Tap + Deck B：Pan Speed |
| `MOD AMT` | Mod Amount | - | Tap + Deck A：Click Mix；Tap + Deck B：Pan Range |
| `SIZE` | Size | Drift：Window Size | Slice + 按住 Tap：按节拍拟合的 Slice 长度 |
| `PITCH` | Speed/Pitch | 调节时量化音高 | 触摸 Flux/Grit：FX Intensity；存储选择时：槽位 |
| `POS` | Start Position | Cue Start Offset | 触摸 Flux：Flux Feedback |
| `ENV` | Envelope Shape | Drift：Envelope Size | - |
| `CROSSFADE` | 全局 Deck A/B Crossfade | - | - |

触摸按键：

- `Play`：正向播放；`Rev`：反向播放。
- `Alt + Play`：外部录音；`Alt + Rev`：内部录音。
- 按住 `Flux` 或 `Grit` 临时开启效果；`Alt + Flux/Grit` 切换效果锁定。
- `Trig`：触发；`Alt + Trig`：sequence arm/disarm；继续长按已 arm 的控制可清空 sequence。
- Internal 时钟模式下，连续点击 Tap 设置速度；`Tap + Alt` 循环切换 Internal、外部 Clock、MIDI Clock。
- 长按 Tap 达到触发时间后使用 `Play/Rev`，进入原版 SD 卡读取/保存流程。

原版三种 Deck 模式：

- Reel：线性磁带式回放。
- Slice：面向时钟的切片/复音回放。
- Drift：扩散/粒子式回放。
- 全局路由开关：Generative Stereo、Double Mono、Stereo。

### Deck A 非 Tape 参数

`SOS/MIX A` 向各引擎提供共享的 Mix/输出电平值。下表只列出每个引擎实际读取的参数；原版 Deck A Flux 和 Grit 位于引擎之后。

| 引擎 | `PITCH` | `MOD FREQ` | `MOD AMT` | `SIZE` | `POS` | `ENV` | `Alt + SOS` |
|---|---|---|---|---|---|---|---|
| AudreyII | 弦音高 | 反馈高通 | 混响反馈 | 反馈延迟 | Echo 延迟 | 反馈低通 | 反馈增益 |
| Oscillator | 基频 | 未使用 | 未使用 | 振荡器 detune | 输入 FM 深度 | 滤波截止 | 滤波共振 |
| Reverb | 干湿比 | 未使用 | 未使用 | 未使用 | 未使用 | 混响低通 | 混响反馈 |
| Microcosm | 干湿比 | 粒子周期 | 播放速度 | 粒子长度 | 延迟窗口 | 反馈/驱动系数 | 反馈 |
| Plaits | Carrier 频率 | Formant 频率 | Bleed | Shape | 立体声偏移 | 未使用 | 未使用 |
| Rings | 干湿比 | 共振器频率 | 未使用 | Structure | 立体声偏移 | Brightness | Damping |
| Elements | 频率 | 激励器频率 | Accent | Structure | 激励器 Formant | Brightness | Damping |
| Benjolin | Osc A 频率 | Osc B/Rungler Clock | Rungler 音高深度 | Osc B 偏移 | Rungler 滤波调制 | 滤波截止 | Resonance |

引擎内部使用的 FX 触摸参数：

- AudreyII：`Flux Mix` 为 Echo Send，`Flux Intensity` 为 Echo Feedback。
- Oscillator：`Flux Mix` 为外部输入量。
- Microcosm：`Flux Mix` 为立体声扩展/历史混合，`Flux Intensity` 为粒子抖动，`Grit Mix` 为 Drive，`Grit Intensity` 为降采样。
- Plaits：`Flux Mix` 为输入/激励混合。
- Rings：`Flux Mix` 与 `Flux Intensity` 调整激励。
- Elements：`Flux Mix` 开启 Sustain，`Flux Intensity` 改变激励器 Shape，`Grit Intensity` 改变激励器 Mode。
- Benjolin：`Flux Mix` 混入 XOR，`Flux Intensity` 混入 Rungler Square，`Grit Mix` 混入外部输入，`Grit Intensity` 混入直接 Rungler 调制。

### 保留输入

第一个 4051 的 channel 7 和 `sr2 bit7` 当前未分配功能，未经明确设计决定不得占用。第二个 4051 的 channel 7 是 Crossfade。

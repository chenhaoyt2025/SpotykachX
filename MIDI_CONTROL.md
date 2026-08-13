# SpotykachX MIDI Control

## English

This document describes the MIDI behavior implemented by the current code.

### Interfaces

- TRS MIDI uses UART: `D14 = RX`, `D13 = TX`.
- USB MIDI uses the external/front-panel USB port as a USB MIDI Device, not a USB Host.
- TRS MIDI and USB MIDI can receive simultaneously and share the same mapping.
- System Realtime Clock is sent from both TRS MIDI Out and USB MIDI.

### Channels and configuration

- Deck A defaults to MIDI Channel 1.
- Deck B defaults to MIDI Channel 2.
- `SK/config.txt` on the SD card can set `mid_ch_a`, `mid_ch_b`, `mid_ps_a`, and `mid_ps_b`.
- Channel values are `1..16`. `mid_ps = 1` enables Start/Continue/Stop control for that deck; the default is `0`.

```text
mid_ch_a
1
mid_ch_b
2
mid_ps_a
0
mid_ps_b
0
```

### Notes and realtime messages

- Only Note On is handled. Note Off and velocity are ignored.
- Note 60 triggers the original `1x` playback speed; semitone offsets change trigger speed chromatically.
- Note On uses the original deck trigger path. It does not directly play the added Deck A synthesis engines.
- In MIDI clock mode, incoming Timing Clock drives the system clock.
- Start and Continue reset the clock position. When `mid_ps` is enabled, they also start a non-empty deck.
- Stop stops a deck only when its `mid_ps` option is enabled.

### CC mapping

Continuous values map `0..127` to `0.0..1.0`.

| CC | Function | Current behavior |
|---:|---|---|
| 3 | Crossfade | Global Deck A/B crossfade |
| 14 | External Record | Value above 0 triggers external record/source selection |
| 15 | Internal Record | Value above 0 triggers internal record/source selection |
| 20 | Start Position | Selected deck start position |
| 21 | Start Offset | Selected deck start-offset interval |
| 22 | Size | Selected deck size |
| 23 | Envelope | Selected deck envelope shape |
| 24 | Pitch | Tape speed/pitch; Deck A non-Tape primary parameter |
| 25 | I/O Mix | Tape I/O Mix; Deck A non-Tape shared engine Mix/output level |
| 26 | Deck Feedback | Selected deck feedback |
| 27 | Envelope Size | Selected deck envelope size |
| 28 | Window Size | Selected deck window size |
| 85 | Forward Play | Value above 0 triggers forward play |
| 86 | Reverse Play | Value above 0 triggers reverse play |
| 89 | Mod Cycle | Selected deck Mod Speed |
| 90 | Mod Glow | Selected deck Mod Amount |
| 102 | Grit On | `0 = off`, `1..127 = on` |
| 103 | Grit Intensity | Selected deck Grit intensity |
| 104 | Grit Mix | Selected deck Grit mix |
| 105 | Flux On | `0 = off`, `1..127 = on` |
| 106 | Flux Intensity | Selected deck Flux intensity |
| 107 | Flux Feedback | Selected deck Flux feedback |
| 108 | Flux Mix | Selected deck Flux mix |

For Deck A non-Tape engines, CC 24 controls pitch on self-generating engines and wet/dry on Reverb, Microcosm, and Rings. CC 25 uses the same shared engine Mix/output-level path as the `SOS/MIX` knob.

### MIDI output and unsupported messages

- Current MIDI output contains System Realtime Clock only.
- Note, CC, Start, Continue, and Stop are not transmitted.
- Pitch Bend, Aftertouch, Program Change, velocity, Note Off, and MIDI engine selection are not implemented.

## 中文

本文记录当前代码实际实现的 MIDI 功能。

### 接口

- TRS MIDI 使用 UART：`D14 = RX`，`D13 = TX`。
- USB MIDI 使用前置/外部 USB，工作于 USB MIDI Device，不是 USB Host。
- TRS MIDI 和 USB MIDI 可以同时接收，并使用相同映射。
- System Realtime Clock 同时从 TRS MIDI Out 和 USB MIDI 发出。

### 通道与配置

- Deck A 默认使用 MIDI Channel 1。
- Deck B 默认使用 MIDI Channel 2。
- SD 卡中的 `SK/config.txt` 可以设置 `mid_ch_a`、`mid_ch_b`、`mid_ps_a`、`mid_ps_b`。
- 通道填写 `1..16`。`mid_ps = 1` 时允许 Start/Continue/Stop 控制对应 Deck，默认值为 `0`。

```text
mid_ch_a
1
mid_ch_b
2
mid_ps_a
0
mid_ps_b
0
```

### Note 与实时消息

- 当前只处理 Note On，不处理 Note Off，Velocity 不参与运算。
- Note 60 对应原速 `1x`，相对 60 的半音差用于改变触发速度。
- Note On 进入原版 Deck 触发路径，不会直接演奏新增的 Deck A 合成引擎。
- 在 MIDI 时钟模式下，接收到的 Timing Clock 控制系统时钟。
- Start 和 Continue 会复位时钟位置；对应 `mid_ps` 开启时，还会启动非空 Deck。
- Stop 只在对应 `mid_ps` 开启时停止 Deck。

### CC 映射

连续参数把 `0..127` 映射为 `0.0..1.0`。

| CC | 功能 | 当前行为 |
|---:|---|---|
| 3 | Crossfade | 全局 Deck A/B Crossfade |
| 14 | External Record | 大于 0 时触发外部录音或外部音源选择 |
| 15 | Internal Record | 大于 0 时触发内部录音或内部音源选择 |
| 20 | Start Position | 对应 Deck 的起始位置 |
| 21 | Start Offset | 对应 Deck 的起始偏移间隔 |
| 22 | Size | 对应 Deck 的 Size |
| 23 | Envelope | 对应 Deck 的 Envelope Shape |
| 24 | Pitch | Tape 速度/音高；Deck A 非 Tape 主参数 |
| 25 | I/O Mix | Tape I/O Mix；Deck A 非 Tape 的引擎 Mix/输出电平共享控制 |
| 26 | Deck Feedback | 对应 Deck 的 Feedback |
| 27 | Envelope Size | 对应 Deck 的 Envelope Size |
| 28 | Window Size | 对应 Deck 的 Window Size |
| 85 | Forward Play | 大于 0 时触发正向播放 |
| 86 | Reverse Play | 大于 0 时触发反向播放 |
| 89 | Mod Cycle | 对应 Deck 的 Mod Speed |
| 90 | Mod Glow | 对应 Deck 的 Mod Amount |
| 102 | Grit On | `0 = off`，`1..127 = on` |
| 103 | Grit Intensity | 对应 Deck 的 Grit Intensity |
| 104 | Grit Mix | 对应 Deck 的 Grit Mix |
| 105 | Flux On | `0 = off`，`1..127 = on` |
| 106 | Flux Intensity | 对应 Deck 的 Flux Intensity |
| 107 | Flux Feedback | 对应 Deck 的 Flux Feedback |
| 108 | Flux Mix | 对应 Deck 的 Flux Mix |

Deck A 非 Tape 模式中，CC 24 在自发声引擎里控制音高，在 Reverb、Microcosm、Rings 中控制干湿比。CC 25 与 `SOS/MIX` 旋钮一样，使用引擎 Mix/输出电平共享控制路径。

### MIDI 输出与未实现消息

- 当前 MIDI Out 只发送 System Realtime Clock。
- 不发送 Note、CC、Start、Continue 或 Stop。
- Pitch Bend、Aftertouch、Program Change、Velocity、Note Off 和 MIDI 引擎切换均未实现。

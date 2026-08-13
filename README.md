# SpotykachX

This project was mainly completed by Codex. Feel free to use it however you like.

Original project: [Synthux-Academy/Spotykach](https://github.com/Synthux-Academy/Spotykach)

Base version: official Spotykach `v1.2.0`

Current SpotykachX release: `v0.2`

## English

SpotykachX keeps the original two-deck Tape workflow and adds switchable sound/effect engines to Deck A. Deck B always remains the original Tape deck.

### Main additions

- `Spot + Trig A` cycles Deck A through `Tape`, `AudreyII`, `Oscillator`, `Reverb`, `Microcosm`, `Plaits`, `Rings`, `Elements`, and `Benjolin`.
- The selected Deck A engine is saved in QSPI and restored after reboot.
- In a non-Tape engine, `Alt + Play A` selects external input and `Alt + Rev A` selects the complete Deck B output.
- Deck A non-Tape output passes through the original Deck A Flux and Grit effects.
- In Deck A non-Tape modes, `SOS/MIX` uses the original shared engine Mix/output-level control path.
- Leaving Tape preserves the Deck A audio buffer and Tape control values. Returning to Tape restores those values and the original Tape ring display.
- USB MIDI Device and TRS MIDI are both enabled.
- The startup LED animation uses a moving color gradient.
- Internal Tap Tempo directly applies the measured BPM to the running internal clock.

All original hardware mappings are used. Apart from the features listed above and the modified Tap update path, original Tape, SD card, routing, CV, gate, Flux, Grit, clock-source, and Deck B behavior are retained.

### Audio and storage

- Audio rate: `48 kHz`.
- Live stereo recording buffer: up to approximately `42 seconds` per deck.
- When the initial recording reaches the buffer limit, its length is fixed and recording continues as loop overdub until recording is stopped.
- SDMMC uses the original 1-bit `MEDIUM_SLOW` configuration.

### Documentation

- [Control Mapping](CONTROL_MAPPING.md)
- [MIDI Control](MIDI_CONTROL.md)

### Build

```sh
make clean
make -j8
```

Output: `build/spotykachx.bin`

### Flash

Enter the Spotykach front-panel USB bootloader DFU mode, then run:

```sh
make program-dfu
```

## 中文

本项目的代码主要由 Codex 完成，请随心所欲地使用。

原版项目：[Synthux-Academy/Spotykach](https://github.com/Synthux-Academy/Spotykach)

基础版本：官方 Spotykach `v1.2.0`

当前 SpotykachX 版本：`v0.2`

SpotykachX 保留原版双 Deck Tape 工作流，只给 Deck A 增加可切换的声音与效果引擎。Deck B 始终保持原版 Tape。

### 主要新增功能

- `Spot + Trig A` 循环切换 Deck A 的 `Tape`、`AudreyII`、`Oscillator`、`Reverb`、`Microcosm`、`Plaits`、`Rings`、`Elements`、`Benjolin`。
- 当前 Deck A 引擎保存到 QSPI，重启后恢复。
- Deck A 非 Tape 引擎中，`Alt + Play A` 选择外部输入，`Alt + Rev A` 选择 Deck B 的完整输出。
- Deck A 非 Tape 引擎输出继续经过 Deck A 原有的 Flux 和 Grit。
- Deck A 非 Tape 模式下，`SOS/MIX` 使用原来的引擎 Mix/输出电平共享控制路径。
- 离开 Tape 时保留 Deck A 录音缓冲和 Tape 控制值；切回 Tape 时恢复这些值与原版 Tape 光环显示。
- USB MIDI Device 与 TRS MIDI 均已开启。
- 开机 LED 使用动态彩色渐变。
- Internal 模式的 Tap Tempo 会把测得的 BPM 直接应用到正在运行的内部时钟。

项目使用全部原版硬件映射。除上述新增功能和 Tap 更新路径外，原版 Tape、SD 卡、路由、CV、Gate、Flux、Grit、时钟来源和 Deck B 行为保持不变。

### 音频与存储

- 音频采样率：`48 kHz`。
- 每个 Deck 的实时立体声录音缓存最长约 `42 秒`。
- 首次录音达到缓存上限后，循环长度固定，并继续循环 overdub，直到停止录音。
- SDMMC 使用原版 1-bit `MEDIUM_SLOW` 配置。

### 文档

- [控制映射](CONTROL_MAPPING.md)
- [MIDI 控制](MIDI_CONTROL.md)

### 编译

```sh
make clean
make -j8
```

输出文件：`build/spotykachx.bin`

### 烧录

进入 Spotykach 前置 USB bootloader DFU 模式后执行：

```sh
make program-dfu
```

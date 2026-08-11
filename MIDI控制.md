# SpotykachX MIDI 控制

这份文档只记录当前 `SpotykachX` 代码已经实现的 MIDI 功能。

## 一、MIDI 接口

1. `TRS MIDI`：UART MIDI，`D14 = RX`，`D13 = TX`
2. `USB MIDI`：使用前置/外部 USB，工作于 USB MIDI Device 模式，不是 USB Host
3. TRS MIDI 与 USB MIDI 使用同一套消息处理和控制映射
4. 两个接口可以同时接收 MIDI
5. MIDI Clock 同时从 TRS MIDI Out 和 USB MIDI 发出

## 二、默认 MIDI 通道

1. `Deck A`：MIDI Channel 1
2. `Deck B`：MIDI Channel 2
3. 其他通道不用于 deck 控制

可以通过 SD 卡中的 `SK/config.txt` 修改：

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

参数含义：

1. `mid_ch_a / mid_ch_b`：设置 Deck A/B 的 MIDI 通道，填写 `1..16`
2. `mid_ps_a / mid_ps_b`：是否让 MIDI Start/Continue/Stop 控制对应 deck
3. `mid_ps = 0`：关闭 MIDI Start/Stop 联动
4. `mid_ps = 1`：开启 MIDI Start/Stop 联动
5. 没有读取到配置文件时，默认使用 Channel 1/2，Start/Stop 联动默认关闭

## 三、MIDI Note

1. 当前只处理 `Note On`
2. Channel 1 默认触发 Deck A
3. Channel 2 默认触发 Deck B
4. `Note 60` 对应原速 `1x`
5. 音符编号相对 60 每变化一个半音，触发播放速度按半音比例变化
6. Velocity 当前不参与运算
7. `Note Off` 当前不处理
8. Note On 进入原 Spotykach deck 的触发路径，不是为新增声音引擎单独实现的音符触发接口

## 四、MIDI Clock / Start / Stop

1. 当 clock source 切换到 `midi` 时，接收到的 `Timing Clock` 控制系统速度
2. `Start` 和 `Continue` 会复位内部时钟位置
3. 当对应的 `mid_ps_a / mid_ps_b = 1` 时，`Start / Continue` 会播放非空 deck
4. 当对应的 `mid_ps_a / mid_ps_b = 1` 时，`Stop` 会停止对应 deck
5. MIDI Clock 来源的 LED 颜色为青色 `#00FFEF`

## 五、MIDI CC 映射

普通连续参数把 MIDI 值 `0..127` 映射为内部值 `0.0..1.0`。

| CC | 功能 | 作用范围 / 说明 |
|---:|---|---|
| 3 | Crossfade | 全局 A/B Crossfade；A 或 B 的有效通道都可控制 |
| 14 | External Record | 外部录音；值大于 0 时触发一次 |
| 15 | Internal Record | 内部录音；值大于 0 时触发一次 |
| 20 | Start Position | 对应 deck 的起始位置 |
| 21 | Start Offset | 对应 deck 的起始偏移间隔 |
| 22 | Size | 对应 deck 的 Size |
| 23 | Envelope | 对应 deck 的 Envelope Shape |
| 24 | Pitch | 对应 deck 的 Speed / Pitch |
| 25 | I/O Mix | 对应 deck 的 I/O Mix；新增 engine 也从该共享参数取得 `SOS / MIX` |
| 26 | Deck Feedback | 对应 deck 的 Feedback |
| 27 | Envelope Size | 对应 deck 的 Envelope Size |
| 28 | Window Size | 对应 deck 的 Window Size |
| 85 | Forward Play | 正向播放切换；值大于 0 时触发一次 |
| 86 | Reverse Play | 反向播放切换；值大于 0 时触发一次 |
| 89 | Mod Cycle | 对应 deck 的 Mod Speed / Cycle |
| 90 | Mod Glow | 对应 deck 的 Mod Amount / Glow |
| 102 | Grit On | `0 = off`，`1..127 = on` |
| 103 | Grit Intensity | 对应 deck 的 Grit Intensity |
| 104 | Grit Mix | 对应 deck 的 Grit Mix |
| 105 | Flux On | `0 = off`，`1..127 = on` |
| 106 | Flux Intensity | 对应 deck 的 Flux Intensity |
| 107 | Flux Feedback | 对应 deck 的 Flux Feedback |
| 108 | Flux Mix | 对应 deck 的 Flux Mix |

对于非 Tape engine，CC 仍修改同名的共享参数。该参数在不同 engine 中的实际含义，按照 `控制映射.md` 中各 engine 的参数映射执行。例如 CC 24 会控制当前 engine 的 `PITCH` 参数。

## 六、MIDI Out 当前内容

1. TRS MIDI Out 和 USB MIDI Out 当前只发送 `System Realtime Clock`
2. 当前不发送 Note、CC、Start、Continue 或 Stop

## 七、当前没有实现的 MIDI 控制

1. `Note Off`
2. Velocity
3. Pitch Bend
4. Aftertouch
5. Program Change
6. MIDI 切换 engine 类型
7. MIDI 切换 `fx_only`
8. MIDI 切换 Deck B `through`

## 八、代码位置

1. MIDI 接口初始化：`src/hw/hardware.cpp`
2. MIDI 消息收发：`src/ui/core.midi.cpp`
3. Note 和 CC 功能执行：`src/ui/core.ui.cpp`
4. CC 编号与默认通道：`src/core/config.h`
5. SD 配置读取：`src/core/config.cpp`、`src/memory/storage.h`

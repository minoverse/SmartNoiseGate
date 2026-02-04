# Smart Noise Gate

Physics-based noise gate plugin for music production, using energy detection inspired by embedded voice activity detection research.

## Screenshot

![SmartNoiseGate Plugin](20260204_033325.png)
![SmartNoiseGate Plugin](20260204_033311.png)
*Standalone application running successfully on Windows*

## Features

- **Energy-based detection**: RMS energy threshold gating
- **Musical envelopes**: Adjustable attack/release parameters for smooth transitions
- **Real-time performance**: Optimized DSP processing
- **Cross-platform**: VST3 + Standalone formats

## Use Cases

### Vocal Recording
Remove room tone and breath noise between phrases while preserving natural vocal dynamics.

### Guitar/Bass
Eliminate amplifier hiss during silent passages without cutting off natural sustain.

### Drums
Reduce microphone bleed between hits while maintaining natural cymbal decay.

### Podcasts
Clean background noise without harsh cuts.

## How It Works

Based on frame-by-frame RMS energy analysis:

1. Calculate energy of incoming audio buffer
2. Compare to adjustable threshold
3. Open/close gate with musical attack/release curves

This approach is derived from my [embedded VAD research](https://github.com/minoverse/embedded-vad-validation) where I validated energy-based detection achieving 95% speech pass rate and 92% noise blocking.

## Parameters

- **Threshold** (0.0001 - 0.1): Energy level below which gate closes
- **Attack** (1-100ms): How quickly gate opens when signal is present
- **Release** (10-1000ms): How quickly gate closes when signal is absent

## Technical Implementation
```cpp// Core gate logic: RMS energy calculation
float energy = 0.0f;
for (int channel = 0; channel < numChannels; ++channel) {
for (int sample = 0; sample < numSamples; ++sample) {
energy += data[sample] * data[sample];
}
}
energy = energy / totalSamples;// Smooth envelope with attack/release
float targetGain = (energy > threshold) ? 1.0f : 0.0f;
float coeff = (targetGain > gateEnvelope) ? attack : release;
gateEnvelope += (targetGain - gateEnvelope) * coeff;// Apply smooth gate
channelData[sample] *= gateEnvelope;

Implements the same energy detection algorithm validated in my VAD research, adapted for musical gating with smooth envelopes.

## Building

**Requirements:**
- JUCE 7.x or later
- Visual Studio 2022 (Windows) or Xcode (macOS)

**Steps:**
1. Open `SmartNoiseGate.jucer` in Projucer
2. Generate project for your IDE
3. Build in Release mode

**Output formats:**
- VST3: `Builds/VisualStudio2022/x64/Release/VST3/SmartNoiseGate.vst3`
- Standalone: `Builds/VisualStudio2022/x64/Release/Standalone Plugin/SmartNoiseGate.exe`

## Platform Support

- ✅ Windows (tested)
- ✅ macOS (JUCE cross-platform support)
- ✅ Linux (JUCE cross-platform support)

## Technical Background

This plugin demonstrates translating DSP research into practical audio tools. The energy-based detection method was validated scientifically in my embedded systems research, achieving:

- 95% speech detection rate
- 92% noise blocking rate
- 9% CPU usage target on ARM Cortex-M4

The same core algorithm is adapted here for music production with musical envelope shaping.

## Related Projects

- [**Embedded VAD Validation**](https://github.com/minoverse/embedded-vad-validation) - Research validation for energy-based audio detection
- [**Mission Control Audio Engine**](https://github.com/AGH-Skylink/mission_control_audio) - Real-time 4-channel audio system with DSP processing

## License

MIT

---

*Built as part of learning JUCE framework and demonstrating the ability to translate DSP research into production audio tools.*

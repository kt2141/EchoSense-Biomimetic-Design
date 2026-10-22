# EchoSense: Biomimetic Haptic Wearable

![Status](https://img.shields.io/badge/status-concept%20%2F%20design%20project-F5A623)
![Platform](https://img.shields.io/badge/platform-Arduino%20Nano-00979D)
![Sensor](https://img.shields.io/badge/sensor-HC--SR04%20(40%20kHz)-1FA3B8)
![CAD](https://img.shields.io/badge/CAD-Fusion%20360-D98A00)
![SDG](https://img.shields.io/badge/UN%20SDG-3%20%7C%209%20%7C%2010%20%7C%2011-0B1B33)

> **Feel your surroundings beyond sight and sound.**

**Live site:** https://kt2141.github.io/EchoSense-Biomimetic-Design/
**Designer:** Aravind K T (Roll No. 240169), B.Tech Electrical Engineering, IIT Kanpur · [Portfolio](https://sites.google.com/view/aravind-k-t-iitk/)
**Course:** Design for Life · Presentation 12 November 2026

![EchoSense concept](assets/echosense_infographic.jpg)

## Project Overview

EchoSense is a conceptual wrist-worn wearable that translates spatial data from ultrasonic echolocation into haptic feedback for visually impaired pedestrians. It senses obstacles ahead, measures their distance and renders that distance as vibration patterns on the skin: silent when the path is clear, a gentle pulse when something is near, a continuous buzz when it is urgent.

- **Silent:** ultrasonic sensing is inaudible, so hearing stays free for traffic and people.
- **Hands-free:** worn on the wrist or forearm with a neoprene strap.
- **Low-cost:** built around commodity parts (Arduino Nano, HC-SR04, coin vibration motors).

## The "Sensory Gap" Problem Statement

Visually impaired urban pedestrians need a way to detect obstacles and judge their distance early in crowded, noisy city streets, but are held back by sensory deprivation of sight and an overloaded sense of hearing that is already busy tracking traffic.

**How might we** translate the space around a visually impaired pedestrian into silent, intuitive touch cues **so that** they can walk confidently and independently without relying on their eyes or ears?

| Sense | Today (busy street) | With EchoSense |
|---|---|---|
| Sight | Deprived | Bypassed (complements, 2/5) |
| Sound | Overloaded by traffic and prompts | Ultrasonic sensing is inaudible (3/5) |
| Touch | Cane tip only, ground level | Primary channel: field of proximity (5/5) |
| Smell | Informal landmarks | Untouched (1/5) |
| Taste | Not used | Not used (1/5) |

## Biomimetic Inspiration (*Apis mellifera* & Bats)

- **Bats (echolocation):** bats emit ultrasonic calls and time the returning echo to map space in darkness. EchoSense mirrors this with a 40 kHz sensor and the echo equation `d = (v × t) / 2` (v ≈ 343 m/s).
- **Honeybees (*Apis mellifera*):** the hexagon is the most space-efficient tiling. EchoSense borrows it for the shell texture and the honeycomb layout of the haptic motors, so cues stay distinct on a small patch of skin.

![Bat to band](assets/double_diamond.png)

## System Architecture (Arduino Nano, HC-SR04, PWM Haptics: conceptual)

![Architecture](assets/architecture.png)

| Block | Part | Notes |
|---|---|---|
| Sense | HC-SR04 | 40 kHz, 2–400 cm, roughly 15° beam, 5 V |
| Think | Arduino Nano (ATmega328P) | Times echo pulse, smooths readings, selects zone |
| Drive | NPN transistor / MOSFET + flyback diode | PWM duty cycle sets intensity |
| Feel | 3 × coin ERM vibration motors | Honeycomb arrangement on the strap |
| Power | 3.7 V Li-ion + boost/charge module | Conceptual; sized in later prototype |

### Tactile language

| Zone | Distance | Pattern | Meaning |
|---|---|---|---|
| Clear | > 200 cm | Silence | Path is clear |
| Aware | 100–200 cm | Gentle pulse (about 1 Hz) | Something ahead |
| Near | 50–100 cm | Faster pulse (about 3 Hz) | Slow down and adjust |
| Danger | < 50 cm | Continuous buzz | Stop |

![Mapping](assets/mapping.png)

### Firmware (C++ / Arduino)

Reference sketch: [`firmware/echosense.ino`](firmware/echosense.ino)

```cpp
// Core idea (see firmware/echosense.ino for the full sketch)
long cm = readDistanceCm();      // HC-SR04: pulseIn(ECHO) * 0.0343 / 2
Zone z  = zoneFor(cm);           // CLEAR, AWARE, NEAR, DANGER
renderHaptics(z);                // PWM pattern on 3 motors
```

## Digital Twin / CAD Models

| Asset | Path |
|---|---|
| Fusion 360 source | `cad/echosense.f3d` *(add)* |
| Assembled render | `assets/render-1.png` *(add)* |
| Exploded view | `assets/render-2.png` *(add)* |
| 3D-printed shell photo | `assets/photo-shell.jpg` *(add)* |
| Neoprene strap photo | `assets/photo-strap.jpg` *(add)* |

## Design Process

Discover (sensory audit, sensitivities, Sensory Gap) → Define (persona, 5W1H, problem statement) → Develop (sensory pentagon, five-sense ideation) → Deliver (storyboard, mood box, harmony test, CAD, live site).

## Repository Structure

```
EchoSense-Biomimetic-Design/
├── index.html          # GitHub Pages site
├── README.md
├── assets/             # diagrams, renders, photos
├── firmware/echosense.ino
└── cad/                # Fusion 360 files and STL exports
```

## Roadmap

- [x] Concept, sensory audit, storyboard, mood box
- [ ] Working Arduino Nano bench prototype
- [ ] User testing with visually impaired participants
- [ ] Multi-sensor sweep and head-height detection
- [ ] Waterproofing, rechargeable battery, cost reduction

## References

1. WHO (2019). *World report on vision.*
2. Griffin, D. R. (1958). *Listening in the Dark.* Yale University Press.
3. HC-SR04 datasheet; Arduino Nano (ATmega328P) documentation.

## Author

**Aravind K T** · Roll No. 240169 · IIT Kanpur · aravindkt24@iitk.ac.in

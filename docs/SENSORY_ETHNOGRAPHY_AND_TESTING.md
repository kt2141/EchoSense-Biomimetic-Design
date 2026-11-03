# EchoSense Sensory Ethnography, Mood Box, and Verification Protocol

**Project:** EchoSense: Biomimetic Haptic Wearable  
**Course:** DES653 Design for Life, IIT Kanpur  
**Author:** Aravind K T (Roll No. 240269, B.Tech Electrical Engineering)

## 1) Double-Diamond Design Process

### Discover
- Conducted contextual observation of visually impaired commuters at mixed-signal urban footpaths (bus stops, kerbs, poles, crowd bottlenecks).
- Identified a recurring sensory bottleneck: hearing channels were saturated by traffic horns, braking noise, and voice-navigation prompts.
- Mapped sensory burden onto a 0-5 pentagon and found touch as the least congested channel for immediate proximity cues.

### Define
- **Core challenge:** “Sensory Gap” between obstacle proximity and timely, low-cognitive-load perception in dense streets.
- **Biomimetic translation:**
  - Bat-like echolocation (40 kHz ultrasound + ToF distance)
  - Honeybee-inspired hexagonal structure for shell texture and tri-motor haptic array geometry

### Develop
- Iterated tactile language into four non-habituating zones (Clear, Aware, Near, Danger) with monotonic PWM scaling.
- Tuned material stack-ups in the mood box to balance haptic clarity and wear comfort (neoprene + silicone damping + PLA/TPU shell).
- Simulated pulse envelope behavior (1 Hz awareness, 3 Hz alert, continuous danger) before hardware flashing.

### Deliver
- Integrated interactive simulation, non-blocking firmware, and analytics pipeline.
- Prepared classroom blindfold protocol for repeatable verification and acceptance thresholding (>=80% distance recognition accuracy).

## 2) Persona (5W1H) and Exact HMW Statement

### Persona: Asha (Primary User)
- **Who:** Asha, 27, postgraduate commuter with severe visual impairment.
- **What:** Needs fast obstacle proximity information while walking independently.
- **When:** During peak urban commute windows with high background noise.
- **Where:** Mixed footpaths with poles, kerbs, crowd clusters, and overhanging branches.
- **Why:** Existing cane + hearing workflow can miss elevated obstacles and causes auditory fatigue.
- **How:** Uses a wrist-worn tactile wearable that converts ultrasonic distance into intuitive haptic pulses.

### HMW Problem Statement
**How might we rebalance sensory load for visually impaired urban pedestrians by converting real-time echolocation distance into an intuitive, low-fatigue tactile language that improves obstacle anticipation without adding auditory clutter?**

## 3) Mood Box Material Swatch Specifications

| Component | Specification | Functional Role |
|---|---|---|
| Wrist substrate | 3 mm breathable neoprene | Comfort, sweat tolerance, conformal contact |
| Outer shell | PLA/TPU hybrid honeycomb | Stiffness-to-weight efficiency, tactile identity, impact resistance |
| Inner liner | Medical-grade silicone damping layer | Skin-safe interface, vibration coupling control |
| Fastener | Hook-and-loop closure | Rapid don/doff and fit adjustability |
| Haptic placement | 3x coin ERM motors in honeycomb cluster | Direction-neutral but intensity-rich tactile alerting |

## 4) Sensory Harmony & Cognitive Dissonance Matrix

| Interaction Pair | Harmony Objective | Dissonance Risk | Mitigation |
|---|---|---|---|
| Sound ↔ Touch | Preserve hearing for environment and prompts while touch handles distance urgency | Competing auditory alerts | Keep ultrasound inaudible (40 kHz) and use tactile-only obstacle coding |
| Touch ↔ Distance | One-to-one urgency mapping from range to pulse intensity/frequency | Habituation from constant buzzing | Clear zone = 0% PWM; pulsed envelopes in Aware/Near; continuous only in Danger |
| Material ↔ Function | Stable vibration transmission with comfortable prolonged wear | Over-damping or skin irritation | Silicone-liner thickness tuning + breathable neoprene backing |
| Sight ↔ Touch | Minimize visual dependence while retaining optional status feedback | Reliance on visual cues in low-vision contexts | Dim LED only for diagnostics; navigation semantics remain tactile |

## 5) Classroom Blindfolded Trial Protocol (No Prior Training)

## Objective
Evaluate whether first-time users can classify obstacle proximity zones from EchoSense tactile cues alone.

## Setup
- Participants: 10-20 classmates, blindfolded.
- Distances randomized across four classes (>2.0 m, 1.0-2.0 m, 0.5-1.0 m, <0.5 m).
- Trial environment: controlled hallway with representative obstacles (pole, kerb edge, crowd proxy, branch proxy).

## Procedure (10 trials per participant)
1. Fit EchoSense snugly on dominant wrist.
2. Start from neutral orientation with no verbal hints.
3. Present randomized obstacle distance.
4. Participant reports inferred zone: Clear / Aware / Near / Danger.
5. Record correctness and response latency.

## Rubric and Acceptance Criteria
- **Primary metric:** Zone classification accuracy over 10 trials.
- **Target threshold:** **>=80%** correct without prior training.
- **Secondary metrics:**
  - Median response latency per zone
  - Confusion matrix (especially Near vs Danger boundary)
  - Subjective fatigue rating (Likert 1-5)

## Data Logging Template
- Participant ID
- Trial number
- Ground-truth distance class
- Reported class
- Correct (0/1)
- Response time (s)
- Notes (hesitation, strap fit, environmental noise)

## Iteration Triggers
- Accuracy <80% in aggregate: retune zone boundaries or pulse contrast.
- Frequent Near/Danger confusion: increase duty/frequency separation near 50 cm threshold.
- High fatigue reports: reduce Aware-zone duty floor and/or pulse on-time ratio.

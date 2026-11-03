#!/usr/bin/env python3
"""Generate sensory analytics plots for EchoSense biomimetic wearable."""

from pathlib import Path

import matplotlib.pyplot as plt
import numpy as np

SPEED_SOUND = 343.0  # m/s


def pwm_duty_from_distance(distance_cm: np.ndarray) -> np.ndarray:
    duty = np.zeros_like(distance_cm, dtype=float)

    aware = (distance_cm >= 100) & (distance_cm <= 200)
    near = (distance_cm >= 50) & (distance_cm < 100)
    danger = distance_cm < 50

    duty[aware] = 35 + ((200 - distance_cm[aware]) / 100) * 15
    duty[near] = 65 + ((100 - distance_cm[near]) / 50) * 15
    duty[danger] = 100
    return duty


def pulse_wave(t: np.ndarray, zone: str, duty: float) -> np.ndarray:
    if zone == "CLEAR":
      return np.zeros_like(t)
    if zone == "DANGER":
      return np.full_like(t, duty / 100.0)

    hz = 1.0 if zone == "AWARE" else 3.0
    phase = (t * hz) % 1.0
    envelope = (phase < 0.5).astype(float)
    return envelope * (duty / 100.0)


def plot_tof_and_pwm(out_dir: Path) -> None:
    dist_cm = np.linspace(0, 400, 801)
    tof_ms = (2 * (dist_cm / 100.0) / SPEED_SOUND) * 1000
    duty = pwm_duty_from_distance(dist_cm)

    fig, ax1 = plt.subplots(figsize=(10, 6), dpi=240)
    ax1.plot(dist_cm, tof_ms, color="#37d6c2", linewidth=2.5, label="Time-of-Flight")
    ax1.set_xlabel("Obstacle Distance (cm)")
    ax1.set_ylabel("ToF (ms)", color="#37d6c2")
    ax1.tick_params(axis="y", labelcolor="#37d6c2")
    ax1.grid(alpha=0.25)

    ax2 = ax1.twinx()
    ax2.plot(dist_cm, duty, color="#ffbf40", linewidth=2.5, label="PWM Duty")
    ax2.set_ylabel("PWM Duty Cycle (%)", color="#ffbf40")
    ax2.tick_params(axis="y", labelcolor="#ffbf40")

    ax1.set_title("EchoSense Response Curves: ToF and Monotonic Distance-to-PWM Mapping")
    fig.tight_layout()
    fig.savefig(out_dir / "tof_vs_distance_and_pwm.png")
    plt.close(fig)


def plot_storyboard_waveforms(out_dir: Path) -> None:
    t = np.linspace(0, 4.0, 2000)
    scenarios = [
        ("CLEAR", 250.0, "Clear (No obstacle >2.0m)"),
        ("AWARE", 150.0, "Aware (1.5m pole)"),
        ("NEAR", 80.0, "Near (0.8m crowd)"),
        ("DANGER", 40.0, "Danger (<0.5m kerb)"),
    ]

    fig, axes = plt.subplots(4, 1, figsize=(11, 8), dpi=240, sharex=True)
    for ax, (zone, distance, title) in zip(axes, scenarios):
        duty = pwm_duty_from_distance(np.array([distance]))[0]
        y = pulse_wave(t, zone, duty)
        ax.plot(t, y, linewidth=2.2, color="#ffbf40")
        ax.fill_between(t, 0, y, alpha=0.18, color="#37d6c2")
        ax.set_ylim(-0.05, 1.05)
        ax.set_ylabel("Norm.\nPWM")
        ax.set_title(f"{title} | Duty={duty:.1f}%")
        ax.grid(alpha=0.25)

    axes[-1].set_xlabel("Time (s)")
    fig.suptitle("EchoSense 4-Stage Sensory Storyboard Waveforms", fontsize=13)
    fig.tight_layout(rect=(0, 0, 1, 0.98))
    fig.savefig(out_dir / "sensory_storyboard_waveforms.png")
    plt.close(fig)


def plot_cognitive_load(out_dir: Path) -> None:
    dimensions = [
        "Auditory\nOverload",
        "Obstacle\nLocalization",
        "Reaction\nConfidence",
        "Navigation\nFatigue",
    ]
    baseline = np.array([4.7, 2.1, 2.4, 4.2])
    echosense = np.array([2.3, 4.1, 4.2, 2.5])

    x = np.arange(len(dimensions))
    width = 0.36

    fig, ax = plt.subplots(figsize=(10, 6), dpi=240)
    ax.bar(x - width / 2, baseline, width, label="Cane + Hearing Only", color="#ff7b7b")
    ax.bar(x + width / 2, echosense, width, label="EchoSense Rebalancing", color="#37d6c2")
    ax.set_xticks(x)
    ax.set_xticklabels(dimensions)
    ax.set_ylim(0, 5)
    ax.set_ylabel("Cognitive Load / Performance Index (0-5)")
    ax.set_title("Cognitive Load Comparison: Baseline vs EchoSense")
    ax.legend()
    ax.grid(axis="y", alpha=0.25)

    fig.tight_layout()
    fig.savefig(out_dir / "cognitive_load_comparison.png")
    plt.close(fig)


def main() -> None:
    out_dir = Path(__file__).resolve().parent / "outputs"
    out_dir.mkdir(parents=True, exist_ok=True)

    plot_tof_and_pwm(out_dir)
    plot_storyboard_waveforms(out_dir)
    plot_cognitive_load(out_dir)
    print(f"Generated plots in: {out_dir}")


if __name__ == "__main__":
    main()

# Game of Life

A C++ implementation of Conway's Game of Life with different rules.

## Features

- **10 Rules**: Standard, HighLife, Life without Death, 34 Life, Freezing, Desert, Seeds, Maze, OnePeriodLife and Fractals.
- **Dual Mode**: Quick start & manual configuration
- **Cross-Platform**: Windows/Linux/macOS support
- **Visualization**: Animated console display

## Rule Sets

| Rule | Name | Behavior |
|------|------|----------|
| **B3/S23** | Standard | Classic Conway's patterns |
| **B36/S23** | HighLife | Self-replicating structures |
| **B3/S012345678** | No Death | Coral growth patterns |
| **B34/S34** | 34 Life | Dense foam structures |
| **B2/S0** | Freezing | Rapid stabilization |
| **B2/S2** | Desert | Minimal survival |
| **B2/S** | Seeds | Exploding patterns |
| **B3/S12345** | Maze | Labyrinth-like growth |
| **B1/S1** | One Period | Fractal patterns |
| **B1/S012345678** | Fractals | Tree-like growth |

## Quick Start

```bash
g++ -o life main.cpp sleep_utils.cpp -std=c++11
./life

# Finite State Machine (FSM) Simulation

## Technical Overview
This project is a high-level logical simulation of a **Vending Machine** implemented in **Standard C++**. It utilizes a **Finite State Machine (FSM)** architecture, a cornerstone of deterministic system design where the system can only be in one of a finite number of states at any given time.

## Architectural Features
* **Deterministic Logic:** Transitions are strictly defined by events (inserting coins), ensuring the system never reaches an "undefined" state.
* **Encapsulation:** The logic is encapsulated within a `Vending Machine` class, separating the internal state from the external interface.
* **Type Safety:** Employs `enum class` (Strongly Typed Enums) to prevent accidental integer comparisons or invalid state assignments, a common source of bugs in large-scale C++ applications.
* **Recursive State Evaluation:** Demonstrates how complex logical flows can be managed through state-driven recursive calls.

## Use Cases
FSMs are not only used in hardware; they are critical in:
- Developing UI navigation flows.
- Game AI behavior trees.
- Network protocol parsing (TCP/IP).
- Lexical analysis in compilers.

## How to Run
This is a header-only/single-file C++ simulation. It can be compiled with any standard C++ compiler (G++, Clang, MSVC):

```bash
g++ vending_machine_fsm.cpp -o fsm_sim
./fsm_sim
```

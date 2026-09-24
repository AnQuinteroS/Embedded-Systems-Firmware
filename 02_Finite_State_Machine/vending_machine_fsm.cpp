/**
 * Finite State Machine (FSM) - Vending Machine Logic
 * * Objective: Implement a robust state machine using standard C++ to handle 
 * the logic of a vending machine. 
 * * Technical Highlights:
 * - Uses 'enum class' for strict type safety (preventing integer conversion bugs).
 * - Switch-case logic for clear, deterministic state transitions.
 * - Separation of state evaluation and state execution.
 */

#include <iostream>
#include <string>

// Strongly typed enum to define the exact possible states of the machine
enum class MachineState {
    IDLE,
    COIN_INSERTED,
    DISPENSING_PRODUCT,
    RETURNING_CHANGE
};

class VendingMachine {
private:
    MachineState current_state;
    int current_balance;
    const int product_price = 150; // Price in cents

public:
    // Constructor initializes the FSM in the IDLE state
    VendingMachine() : current_state(MachineState::IDLE), current_balance(0) {}

    // Method to handle state transitions based on external inputs (events)
    void insertCoin(int amount) {
        if (amount <= 0) {
            std::cout << "[Error] Invalid coin amount.\n";
            return;
        }

        current_balance += amount;
        std::cout << "Inserted: " << amount << " cents. Total Balance: " << current_balance << " cents.\n";

        // State Transition Logic
        if (current_state == MachineState::IDLE) {
            current_state = MachineState::COIN_INSERTED;
        }

        evaluateState();
    }

    // Core logic engine: Decides what happens inside each state
    void evaluateState() {
        switch (current_state) {
            case MachineState::IDLE:
                std::cout << "[State: IDLE] Waiting for customer...\n";
                break;

            case MachineState::COIN_INSERTED:
                if (current_balance >= product_price) {
                    std::cout << "[State: COIN_INSERTED] Sufficient funds reached. Preparing to dispense...\n";
                    current_state = MachineState::DISPENSING_PRODUCT;
                    evaluateState(); // Recursive call to trigger the next state immediately
                } else {
                    std::cout << "[State: COIN_INSERTED] Waiting for more coins. Short by " 
                              << (product_price - current_balance) << " cents.\n";
                }
                break;

            case MachineState::DISPENSING_PRODUCT:
                std::cout << "[State: DISPENSING] Product dropped! Enjoy.\n";
                current_balance -= product_price;
                current_state = MachineState::RETURNING_CHANGE;
                evaluateState();
                break;

            case MachineState::RETURNING_CHANGE:
                if (current_balance > 0) {
                    std::cout << "[State: RETURNING CHANGE] Dispensing " << current_balance << " cents in change.\n";
                }
                // Reset machine for the next customer
                current_balance = 0;
                current_state = MachineState::IDLE;
                std::cout << "--- Transaction Complete ---\n\n";
                break;
        }
    }
};

// Main function to simulate user interaction
int main() {
    std::cout << "=== Vending Machine FSM Simulation ===\n";
    VendingMachine myMachine;

    // Simulating a user inserting coins
    myMachine.insertCoin(50);
    myMachine.insertCoin(50);
    myMachine.insertCoin(100); // This should trigger dispensing and change return

    return 0;
}

# ⚡ The Resistor Network Program ⚡

This is a command-line application implemented in C++ that simulates an electrical resistor network. It allows a user to define a circuit by specifying resistors connected between nodes and then uses the **Nodal Analysis** technique with the **Gauss-Seidel iterative method** to solve for the voltage at every node in the circuit.

## ✨ Features

* **Dynamic Network Allocation:** Supports initialization and resizing of the circuit network (nodes and resistors) using dynamic memory allocation (`new` and `delete[]`).
* **Circuit Construction:** Users can insert, modify, and delete resistors, specifying a name, resistance (Ohms), and the two connected nodes.
* **Node Voltage Setting:** Allows for defining known voltage boundary conditions (e.g., ground or a power source).
* **Circuit Solver:** Implements a robust `solve` command using the Gauss-Seidel method to iteratively calculate unknown node voltages until a specified convergence criterion is met.
* **Error Handling:** Extensive input validation and error reporting for invalid commands, arguments, negative resistance, out-of-range nodes, and duplicate resistor names.
* **Object-Oriented Design:** Uses dedicated C++ classes (`Node` and `Resistor`) to encapsulate data and behavior related to the circuit components.

## ⚙️ Technical Details & Implementation

The program is built around two primary classes:

1.  **`Node` Class:** Represents a circuit node. It stores the calculated **voltage**, a flag indicating if the voltage is externally set, and an array of indices pointing to the resistors connected to it.
2.  **`Resistor` Class:** Represents a resistor, storing its **name**, **resistance value**, and the indices of the two nodes it connects.

### Nodal Analysis Implementation

The `handleSolve` function uses the **Gauss-Seidel method** to solve the system of linear equations derived from Kirchhoff's Current Law (KCL) at each unknown node.

For each unknown node $i$, the new voltage ($V_i^{new}$) is calculated iteratively using the formula:

$$V_i^{new} = \frac{\sum_{k=1}^{N} \frac{V_k}{R_{ik}}}{\sum_{k=1}^{N} \frac{1}{R_{ik}}}$$

Where:
* $N$ is the number of resistors connected to Node $i$.
* $R_{ik}$ is the resistance of the resistor connecting Node $i$ and Node $k$.
* $V_k$ is the current voltage estimate of the node connected to Node $i$ through $R_{ik}$.

The iteration continues until the change in all node voltages is below the convergence threshold ($\pm 0.0001$ Volts).

## 🚀 Usage (Command-Line Interface)

The program accepts commands interactively:

| Command | Arguments | Description |
| :--- | :--- | :--- |
| `maxVal` | `<max_nodes> <max_resistors>` | Initializes the network with the given maximum sizes. |
| `insertR` | `<name> <R> <nodeA> <nodeB>` | Inserts a resistor with resistance **R** between the 1-indexed nodes. |
| `modifyR` | `<name> <new_R>` | Changes the resistance of an existing resistor. |
| `printR` | `<name>` | Prints the details of a single resistor. |
| `deleteR` | `<name>` / `all` | Deletes a single resistor or all resistors from the network. |
| `setV` | `<node> <voltage>` | Sets the fixed voltage of a node (boundary condition). |
| `solve` | (None) | Solves the circuit for all unknown node voltages and prints the results. |

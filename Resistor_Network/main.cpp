//
//  main.cpp
//  Lab 3 The Resistor Network Program
//
//  Created by Nathan Hung on 2024-09-11.
//  Modified by Salma Emara on 2024-09-30
//
//  Permission is hereby granted to use this code in ECE244 at
//  the University of Toronto. It is prohibited to distribute
//  this code, either publicly or to third parties.
//
//  ECE244 Student: Complete the skeletal code of the main function in this file

#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

#include "Node.h"
#include "Resistor.h"

using namespace std;

Resistor** resistors =
nullptr;  // Pointer that should point to an array of Resistor pointers
Node* nodes = nullptr;  // pointer that should hold address to an array of Nodes
int maxNodeNumber = 0;  // maximum number of nodes as set in the command line
int maxResistors = 0;  // maximum number of resistors as set in the command line
int resistorsCount = 0;  // count the number of resistors

string errorArray[10] = {
    "invalid command",                                  // 0
    "invalid argument",                                 // 1
    "negative resistance",                              // 2
    "node value is out of permitted range",             // 3
    "resistor name cannot be keyword \"all\"",          // 4
    "both terminals of resistor connect to same node",  // 5
    "too few arguments",                                // 6
};

// Function Prototypes
bool getInteger(stringstream& ss, int& x);
bool getString(stringstream& ss, string& s);
bool getDouble(stringstream& ss, double& s);

void handleMaxVal(stringstream& ss);
void handleInsertR(stringstream& ss);
void handleModifyR(stringstream& ss);
void handlePrintR(stringstream& ss);
void handleDeleteR(stringstream& ss);
void handleSetV(stringstream& ss);
void handleSolve();
void cleanup();
void printError(const string& message);
bool checkExtraArgs(stringstream& ss);

int main() {
  string line;
  cout << ">>> ";
  cout.flush();
  while (getline(cin, line)) {
    if (line.size() == 0) {
      cout << ">>> ";
      cout.flush();
      continue;
    }
    stringstream ss(line);
    string cmd;
    if (!(ss >> cmd)) {
      // output empty line
      cout << ">>> ";
      cout.flush();
      continue;
    }

    if(cmd == "maxVal") {
      handleMaxVal(ss);

    } else if(cmd == "insertR") {
      handleInsertR(ss);
      
    } else if(cmd == "modifyR") {
      handleModifyR(ss);
    } else if(cmd == "printR") {
      handlePrintR(ss);
    } else if(cmd == "deleteR") {
      handleDeleteR(ss);
    } else if(cmd == "setV") {
      handleSetV(ss);

    } else if (cmd == "solve") {
      // ensure that there are no extra argums
      string extra;
      if (ss >> extra) {
        cout << "Error: " << errorArray[1] << endl;
      } else {
        
        handleSolve();

      }
    } else {
      cout << "Error: " << errorArray[0] << endl;
    }

    cout << ">>> ";
    cout.flush();
  }
  
  // deallocate dynamic memory before exit memory
  cleanup();
  return 0;
}

// TODO: Implement functions here
// Input helpers
bool getInteger(stringstream& ss, int& x) {
  string token;
  if (!(ss >> token)) return false;
  // try to parse integer and ensure whole token is integer
  stringstream ts(token);
  int val;
  char c;
  if (!(ts >> val) || (ts >> c)) return false;
  x = val;
  return true;
}

bool getString(stringstream& ss, string& s) {
  if (!(ss >> s)) {
    return false;
  }
  return true;
}

bool getDouble(stringstream& ss, double& d) {
    string token;
    if (!(ss >> token)) {
      return false;
    }
    stringstream ts(token);
    double val;
    char c;
    if (!(ts >> val) || (ts >> c)) {
      return false;
    }
    d = val;
    return true;
}

// helpful functions

void printError(const string& message) {
    cout << "Error: " << message << endl;
}
bool checkExtraArgs(stringstream& ss) {
    string extra;
    return !(ss >> extra);
}
int findResistorByName(const string& name) {
    if (!resistors) {
      return -1;
    }
    for (int i = 0; i < maxResistors; ++i) {

        if (resistors[i] && resistors[i]->getName() == name) {
          return i;
        }
    }
    return -1;
}

void deleteAllResistors() {
    if (!resistors) {
      return;
    }
    for (int i = 0; i < maxResistors; ++i) {
        if (resistors[i]) {
            delete resistors[i];
            resistors[i] = nullptr;
        }
    }
    resistorsCount = 0;
    // reset node resistor lists if exist
    if (nodes) {
        for (int i = 0; i < maxNodeNumber; ++i) {

          nodes[i] = Node();
        }
    }
}

void cleanup() {
    
    deleteAllResistors();
    if (resistors) { 
      delete[] resistors; resistors = nullptr; 
    }
    if (nodes) { 
      delete[] nodes; nodes = nullptr; 
    }
    maxNodeNumber = 0;
    maxResistors = 0;
    resistorsCount = 0;
}

// Command handlers
void handleMaxVal(stringstream& ss) {
    int newMaxNodes, newMaxRes;

    // check for maxNodeNumber
    if (!getInteger(ss, newMaxNodes)) {
        printError("too few arguments");
        return;
    }

    // check for maxResistors 
    if (!getInteger(ss, newMaxRes)) {
        printError("too few arguments");
        return;
    }

    // Check for extra arguments
    if (!checkExtraArgs(ss)) {
        printError("invalid argument");
        return;
    }

    // Error check: node value is out of permitted range
    if (newMaxNodes <= 0 || newMaxRes <= 0) {
        printError("node value is out of permitted range 1 - " + to_string(maxNodeNumber));
        return;
    }

    // de-allocate old arrays
    deleteAllResistors();  // Clean up resistor objects
    if (resistors != nullptr) {
      delete[] resistors;
      resistors = nullptr;
    }
    if (nodes != nullptr) {
      delete[] nodes;
      nodes = nullptr;
    }

    // Allocate new arrays
    maxNodeNumber = newMaxNodes;
    maxResistors = newMaxRes;
    resistorsCount = 0;

    nodes = new Node[maxNodeNumber];
    resistors = new Resistor*[maxResistors];
    for (int i = 0; i < maxResistors; i++) {
      resistors[i] = nullptr;
    }    // reset nodes
    for (int i = 0; i < maxNodeNumber; i++) {
      nodes[i] = Node();  // calls the default constructor again
    } 

    cout << "New network: max node number is " << maxNodeNumber
         << "; max resistors is " << maxResistors << endl;
}

void handleInsertR(stringstream& ss) {
    string name;
    double resistance;
    int nodeA_in, nodeB_in;

    string token1, token2, token3, token4;
    if (!(ss >> token1)) {
        cout <<"Error: " << errorArray[6] << endl;
        return;
    }
    if (!(ss >>token2)) {
        cout << "Error: " << errorArray[6] << endl;
        return;
    }
    if (!(ss >>token3)) {
        cout << "Error: " << errorArray[6] << endl;
        return;
    }
    if (!(ss>> token4)) {
        cout<< "Error: " << errorArray[6] << endl;
        return;
    }
    name = token1;
    stringstream ss_res(token2);
    double res_val;
    char c;
    if (!(ss_res >> res_val) || (ss_res >> c)) {
        cout << "Error: " << errorArray[1] << endl;
        return;
    }
    resistance = res_val;

    stringstream ss_nodeA(token3);
    int nodeA_val;
    if (!(ss_nodeA >> nodeA_val) || (ss_nodeA >> c)) {
        cout << "Error: " << errorArray[1] << endl;
        return;
    }
    nodeA_in = nodeA_val;

    stringstream ss_nodeB(token4);
    int nodeB_val;
    if (!(ss_nodeB >> nodeB_val) || (ss_nodeB >> c)) {
        cout << "Error: " << errorArray[1] << endl;
        return;
    }
    nodeB_in = nodeB_val;
    string extra;
    if (ss >> extra) {
        cout << "Error: " << errorArray[1] << endl;
        return;
    }

    // name cannot be "all" so error
    if (name == "all") {
        cout << "Error: " << errorArray[4] << endl;
        return;
    }

    // error if negative resistance, not possible
    if (resistance < 0.0) {
        cout << "Error: " << errorArray[2] << endl;
        return;
    }

    // node range check, the possible user nodes are 1 up to maxNodeNumber
    if (nodeA_in < 1 || nodeA_in > maxNodeNumber || nodeB_in < 1 || nodeB_in > maxNodeNumber) {
        cout << "Error: " << errorArray[3] << endl;
        return;
    }

    // error if both terminals are the same b/c not possible
    if (nodeA_in == nodeB_in) {
        cout << "Error: " << errorArray[5] << endl;
        return;
    }

    // error if resistor name already exists
    if (findResistorByName(name) != -1) {
        cout << "Error: resistor " << name << " already exists" << endl;
        return;
    }

    // check for space for more resistors
    if (resistorsCount >= maxResistors) {
        // semantically incorrect as full, so just state as an error
        cout << "Error: " << errorArray[1] << endl;
        return;
    }

    // find first null slot
    int slot = -1;
    for (int i = 0; i < maxResistors; ++i) {
        if (resistors[i] == nullptr) { slot = i; break; }
    }
    if (slot == -1) {

        cout << "Error: " << errorArray[1] << endl;
        return;
    }

    // create resistor and assign
    int endpoints[2];
    endpoints[0] = nodeA_in - 1;
    endpoints[1] = nodeB_in - 1;
    resistors[slot] = new Resistor(name, resistance, endpoints);
    resistorsCount++;

    // add resistor to node lists
    nodes[endpoints[0]].addResistor(slot);
    nodes[endpoints[1]].addResistor(slot);

    cout << "Inserted: resistor " << name << " " << fixed << setprecision(2) << resistance
    << " Ohms " << nodeA_in << " -> " << nodeB_in << endl;

}

void handleModifyR(stringstream& ss) {
    string name;
    double resistance;

    if (!getString(ss, name) || !getDouble(ss, resistance)) {
        cout << "Error: " << errorArray[6] << endl;
        return;
    }
    string extra;
    if (ss >> extra) {
        cout << "Error: " << errorArray[1] << endl;
        return;
    }

    // same errors as before for all and negative resistance

    if (name == "all") {
        cout << "Error: " << errorArray[4] << endl;
        return;
    }
    if (resistance < 0.0) {
        cout << "Error: " << errorArray[2] << endl;
        return;
    }

    int idx = findResistorByName(name);

    if (idx == -1) {
        cout << "Error: resistor " << name << " not found" << endl;
        return;
    }

    double oldR = resistors[idx]->getResistance();
    resistors[idx]->setResistance(resistance);
    cout << "Modified: resistor " << name << " from " << fixed << setprecision(2) << oldR
         << " Ohms to " << fixed << setprecision(2) << resistance << " Ohms" << endl;
}

void handlePrintR(stringstream& ss) {
    string name;
    if (!getString(ss, name)) {
        cout << "Error: " << errorArray[6] << endl;
        return;
    }
    string extra;
    if (ss >> extra) {
        cout << "Error: " << errorArray[1] << endl;
        return;
    }

    int idx = findResistorByName(name);
    if (idx == -1) {
        cout << "Error: resistor " << name << " not found" << endl;
        return;
    }

    cout << "Print:" << endl;
    resistors[idx]->print();
}

void handleDeleteR(stringstream& ss) {
    string arg;
    if (!getString(ss, arg)) {
        cout << "Error: " << errorArray[6] << endl;
        return;
    }
    string extra;
    if (ss >> extra) {
        cout << "Error: " << errorArray[1] << endl;
        return;
    }

    if (arg == "all") {
        deleteAllResistors();
        cout << "Deleted: all resistors" << endl;
        return;
    } else {
        // to be safe
        int idx = findResistorByName(arg);
        if (idx == -1) {
            cout << "Error: resistor " << arg << " not found" << endl;
            return;
        }
        // remove resistor from nodes
        // rebuild node lists by clearing and readd remaining resistors
        delete resistors[idx];
        resistors[idx] = nullptr;
        resistorsCount--;
        for (int i = 0; i < maxNodeNumber; ++i) {
        
          nodes[i] = Node();
        }

        for (int i = 0; i < maxNodeNumber; ++i){
          nodes[i] = Node();
        } 
        for (int r = 0; r < maxResistors; ++r) {
            if (!resistors[r]) {
              continue;
            }
            // find endpoints by scanning through nodes
            int attachedCount = 0;
            for (int n = 0; n < maxNodeNumber; n++) {
                int other = resistors[r]->getOtherEndpoint(n);
                if (other != -1) {
                    // resistor is attached to n
                    nodes[n].addResistor(r);
                    attachedCount++;
                }
            }
        }

        cout << "Deleted: resistor " << arg << endl;
        return;
    }
}

void handleSetV(stringstream& ss) {
    int nodeid_in;
    double voltage;
    if (!getInteger(ss, nodeid_in) || !getDouble(ss, voltage)) {
        cout << "Error: " << errorArray[6] << endl;
        return;
    }
    string extra;
    if (ss >>extra) {
        cout <<"Error: " << errorArray[1] << endl;
        return;
    }

    if (nodeid_in < 1|| nodeid_in > maxNodeNumber) {
        cout << "Error: " << errorArray[3] << endl;
        return;
    }
    nodes[nodeid_in-1].setVoltage(voltage);
    cout << "Set: node " << nodeid_in << " to " << fixed << setprecision(2) << voltage << " Volts" << endl;
}

void handleSolve() {
    const double MIN_ITERATION_CHANGE = 0.0001;
    if (maxNodeNumber <= 0) {
        cout << "Solve:" << endl;
        return;
    }
    // initialize internal voltages, nodes without setV to 0
    for (int i = 0; i < maxNodeNumber; i++) {
        if (!nodes[i].isVoltageSet()) nodes[i].setVoltageInternally(0.0);
    }

    bool converged = false;
    while (!converged) {
        converged = true;
        for (int i = 0; i < maxNodeNumber; i++) {
            if (nodes[i].isVoltageSet()) {
              continue;
            }
            // skip initialized nodes
            int numRes = nodes[i].getNumRes();
            if (numRes == 0) {
              continue; // no resistors attached, means not printed later
            }

            int* resList = nodes[i].getResIDArray();
            double numerator = 0.0;
            double denom = 0.0;
            for (int k = 0; k < numRes; k++) {
                int rID = resList[k];
                if (rID < 0 || rID >= maxResistors) {
                  continue;
                }
                Resistor* r = resistors[rID];
                if (!r) {
                  continue;
                }
                double R = r->getResistance();
                int other = r->getOtherEndpoint(i);

                double v_other = nodes[other].getVoltage();
                numerator += v_other / R;
                denom += 1.0 / R;
            }
            if (denom == 0.0) {
              continue;
            }

            double newV = numerator / denom;
            double oldV = nodes[i].getVoltage();
            double difference = newV - oldV;
            if (difference > MIN_ITERATION_CHANGE || difference < -MIN_ITERATION_CHANGE) {
              converged = false;
            } 
            nodes[i].setVoltageInternally(newV);

        }
    }

    // Print voltages of valid nodes in ascending node order
    cout << "Solve:" << endl;
    for (int i = 0; i < maxNodeNumber; ++i) {
        if (nodes[i].getNumRes() > 0) {
            cout << "Node " << (i + 1) << ": " << fixed << setprecision(2) << nodes[i].getVoltage() << " V" << endl;
        }
    }
}
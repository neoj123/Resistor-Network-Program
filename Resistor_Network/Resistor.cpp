//
//  Resistor.cpp
//  Lab 3 The Resistor Network Program
// Author: Neo Li 
// Date: October 11, 2025

#include "Resistor.h"
#include <iostream>
#include <iomanip>

Resistor::Resistor(string name_, double resistance_, int endpoints_[2]){
    name = name_;
    resistance = resistance_;
    endpointNodeIDs[0] = endpoints_[0];
    endpointNodeIDs[1] = endpoints_[1];
}
string Resistor::getName() {
    return name;
}
double Resistor::getResistance() {
    return resistance;
}
void Resistor::setResistance(double resistance_) {
    resistance = resistance_;
}
void Resistor::print() {
    // prints id 1 indexed
    cout << left << setw(20) << name
         << " " << fixed << setprecision(2) << right << setw(7) << resistance
         << " Ohms " << (endpointNodeIDs[0] + 1) << " -> " << (endpointNodeIDs[1] + 1)
         << endl;
}

int Resistor::getOtherEndpoint(int nodeIndex) {
    // nodeIndex is the internal index, 0 indexed not like print
    if (endpointNodeIDs[0] == nodeIndex) {
      return endpointNodeIDs[1];
    }
    if (endpointNodeIDs[1] == nodeIndex){
       return endpointNodeIDs[0];
    }
    return -1; // not attached
}
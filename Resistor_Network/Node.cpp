//
//  Node.cpp
//  Lab 3 The Resistor Network Program
// Author: Neo Li
// Date: October 11, 2025
#include "Node.h"

Node::Node(){
    numRes = 0;
    voltage = 0.0;
    voltageIsSet = false;
    for (int i = 0; i < MAX_RESISTORS_PER_NODE; i++) {
        resIDArray[i] = -1;
    }
}

bool Node::canAddResistor() {
    return (numRes < MAX_RESISTORS_PER_NODE);
}
void Node::addResistor(int rIndex){
    if (numRes >= MAX_RESISTORS_PER_NODE) {
        return;
    }
    resIDArray[numRes]= rIndex;
    ++numRes;
}
double Node::getVoltage(){
    return voltage;
}
void Node::setVoltage(double voltage_){
    voltage = voltage_;
    voltageIsSet = true;
}
void Node::setVoltageInternally(double voltage_) {
    voltage = voltage_;
    // meant not change voltageIsSet
}
bool Node::isVoltageSet() {
    return voltageIsSet;
}
int Node::getNumRes(){
    return numRes;
}

int* Node::getResIDArray() {
    return resIDArray;
}
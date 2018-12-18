#include "person.h"
#include <iostream>
#include <stdlib.h>

Person::Person(int seq, int pri, long washTime, int clothes) {
    this->sequence = seq;
    this->priority = pri;
    this->clothes = clothes;
    this->washTime = washTime;
}

int Person::getPriority() const {
    return priority;
}

int Person::getSequence() const {
    return sequence;
}

long Person::getWashTime() const {
    return washTime;
}

int Person::getClothes() const {
    return clothes;
}

Person::Person() {
   // std::cout << "Creating object\n";
}

Person::~Person() {
    //std::cout << "Destroying object\n";
}

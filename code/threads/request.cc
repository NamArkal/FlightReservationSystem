#include "request.h"

int Request::requestCounter=0;

int Request::getSourceCity() const {
    return sourceCity;
}

int Request::getDestCity() const {
    return destCity;
}

Request::Request(int source) {
    sourceCity = source;
    destCity = destProb(source);
    reqNumber = ++requestCounter;
    numberOfPassengers = rand()%5 + 1;
}

int Request::destProb(int source) {
    int dest;
    int p = rand()%100 + 1;
    switch(source){
        case 0:
            if(p>0 && p<41) dest = 4;
            else if(p>40 && p<71) dest = 3;
            else if(p>70 && p<91) dest = 2;
            else dest = 1;
            break;
        case 1:
            if(p>0 && p<11) dest = 2;
            else if(p>10 && p<51) dest = 3;
            else dest = 4;
            break;
        case 2:
            if (p>0 && p<31) dest = 3;
            else dest = 4;
            break;
        default: dest = 4;
    }
    return dest;
}

List<int> *Request::getSeatNumbers() const {
    return seatNumbers;
}

int Request::getReqNumber() const {
    return reqNumber;
}

int Request::getNumberOfPassengers() const {
    return numberOfPassengers;
}

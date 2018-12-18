#include "../lib/list.h"
#include "../lib/bitmap.h"

class Request{
    int reqNumber;
    int numberOfPassengers;
    int sourceCity;
    int destCity;
    static int requestCounter;

public:
    Request(int source);

    Request() {};

    List<int> *getSeatNumbers() const;

    virtual ~Request(){};

    int getReqNumber() const;

    int getNumberOfPassengers() const;

    int getSourceCity() const;

    int getDestCity() const;

    int destProb(int source);

    List<int> *seatNumbers = new List<int>();

    bool operator == (Request r) {
        if(this->getReqNumber() == r.getReqNumber())
            return true;
        else return false;
    }

};

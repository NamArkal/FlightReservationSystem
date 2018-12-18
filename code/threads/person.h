#include "../lib/list.h"

class Person {

public:
    Person(int seq, int priority, long washTime, int clothes);
    Person();
    virtual ~Person();
    int getPriority() const;
    int getSequence() const;
    long getWashTime() const;
    int getClothes() const;

bool operator == (Person p) {
        if(this->getSequence() == p.getSequence())
            return true;
        else return false;
    }

private:
    int priority;
    int sequence;
    long washTime;
    int clothes;

};

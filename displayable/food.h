#ifndef FOOD_H
#define FOOD_H
#include "../displayable.h"

class Food : public Displayable {

    bool avaliable;

    public:
        Food();
        void regenerate();
        bool isAvaliable();
        void setStatus(bool st);
        void isEaten();
    
};
#endif
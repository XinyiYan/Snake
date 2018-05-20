#ifndef FRUIT_H
#define FRUIT_H
#include "../food.h"
struct XInfo;

class Fruit : public Food {
    public:
        Fruit();
        void paint(XInfo &xinfo) override;
};
#endif

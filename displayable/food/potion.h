#ifndef POTION_H
#define POTION_H
#include "../food.h"
struct XInfo;

class Potion : public Food {
    public:
        Potion();
        void paint(XInfo &xinfo) override;
};
#endif
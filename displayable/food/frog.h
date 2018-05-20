#ifndef FROG_H
#define FROG_H
#include "../food.h"
struct XInfo;

class Frog : public Food {

    public:
        Frog();
        void paint(XInfo &xinfo) override;
        void jump();
};
#endif
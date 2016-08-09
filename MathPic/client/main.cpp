#include <iostream>
#include "draw.h"

using namespace std;


int main()
{
    Draw test(1000, 1000);
    test.setLogicalPost(500, 500);
    test.calcUnitPosition();
    test.setThreadNumber(4);
    test.start();
    test.save("MathPic.ppm");
}

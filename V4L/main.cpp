#include "camera.h"
#include <iostream>

int main(void)
{
    Camera video;
    video.start(640, 480);
    video.stop();
    return 0;
}


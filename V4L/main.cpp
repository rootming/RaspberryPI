#include "video.h"
#include <iostream>

int main(void)
{
    Video video;
    video.start(640, 480);
    video.stop();
    return 0;
}


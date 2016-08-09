#include <iostream>
#include <unistd.h>
#include "server.h"

using namespace std;

int main()
{
    int w, h;
    Server a;
    a.startUDPServer();
    sleep(5);
    w = 9600;
    h = 9600;
}

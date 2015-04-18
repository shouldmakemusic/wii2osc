//
//  main.cpp
//  Wii2Osc
//
//  Created by Manuel Hirschauer on 18.04.15.
//  Copyright (c) 2015 Hirschauer. All rights reserved.
//

#include <iostream>
#include <time.h>
#include <OSCSender.h>
#include <WiiReceiver.h>
#include <thread>

using namespace std;

void wiireceive()
{
    OSCSender oscSender("127.0.0.1", 9050);
    WiiReceiver wiiReceiver(oscSender);
    wiiReceiver.FindAndReceive();
    wiiReceiver.~WiiReceiver();
    
}

int main(int argc, const char * argv[]) {

    //RangeUtil ru;
    //ru.test();
    
    do {
        std::thread t1 (wiireceive);
        t1.join();
        t1.~thread();
    } while (false);
    
    cout << "=======================================" << endl;
    cout << "Bye!" << endl;
    return 0;
}



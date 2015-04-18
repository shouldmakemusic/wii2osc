//
//  OSCSender.h
//  Wii2Osc
//
//  Created by Manuel Hirschauer on 18.04.15.
//
//

#ifndef __Wii2Osc__OSCSender__
#define __Wii2Osc__OSCSender__

#include <iostream>
#include "ip/UdpSocket.h"

using namespace std;

class OSCSender
{
public:
    OSCSender( string host = "127.0.0.1", int port = 9050 );
    
    void sendOsc(string name, float arg1 = 0.0, float arg2 = 0.0, float arg3 = 0.0, float arg4 = 0.0);
    
private:
    int port;
    string host;
};

#endif /* defined(__Wii2Osc__OSCSender__) */

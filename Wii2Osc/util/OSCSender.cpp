//
//  OSCSender.cpp
//  Wii2Osc
//
//  Created by Manuel Hirschauer on 18.04.15.
//
//

#include "OSCSender.h"
#include "osc/OscOutboundPacketStream.h"
#include "ip/UdpSocket.h"
#include <iostream>

using namespace std;

#define OUTPUT_BUFFER_SIZE 1024


OSCSender::OSCSender( string host, int port )
{
    this->host = host;
    this->port = port;
}

void OSCSender::sendOsc(string name, float arg1, float arg2, float arg3, float arg4)
{
    UdpTransmitSocket transmitSocket(IpEndpointName( this->host.c_str(), this->port ));
    char buffer[OUTPUT_BUFFER_SIZE];
    osc::OutboundPacketStream p( buffer, OUTPUT_BUFFER_SIZE );
    
    std::string message = "/wii/1/" + name;
    
    p << osc::BeginBundleImmediate
    << osc::BeginMessage( message.c_str() )
    << arg1 << arg2 << arg3 << arg4 << osc::EndMessage
    << osc::EndBundle;
    
    transmitSocket.Send( p.Data(), p.Size() );
    
}
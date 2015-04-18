//
//  WiiReceiver.h
//  Wii2Osc
//
//  Created by Manuel Hirschauer on 18.04.15.
//
//

#ifndef __Wii2Osc__WiiReceiver__
#define __Wii2Osc__WiiReceiver__

#include <stdio.h>
#include <wiicpp.h>
#include <OSCSender.h>
#include <RangeUtil.h>

class WiiReceiver
{
    public:
        
        WiiReceiver( OSCSender& oscSender );
        int FindAndReceive();
    
        void HandleEvent(CWiimote &wm);
        void HandleStatus(CWiimote &wm);
        void HandleDisconnect(CWiimote &wm);
        void HandleReadData(CWiimote &wm);
        void HandleNunchukInserted(CWiimote &wm);
        void HandleClassicInserted(CWiimote &wm);
        void HandleGH3Inserted(CWiimote &wm);
        
    private:
        int LED_MAP[4] = {CWiimote::LED_1, CWiimote::LED_2, CWiimote::LED_3, CWiimote::LED_4};
        OSCSender oscSender;
        int reloadWiimotes = 0;
};

#endif /* defined(__Wii2Osc__WiiReceiver__) */

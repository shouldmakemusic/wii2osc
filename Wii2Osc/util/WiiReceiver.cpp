//
//  WiiReceiver.cpp
//  Wii2Osc
//
//  Created by Manuel Hirschauer on 18.04.15.
//
//

#include "WiiReceiver.h"

WiiReceiver::WiiReceiver( OSCSender& oscSender )
{
    this->oscSender = oscSender;
    
}

int WiiReceiver::FindAndReceive()
{
    
    std::cout << "Finding wiimotes...\n";
    
    CWii wii; // Defaults to 4 remotes
    std::vector<CWiimote>::iterator i;
    
    int index;
    
    // Find and connect to the wiimotes
    std::vector<CWiimote>& wiimotes = wii.FindAndConnect();
    
    if(!wiimotes.size()) {
        cout << "Error: no connected Wiimote" << endl;
        return 1;
    }
    this->oscSender.sendOsc("wiimote connected", 1.0);

    // Setup the wiimotes
    for(index = 0, i = wiimotes.begin(); i != wiimotes.end(); ++i, ++index) {
        // Use a reference to make working with the iterator handy.
        CWiimote & wiimote = *i;
        
        //Set Leds
        wiimote.SetLEDs(LED_MAP[index]);
    }

    do
      {
          if(reloadWiimotes)
          {
              // Regenerate the list of wiimotes
              wiimotes = wii.GetWiimotes();
              reloadWiimotes = 0;
          }
          
          //Poll the wiimotes to get the status like pitch or roll
          if(wii.Poll())
          {
              for(i = wiimotes.begin(); i != wiimotes.end(); ++i)
              {
                  // Use a reference to make working with the iterator handy.
                  CWiimote & wiimote = *i;
                  switch(wiimote.GetEvent())
                  {
                          
                      case CWiimote::EVENT_EVENT:
                          HandleEvent(wiimote);
                          
                          if(wiimote.Buttons.isPressed(CButtons::BUTTON_HOME))
                          {
                              wiimote.Disconnect();
                              reloadWiimotes = 1;
                          }
                          break;
                          
                      case CWiimote::EVENT_STATUS:
                          HandleStatus(wiimote);
                          break;
                          
                      case CWiimote::EVENT_DISCONNECT:
                      case CWiimote::EVENT_UNEXPECTED_DISCONNECT:
                          HandleDisconnect(wiimote);
                          reloadWiimotes = 1;
                          break;
                          
                      case CWiimote::EVENT_READ_DATA:
                          HandleReadData(wiimote);
                          break;
                          
                      case CWiimote::EVENT_NUNCHUK_INSERTED:
                          HandleNunchukInserted(wiimote);
                          reloadWiimotes = 1;
                          break;
                          
                      case CWiimote::EVENT_CLASSIC_CTRL_INSERTED:
                          HandleClassicInserted(wiimote);
                          reloadWiimotes = 1;
                          break;
                          
                      case CWiimote::EVENT_GUITAR_HERO_3_CTRL_INSERTED:
                          HandleGH3Inserted(wiimote);
                          reloadWiimotes = 1;
                          break;
                          
                      case CWiimote::EVENT_MOTION_PLUS_INSERTED:
                          cout << "Motion Plus inserted." << endl;
                          break;
                          
                      case CWiimote::EVENT_BALANCE_BOARD_INSERTED:
                          cout << "Balance Board connected.\n"  << endl;
                          break;
                          
                      case CWiimote::EVENT_BALANCE_BOARD_REMOVED:
                          cout << "Balance Board disconnected.\n"  << endl;
                          break;
                          
                      case CWiimote::EVENT_NUNCHUK_REMOVED:
                      case CWiimote::EVENT_CLASSIC_CTRL_REMOVED:
                      case CWiimote::EVENT_GUITAR_HERO_3_CTRL_REMOVED:
                      case CWiimote::EVENT_MOTION_PLUS_REMOVED:
                          cout << "An expansion was removed." << endl;
                          HandleStatus(wiimote);
                          reloadWiimotes = 1;
                          break;
                          
                      default:
                          break;
                  }
              }
          }
          
      } while(wiimotes.size()); // Go so long as there are wiimotes left to poll
}

void WiiReceiver::HandleEvent(CWiimote &wm)
{
    char prefixString[64];
    
    if(wm.Buttons.isJustPressed(CButtons::BUTTON_MINUS))
    {
        wm.SetMotionSensingMode(CWiimote::OFF);
    }
    
    if(wm.Buttons.isJustPressed(CButtons::BUTTON_PLUS))
    {
        wm.SetMotionSensingMode(CWiimote::ON);
        printf("%s MotionSensingMode=On\n", prefixString);
    }
    
    if(wm.Buttons.isJustPressed(CButtons::BUTTON_DOWN))
    {
        wm.IR.SetMode(CIR::OFF);
        printf("%s IR=Off\n", prefixString);
    }
    
    if(wm.Buttons.isJustPressed(CButtons::BUTTON_UP))
    {
        wm.IR.SetMode(CIR::ON);
        printf("%s IR=On\n", prefixString);
    }
    
    if(wm.Buttons.isJustPressed(CButtons::BUTTON_RIGHT))
    {
        wm.EnableMotionPlus(CWiimote::ON);
        printf("%s MotionPlus=On\n", prefixString);
    }
    
    if(wm.Buttons.isJustPressed(CButtons::BUTTON_LEFT))
    {
        printf("%s MotionPlus=Off\n", prefixString);
        wm.EnableMotionPlus(CWiimote::OFF);
    }
    
    if(wm.Buttons.isJustPressed(CButtons::BUTTON_B))
    {
        wm.ToggleRumble();
    }
    
    sprintf(prefixString, "Controller [%i]: ", wm.GetID());
    
    if(wm.Buttons.isPressed(CButtons::BUTTON_A))
    {
        printf("%s A pressed\n", prefixString);
        this->oscSender.sendOsc("button/A", 1.0);
    }
    
    if(wm.Buttons.isPressed(CButtons::BUTTON_B))
    {
        printf("%s B pressed\n", prefixString);
        this->oscSender.sendOsc("button/B", 1.0);
    }
    
    if(wm.Buttons.isPressed(CButtons::BUTTON_UP))
    {
        printf("%s Up pressed\n", prefixString);
        this->oscSender.sendOsc("button/Up", 1.0);
    }
    
    if(wm.Buttons.isPressed(CButtons::BUTTON_DOWN))
    {
        printf("%s Down pressed\n", prefixString);
        this->oscSender.sendOsc("button/Down", 1.0);
    }
    
    if(wm.Buttons.isPressed(CButtons::BUTTON_LEFT))
    {
        printf("%s Left pressed\n", prefixString);
        this->oscSender.sendOsc("button/Left", 1.0);
    }
    
    if(wm.Buttons.isPressed(CButtons::BUTTON_RIGHT))
    {
        printf("%s Right pressed\n", prefixString);
        this->oscSender.sendOsc("button/Right", 1.0);
    }
    
    if(wm.Buttons.isPressed(CButtons::BUTTON_MINUS))
    {
        printf("%s Minus pressed\n", prefixString);
        this->oscSender.sendOsc("button/Minus", 1.0);
    }
    
    if(wm.Buttons.isPressed(CButtons::BUTTON_PLUS))
    {
        printf("%s Plus pressed\n", prefixString);
        this->oscSender.sendOsc("button/Plus", 1.0);
    }
    
    if(wm.Buttons.isPressed(CButtons::BUTTON_ONE))
    {
        printf("%s One pressed\n", prefixString);
        this->oscSender.sendOsc("button/1", 1.0);
    }
    
    if(wm.Buttons.isPressed(CButtons::BUTTON_TWO))
    {
        printf("%s Two pressed\n", prefixString);
        this->oscSender.sendOsc("button/2", 1.0);
    }
    
    if(wm.Buttons.isPressed(CButtons::BUTTON_HOME))
    {
        printf("%s Home pressed\n", prefixString);
    }
    
    // if the accelerometer is turned on then print angles
    if(wm.isUsingACC())
    {
        float pitch, roll, yaw, accel;
        wm.Accelerometer.GetOrientation(pitch, roll, yaw);
//        printf("%s wiimote roll = %f\n", prefixString, roll);
//        printf("%s wiimote pitch = %f\n", prefixString, pitch);
//        printf("%s wiimote yaw = %f\n", prefixString, yaw);
        RangeUtil rangeUtilRoll(-180.0, 180.0, 0.0, 1.0);
        RangeUtil rangeUtilPitch(-180.0, 180.0, 0.0, 1.0);
        roll = rangeUtilRoll.getValue(roll);
        pitch = rangeUtilPitch.getValue(pitch);
        accel = wm.Accelerometer.GetOrientThreshold();
        printf("%s wiimote roll = %f\n", prefixString, roll);
        printf("%s wiimote pitch = %f\n", prefixString, pitch);
        printf("%s wiimote yaw = %f\n", prefixString, yaw);
        printf("%s wiimote accell = %f\n", prefixString, accel);
        this->oscSender.sendOsc("accel/pry", roll, pitch, yaw, accel);

        float x, y, z;
        wm.Accelerometer.GetRawGravityVector(x, y, z);
        RangeUtil rangeUtilXYZ(-1.0, 1.0, 0.4, 0.6);
        x = rangeUtilXYZ.getValue(x);
        y = rangeUtilXYZ.getValue(y);
        z = rangeUtilXYZ.getValue(z);
        printf("%s wiimote x = %f\n", prefixString, x);
        printf("%s wiimote y = %f\n", prefixString, y);
        printf("%s wiimote z = %f\n", prefixString, z);
        this->oscSender.sendOsc("accel/xyz", x, y, z);
        
    }
    
    // if the Motion Plus is turned on then print angles
    if(wm.isUsingMotionPlus()) {
        float roll_rate, pitch_rate, yaw_rate;
        wm.ExpansionDevice.MotionPlus.Gyroscope.GetRates(roll_rate,pitch_rate,yaw_rate);
        
        printf("%s motion plus roll rate = %f\n", prefixString,roll_rate);
        printf("%s motion plus pitch rate = %f\n", prefixString,pitch_rate);
        printf("%s motion plus yaw rate = %f\n", prefixString,yaw_rate);
    }
    
    // if(IR tracking is on then print the coordinates
    if(wm.isUsingIR())
    {
        std::vector<CIRDot>::iterator i;
        int x, y;
        int index;
        
        printf("%s Num IR Dots: %i\n", prefixString, wm.IR.GetNumDots());
        printf("%s IR State: %u\n", prefixString, wm.IR.GetState());
        
        std::vector<CIRDot>& dots = wm.IR.GetDots();
        
        for(index = 0, i = dots.begin(); i != dots.end(); ++index, ++i)
        {
            if((*i).isVisible())
            {
                (*i).GetCoordinate(x, y);
                printf("%s IR source %i: (%i, %i)\n", prefixString, index, x, y);
                
                wm.IR.GetCursorPosition(x, y);
                printf("%s IR cursor: (%i, %i)\n", prefixString, x, y);
                printf("%s IR z distance: %f\n", prefixString, wm.IR.GetDistance());
            }
        }
    }
    
    int exType = wm.ExpansionDevice.GetType();
    if(exType == wm.ExpansionDevice.TYPE_NUNCHUK)
    {
        float pitch, roll, yaw, a_pitch, a_roll;
        float angle, magnitude;
        
        CNunchuk &nc = wm.ExpansionDevice.Nunchuk;
        
        sprintf(prefixString, "Nunchuk [%i]: ", wm.GetID());
        
        if(nc.Buttons.isPressed(CNunchukButtons::BUTTON_C))
        {
            printf("%s C pressed\n", prefixString);
        }
        
        if(nc.Buttons.isPressed(CNunchukButtons::BUTTON_Z))
        {
            printf("%s Z pressed\n", prefixString);
        }
        
        nc.Accelerometer.GetOrientation(pitch, roll, yaw);
        printf("%s roll = %f\n", prefixString, roll);
        printf("%s pitch = %f\n", prefixString, pitch);
        printf("%s yaw = %f\n", prefixString, yaw);
        
        nc.Joystick.GetPosition(angle, magnitude);
        printf("%s joystick angle = %f\n", prefixString, angle);
        printf("%s joystick magnitude = %f\n", prefixString, magnitude);
    }
    else if(exType == wm.ExpansionDevice.TYPE_CLASSIC)
    {
        float angle, magnitude;
        
        CClassic &cc = wm.ExpansionDevice.Classic;
        
        sprintf(prefixString, "Classic [%i]: ", wm.GetID());
        
        if(cc.Buttons.isPressed(CClassicButtons::BUTTON_A))
        {
            printf("%s A pressed\n", prefixString);
        }
        
        if(cc.Buttons.isPressed(CClassicButtons::BUTTON_B))
        {
            printf("%s B pressed\n", prefixString);
        }
        
        if(cc.Buttons.isPressed(CClassicButtons::BUTTON_X))
        {
            printf("%s X pressed\n", prefixString);
        }
        
        if(cc.Buttons.isPressed(CClassicButtons::BUTTON_Y))
        {
            printf("%s Y pressed\n", prefixString);
        }
        
        if(cc.Buttons.isPressed(CClassicButtons::BUTTON_LEFT))
        {
            printf("%s Left pressed\n", prefixString);
        }
        
        if(cc.Buttons.isPressed(CClassicButtons::BUTTON_UP))
        {
            printf("%s Up pressed\n", prefixString);
        }
        
        if(cc.Buttons.isPressed(CClassicButtons::BUTTON_RIGHT))
        {
            printf("%s Right pressed\n", prefixString);
        }
        
        if(cc.Buttons.isPressed(CClassicButtons::BUTTON_DOWN))
        {
            printf("%s Down pressed\n", prefixString);
        }
        
        if(cc.Buttons.isPressed(CClassicButtons::BUTTON_PLUS))
        {
            printf("%s Plus pressed\n", prefixString);
        }
        
        if(cc.Buttons.isPressed(CClassicButtons::BUTTON_MINUS))
        {
            printf("%s Minus pressed\n", prefixString);
        }
        
        if(cc.Buttons.isPressed(CClassicButtons::BUTTON_HOME))
        {
            printf("%s Home pressed\n", prefixString);
        }
        
        if(cc.Buttons.isPressed(CClassicButtons::BUTTON_ZL))
        {
            printf("%s ZL pressed\n", prefixString);
        }
        
        if(cc.Buttons.isPressed(CClassicButtons::BUTTON_FULL_L))
        {
            printf("%s ZR pressed\n", prefixString);
        }
        
        if(cc.Buttons.isPressed(CClassicButtons::BUTTON_FULL_R))
        {
            printf("%s ZR pressed\n", prefixString);
        }
        
        printf("%s L button pressed = %f\n", prefixString, cc.GetLShoulderButton());
        printf("%s R button pressed = %f\n", prefixString, cc.GetRShoulderButton());
        
        cc.LeftJoystick.GetPosition(angle, magnitude);
        printf("%s left joystick angle = %f\n", prefixString, angle);
        printf("%s left joystick magnitude = %f\n", prefixString, magnitude);
        
        cc.RightJoystick.GetPosition(angle, magnitude);
        printf("%s right joystick angle = %f\n", prefixString, angle);
        printf("%s right joystick magnitude = %f\n", prefixString, magnitude);
    }
    else if(exType == wm.ExpansionDevice.TYPE_GUITAR_HERO_3)
    {
        float angle, magnitude;
        
        CGuitarHero3 &gh = wm.ExpansionDevice.GuitarHero3;
        
        sprintf(prefixString, "Guitar [%i]: ", wm.GetID());
        
        if(gh.Buttons.isPressed(CGH3Buttons::BUTTON_STRUM_UP))
        {
            printf("%s Strum Up pressed\n", prefixString);
        }
        
        if(gh.Buttons.isPressed(CGH3Buttons::BUTTON_STRUM_DOWN))
        {
            printf("%s Strum Down pressed\n", prefixString);
        }
        
        if(gh.Buttons.isPressed(CGH3Buttons::BUTTON_YELLOW))
        {
            printf("%s Yellow pressed\n", prefixString);
        }
        
        if(gh.Buttons.isPressed(CGH3Buttons::BUTTON_GREEN))
        {
            printf("%s Green pressed\n", prefixString);
        }
        
        if(gh.Buttons.isPressed(CGH3Buttons::BUTTON_BLUE))
        {
            printf("%s Blue pressed\n", prefixString);
        }
        
        if(gh.Buttons.isPressed(CGH3Buttons::BUTTON_RED))
        {
            printf("%s Red pressed\n", prefixString);
        }
        
        if(gh.Buttons.isPressed(CGH3Buttons::BUTTON_ORANGE))
        {
            printf("%s Orange pressed\n", prefixString);
        }
        
        if(gh.Buttons.isPressed(CGH3Buttons::BUTTON_PLUS))
        {
            printf("%s Plus pressed\n", prefixString);
        }
        
        if(gh.Buttons.isPressed(CGH3Buttons::BUTTON_MINUS))
        {
            printf("%s Minus pressed\n", prefixString);
        }
        
        printf("%s whammy bar = %f\n", prefixString, gh.GetWhammyBar());
        
        gh.Joystick.GetPosition(angle, magnitude);
        printf("%s joystick angle = %f\n", prefixString, angle);
        printf("%s joystick magnitude = %f\n", prefixString, magnitude);
    }
    else if(exType == wm.ExpansionDevice.TYPE_BALANCE_BOARD)
    {
        CBalanceBoard &bb = wm.ExpansionDevice.BalanceBoard;
        float total, topLeft, topRight, bottomLeft, bottomRight;
        
        bb.WeightSensor.GetWeight(total, topLeft, topRight, bottomLeft, bottomRight);
        printf("balance board top left weight: %f\n", topLeft);
        printf("balance board top right weight: %f\n", topRight);
        printf("balance board bottom left weight: %f\n", bottomLeft);
        printf("balance board bottom right weight: %f\n", bottomRight);
        printf("balance board total weight: %f\n", total);
    }
}

void WiiReceiver::HandleStatus(CWiimote &wm)
{
    printf("\n");
    printf("--- CONTROLLER STATUS [wiimote id %i] ---\n\n", wm.GetID());
    
    printf("attachment: %i\n", wm.ExpansionDevice.GetType());
    printf("speaker: %i\n", wm.isUsingSpeaker());
    printf("ir: %i\n", wm.isUsingIR());
    printf("leds: %i %i %i %i\n", wm.isLEDSet(1), wm.isLEDSet(2), wm.isLEDSet(3), wm.isLEDSet(4));
    printf("battery: %f %%\n", wm.GetBatteryLevel());
}

void WiiReceiver::HandleDisconnect(CWiimote &wm)
{
    printf("\n");
    printf("--- DISCONNECTED [wiimote id %i] ---\n", wm.GetID());
    printf("\n");
}

void WiiReceiver::HandleReadData(CWiimote &wm)
{
    printf("\n");
    printf("--- DATA READ [wiimote id %i] ---\n", wm.GetID());
    printf("\n");
}

void WiiReceiver::HandleNunchukInserted(CWiimote &wm)
{
    printf("Nunchuk inserted on controller %i.\n", wm.GetID());
}

void WiiReceiver::HandleClassicInserted(CWiimote &wm)
{
    printf("Classic controler inserted on controller %i.\n", wm.GetID());
}

void WiiReceiver::HandleGH3Inserted(CWiimote &wm)
{
    printf("GH3 inserted on controller %i.\n", wm.GetID());
}

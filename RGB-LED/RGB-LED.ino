/*************************************************************
 Created with PROGRAMINO ARDUINO-IDE - 25.01.2016 13:25:48
 Project:       RGB-Selector Demo
 Libraries:
 Author:        UlliS
 Description:
**************************************************************/

// Output Anode = +5V / Kathode = digital pin x
int redPin = 9;   // Red LED,   connected to digital pin 9
int grnPin = 10;  // Green LED, connected to digital pin 10
int bluPin = 11;  // Blue LED,  connected to digital pin 11

// Color arrays (R, G, B)
int RGB_Color_white[3]  = {255, 255, 255};
int RGB_Color_dim_white[3]  = {30, 30, 30};
int RGB_Color_red[3] = {255, 0, 0};
int RGB_Color_green[3] = {0, 255, 0};
int RGB_Color_blue[3] = {0, 0, 255};
//ect.

void setup()
{
    // sets the pins as output  
    pinMode(redPin, OUTPUT);   
    pinMode(grnPin, OUTPUT);   
    pinMode(bluPin, OUTPUT); 
}

void loop()
{
    int redVal;
    int grnVal;
    int bluVal;
 
    // white
    redVal = RGB_Color_white[0];
    grnVal = RGB_Color_white[1];
    bluVal = RGB_Color_white[2];
    
    SetColor(redVal, grnVal, bluVal);   
    delay(1000);
    
    // dim white
    redVal = RGB_Color_dim_white[0];
    grnVal = RGB_Color_dim_white[1];
    bluVal = RGB_Color_dim_white[2];
    
    SetColor(redVal, grnVal, bluVal);   
    delay(1000);

    // red
    redVal = RGB_Color_red[0];
    grnVal = RGB_Color_red[1];
    bluVal = RGB_Color_red[2];
    
    SetColor(redVal, grnVal, bluVal);   
    delay(1000);
    
    // green
    redVal = RGB_Color_green[0];
    grnVal = RGB_Color_green[1];
    bluVal = RGB_Color_green[2];
    
    SetColor(redVal, grnVal, bluVal);   
    delay(1000);
    
    // blue
    redVal = RGB_Color_blue[0];
    grnVal = RGB_Color_blue[1];
    bluVal = RGB_Color_blue[2];
    
    SetColor(redVal, grnVal, bluVal);   
    delay(1000);    
}

void SetColor(byte red, byte grn, byte blue)
{
    // write current values to LED pins
    analogWrite(redPin, 255 - red);   
    analogWrite(grnPin, 255 - grn);      
    analogWrite(bluPin, 255 - blue); 
}



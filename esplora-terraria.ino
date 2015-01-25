#include <Esplora.h>

int JoyX;                                        // Drift value X
int JoyY;                                        // Drift value Y
int activate = 0;                                // Is controller activated
int oldSliderVal = 0;                                // Slider val from last loop()

void setup()
{
  Esplora.writeRGB(10, 0, 0);                    // Signal Esplora is waiting to calibrate
  int start = Esplora.readButton(SWITCH_3);      // Read First Button
  while(start == HIGH) {                         // Wait until button is pressed
    start = Esplora.readButton(SWITCH_3);
  }
  JoyX = Esplora.readJoystickX();                // Set drift variable X
  JoyY = Esplora.readJoystickY();                // Set drift variable Y
  Esplora.writeRGB(10, 10, 10);                  // Calibration is done, waiting to run
} 

void loop()
{
  int xValue = Esplora.readJoystickX();          // Read the joystick's X position
  int yValue = Esplora.readJoystickY();          // Read the joystick's Y position
  xValue = xValue - JoyX;                        // Remove drift on X position
  yValue = yValue - JoyY;                        // Remove drift on Y position
  int JoyButton = Esplora.readJoystickButton();
  int button1 = Esplora.readButton(SWITCH_1);    // Read Buttons
  int button2 = Esplora.readButton(SWITCH_2);    // Read Buttons
  int button3 = Esplora.readButton(SWITCH_3);    // Read Buttons
  int button4 = Esplora.readButton(SWITCH_4);    // Read Buttons
  int slide = 200;  //Esplora.readSlider();        // Hack-ish mouse speed fix

  int mousespeed = map(slide, 0, 1023, 10, 0);                  // Max Mouse Speed is Calculated from slider
  int mouseX = map( xValue,-512, 512, mousespeed, -mousespeed);  // map the X value to a range of movement for the mouse X, Changes input range to include new values
  int mouseY = map( yValue,-512, 512, -mousespeed, mousespeed);  // map the Y value to a range of movement for the mouse Y, Changes input range to include new values

  if(button3 == LOW){                          // Check if the Enable/Disable button is pressed
    if(activate == 1) activate = 0;
    else activate = 1;
    JoyX = Esplora.readJoystickX();                // Reset the drift variable X
    JoyY = Esplora.readJoystickY();                // Reset the drift variable Y
    delay(500);
  }
  
  int sliderVal = map(Esplora.readSlider(), 0, 1023, 10, 0);                        // Read Slider Values
  if(oldSliderVal != sliderVal){
    if(sliderVal == 10){
      Keyboard.write(101);                                    // Open menu ("e" key) 
    }
    else if(oldSliderVal == 10){
      Keyboard.write(101);
    }
    else {
      Keyboard.print(String(oldSliderVal));                      // Print the correct number key
    }
    oldSliderVal = sliderVal;
    //Serial.println(oldSliderVal);  // Debug
    //Serial.println(sliderVal);     // Debug
    //delay(10);                     // Debug
  }

  if(activate == 1){                               // Check if mouse is enabled

    Esplora.writeRGB(0 , 0, 10 );                  // Set light to blue
    Mouse.begin();                                 // Take control of the mouse

    if(JoyButton == LOW) Keyboard.press(32);       // Jump (space)
    else Keyboard.release(32);
    
    if(Esplora.readMicrophone() > 10) Keyboard.write(KEY_ESC); // Menu
    else Keyboard.release(KEY_ESC);

    if(button2 == LOW) Mouse.press(MOUSE_LEFT);        // Test if Left click is activated
    else Mouse.release(MOUSE_LEFT);

    if(button1 == LOW) Keyboard.press(119);            // Forward (W)    
    else Keyboard.release(119);

    if(button4 == LOW)  Mouse.press(MOUSE_RIGHT);      // Right Button
    else Mouse.release(MOUSE_RIGHT);

    if(Esplora.readLightSensor() < 800) Keyboard.press(32);
    else Keyboard.release(32);

    Mouse.move(mouseX, mouseY, 0);                 // Move the mouse

  }

  else {
    Mouse.end();
    Esplora.writeRGB(255, 0, 0 );  
  }
  
  delay(5);                                  // A short delay before moving again

}


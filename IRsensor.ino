#include <IRremote.h>
#include <Servo.h>
#include <EEPROM.h>

// Maps the button hex values to String 
#define	UP	      0xF609FF00
#define	DOWN	    0xF807FF00
#define	Button_0	0xE916FF00
#define	Button_1	0xF30CFF00
#define	Button_2	0xE718FF00
#define	Button_3	0xA15EFF00
#define	Button_4	0xF708FF00
#define	Button_5	0xE31CFF00
#define	Button_6	0xA55AFF00
#define	Button_7	0xBD42FF00
#define	Button_8	0xAD52FF00
#define	Button_9	0xB54AFF00
#define	SAVE	    0xB847FF00
#define ENTER     0xBF40FF00
#define CLEAR     0xE619FF00
#define PLAYSAVED 0xB946FF00


// Both different modes
#define Recording 0xBB44FF00
#define Playing   0xBC43FF00

// Maximum number of angles allowed to be saved
#define MAX_ANGLES 50

// Pin numbers for the sensor and motor used
#define sensorPin 2
#define motorPin 9


String angle;
// Temporary list to hold angle values in volitile memory
int tempList[MAX_ANGLES];
int angleCount = 0;

// Object for the servo motor being used
Servo myServo;

void setup() {
  // Serial.begin(9600);
  // Starts the IR receiver and makes the LED on it to light up whenever it detects a signal
  IrReceiver.begin(sensorPin, ENABLE_LED_FEEDBACK);

  myServo.attach(motorPin);
}

// Adds an angle value to the tempList 
void addAngle(int angle){
  if(angleCount < MAX_ANGLES){
      tempList[angleCount] = angle;
      angleCount++;
  }
}

// Executes the saved input values 
void playInputs(){
 //Serial.println("Playing");
  for(int i = 0; i < angleCount; i++){
    myServo.write(tempList[i]);
    delay(1000);
  }
}

void saveInputs(){
  //Serial.println("Inputs are being saved");
  
  EEPROM.write(0, angleCount);

  for(int i = 1; i <= angleCount; i++){
    EEPROM.write(i, tempList[i-1]);
  }
  
}

void playSavedInputs(){
  //Serial.println("Playing Saved Inputs");
  
  for(int i = 0; i < EEPROM.read(0); i++){
    myServo.write(EEPROM.read(i+1));
    //Serial.println(EEPROM.read(i+1));
    delay(1000);
  }
  
}

void loop() {
  if (IrReceiver.decode()) {
    uint32_t code = IrReceiver.decodedIRData.decodedRawData;
    /*
    if (code != 0x0) {
     //Serial.print("Raw: ");
     //Serial.println(code, HEX);
    }
    */
    switch(code){
      case UP:
       //Serial.println("Up");
        //angle += "10";
        angle = String(angle.toInt() + 10);
        break;
      case DOWN:
       //Serial.println("Down");
        angle = String(angle.toInt() - 10);
        break;
      case Button_0:
       //Serial.println("0");
        angle += "0";
        break;
      case Button_1:
       //Serial.println("1");
        angle += "1";
        break;
      case Button_2:
       //Serial.println("2");
        angle += "2";
        break;
      case Button_3:
       //Serial.println("3");
        angle += "3";
        break;
      case Button_4:
       //Serial.println("4");
        angle += "4";
        break;
      case Button_5:
       //Serial.println("5");
        angle += "5";
        break;
      case Button_6:
       //Serial.println("6");
        angle += "6";
        break;
      case Button_7:
       //Serial.println("7");
        angle += "7";
        break;
      case Button_8:
       //Serial.println("8");
        angle += "8";
        break;
      case Button_9:
       //Serial.println("9");
        angle += "9";
        break;
      
      case ENTER:
       //Serial.println("ENTER");
        if(angle.toInt() % 180 == 0 && angle.toInt() > 0){
          addAngle(180);
        }
        else {
         addAngle(angle.toInt() % 180);
        }
        angle = "";
        break;

      case SAVE:     
       //Serial.println("SAVE RECORDING");
        if(angleCount > 0){
          saveInputs();
        } //else{
         //Serial.println("No inputs found");
         //}
        
        break;

      case PLAYSAVED:
        playSavedInputs();
        break;

      case Recording:
       //Serial.println("Recording");
        for(int i = 0; i < angleCount; i++){
         //Serial.println(tempList[i]);
        }
        break;
      
      case Playing:
        playInputs();
        break;

      case CLEAR:
        // Effectively clears the list as it resets the counter for which the list is read
        angleCount = 0;
       //Serial.println("CLEAR");
        break;
    }

    IrReceiver.resume();
  }
}


#define OP_DECODEMODE 8
#define OP_SCANLIMIT 10
#define OP_SHUTDOWN 11
#define OP_DISPLAYTEST 14
#define OP_INTENSITY 10
int LED1 = 47;
int LED2 = 48;
int LED3 = 49;
byte spidata[2];
int DIN = 11;
int CS = 12;
int CLK = 13;

int xValue = 0;
int yValue = 0;
//byte = OCP4;
#define VRX_PIN  A0 // Arduino pin connected to VRX pin
#define VRY_PIN  A1 // Arduino pin connected to VRY pin
void spiTransfer(volatile byte row, volatile byte data);
void printCoords(int xPin, int yPin);
int xCoords(int xPin);
int yCoords(int yPin);

void setup() {
  // put your setup code here, to run once:
  // Change these pins as necessary

  Serial.begin(115200);
  // Set all digital pins to OUTPUT mode and set the pin for CS to HIGH
  // Do this by accessing the registers directly

  //must do this setup
  pinMode(DIN, OUTPUT);
  pinMode(CS, OUTPUT);
  pinMode(CLK, OUTPUT);

  spiTransfer(OP_DISPLAYTEST,0);
  spiTransfer(OP_SCANLIMIT,7);
  spiTransfer(OP_DECODEMODE,0);
  spiTransfer(OP_SHUTDOWN,1);

  // part 1.2  begin
  // Setting up joystick
  
  // pinMode(LED1, OUTPUT);
  // pinMode(LED2, OUTPUT);
  // pinMode(LED3, OUTPUT);
  
  // part 1.4
  // DDRL |= (1 << DDL2) | (1 << DDL3) | (1 << DDL4);  // DDRL for direction register

  // Part 2
  // DDRL |= (1 << PORTH3);
  DDRL |= (1 << DDF0) | (1 << DDF1);
}

void loop() {

  //VRX_PIN
  //VRY_PIN
  ledDisplay(xCoords(PORTF0), yCoords(PORTF1));
  //printCoords(xCoords(PORTF0), yCoords(PORTF1));
  //delay(1);
  //x_val = xCoords(VRX_PIN);
  //y_val = yCoords(VRY_PIN);

  // int j = 0;
  // int i = 0;
  // for (j = 0; j < 8; j++){ //for each row, set the LEDs
  //   spiTransfer(j, 0b10100101);
    
  // }
  // delay(500);
  // for (i = 0; i < 8; i++){ //for each row, clear the LEDs
  //   spiTransfer(i, 0b00000000);
  // }
  // delay(500);
  // Part 1.2 begin 47-->48-->49-->47-->48
  // digitalWrite(LED1, HIGH);
  // delay(333);
  // digitalWrite(LED1, LOW);
  // digitalWrite(LED2, HIGH);
  // delay(333);
  // digitalWrite(LED2, LOW);
  // digitalWrite(LED3, HIGH);
  // delay(333);
  // digitalWrite(LED3, LOW);
  
  // Part 1.4
  //Turn on PORTL2 (pin 47), delay, then turn off
  // PORTL |= (1 << PORTL2);
  // delay(333);
  // PORTL &= ~(1 << PORTL2);

  // // Turn on PORTL3 (pin 48), delay, then turn off
  // PORTL |= (1 << PORTL1);
  // delay(333);
  // PORTL &= ~(1 << PORTL1);

  // // Turn on PORTL4 (pin 49), delay, then turn off
  // PORTL |= (1 << PORTL0);
  // delay(333);
  // PORTL &= ~(1 << PORTL0);
  
  // Part 2
  


}

// debugging method
void printCoords(int xPin, int yPin) {
  // read analog X and Y analog values
  xValue = analogRead(xPin);
  yValue = analogRead(yPin);

  // print data to Serial Monitor on Arduino IDE
  Serial.print("x = ");
  Serial.print(xValue);
  Serial.print(", y = ");
  Serial.println(yValue);
  delay(200);
}

void ledDisplay(int x_coord, int y_coord){
  // Map the joystick coordinates (0-1023) to the 8x8 display range (0-7)
  int x_display = map(x_coord, 0, 1000, 0, 7);
  int y_display = map(y_coord, 0, 1000, 7, 0);

  // Clear the display first
  for (int i = 0; i < 8; i++) {
    spiTransfer(i, 0b00000000);  // Clear all rows
  }

  // Set the specific LED for x_display and y_display
  byte rowValue = 0b00000001 << x_display;  // Set the bit for x_display
  spiTransfer(y_display, rowValue);         // Send to the row corresponding to y_display
}

int xCoords(int xPin) {
  return analogRead(xPin);
  //delay(200);
}

int yCoords(int yPin) {
  // read analog X and Y analog values
  return analogRead(yPin);
  //delay(200);
}

void spiTransfer(volatile byte opcode, volatile byte data){
  int offset = 0; //only 1 device
  int maxbytes = 2; //16 bits per SPI command
  
  for(int i = 0; i < maxbytes; i++) { //zero out spi data
    spidata[i] = (byte)0;
  }
  //load in spi data
  spidata[offset+1] = opcode+1;
  spidata[offset] = data;
  digitalWrite(CS, LOW); //
  for(int i=maxbytes;i>0;i--)
    shiftOut(DIN,CLK,MSBFIRST,spidata[i-1]); //shift out 1 byte of data starting with leftmost bit
  digitalWrite(CS,HIGH);
}
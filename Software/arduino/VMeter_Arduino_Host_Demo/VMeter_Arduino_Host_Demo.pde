// adapted from code by Collin Cunningham: http://www.circuitsathome.com/arduino_usb_host_shield_projects
// the LPK25_Host_pde: http://blog.makezine.com/archive/2010/12/usbhacking.html
// the Host shield uses the MAX3421E http://datasheets.maximintegrated.com/en/ds/MAX3421E.pdf

// This code can do several things, depending on what you comment and uncomment
// It reads both pressure and position outputs from the VMeter, but you must make
// sure to configure those outputs to be enabled, and also transmit on MIDI controler #20
// #18, respectively.

// The position and pressure outputs can be routed to control a DC motor via analogWrite()
// or servo motors.

// This also reads from the analog 0 input, and writes out on controller #20 to the VMeter.


//////////////////////////////////////////////////////
// LPK25 USB-MIDI to Serial MIDI converter
// for use with USB Host Shield from Circuitsathome.com
// by Collin Cunningham - makezine.com, narbotic.com
// 
//////////////////////////////////////////////////////

//#include <Spi.h>  // NOTE: alternate Spi Library from Circuitsathome.com
#include <Max3421e.h>
#include <Usb.h>
#include <avr/pgmspace.h>
#include <Servo.h>
//	For general USB-MIDI compliance, we need to determine which endpoint has a direction of IN
//	and likely also need to get the poll duration(EP_POLL), not sure if necessary though
//	
Servo myservo;
Servo myservo2;
/*   USB ERROR CODES
 hrSUCCESS   0x00
 hrBUSY      0x01
 hrBADREQ    0x02
 hrUNDEF     0x03
 hrNAK       0x04
 hrSTALL     0x05
 hrTOGERR    0x06
 hrWRONGPID  0x07
 hrBADBC     0x08
 hrPIDERR    0x09
 hrPKTERR    0x0A
 hrCRCERR    0x0B
 hrKERR      0x0C
 hrJERR      0x0D
 hrTIMEOUT   0x0E
 hrBABBLE    0x0F
 */
 
 // The VMeter has 3 endpoints, a "control" endpoint, an input, and an output

// descriptors retrieved using descriptor_parser.pde (Usb library example sketch)
#define LPK25_ADDR            1 // should be an IN direction (device to host)
#define LPK25_VID_LO          0xFFFFFFD0  // vmeter VID and PID ids
#define LPK25_VID_HI          0x16
#define LPK25_PID_LO          0x68  
#define LPK25_PID_HI          0x06
#define LPK25_CONFIG          1
#define LPK25_IF              1
#define LPK25_NUM_EP          3 // control, output and input
#define EP_MAXPKTSIZE         64
#define EP_BULK		      0x02 
#define EP_POLL               0x00	
#define CONTROL_EP            0
#define OUTPUT_EP             2 
#define INPUT_EP              1
#define LPK25_01_REPORT_LEN   0x09
#define LPK25_DESCR_LEN       0x0C // was 0x0C. def doesn't work if this changes.

EP_RECORD ep_record[ LPK25_NUM_EP ];  //endpoint record structure for the LPK25 controller

char descrBuf[ 12 ] = { 
  0 };	//buffer for device description data
char buf[ 8 ] = { 
  0 };			//buffer for USB-MIDI data, if both position and pressure outputs
                        // are on, need 8 bytes here.


MAX3421E Max;
USB Usb;

int sensorValue = 0;        
int outputValue = 0; 

void setup() {
  Serial.begin( 115200 );  
  Max.powerOn();
  myservo.attach(5);
  myservo2.attach(6);
  delay(200);
}

void loop() {

  Max.Task();
  Usb.Task();

  if( Usb.getUsbTaskState() == USB_STATE_CONFIGURING ) {  //wait for addressing state
    LPK25_init();
    Usb.setUsbTaskState( USB_STATE_RUNNING );
  }
  if( Usb.getUsbTaskState() == USB_STATE_RUNNING ) {  //poll the LPK25 Controller 
    LPK25_poll();
  }
  read_analog_0(); // comment this line out if you aren't reading an analog input, otherwise the display will
                  // show any noise, and may fight with finger inputs.
}

/* Initialize LPK25 Controller */
void LPK25_init( void ){

  byte rcode = 0;  //return code
  byte i;

  /* Initialize data structures for endpoints of device 1*/
  ep_record[ CONTROL_EP ] = *( Usb.getDevTableEntry( 0,0 ));  //copy endpoint 0 parameters
  ep_record[ OUTPUT_EP ].epAddr = 0x01;    // Output endpoint, 0x02 for Korg nanoKey
  ep_record[ OUTPUT_EP ].Attr  = EP_BULK;
  ep_record[ OUTPUT_EP ].MaxPktSize = EP_MAXPKTSIZE;
  ep_record[ OUTPUT_EP ].Interval  = EP_POLL;
  ep_record[ OUTPUT_EP ].sndToggle = bmSNDTOG0;
  ep_record[ OUTPUT_EP ].rcvToggle = bmRCVTOG0;
  ep_record[ INPUT_EP	].epAddr = 0x02;			// Input endpoint, 0x02 for Korg nanoKey 
  ep_record[ INPUT_EP ].Attr  = EP_BULK;
  ep_record[ INPUT_EP ].MaxPktSize = EP_MAXPKTSIZE;
  ep_record[ INPUT_EP ].Interval  = EP_POLL;
  ep_record[ INPUT_EP ].sndToggle = bmSNDTOG0;
  ep_record[ INPUT_EP ].rcvToggle = bmRCVTOG0;

  Usb.setDevTableEntry( LPK25_ADDR, ep_record );             //plug kbd.endpoint parameters to devtable

  /* read the device descriptor and check VID and PID*/
  rcode = Usb.getDevDescr( LPK25_ADDR, ep_record[ CONTROL_EP ].epAddr, LPK25_DESCR_LEN, descrBuf );
  if( rcode ) {
    Serial.print("Error attempting read device descriptor. Return code: ");
    Serial.println( rcode, HEX );
    while(1);  //stop
  }

  // Test device for VMeter (original code was for LPK25)
  if((descrBuf[ 8 ] != LPK25_VID_LO) || (descrBuf[ 9 ] != LPK25_VID_HI) || (descrBuf[ 10 ] != LPK25_PID_LO) || (descrBuf[ 11 ] != LPK25_PID_HI) ){
    Serial.print("Unsupported USB Device");
    Serial.println(descrBuf[ 8 ], HEX);
    while(1);  //stop
  }


  /* Configure device */
  rcode = Usb.setConf( LPK25_ADDR, ep_record[ CONTROL_EP ].epAddr, LPK25_CONFIG );                    
  if( rcode ) {
    Serial.print("Error attempting to configure LPK25. Return code :");
    Serial.println( rcode, HEX );
    while(1);  //stop
  }
  Serial.println("VMeter initialized");
  Serial.println("");
  Serial.println("");
  Serial.println("");
  delay(200);
  
    
}

// Poll LPK25 and print result
void LPK25_poll( void ){

  byte rcode = 0;     //return code 
   
  /* poll LPK25 */
  rcode = Usb.inTransfer( LPK25_ADDR, ep_record[ INPUT_EP ].epAddr, 8, buf, 1 );
  // the last arg to inTransfer is how many times it should check for data before giving up.
  // If you don't include that last argumnet, the code blocks here for about 1s if nothing
  // is being sent!
  if( rcode != 0 ) {
    //Serial.print("error: ");
    //Serial.println(rcode, HEX);
    return;
  }
  process_report();
  return;
}

void read_analog_0() {
  // read analog pin 0
  sensorValue = analogRead(0); 
  int new_output_val;
  // map it to the range of the MIDI controller msgs, 0-127:
  new_output_val = map(sensorValue, 0, 512, 0, 127);  
  if (new_output_val > 127) // cap the high and low values
    new_output_val = 127;
  if (new_output_val < 0)
    new_output_val = 0;
  if (abs(new_output_val - outputValue) > 1) {
    // ouput has changed, send new output out.
    outputValue = new_output_val;
    send_vmeter_column(outputValue);
    Serial.println(sensorValue); 
  } 
}

void send_vmeter_column(int h) { // h is 0-127
  char data[4];
  byte rcode = 0;  //return code
  Serial.print("sending: ");
  Serial.println(h);
  
  data[0] = 0x00; // not sure why this is needed, we're only sending three bytes
  data[1] = 0xB0;
  data[2] = 0x14;
  data[3] = h; // 127 full display
  rcode = Usb.outTransfer( LPK25_ADDR, ep_record[ OUTPUT_EP ].epAddr, 4, data );                 
  if( rcode ) {
    Serial.print("Error sending: ");
    Serial.println( rcode, HEX );
    //while(1);  //stop
  }
}

int print_count;

void process_report(void){
  byte i, codeIndexNumber;
  int val;
  
  // Uncomment the servo output lines or the analogWrite() line


  // Send servo commands out based on position and pressure
  // up to two packets, or 8 bytes may be received in buf
  // check first 4 bytes (packet) for position or pressure output
  if (buf[2] == 20) { // 
    Serial.print("pos: ");Serial.println(buf[3],HEX);  
    output_servo1(buf[3]);  
  }
  else if (buf[2] == 18) { // pressure
    Serial.print("pres: ");Serial.println(buf[3],HEX); 
    output_servo2(buf[3]);
  }
  
  // check 2nd packet
  if (buf[6] == 20) { // 
    Serial.print("pos: ");Serial.println(buf[7],HEX);    
    output_servo1(buf[7]);
  }
  else if (buf[6] == 18) { // pressure
    Serial.print("pres: ");Serial.println(buf[7],HEX); 
    output_servo2(buf[7]);
  }
  
     
  //analogWrite(5, outBuf[2]*2);   
  return;
  
}

void output_servo1(int p) {
  p = map(p, 0, 127, 20, 179); 
  myservo.write(p); 
}
void output_servo2(int p) {
  p = map(p, 0, 127, 25, 179); 
  myservo2.write(p); 
}

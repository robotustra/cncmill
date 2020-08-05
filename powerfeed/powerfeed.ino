/*
	Power drive test.
	Pin 2 - input encoder channel A,
	Pin 3 - input encoder channel B

	Encoder interrupt pin = 2;

*/




int PIN_ENC_A = 2;
int PIN_INT 	= 1; // interrupt pin for arduino Leonardo 32u4
int PIN_ENC_B = 3;

int PIN_MODE = 5; // input mode, direction 

int PIN_PULS = 13;
int PIN_DIR = 12;

volatile unsigned long debounceThreshold = 700; // microseconds
volatile unsigned long debounceLast = 0;
 
volatile int pos = 0;
int lastPos = 0;
int pos_diff = 0;

volatile int mode = 0;
int speed = 0; // speed of turning ()

void setup() { 
 	//Initialize serial and wait for port to open:
  	/*Serial.begin(9600); 
  	while (!Serial) {
    	; // wait for serial port to connect. Needed for Leonardo only
  	}*/
	
  	pinMode(PIN_ENC_A, INPUT_PULLUP);
  	pinMode(PIN_ENC_B, INPUT_PULLUP);
    pinMode(PIN_MODE,  INPUT_PULLUP);


  	attachInterrupt(PIN_INT, doEncoder, CHANGE);

  	pinMode(PIN_PULS, OUTPUT);
  	pinMode(PIN_DIR, OUTPUT);

  	digitalWrite(PIN_DIR, HIGH);
  	digitalWrite(PIN_PULS, HIGH);

  
} 



void loop ()
{
	mode = digitalRead(PIN_MODE);
  	if (mode)
  	{
  		//continuous mode on the base of speed value
  		if (pos != lastPos) 
		{
			pos_diff = lastPos - pos;
			speed += pos_diff;
		}

		if (speed > 0 )
		{
			digitalWrite (PIN_DIR, HIGH);
		}
		else
		{
			digitalWrite (PIN_DIR, LOW);
		}

		delay(10);
		doStep();

  	}
  	else
  	{
  		//step mode;
  		if (pos != lastPos) 
		{
			if (pos < lastPos)
			{
				digitalWrite (PIN_DIR, HIGH);
			}
			else
			{
				digitalWrite (PIN_DIR, LOW);
			}

		    doStep();
		 
		    lastPos = pos;
	  	}
	  	// reset speed all the time in step mode
	  	speed = 0;
  	}
   

}


void doStep()
{
    digitalWrite (PIN_PULS,LOW);
    digitalWrite (PIN_PULS,HIGH);
}

void doEncoder() {
	if (micros() - debounceLast > debounceThreshold) 
	{
		debounceLast = micros();

		if (PIND & B00000010) {
			if (PIND & B00000001) {
			    pos--; // counter-clock-wise
			}
			else {
			    pos++; // clock-wise
			}
		}	
	}
}

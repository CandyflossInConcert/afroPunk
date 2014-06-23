import processing.serial.*;
import spacebrew.*;

String server="sandbox.spacebrew.cc";
String name="FlexSensor";
String description ="Client that sends and receives range messages. Range values go from 0 to 1023.";



Spacebrew sb;
Serial myPort;        // The serial port
int xPos = 1;         // horizontal position of the graph
float inByte;

void setup () {
  // set the window size:
  size(400, 300);        

  // instantiate the spacebrewConnection variable
  sb = new Spacebrew( this );

  // declare your publishers
  sb.addPublish( "flexsensorVal", "range", int(inByte)); 


  // connect!
  sb.connect(server, name, description );

  // List all the available serial ports
  println(Serial.list());
  // I know that the first port in the serial list on my mac
  // is always my  Arduino, so I open Serial.list()[0].
  // Open whatever port is the one you're using.
  myPort = new Serial(this, Serial.list()[0], 9600);
  // don't generate a serialEvent() unless you get a newline character:
  myPort.bufferUntil('\n');
  // set inital background:
  background(0);
}
void draw () {
if (sb.connected()) {
    //print client name to screen
    fill(255);
    textSize(12);
    text("Connected as: " + name, width/2, 25);
    //print current value to screen

    text(int(inByte), 25, 25);

    sb.send("flexsensorVal", "range", str(inByte));
  }
  else {

    text("Not Connected", 25, 25);
  }
}

void serialEvent (Serial myPort) {
  // get the ASCII string:
  String inString = myPort.readStringUntil('\n');

  if (inString != null) {
    // trim off any whitespace:
    inString = trim(inString);
    // convert to an int and map to the screen height:
    inByte = float(inString); 
    inByte = map(inByte, 0, 1023, 0, height);

    // draw the line:
    stroke(127, 34, 255);
    line(xPos, height, xPos, height - inByte);

    // at the edge of the screen, go back to the beginning:
    if (xPos >= width) {
      xPos = 0;
      background(0);
    } 
    else {
      // increment the horizontal position:
      xPos++;
    }
  }
}


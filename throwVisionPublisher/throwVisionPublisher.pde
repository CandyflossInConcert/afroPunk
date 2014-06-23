import spacebrew.*;
import processing.serial.*;

String server="sandbox.spacebrew.cc";
String name="FlexSensor";
String description ="Client that sends and receives range messages. Range values go from 0 to 1023.";


Spacebrew sb;
Serial myPort;

int xPos = 1;
int incomingRange = 0;

void setup() {
  size(400, 400);
  background(0);

  // instantiate the spacebrewConnection variable
  sb = new Spacebrew( this );

  // declare your publishers
  sb.addPublish( "flexsensorVal", "range", incomingRange ); 


  // connect!
  sb.connect(server, name, description );

  //print list of serial devices to console
  println(Serial.list());
  myPort = new Serial(this, Serial.list()[0], 9600);
  myPort.bufferUntil('\n');
}


void draw() {
  background(0);
  textAlign(CENTER);
  fill (255);
  textSize(40);
  text("THROW VISION", width/2, height/2); 

  //send the value coming from arduino to spacebrew   
  if (sb.connected()) {
    //print client name to screen
    fill(255);
    textSize(12);
    text("Connected as: " + name, width/2, 25);
    //print current value to screen

    text(incomingRange, 25, 25);

    sb.send("flexsensorVal", "range", str(incomingRange));
  }
  else {

    text("Not Connected", 25, 25);
  }
}

//is arduino communicating with processing?
void serialEvent (Serial myPort) {
  // get the ASCII string:
  String inString = myPort.readStringUntil('\n');

  if (inString != null) {
    // trim off any whitespace:
    inString = trim(inString);
    // convert to an int and map to the screen height:
    float inByte = float(inString); 
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



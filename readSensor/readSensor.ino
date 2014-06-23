  
int sensorVal;
  
void setup()
{
  // initialize serial communications
  Serial.begin(9600); 
}

void loop()
{
  
  // read the input
  sensorVal = analogRead(0);
  
  // print out the result
  //Serial.println(sensorVal);
  
 Serial.write(sensorVal);
  
  // pause before taking the next reading
                       
}

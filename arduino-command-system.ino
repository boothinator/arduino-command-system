// Keep track of the current command using a single character to represent the command
// Use the nul character '\0' when there is no command
char currentCommand = '\0';

void setup() {
  Serial.begin(9600);
}

void loop() {
  Serial.print("Current Command: ");
  Serial.println(currentCommand);

  // Echo command to read from serial port input and output back to serial port
  if ('E' == currentCommand) {

    // Is there input available?
    if (Serial.available() > 0) {

      // Input available, lets get the value
      char input = (char)Serial.read();

      // Only process input if it is not a new-line or carriage-return
      if (input != '\n' && input != '\r') {

        // Echo input back to serial port
        Serial.print("Got value: ");
        Serial.println(input);

        // Clear out the current command so we can do something else
        currentCommand = '\0';
      }
    }
  }


  // Include a delay so we can see how the system works
  delay(1000);
}
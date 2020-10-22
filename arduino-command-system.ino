// Keep track of the current command using a single character to represent the command
// Use the nul character '\0' when there is no command
char currentCommand = '\0';

void setup() {
  Serial.begin(9600);
}

void loop() {
  Serial.print("Current Command: ");
  Serial.println(currentCommand);

  // Set the current command to the default command when no command is running
  if ('\0' == currentCommand) {

    // Use Echo as the default command
    currentCommand = 'S';
  }

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
  } else if ('S' == currentCommand) {
    // Serial command to read input from serial port and use that as the next command

    // Is there input available?
    if (Serial.available() > 0) {

      // Input available, lets get the value
      char input = (char)Serial.read();

      // Only process input if it is not a new-line or carriage-return
      if (input != '\n' && input != '\r') {

        // Echo input back to serial port
        Serial.print("Got command: ");
        Serial.println(input);

        // Clear out the current command to the value we received
        currentCommand = input;
      }
    }
  } else {
    Serial.print("Unknown Command: ");
    Serial.println(currentCommand);

    // Clear out command if we didn't recognize it
    currentCommand = '\0';
  }


  // Include a delay so we can see how the system works
  delay(1000);
}
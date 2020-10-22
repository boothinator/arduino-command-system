// Keep track of the current commands using a single character to represent the command
#define MAX_CURRENT_COMMAND_ARRAY_LENGTH 10
char currentCommandArray[MAX_CURRENT_COMMAND_ARRAY_LENGTH];
int currentCommandArrayLength = 0;

// Represents time the drive command started running
// 0 means that command has not started
unsigned long driveStartTime = 0;

void setup() {
  Serial.begin(9600);
}

void loop() {
  // Process every command in the array
  for (int currentCommandIndex = 0; currentCommandIndex < currentCommandArrayLength; currentCommandIndex++) {

    // Store the current command in a convenience variable
    char currentCommand = currentCommandArray[currentCommandIndex];

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
    } else if ('F' == currentCommand) {
      // Move robot forward for 5 seconds
      // Note: this is simulated for now. Only outputs to serial port and doesn't drive motors

      // We need to start driving if driveStartTime is 0
      if (0 == driveStartTime) {
        Serial.println("Starting to move forward");

        // Record the time we started driving
        // Time is the number of milliseconds since program started running
        driveStartTime = millis();

      } else if ((millis() - driveStartTime) > 5000) {
        // It's been more than 5 seconds (5000 milliseconds)
        // Time to stop driving
        Serial.println("Stopping forward movement");

        // Reset drive start time
        driveStartTime = 0;

        // Clear out current command so next command can run
        currentCommand = '\0';
      } else {
        Serial.print("Moving forward for ");
        Serial.print(5000 - (millis() - driveStartTime));
        Serial.println(" more milliseconds");
      }
    } else {
      Serial.print("Unknown Command: ");
      Serial.println(currentCommand);

      // Clear out command if we didn't recognize it
      currentCommand = '\0';
    }
  }


  // Include a delay so we can see how the system works
  delay(1000);
}
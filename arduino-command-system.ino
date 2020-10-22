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
  Serial.print("Commands: ");
  Serial.write(currentCommandArray, currentCommandArrayLength);
  Serial.println();

  // Set the current command to the default command when no command is running
  if (0 == currentCommandArrayLength) {

    // Use Echo as the default command
    // Add it to the array and increase length of the array
    currentCommandArray[0] = 'S';
    currentCommandArrayLength++;
  }

  // Process every command in the array
  for (int currentCommandIndex = 0; currentCommandIndex < currentCommandArrayLength; currentCommandIndex++) {

    // Store the current command in a convenience variable
    // Note: this variable should not change in this loop
    const char currentCommand = currentCommandArray[currentCommandIndex];

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

          // Remove the current command so we can do something else
          // First, replace current command in array with the command at the end of the array
          currentCommandArray[currentCommandIndex] = currentCommandArray[currentCommandArrayLength - 1];
          // Then, shorten the length of the array by 1
          currentCommandArrayLength--;
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

          // Add a new command to the array using the value we received
          currentCommandArray[currentCommandArrayLength] = input;
          currentCommandArrayLength++;
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

        // Remove the current command so next command can run
        // First, replace current command in array with the command at the end of the array
        currentCommandArray[currentCommandIndex] = currentCommandArray[currentCommandArrayLength - 1];
        // Then, shorten the length of the array by 1
        currentCommandArrayLength--;
      } else {
        Serial.print("Moving forward for ");
        Serial.print(5000 - (millis() - driveStartTime));
        Serial.println(" more milliseconds");
      }
    } else {
      Serial.print("Unknown Command: ");
      Serial.println(currentCommand);

      // Remove the command if we didn't recognize it
      // First, replace current command in array with the command at the end of the array
      currentCommandArray[currentCommandIndex] = currentCommandArray[currentCommandArrayLength - 1];
      // Then, shorten the length of the array by 1
      currentCommandArrayLength--;
    }
  }


  // Include a delay so we can see how the system works
  delay(1000);
}
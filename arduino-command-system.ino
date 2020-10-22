// Keep track of the current command using a single character to represent the command
// Use the nul character '\0' when there is no command
char currentCommand = '\0';

void setup() {
  Serial.begin(9600);
}

void loop() {
  Serial.print("Current Command: ");
  Serial.println(currentCommand);

  // Include a delay so we can see how the system works
  delay(1000);
}
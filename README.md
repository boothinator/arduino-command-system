# Arduino Command System
This is an Arduino command-based programming system inspired by FRC's command-based programming. Mainly this is a teaching tool where, starting with the basic idea of a command-based system, then building on and refactoring that code until it becomes a usable, generic system.

Each branch can be used as a lesson and is a usable program. The changes within each branch are well-documented and show how we got from one place to the next.

## Basic Command-based System
Branch: basic-command-based-system

### Tracking the Current Command
First, we set up the repository and code file. We start off our code by tracking the current command, which we represent as a single character in the currentCommand variable. If currentCommand has the nul character ('\0'), then no command is being run at all.

To make this more concrete, we set up the serial interface and output the current command in the loop() function. We also add a delay in the loop to ensure that we have time to read the output before the loop runs again. When we open the serial monitor, we simply see the text "Current Command: ", showing us that there is currently no command.

### Echo Command
Next, we create the first command: an echo command. To create commands in this system we simply add an if statement to the loop() that runs if currentCommand matches our command code. For the echo command, we choose the character code 'E'. That way, we run our echo code any time currentCommand is set to 'E'.

The echo command code has to take input from the serial port and then output it back to the serial port. We do that by checking if input is available. If not, then the command stops processing and the rest of the loop() code runs. The next time loop() runs the echo command will run again.

If input is available, the echo command will read that input and print it back to the serial port. For example, if an 'x' was received, the following message will be sent: "Got value: x". However, newline (\n) and carriage return characters ('r') will be ignored.

Once the character is echoed back to the serial port, the current command is cleared out.

### Default Command
The program will not run the echo command yet since we don't have a way to set the current command. The simplest way to set the command is to check if the current command is empty and set it to the default command. We will set the current command to the echo command since we have no other commands at this point.

When a command is finished, it will clear out the current command by setting it to '\0'. When loop() runs again, we check if currentCommand is empty ('\0'). If so, we set the current command to the echo command.

### Serial Command
It would be nice to use the serial interface to change which command is running. We will create a new "serial" command to take input from the serial interface and switch to that command. We'll also use 'S' to represent the command.

We can create the serial command by simply copying the code for the echo command and changing the code from 'E' to 'S'. We'll also update the command to do more than just sending the input back to the serial port. At the end of the command we'll set the current command to the value that we got from the serial port. That way the command for that code will be run on the next loop().

We also need to change the default command to the serial command so that it runs when the program starts.

We can try this by uploading the code to our arduino, loading up the Serial Monitor tool, and sending an E to the arduino. You should see the command change from S to E. When you send another character, you should also see the command change back to S.

You may notice that you can send an invalid command, such as 'X', and that the program will then become stuck on your invalid command. This can be fixed by checking if the command is invalid, and clearing out the command if it is invalid.

### Forward Command
Finally, we will create a third command to represent driving the robot forward for five seconds. It won't actually drive the robot, but will output to the serial port to describe what it would do in reality.

We need to add another variable to help us figure out how long the forward command has been running. We'll call this variable driveStartTime, and we'll initially set it to 0. It will hold the time that the forward command started running, represented by the number of milliseconds since the program started.

To create the actual command code we'll add another if statement that matches the 'F' command code. In the if statement, we'll figure out if we need to start moving foward by checking if driveStartTime is equal to zero. If so, then we know that we haven't started moving yet. If that is the case, we'll save the current time to driveStartTime and also send the message "Starting to move forward".

We also need to check if our five seconds are done. We'll do this with an "else if" statement which calculates how long the command has been running, and stops the command if the time is longer than five seconds. If it's time to stop, then we'll send the message "Stopping forward movement", reset the driveStartTime back to 0, and clear the current command by setting it to '\0'. This allows the default command to take over again.

Finally, we'll output a message while the command is running to count down how much time is left. This only runs when not starting or stopping by using an "else" statement.

Now, we can test this out by sending the "F" command to the program. We'll see the forward command start, countdown, stop, and return control to the S command.
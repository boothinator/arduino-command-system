# Arduino Command System
This is an Arduino command-based programming system inspired by FRC's command-based programming. Mainly this is a teaching tool where, starting with the basic idea of a command-based system, then building on and refactoring that code until it becomes a usable, generic system.

Each branch can be used as a lesson and is a usable program. The changes within each branch are well-documented and show how we got from one place to the next.

- [Arduino Command System](#arduino-command-system)
  - [Basic Command-based System](#basic-command-based-system)
    - [Tracking the Current Command](#tracking-the-current-command)
    - [Echo Command](#echo-command)
    - [Default Command](#default-command)
    - [Serial Command](#serial-command)
    - [Forward Command](#forward-command)
  - [Allow Multiple Commands](#allow-multiple-commands)
    - [Looping Over Commands in the Command Array](#looping-over-commands-in-the-command-array)
    - [Fix the Default Command Code](#fix-the-default-command-code)
    - [Finding All the Bugs](#finding-all-the-bugs)
    - [Fixing the Bugs](#fixing-the-bugs)
    - [Displaying All Running Commands](#displaying-all-running-commands)
    - [Fixing the Too Many Commands Bug](#fixing-the-too-many-commands-bug)
    - [Preventing Processing Delays](#preventing-processing-delays)
    - [Refactor Code](#refactor-code)
    - [Multiple Command Bug](#multiple-command-bug)
  - [Subsystems](#subsystems)

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

## Allow Multiple Commands
Branch: Allow Multiple Commands, starting at commit 35c6539 Track array of commands

One of the major goals of this system is to run multiple commands at the same time. Even though an Arduino can't actually run multiple things at the same time, they run code so quickly that simply switching between pieces of code makes it appear that the code runs at the same time.

To accomplish this, we'll replace our currentCommand variable with an array that can hold multiple commands at the same time. In this case, we are creating a character array called currentCommandArray that can store a maximum of 10 command characters. We are also storing that maximum length in a #define called MAX_CURRENT_COMMAND_ARRAY_LENGTH. The actual number of command characters will be less than or equal to the maximum, so we store the actual number of characters in a variable called currentCommandArrayLength. At the start, the number of commands is zero.

### Looping Over Commands in the Command Array
Getting rid of currentCommand prevents the code from compiling since so much of our code needs to know the current command. We really want to run the command processing code for every code in the array, so we don't need to make major changes to fix things. The easiest way to fix this is to add a for loop that runs for each item in the currentCommandArray and put the command processing code in that for loop. Then, we can create a new local variable inside the for loop called currentCommand and set it. The command processing code now works mostly the same way as before.

### Fix the Default Command Code
Adding the for loop makes our code compile, but now no commands will run. It seems that our default command code doesn't work anymore since it's inside the for loop. The command array starts off with no commands, so the for loop doesn't run. Let's move the default command code out of the for loop and fix the logic. Now, we know that there are no commands because the command array has a length of zero, not because the current command is the nul character. We also need to change the way we add commands. To set the default command, we need to put it in the first slot of the array (position 0) and increase the array length by one.

### Finding All the Bugs
As a result of these changes, the code now runs our default command again! However, the Serial command now isn't changing commands when you tell it to. You may notice that the Serial command is updating currentCommand still. This doesn't work because currentCommand doesn't control the next command to run anymore. Instead, currentCommandArray controls the next command to run. We want to fix all the bugs like this, and one quick way to identify all of them is to make currentCommand a constant by using the const keyword. This prevents our code from compiling, but now the compiler is helping us to identify when our code is misusing the currentCommand variable.

### Fixing the Bugs
Our first fix is to change the Serial command to add the command to the command array. This is similar to how we modified the default command code. However, we can't assume that the array will be empty as in the default command code. Instead, our code needs to always add to the next slot in the array, not just slot zero. We can reuse the code from the default command code, and then use currentCommandArrayLength to tell us the next slot.

We also need to fix the places where we clear out the current command. This is trickier, since the command could be in the middle of the array. If we have a three-character array like SEF, and we try to remove the E command, we can't simply change the length to 2. That would produce SE, which isn't what we want. There also isn't a way to just tell the computer that we want to get rid of the item in the middle, so we need to do it ourself. The easiest way to do this is to copy F to E's slot, and then shorten the array. After copying, we get SFF, and after shortening the array, we get SF. This is exactly what we want, and is simple to code.

So, our command removal code now has three steps: use the array length to figure out the last slot in the array, set the current slot to the value in the last slot, and shorten the length of the array. The last slot in the array is simply the array length minus one. This is because the first slot starts at zero, so when your array has a length of one, the only slot that exists is slot zero. The rest is straight-forward.

### Displaying All Running Commands
At this point, it's a bit confusing to figure out which commands are running, so we'll take advantage of the fact that our current command array is also the same format as a character buffer. We can then give the command array and the array length directly to Serial.write(), which will then send that over the serial port. Adding a few Serial.print() and Serial.println() calls helps to make it clear exactly which commands are unning.

### Fixing the Too Many Commands Bug
Our command array is only big enough to store 10 commands, but there is currently no limit to how many commands you can add using the Serial command. This is a common bug called a "buffer overflow" and is the cause of billions of dollars worth of software problems. We can guard against this bug by making sure there is space in the command array before trying to add to the array. To fix this, we simply put our add code inside an if statement, and only run it if the array length is smaller than the maximum array length. If the array length is equal to the maximum, then we refuse to add another and reply that the array is full.

### Preventing Processing Delays
This bug is not a bug that will cause immediate problems, but it could cause unexpected results in the future. Normally, people write for loops to process from smallest to largest. When we remove a command from our command array, we take a command from the end, put it where the current command is, and then move to the next slot. Effectively, we've skipped over the last command! That last command won't run again until the next loop.

To clarify, here's an example. We have the commands SEF in the array, and the code processes command S, then E. Then, we remove E, put F in E's slot, and move on. But now we're at the end of the array, and we haven't processed F yet!

There are multiple ways to fix this, but a simple one is to simply process from the end of the array to the start. Looking back at our example with commands SEF, we process F first, then E. We remove E, put F in E's slot to get the commands SF, and then move on to process S. No commands are randomly skipped anymore.

### Refactor Code
In math, a refactored equation looks different but produces the same result. In coding, refactored code is the same, and we often do this to simplify our code. In our case, we have two spots that add commands, and three spots that remove commands, and they all duplicate code. This duplication makes our code hard to update and also makes it hard to read. So, we refactor our code.

First, we'll create a new function called addCommand() and place it above the loop() function. The add command code in the Serial command works in more circumstances and has more features than the code in the default command code, so we'll take that code and put it into our new addCommand() function. Then, any place we had code to add a command we simply use the addCommand() function.

Next, we'll create a new function called removeCommand() and place it above the loop() function. All of the remove command code is the same, so we simply take one example and put it in the removeCommand() function. Then, any place we had code to remove a command we just use the removeCommand() function instead.

As you can see, it is more clear what the code does. Any place you see addCommand() you know that a command is being added, and any place you see removeCommand() a command is being removed. Also, if you want to improve the way commands are added or removed, there is only one place that you need to update. Finally, the code works exactly the same way as before.

### Multiple Command Bug
We've created one additional bug: some commands will run when they shouldn't. For example, say you want the robot to move forward for 10 seconds instead of 5. It would be easy to just add two forward commands. However, both commands will start running at the same time! This will be solved by creating subsystems.

## Subsystems

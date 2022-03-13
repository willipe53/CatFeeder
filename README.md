# CatFeeder
 
A project that I originally did using Raspberry Pi, now ported to ESP32.  Hardware is still the same as before, with the exception of a flyback diode added across the electromagnet to keep the controller from going insane.  See FeederSchematic.pdf.

There are three servos connected in an arm that hold a box over dish with catfood. In the code, the servos are named Shoulder, Elbow and Wrist.  See CatFeeder.png.  Their relative positions are saved in non-volatile memory on the controller using the Preferences.h library.

Commands in the form of <char><value> can be sent to the device via AWS IoT or the serial port.  For example, l99 locks the electromagnet and l0 unlocks it. f99 will do a full arm open, including a jiggle of the arm to ring a bell that I have attached to summon the cat.  See CatFeeder.ino for a complete list of commands (or enter p99 to print all commands in the Serial Monitor).
 
There is also a button on the circuit board.  If the electromagnet is locked, the button will do a full arm open with jiggle.  Otherwise, the button will lock the electromagnet.  The normal usage is to load the cat dish, manually close the lid, press the button to lock the lid in place with the electromagnet, and then go to bed. 
 
As of this writing I still need to figure out how to schedule a lambda function in AWS to feed the cat every morning at 5AM.  Work in progress....

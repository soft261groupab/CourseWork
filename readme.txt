SOFT261 ReadMe
Part 2A
i. Potentiometer value is read by pin A0 of analogue to digital converter once every 0.1 seconds (10Hz).

ii. An array called bufferArray is filled with the values of AIN and once this is done the same number of times
	as the frequency, 1 second has passed and the average is calculated

iii. Both the average value using the circular buffer and the current sample are outputted using PuTTY.

iv. When the switch D4 is pressed, the program prompts the user to enter a value for the frequency that the program 
	uses to sample data. This is checked by an if-statement to make sure that it is between 5Hz and 100Hz. The
	terminal then prints data according to the new rate. The green LED flashes according to the sample rate and
	stops when the button is pressed, the red LED is then lit. strcmp() is used to avoid buffer overflows.

Part B

i. Applied Vectorization using _SMLAD() and __SADD16() intrinsics where possible

ii. Register Blocking applied Using Duffs Devices.	

iii. Strength Reduction Applied by eliminating un-needed oporations, as well as using more efficient methods where possible, such as using  __SADD16() instead of multiplication

iv. Atan2 function rewrite not attempted.

v.  loop merge not attempted.
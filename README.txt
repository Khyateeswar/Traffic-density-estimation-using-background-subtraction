######### ANALYSIS OF ERRORS AND RUNTIMES OF THE QUEUE DENSITY OF TRAFFIC VIDEO IN DIFFERENT METHODS #########

-- By MUKKU VAMSI KRISHNA REDDY 2019CS10374 & NALLA KHYATEESWAR NAIDU 2019CS10376---------

-------------------------------------------------------------------------------------------


## Files in this directory
	method1.cpp
	method2.cpp
	method3.cpp
	method4.cpp
	baseline.txt - contains the no of pixels differed in the baseline
	base.cpp - to print out the baseline valuse to baseline.txt
	requiredimage.jpg
	trafficvideo.mp4
	Makefile
-------------------------------------------------------------------------------------------

## some common terms and definitions for all methods 

	queoriginal-------which contains queue density values of baseline.
	quemodified-------which contains queue density values of the obtained frames in the methods.
	error------------- average of absolute sum of the differnce between the rate of quedensities in queoriginal and quemodified.
	utility-----------1/error.(for method 1 and 2)
	utility-----------10-error.(for method 3 and 4)

--------------------------------------------------------------------------------------------

########   METHOD-1   #########

>> Usage
	1.Go to the directory of the code folder and open a terminal there.
	2.Run command "make" to remove the executables.
	3.Run the command "make m1" for compiling the method1.cpp.
	4.Run the command "make runm1 A=x" where x is the variable i.e the no of frames to be skipped.
	5.We get the output "utility" and "runtime".

-----------------------------------------------------

>> Description

	x---- skip number.
	We have taken the values of the queoriginal by reading the text "baseline.txt"(which contains all quedensity values of baseline).
	we are reading the trafficvideo.mp4 and then taking the quedensity values of the frames b skipping x.(if we take Nth frame then we have taken N+x and not for between frames).
	After this we have to give utility.for that we have taken rates of quedensity of quemodified frames between N and N+x have same rate as of N+x.
	for runtime we have considered from the starting of video reading to the end of giving the utility.

---------------------------------------------------------------------------------------------

########   METHOD-2   ###########

>> Usage
	1.Go to the directory of the code folder and open a terminal there.
	2.Run command "make" to remove the executables.
	3.Run the command "make m2" for compiling the method2.cpp.
	4.Run the command "make runm1 A=x B=y" where x and y are variables i.e x&y is the screen size to which the frame is to be reduced.
	5.Give the values of x and y in certain ratio so that the image doesnt get reduced.
	5.We get the output "utility" and "runtime".

-------------------------------------------------------

>> Description

	X,Y-----image width and height respectively.
	We have taken the values of the queoriginal by reading the text "baseline.txt"(which contains all quedensity values of baseline).
	We are reading the traffic video and made the size of each frame to the x and y.
	And then calucalated utility with the given values of queoriginal and obtained values of the quemodified.
	for runtime we have considered from the starting of video reading to the end of giving the utility.

-----------------------------------------------------------------------------------------------

########   METHOD-3   ###########

>> Usage
	1.Go to the directory of the code folder and open a terminal there.
	2.Run command "make" to remove the executables.
	3.Run the command "make m3" for compiling the method3.cpp.
	4.Run the command "make runm3 A=x" where x is the variable i.e the no of threads to be used<100.
	5.We get the output "utility" and "runtime".
	6.open new terminal and run "htop";
	7.We use htop for the pid of the executable.
	8.Open new terminal and run "pidstat 40 -ru -p <pid>"
	9.From here we obtain average cpu utilisation for every 40 seconds.


--------------------------------------------------------

>> Description

	x---- number of threads.
	We have taken the values of the queoriginal by reading the text "baseline.txt"(which contains all quedensity values of baseline).
	we are reading the traffic video and whenever we obtain a frame we have splitted the work of calucalating queue density for "x" different threads (ex: 1st thread calucalates 0th row ,x th row,2x th row.....)
	Here as we are using the same frames and using same queue density rate formula for the queoriginal and quemodified.so error we will get zero
 	for runtime we have considered from the starting of video reading to the end of giving the utility.

--------------------------------------------------------------------------------------------------

########    METHOD-4   ###########

>> Usage
	1.Go to the directory of the code folder and open a terminal there.
	2.Run command "make" to remove the executables.
	3.Run the command "make m4" for compiling the method4.cpp.
	4.Run the command "make runm4 A=x" where x is the variable i.e the no of threads to be used<100.
	5.We get the output "utility" and "runtime".
	6.open new terminal and run "htop";
	7.We use htop for the pid of the executable.
	8.Open new terminal and run "pidstat 40 -ru -p <pid>"
	9.From here we obtain average cpu utilisation for every 40 seconds.

-----------------------------------------------------------

>> Description 

	x---- number of threads.
	We have taken the values of the queoriginal by reading the text "baseline.txt"(which contains all quedensity values of baseline).
	We are reading the traffic video and we are taking the "x" framesat a time for the calucalation of the quedensity at the same time in those "x" threads.
	Here as we are using the same frames and using same queue density rate formula for the queoriginal and quemodified.so error we will get zero
 	for runtime we have considered from the starting of video reading to the end of giving the utility.

----------------------------------------------------------------------------------------------------
## Installation
	installations from subtask1&2.
	Install "htop".
	Install "sysstat".
----------------------------------------------------------------------------------------------------

## References

	Code in the subtask 1&2.
	references from subtask 1&2.
	multithreading tutorialspoint.
	

______________________________________________________________________________________________________.
	
	

	
	 
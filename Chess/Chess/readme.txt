Usage:
The program needs a minimum of 8 and a maximum of 9 parameters to execute.
-h shows help and a sample usage. More example are provided in accompanying .bat files. Sample layout files are also provided.

The arguments are (in that order): Level[1-5] [Height] [Width] DisplayBoard[T/F] [Start_X] [Start_Y] [End_X] [End_Y] [Full path to layout file (optional)] 
Level[1-5]: value from 1-5 corresponding to each Level in the challenge
	Height: Height of checker board
	Width: Width of checker board
	DisplayBoard[T/F]: Flag to display the Knight movement after each step
	Start_X: X coordinate of Starting position
	Start_Y: Y coordinate of Starting position
	End_X: X coordinate of Ending position
	End_Y: Y coordinate of Ending position
	Full path to layout file (optional): optional argument to provide name of layout file. If the layout file contains S and E, they will be rejected and replaced with regular '.' points. Any delimiter can be used.
	
All levels support input layouts with W R B and T.
Level[1] is an interactive utility which shows your board layout and asks for location to move the knight. It then either moves the knight if valid position or returns an error.
Level[2] calculates shortest path naiively.
Level[3] calculates shortest path using A*.
Level[4] is exactly like Level[3].
Level[5] calculates longest path using recursion and a distance metric.
	
Expected behavior:
1. The height and width should be integers greater than 0. if not program will show an error and exit.
2. If DisplayBoard is True(T), the entire board layout is displayed with the position of the knight at every move. If set to False(F), only the final path is displayed. 
3. [S]tart and [E]nd co-ordinates are checked for validity. They should lie within the boundary of the board and not lie on [R]ock or [B]arrier cells. if not, the program will complain and exit.
4. If there is a problem in opening the layout file, a choice is provided to continue or exit. If continues, the entire grid is initialised to regular '.' points.
5. The program only reads 'height x width' characters from the file. If the file has fewer characters, extra '.' are appended.
6. If the layout only has one [T]eleport point instead of a pair, it is replaced with a regular '.' point.


Salient features:
1. A* was used for shortest path from S to E. A 1:1 weighting between Cell Cost (G) and Heuristic Distance (H) did not work. Since the knight covers a larger distance in 1 move than the cost of a move, the H cost was down weighted by 2.50. The G cost was used to break ties during sorting of nodes.
2. The longest path uses the same H measure but in an opposite way. It tries to move (or keep, depending on where you S) the knight away from the E in the initial part of the path finding. All moves are then filled till the E is reached. It tries to move the Knight to every possible cell atleast once, resulting in the longest path.
3. For large boards (32x32 or more), there is no way (or one that I could think of), to ascertain if the longest path is reached. I have used a "watchdog" counter of sorts. The recursive algorithm keeps track of the longest path and if we don't see a longer path for more than 'height x width' iterations, we bail out having found some "longest" path.

Cool features:
1. Starting or ending at a T square is supported. if both S and E are Teleport pairs, then the program handles it with a message.
2. Multiple [T]eleport locations are supported. Every pair of consecutive Ts becomes a teleport location. See 'Expected Behavior #6'.

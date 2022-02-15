## Resource Manager
### Test tips
Please read the instruction inside each folder.
***
### This is an exercise with the requirement down below:

Create a Resource Manager for a Real Time Strategy (RTS) game. Your program manages a "directed graph" (a system of nodes and links between them going in one direction, much like web pages and their links). On startup, your program reads a file "resource.txt" (from the current directory) which describes resources and the resources they depend on. The file resource.txt could, for example, contain (exactly 4 lines):

handgun bullets  
bullets ore  
bombs ore  
turret bullets

The first line says that there is a link from a node called "handgun" to a node called 'bullet". This means for a handgun to be a useable, it relies on the resource bullets.

#### Requirements:

-   Your program should work with any amount of nodes and any amount of links between them (your program may be tested with a larger resource.txt made by SMU Guildhall faculty). To this end, you should represent nodes with a Node class, and the set of links for a single node using an STL container of your choice.
    
-   Your program should loop and display two items on the screen: (1) a current view of the graph, and (2) a list of each node and whether it is usable or not. For any current node, if any of the nodes it relies on get deleted that node becomes unusable.
    
-   Your program should handle two types of input. A user should be allowed to delete a node and quit at any time. If using a console program, input should be "q" for quit, or the name of a node to delete it. When you delete a node, make sure any links to it are deleted correctly too. Be certain to have correct memory management for these operations ensuring no memory leaks when the program exits.
    

#### Extra Credit:  
Add additional commands to insert new nodes and new links. Save the modified graph upon exiting. Create a nice graphical display. Take it as far as you can. 
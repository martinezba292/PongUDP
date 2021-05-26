This project is a clone of the Atari Pong game with online features. 
It uses SDL2 for graphics and winsock for server-client communication.
For the game to begin, a server instance and 2 clients must be running.

To execute the game, follow this instructions:

- You will need to open the Visual Studio solution and compile the 2 projects(server and client)

- If you're running server and clients from the same computer, you'll need to assign the number 
of the local IP address(127.0.0.1) in the "common_def.h" header, include folder. 
This is the IP assigned by default.

- If you're running them from different computers, 
you will need to set the IP of the server manually in "common_def" and compile.

- The game will start when 2 clients are connected to the server. 
A window will open for each client and the ball will start moving after 3 seconds.

- The game will end when one of the two player closes the window or press escape.
A message will show in console with the player number that finished the game.
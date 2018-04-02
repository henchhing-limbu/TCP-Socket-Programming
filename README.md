# Socket_Programming #
Client-Server architecture
Developed two network programs, one for client and one for server
* The client program sends a file containing units as defied in the practice project to the server. 
* The server translates the units to the format specified by the client, and saves the units to a 
file of the name specified by the cliet.
* Before translating the units, the server checks if received units have a correct format.
* If not, the server sends back an error message immediately without saving the file.
* If yes, the server sends back a confirmation message after saving the file. 
* The client prints the result, "Format error" or "Success" after getting the message from the server.

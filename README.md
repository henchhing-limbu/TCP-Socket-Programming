# TCP SOCKET PROGRAMMING #
## Developed two network programs, one for client and one for server  
* The client sends a file containing units as defined in the practice project
to the server. 
* The server translates the units to the format specified by the client, and
saves the units to a file name specified by the cliet.
* Before translating the units, the server checks if received units have a
correct format.
* If not, the server sends back an error message immediately without saving the
file.
* If yes, the server sends back a confirmation message after saving the file. 
* The client prints the result, "Format error" or "Success" after getting the
message from the server.

## APPLICATION PROTOCOL
### Architecuture
Client-server architecture is used for the network application. Server is given
an IP address which does not change throughout the session. They need to
establish connection before they can communicate. Client requests services from
the server and server responds to it.
#### Types of Messages
* Request
    After a connection is established between the client and the server, client
    sends a request to the server.

* Response
    To answer to the request of a client, the server either sends client data
    or do the data translation job.

#### Syntax and Semantics of message type
##### Request
* Format of the message
    [file size] [data] [file format] [output filename size] [output filename]
    [file size]: size of long
    [data]: 1000 bytes of ASCII characters
    [file format]: size of int
    [output filename size]: size of int
    [output filename]: (filename size) bytes of ASCII characters 

* Meaning of each field in the format
    [file size]: size of the file
    [data]: data of the file stored in buffer
    [file format]: format number that specifies the different types of
                   translation
    [output filename size]: size of the output file name
    [output filename]: name of the output file where the translated data are
                       written

* The value range of each field
    Size of primitive data types may differ in different platform
    [file size]: 0 to 4294967295
    [data]: file size
    [file format]: -32768 to 32767 or -2147483648 to 2147483647
    [output filename size]: -32768 to 32767 or -2147483648 or 2147483647
    [output filename]: output filename size

##### Response
* Format of the message
    file size] [ error message]
    file size]: size of long
    error message]: size of int

* Meaning of each field in the format
    file size]: size of the file received by the client
    error message]: integer that tells whether the format of the units in the
    file was ok or not

* The value range of each field
    [file size]: 0 to 4294967295
    [error message]: 0 or -1

##### Rules of sending messages
➢  Client always sends the request to the server
➢  Server responds to the request of the server
➢  There is always one response for one request.
➢  Messages must follow the format and syntax mentioned above.
➢  If not followed, the program will throw an error.

### Test Cases
| Input File | Expected Output | Actual Ouput | Rationale | Content | Errors |
| :------ | :------ | :----- | :----- | :------ | :----- |
| practice_projcet_test_file_1 | Success | Success | Normal functionality | Data from test file 1 from practice projcet
| No Error |

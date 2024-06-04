// TODO: You should revise the following code
int NetworkManager::sendData(uint8_t *data, int dlen)
{
    int sock, tbs, sent, offset, num, jlen; // Declare variables for socket, bytes to send, bytes sent, offset, number, and length
    unsigned char opcode;                   // Declare variable for opcode
    uint8_t n[4];                           // Declare a 4-byte array
    uint8_t *p;                             // Declare a pointer for data

    sock = this->sock; // Get the socket descriptor from the object
    // Example) data (processed by ProcessManager) consists of:
    // Example) minimum temperature (1 byte) || minimum humidity (1 byte) || minimum power (2 bytes) || month (1 byte)
    // Example) edge -> server: opcode (OPCODE_DATA, 1 byte)
    opcode = OPCODE_DATA; // Set opcode to OPCODE_DATA
    tbs = 1;
    offset = 0;          // Set total bytes to send to 1 and offset to 0
    while (offset < tbs) // Loop until all bytes are sent
    {
        sent = write(sock, &opcode + offset, tbs - offset); // Write opcode to socket
        if (sent > 0)                                       // If bytes were sent successfully
            offset += sent;                                 // Update the offset
    }
    assert(offset == tbs); // Ensure all bytes were sent

    // Example) edge -> server: temperature (1 byte) || humidity (1 byte) || power (2 bytes) || month (1 byte)
    tbs = 5;
    offset = 0;          // Set total bytes to send to 5 and offset to 0
    while (offset < tbs) // Loop until all bytes are sent
    {
        sent = write(sock, data + offset, tbs - offset); // Write data to socket
        if (sent > 0)                                    // If bytes were sent successfully
            offset += sent;                              // Update the offset
    }
    assert(offset == tbs); // Ensure all bytes were sent

    return 0; // Return 0 to indicate success
}

uint8_t NetworkManager::receiveCommand()
{
  // We initialize the following variables:
  int sock;       // sock: An integer representing the socket file descriptor.
  uint8_t opcode; // opcode: An 8-bit unsigned integer for storing the opcode.
  uint8_t *p;     // *p: A pointer to an 8-bit unsigned integer


  sock = this->sock;    // sock is assigned the value of this->sock, which is initialized in the init() function as "this->sock = socket(PF_INET, SOCK_STREAM, 0);"
                        // this->sock = socket(PF_INET, SOCK_STREAM, 0); creates a new socket with IPv4(PF_INET) and TCP(SOCK_STREAM) protocols.
  opcode = OPCODE_WAIT; // opcode is initialized to OPCODE_WAIT, which is defined in opcode.h as 2.

  while (opcode == OPCODE_WAIT)
    read(sock, &opcode, 1); // This loop continues to read one byte from the socket into opcode as long as opcode equals OPCODE_WAIT

  assert(opcode == OPCODE_DONE || opcode == OPCODE_QUIT); // After exiting the loop, it asserts that opcode must be either OPCODE_DONE or OPCODE_QUIT. If opcode is neither, the program will terminate with an assertion failure.

  return opcode; // The function returns the opcode read from the socket.
}

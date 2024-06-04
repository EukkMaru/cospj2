void protocol_execution(int sock)
{
    // Define a message containing the name "Alice"
    char msg[] = "Alice";

    // Buffer to hold received data
    char buf[BUFLEN];

    // Variables for tracking bytes to send/receive, bytes sent/received, and offset within the message
    int tbs, sent, tbr, rcvd, offset;

    // Variable to hold the length of the message
    int len;

    // Calculate the length of the name message
    len = strlen(msg);
    printf("[*] Length information to be sent: %d\n", len);

    // Convert the length to network byte order (big-endian)
    len = htonl(len);

    // Set the number of bytes to send for the length information
    tbs = 4;
    offset = 0;

    // Loop to send the length information (4 bytes) in case not all bytes are sent in one go
    while (offset < tbs)
    {
        sent = write(sock, &len + offset, tbs - offset); // Send part of the length information
        if (sent > 0)
            offset += sent; // Update offset by the number of bytes sent
    }

    // Convert the length back to host byte order (little-endian)
    tbs = ntohl(len);
    offset = 0;

    // Print the name to be sent
    printf("[*] Name to be sent: %s\n", msg);

    // Loop to send the name message in case not all bytes are sent in one go
    while (offset < tbs)
    {
        sent = write(sock, msg + offset, tbs - offset); // Send part of the name message
        if (sent > 0)
            offset += sent; // Update offset by the number of bytes sent
    }

    // Set the number of bytes to receive for the length information
    tbr = 4;
    offset = 0;

    // Loop to receive the length information (4 bytes) in case not all bytes are received in one go
    while (offset < tbr)
    {
        rcvd = read(sock, &len + offset, tbr - offset); // Receive part of the length information
        if (rcvd > 0)
            offset += rcvd; // Update offset by the number of bytes received
    }

    // Convert the received length from network byte order to host byte order
    len = ntohl(len);
    printf("[*] Length received: %d\n", len);

    // Set the number of bytes to receive for the name message
    tbr = len;
    offset = 0;

    // Loop to receive the name message in case not all bytes are received in one go
    while (offset < tbr)
    {
        rcvd = read(sock, buf + offset, tbr - offset); // Receive part of the name message
        if (rcvd > 0)
            offset += rcvd; // Update offset by the number of bytes received
    }

    // Print the received name
    printf("[*] Name received: %s \n", buf);

    // Implement following the instructions below
    // Let's assume there are two opcodes:
    //     1: summation request for the two arguments
    //     2: reply with the result
    // 3. Alice -> Bob: opcode (4 bytes) || arg1 (4 bytes) || arg2 (4 bytes)
    // The opcode should be 1

    // Define the opcode for summation request
    int opcode = htonl(1);
    int arg1 = htonl(10); // First argument (example value)
    int arg2 = htonl(20); // Second argument (example value)

    // Send the opcode
    tbs = 4;
    offset = 0;
    while (offset < tbs)
    {
        sent = write(sock, &opcode + offset, tbs - offset); // Send part of the opcode
        if (sent > 0)
            offset += sent; // Update offset by the number of bytes sent
    }

    // Send the first argument
    offset = 0;
    while (offset < tbs)
    {
        sent = write(sock, &arg1 + offset, tbs - offset); // Send part of the first argument
        if (sent > 0)
            offset += sent; // Update offset by the number of bytes sent
    }

    // Send the second argument
    offset = 0;
    while (offset < tbs)
    {
        sent = write(sock, &arg2 + offset, tbs - offset); // Send part of the second argument
        if (sent > 0)
            offset += sent; // Update offset by the number of bytes sent
    }

    // 4. Bob -> Alice: opcode (4 bytes) || result (4 bytes)
    // The opcode should be 2

    // Receive the opcode
    tbr = 4;
    offset = 0;
    while (offset < tbr)
    {
        rcvd = read(sock, &opcode + offset, tbr - offset); // Receive part of the opcode
        if (rcvd > 0)
            offset += rcvd; // Update offset by the number of bytes received
    }

    // Convert the received opcode from network byte order to host byte order
    opcode = ntohl(opcode);
    if (opcode != 2)
    {
        printf("Unexpected opcode received: %d\n", opcode);
        return;
    }

    // Receive the result
    offset = 0;
    while (offset < tbr)
    {
        rcvd = read(sock, &len + offset, tbr - offset); // Receive part of the result
        if (rcvd > 0)
            offset += rcvd; // Update offset by the number of bytes received
    }

    // Convert the received result from network byte order to host byte order
    int result = ntohl(len);
    printf("[*] Summation result received: %d\n", result);
}

void error_handling(const char *message)
{
    fputs(message, stderr); // Print the error message to stderr
    fputc('\n', stderr);    // Print a newline character to stderr
    exit(1);                // Exit the program with a status of 1 (indicating an error)
}

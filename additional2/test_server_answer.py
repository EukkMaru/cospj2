def protocol_execution(sock):
    # 1. Alice -> Bob: length of the name (4 bytes) || name (length bytes)
    # Get the length information (4 bytes)
    buf = sock.recv(4)  # Receive 4 bytes from the socket which represent the length of the name
    length = int.from_bytes(buf, "big")  # Convert the 4 bytes from big-endian to an integer
    logging.info("[*] Length received: {}".format(length))  # Log the received length

    # Get the name (Alice)
    buf = sock.recv(length)  # Receive 'length' bytes from the socket which represent the name
    logging.info("[*] Name received: {}".format(buf.decode()))  # Log the received name

    # 2. Bob -> Alice: length of the name (4 bytes) || name (length bytes)
    # Send the length information (4 bytes)
    name = "Bob"  # Define the name to be sent
    length = len(name)  # Calculate the length of the name
    logging.info("[*] Length to be sent: {}".format(length))  # Log the length to be sent
    sock.send(int.to_bytes(length, 4, "big"))  # Send the length as 4 bytes in big-endian order

    # Send the name (Bob)
    logging.info("[*] Name to be sent: {}".format(name))  # Log the name to be sent
    sock.send(name.encode())  # Send the name as bytes

    # Implement following the instructions below
    # 3. Alice -> Bob: opcode (4 bytes) || arg1 (4 bytes) || arg2 (4 bytes)
    # The opcode should be 1
    buf = sock.recv(12)  # Receive 12 bytes from the socket which represent opcode, arg1, and arg2

    # The values are encoded in the big-endian style and should be translated into the little-endian style (because my machine follows the little-endian style)
    opcode = int.from_bytes(buf[0:4], "little")  # Convert the first 4 bytes from little-endian to an integer (opcode)
    arg1 = int.from_bytes(buf[4:8], "little")  # Convert the next 4 bytes from little-endian to an integer (arg1)
    arg2 = int.from_bytes(buf[8:12], "little")  # Convert the last 4 bytes from little-endian to an integer (arg2)

    logging.info("[*] Opcode: {}".format(opcode))  # Log the received opcode
    logging.info("[*] Arg1: {}".format(arg1))  # Log the received arg1
    logging.info("[*] Arg2: {}".format(arg2))  # Log the received arg2

    # 4. Bob -> Alice: opcode (4 bytes) || result (4 bytes)
    # The opcode should be 2
    result = arg1 + arg2  # Calculate the result by summing arg1 and arg2
    logging.info("[*] Result: {}".format(result))  # Log the calculated result
    opcode = 2  # Set the opcode for the reply
    sock.send(int.to_bytes(OPCODE_REPLY, 4, "big"))  # Send the reply opcode as 4 bytes in big-endian order
    sock.send(int.to_bytes(result, 4, "big"))  # Send the result as 4 bytes in big-endian order

    sock.close()  # Close the socket connection

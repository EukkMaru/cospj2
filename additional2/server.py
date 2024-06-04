import socket
import requests
import threading
import argparse
import logging
import json
import sys

OPCODE_DATA = 1
OPCODE_WAIT = 2
OPCODE_DONE = 3
OPCODE_QUIT = 4

class Server:
    def __init__(self, name, algorithm, dimension, index, port, caddr, cport, ntrain, ntest):
        logging.info("[*] Initializing the server module to receive data from the edge device")
        self.name = name
        self.algorithm = algorithm
        self.dimension = dimension
        self.index = index
        self.caddr = caddr
        self.cport = cport
        self.ntrain = ntrain
        self.ntest = ntest
        success = self.connecter()

        if success:
            self.port = port
            self.socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            self.socket.bind(("0.0.0.0", port))
            self.socket.listen(10)
            self.listener()

    def connecter(self):
        success = True # Initializes the variable "success" to True.
        self.ai = socket.socket(socket.AF_INET, socket.SOCK_STREAM) #A TCP/IP socket is created using the AF_INET address family and SOCK_STREAM socket type.
        self.ai.connect((self.caddr, self.cport)) # The socket is connected to the address and port specified by self.caddr and self.cport
        url = "http://{}:{}/{}".format(self.caddr, self.cport, self.name)  # The URL for the AI module is constructed.
        request = {} # Initializes an empty dictionary named "request".
        request['algorithm'] = self.algorithm # The algorithm parameter is set in the dictionary request.
        request['dimension'] = self.dimension # The dimension parameter is set in the dictionary request.
        request['index'] = self.index # The index parameter is set in the dictionary request.
        js = json.dumps(request) # The dictionary is converted to a JSON string "js" using json.dumps.
        logging.debug("[*] To be sent to the AI module: {}".format(js)) # The JSON string is logged for debugging purposes.
        result = requests.post(url, json=js)  # An HTTP POST request is sent to the AI module with the JSON string as the payload.
        response = json.loads(result.content) # The response is received and parsed from JSON.
        logging.debug("[*] Received: {}".format(response)) # The response is logged for debugging purposes.
        
        if "opcode" not in response:
            logging.debug("[*] Invalid response")
            success = False
        else:
            if response["opcode"] == "failure":
                logging.error("Error happened")
                if "reason" in response:
                    logging.error("Reason: {}".format(response["reason"]))
                    logging.error("Please try again.")
                else:
                    logging.error("Reason: unknown. not specified")
                success = False
            else:
                assert response["opcode"] == "success"
                logging.info("[*] Successfully connected to the AI module")
        return success
        # The response is checked for the presence of an "opcode" key.
        # If "opcode" is not present, the response is considered invalid, and the connection is marked as unsuccessful.
        # If "opcode" is "failure", an error message is logged along with the reason.
        # If "opcode" is "success", a success message is logged.
        # The method returns success, indicating whether the connection and communication were successful.

    def listener(self):
        logging.info("[*] Server is listening on 0.0.0.0:{}".format(self.port)) # Logs "Server is listening on 0.0.0.0:(port)", 0.0.0.0 represents all available interfaces.

        while True: # Loops indefinitely to accept incoming connections.
            client, info = self.socket.accept()
            logging.info("[*] Server accept the connection from {}:{}".format(info[0], info[1]))
            # When a connection is accepted, it logs the client's IP address and port
            
            client_handle = threading.Thread(target=self.handler, args=(client,))
            client_handle.start()
            # For each accepted connection, it creates a new thread to handle the connection using the handler method.
            # Starts the thread to handle the connection concurrently.

    def send_instance(self, vlst, is_training):
        if is_training:
            url = "http://{}:{}/{}/training".format(self.caddr, self.cport, self.name)
        else:
            url = "http://{}:{}/{}/testing".format(self.caddr, self.cport, self.name)
        data = {}
        data["value"] = vlst
        req = json.dumps(data)
        response = requests.put(url, json=req)
        resp = response.json()

        if "opcode" in resp:
            if resp["opcode"] == "failure":
                logging.error("fail to send the instance to the ai module")

                if "reason" in resp:
                    logging.error(resp["reason"])
                else:
                    logging.error("unknown error")
                sys.exit(1)
        else:
            logging.error("unknown response")
            sys.exit(1)

    def parse_data(self, buf, is_training):
        temp = int.from_bytes(buf[0:1], byteorder="big", signed=True)
        humid = int.from_bytes(buf[1:2], byteorder="big", signed=True)
        power = int.from_bytes(buf[2:4], byteorder="big", signed=True)
        month = int.from_bytes(buf[4:5], byteorder="big", signed=True)

        lst = [temp, humid, power, month]
        logging.info("[temp, humid, power, month] = {}".format(lst))

        self.send_instance(lst, is_training)


    # TODO: You should implement your own protocol in this function
    # The following implementation is just a simple example
    def handler(self, client):
        logging.info("[*] Server starts to process the client's request")
        # Defines a method named handler that takes self and client as parameters. This method handles client requests.
        # Logs an informational message indicating that the server has started processing the client's request.

        ntrain = self.ntrain
        url = "http://{}:{}/{}/training".format(self.caddr, self.cport, self.name)
        # Assigns the value of self.ntrain to the variable ntrain, representing the number of training iterations.
        # Constructs the URL for sending training data to the AI module using the AI module's IP address, port, and model name.
        while True:
            # opcode (1 byte): 
            rbuf = client.recv(1) # Receives 1 byte of data from the client, which is expected to be an opcode.
            opcode = int.from_bytes(rbuf, "big") # Converts the received byte to an integer using big-endian byte order.
            logging.debug("[*] opcode: {}".format(opcode)) # Logs the received opcode for debugging purposes.
            # Receives 1 byte of data from the client socket, representing the opcode, and stores it in rbuf. Converts the received bytes to an integer using big-endian byte order and assigns it to the variable opcode. Logs the received opcode for debugging purposes.

            if opcode == OPCODE_DATA:
                logging.info("[*] data report from the edge") # Logs an informational message indicating that data is being reported from the client (edge device).
                rbuf = client.recv(5) # Receives an additional 5 bytes of data from the client.
                logging.debug("[*] received buf: {}".format(rbuf)) # Logs the received buffer for debugging purposes.
                self.parse_data(rbuf, True) # Calls the parse_data method with the received buffer and a flag set to True to indicate that this is training data.
                # The parse_data method processes the received data, extracts relevant values, and sends them to the AI module for training.
                # Logs an informational message indicating receipt of data from the client. Receives a buffer of size 5 bytes from the client and logs its content. Calls the parse_data method with the received buffer and True flag to indicate training data processing.
            else:
                logging.error("[*] invalid opcode")
                logging.error("[*] please try again")
                sys.exit(1)
                # If the received opcode is not OPCODE_DATA, logs an error message indicating an invalid opcode.
                # Logs a message asking to try again and exits the program with an error status.

            ntrain -= 1 # Decrements the training counter ntrain by one.

            if ntrain > 0:
                opcode = OPCODE_DONE # If there are still training iterations left, sets the opcode to OPCODE_DONE.
                logging.debug("[*] send the opcode OPCODE_DONE") # Logs the action of sending the OPCODE_DONE opcode.
                client.send(int.to_bytes(opcode, 1, "big")) # Sends the OPCODE_DONE opcode back to the client as a 1-byte integer in big-endian format.
            else:
                opcode = OPCODE_WAIT # If there are no training iterations left, sets the opcode to OPCODE_WAIT.
                logging.debug("[*] send the opcode OPCODE_WAIT") # Logs the action of sending the OPCODE_WAIT opcode.
                client.send(int.to_bytes(opcode, 1, "big")) # Sends the OPCODE_WAIT opcode back to the client as a 1-byte integer in big-endian format.
                break
            # Sends the appropriate opcode to the client based on whether more training data is needed.

        result = requests.post(url) # Sends a POST request to the AI module at the training URL.
        response = json.loads(result.content) # Parses the JSON response from the AI module.
        logging.debug("[*] return: {}".format(response["opcode"])) # Logs the opcode received in the response for debugging purposes.
    
        ntest = self.ntest # Sets ntest to the number of testing iterations specified in the instance variable self.ntest.
        url = "http://{}:{}/{}/testing".format(self.caddr, self.cport, self.name) #Constructs a URL for the testing endpoint of the AI module.
        opcode = OPCODE_DONE # Sets the opcode to OPCODE_DONE to indicate the end of the training phase.
        logging.debug("[*] send the opcode OPCODE_DONE") # Logs the action of sending the OPCODE_DONE opcode.
        client.send(int.to_bytes(opcode, 1, "big")) # Sends the OPCODE_DONE opcode to the client as a 1-byte integer in big-endian format.

        while ntest > 0:
            # opcode (1 byte): 
            rbuf = client.recv(1) # Receives 1 byte of data from the client, which is expected to be an opcode.
            opcode = int.from_bytes(rbuf, "big") # Converts the received byte to an integer using big-endian byte order.
            logging.debug("[*] opcode: {}".format(opcode)) # Logs the received opcode for debugging purposes.

            if opcode == OPCODE_DATA:
                logging.info("[*] data report from the edge") # Logs an informational message indicating that data is being reported from the client.
                rbuf = client.recv(5) # Receives an additional 5 bytes of data from the client.
                logging.debug("[*] received buf: {}".format(rbuf)) # Logs the received buffer for debugging purposes.
                self.parse_data(rbuf, False) # Calls the parse_data method with the received buffer and a flag set to False to indicate that this is testing data.

            else:
                logging.error("[*] invalid opcode") #If the received opcode is not OPCODE_DATA, logs an error message indicating an invalid opcode.
                logging.error("[*] please try again") # Logs a message asking to try again and exits the program with an error status
                sys.exit(1) 

            ntest -= 1 # Decrements the testing counter ntest by one.

            if ntest > 0:
                opcode = OPCODE_DONE # If there are still testing iterations left, sets the opcode to OPCODE_DONE
                client.send(int.to_bytes(opcode, 1, "big")) # Sends the OPCODE_DONE opcode back to the client as a 1-byte integer in big-endian format.
            else:
                opcode = OPCODE_QUIT # If there are no testing iterations left, sets the opcode to OPCODE_QUIT.
                client.send(int.to_bytes(opcode, 1, "big")) # Sends the OPCODE_QUIT opcode back to the client as a 1-byte integer in big-endian format.
                break

        url = "http://{}:{}/{}/result".format(self.caddr, self.cport, self.name) # Constructs a URL for the result endpoint of the AI module.
        result = requests.get(url) # Sends a GET request to the AI module at the result URL.
        response = json.loads(result.content) # Parses the JSON response from the AI module.
        logging.debug("response: {}".format(response)) # Logs the response for debugging purposes.
        if "opcode" not in response:
            logging.error("invalid response from the AI module: no opcode is specified")
            logging.error("please try again")
            sys.exit(1)
            # Checks if the response contains an "opcode" key. If not, logs an error message indicating an invalid response and exits the program with an error status.
        else:
            if response["opcode"] == "failure":
                logging.error("getting the result from the AI module failed")
                if "reason" in response:
                    logging.error(response["reason"])
                logging.error("please try again")
                sys.exit(1)
                # If the "opcode" is "failure", logs an error message indicating that getting the result from the AI module failed. If the response contains a "reason" key, logs the reason. Exits the program with an error status.
            elif response["opcode"] == "success":
                self.print_result(response)
                # If the "opcode" is "success", calls the print_result method with the response to print the result.
            else:
                logging.error("unknown error")
                logging.error("please try again")
                sys.exit(1)
                # If the "opcode" is neither "failure" nor "success", logs an error message indicating an unknown error and exits the program with an error status.

    def print_result(self, result):
        logging.info("=== Result of Prediction ({}) ===".format(self.name))
        logging.info("   # of instances: {}".format(result["num"]))
        logging.debug("   sequence: {}".format(result["sequence"]))
        logging.debug("   prediction: {}".format(result["prediction"]))
        logging.info("   correct predictions: {}".format(result["correct"]))
        logging.info("   incorrect predictions: {}".format(result["incorrect"]))
        logging.info("   accuracy: {}\%".format(result["accuracy"]))

def command_line_args():
    parser = argparse.ArgumentParser()
    parser.add_argument("-a", "--algorithm", metavar="<AI algorithm to be used>", help="AI algorithm to be used", type=str, required=True)
    parser.add_argument("-d", "--dimension", metavar="<Dimension of each instance>", help="Dimension of each instance", type=int, default=1)
    parser.add_argument("-b", "--caddr", metavar="<AI module's IP address>", help="AI module's IP address", type=str, required=True)
    parser.add_argument("-c", "--cport", metavar="<AI module's listening port>", help="AI module's listening port", type=int, required=True)
    parser.add_argument("-p", "--lport", metavar="<server's listening port>", help="Server's listening port", type=int, required=True)
    parser.add_argument("-n", "--name", metavar="<model name>", help="Name of the model", type=str, default="model")
    parser.add_argument("-x", "--ntrain", metavar="<number of instances for training>", help="Number of instances for training", type=int, default=10)
    parser.add_argument("-y", "--ntest", metavar="<number of instances for testing>", help="Number of instances for testing", type=int, default=10)
    parser.add_argument("-z", "--index", metavar="<the index number for the power value>", help="Index number for the power value", type=int, default=0)
    parser.add_argument("-l", "--log", metavar="<log level (DEBUG/INFO/WARNING/ERROR/CRITICAL)>", help="Log level (DEBUG/INFO/WARNING/ERROR/CRITICAL)", type=str, default="INFO")
    args = parser.parse_args()
    return args

def main():
    args = command_line_args()
    logging.basicConfig(level=args.log)

    if args.ntrain <= 0 or args.ntest <= 0:
        logging.error("Number of instances for training or testing should be larger than 0")
        sys.exit(1)

    Server(args.name, args.algorithm, args.dimension, args.index, args.lport, args.caddr, args.cport, args.ntrain, args.ntest)

if __name__ == "__main__":
    main()

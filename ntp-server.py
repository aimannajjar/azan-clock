import socket
import struct
import time

# Define the server host and port
HOST = '0.0.0.0'   # Listen on all interfaces
PORT = 123         # Standard NTP port

# NTP epoch (January 1, 1900)
NTP_EPOCH = 2208988800  # Seconds between 1900 and 1970

def get_current_time():
    """Get the current time in seconds since the NTP epoch."""
    return time.time() + NTP_EPOCH

def run_ntp_server():
    """Run a simple NTP server."""
    with socket.socket(socket.AF_INET, socket.SOCK_DGRAM) as sock:
        sock.bind((HOST, PORT))
        print(f"NTP server listening on {HOST}:{PORT}")

        while True:
            # Receive data from a client
            data, address = sock.recvfrom(1024)  # Buffer size of 1024 bytes
            print(f"Received request from {address}")

            if len(data) < 48:
                print("Invalid NTP request")
                continue

            # Prepare the response (simple NTP format)
            transmit_time = get_current_time()
            response = b'\x1c' + (b'\0' * 39) + struct.pack('!d', transmit_time)

            # Send the response back to the client
            sock.sendto(response, address)
            print(f"Sent response to {address}")

if __name__ == "__main__":
    try:
        run_ntp_server()
    except KeyboardInterrupt:
        print("\nNTP server stopped.")
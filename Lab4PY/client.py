import socket
import pickle
from rsa import generateKeys, encryptMessage, randomString, measureEncryptTime, \
    decryptMessage  # Import RSA functions from the rsa_module
import lorem

# Define server host and port
HOST = '127.0.0.1'
PORT = 65432

def main():
    publicKey, privateKey = (7, 3233), (1783, 3233)
    n = publicKey[1]

    message = input("Enter message to authenticate: ")

    measureEncryptTime(message, publicKey, n)

    print (f"Message to send: {message}")

    # Encrypt the message
    encrypted_message = encryptMessage(message, publicKey[0], n)

    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.connect((HOST, PORT))
        # Send the encrypted message
        s.sendall(pickle.dumps(encrypted_message))

        # Receive response from server
        data = s.recv(1024)
        print(f"Received from server: {data.decode()}")
        data = s.recv(4096)
        encrypted_message = pickle.loads(data)
        decrypted_message = decryptMessage(encrypted_message, privateKey[0], n)
        print(f"Decrypted message from server: {decrypted_message}")

if __name__ == "__main__":
    main()

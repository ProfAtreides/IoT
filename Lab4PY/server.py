import socket
import threading
import pickle
from rsa import generateKeys, decryptMessage, randomString, encryptMessage

HOST = '127.0.0.1'
PORT = 65432

publicKey, privateKey = generateKeys(61, 53)
n = publicKey[1]


def handle_client(conn, addr, message):
    print(f"Connected by {addr}")
    try:
        data = conn.recv(4096)
        encrypted_message = pickle.loads(data)
        decrypted_message = decryptMessage(encrypted_message, privateKey[0], n)
        print(f"Decrypted message from client: {decrypted_message}")

        if decrypted_message != message:
            response = "Authentication failed"
            conn.sendall(response.encode())
            return

        response = "Authentication successful"
        msg_to_send = encryptMessage('Secret message', publicKey[0], n)
        conn.send(response.encode())
        conn.send(pickle.dumps(msg_to_send))
    finally:
        conn.close()


def main():
    print(f"Public key: {publicKey}")
    print(f"Private key: {privateKey}")
    message = randomString(1024)
    print (f"Message to authenticate: {message}")
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.bind((HOST, PORT))
        s.listen()
        print(f"Server listening on {HOST}:{PORT}")

        while True:
            conn, addr = s.accept()
            threading.Thread(target=handle_client, args=(conn, addr, message)).start()


if __name__ == "__main__":
    main()

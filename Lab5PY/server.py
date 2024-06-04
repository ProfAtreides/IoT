from cryptography.hazmat.primitives.ciphers import Cipher, algorithms, modes
from cryptography.hazmat.backends import default_backend
from cryptography.hazmat.primitives import padding
import random
import hashlib
import time


def generate_prime():
    while True:
        num = random.getrandbits(16)
        if is_prime(num):
            return num


def is_prime(n, k=5):
    if n <= 1:
        return False
    if n <= 3:
        return True
    for _ in range(k):
        a = random.randint(2, n - 1)
        if pow(a, n - 1, n) != 1:
            return False
    return True


def generate_keys():
    prime = generate_prime()
    primitive_root = random.randint(2, prime - 2)
    private_key = random.randint(2, prime - 1)
    public_key = pow(primitive_root, private_key, prime)
    return prime, primitive_root, private_key, public_key


def generate_shared_secret(private_key, other_public_key, prime):
    return pow(other_public_key, private_key, prime)


def pad_message(message):
    padder = padding.PKCS7(64).padder()
    padded_data = padder.update(message) + padder.finalize()
    return padded_data


def unpad_message(padded_message):
    unpadder = padding.PKCS7(64).unpadder()
    data = unpadder.update(padded_message)
    return data + unpadder.finalize()


def encrypt_message(message, key):
    iv = bytes([0] * 8)
    cipher = Cipher(algorithms.TripleDES(key), modes.CFB(iv), backend=default_backend())
    encryptor = cipher.encryptor()
    padded_message = pad_message(message)
    return encryptor.update(padded_message) + encryptor.finalize()


def decrypt_message(encrypted_message, key):
    iv = bytes([0] * 8)
    cipher = Cipher(algorithms.TripleDES(key), modes.CFB(iv), backend=default_backend())
    decryptor = cipher.decryptor()
    padded_message = decryptor.update(encrypted_message) + decryptor.finalize()
    return unpad_message(padded_message)


prime, primitive_root, private_key_a, public_key_a = generate_keys()
_, _, private_key_b, public_key_b = generate_keys()

shared_secret_a = generate_shared_secret(private_key_a, public_key_b, prime)
shared_secret_b = generate_shared_secret(private_key_b, public_key_a, prime)

shared_key = hashlib.sha256(str(shared_secret_a).encode()).digest()[:24]
message = b"Secret message to be encrypted with DES"

start_time = time.time()
encrypted_message = encrypt_message(message, shared_key)
end_time = time.time()
decrypted_message = decrypt_message(encrypted_message, shared_key)

execution_time = end_time - start_time
print(f"The encrypt_message function took {execution_time} seconds to finish.")
print("Shared Secret (in hex):", hex(shared_secret_a))
print("Encrypted Message (in hex):", encrypted_message.hex())
print("Decrypted Message:", decrypted_message.decode())

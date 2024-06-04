import math

def randomString(length):
    import random
    return ''.join(random.choice('abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789') for i in range(length))

def gcd(a, b):
    while b != 0:
        a, b = b, a % b
    return a


def modInverse(a, m):
    a = a % m
    for x in range(1, m):
        if (a * x) % m == 1:
            return x
    return 1


def generateKeys(p, q):
    n = p * q
    phi = (p - 1) * (q - 1)

    e = 2
    while e < phi:
        if gcd(e, phi) == 1:
            break
        else:
            e += 1

    d = modInverse(e, phi)

    return (e, n), (d, n)


def encryptChar(ch, publicKey, n):
    return pow(ord(ch), publicKey, n)


def decryptChar(encryptedChar, privateKey, n):
    return chr(pow(encryptedChar, privateKey, n))


def encryptMessage(message, publicKey, n):
    encryptedMessage = [encryptChar(ch, publicKey, n) for ch in message]
    return encryptedMessage


def decryptMessage(encryptedMessage, privateKey, n):
    decryptedMessage = ''.join(decryptChar(ch, privateKey, n) for ch in encryptedMessage)
    return decryptedMessage


def measureEncryptTime(message, publicKey, n):
    import time
    n = publicKey[1]
    start = time.perf_counter()
    encrypted_message = encryptMessage(message, publicKey[0], n)
    end = time.perf_counter()
    print(f"Time taken to encrypt message: {end - start:.6f} seconds")

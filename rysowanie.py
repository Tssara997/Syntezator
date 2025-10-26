import socket
#import pandas as pd
import matplotlib.pyplot as plt

# połącz się z serwerem C++
HOST = "127.0.0.1"
PORT = 12345

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    s.connect((HOST, PORT))
    # wyślij typ fali
    s.sendall(b"sin")
    # odbierz dane
    data = s.recv(1000000).decode("utf-8")

# przetwórz dane CSV
lines = [line.split(",") for line in data.strip().split("\n")]
x = [float(a) for a, _ in lines]
y = [float(b) for _, b in lines]

plt.plot(x, y)
plt.title("Wykres fali z C++ przez socket")
plt.show()
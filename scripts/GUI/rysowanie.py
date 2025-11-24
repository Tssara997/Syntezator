from base64 import encode
from pickle import NONE
import socket
import tkinter as tk
from turtle import st, width
import numpy as np
import matplotlib.figure as figure
import matplotlib.backends.backend_tkagg as backend
import matplotlib.pyplot as plt
import threading
from Przyciski import Metronom
from PIL import Image, ImageTk
from tkdial import Dial

# połącz się z serwerem C++
HOST = "127.0.0.1"
PORT = 12345

server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server_socket.connect((HOST, PORT))
server_socket.setblocking(False)
delay = None
delayTime = 0  # milliseconds

def sendValue(x):
    server_socket.sendall(x.encode())

def checkDelay(msg):
    global delay
    if delay:
        master.after_cancel(delay)
    delay = master.after(delayTime, lambda: sendValue(msg))

def przetwarzanieDanych(dane):
    y = []
    if dane == NONE:
        return y
    while "\n" in dane:
        line_data, dane = dane.split('\n', 1)
        y.append(float(line_data))
    return y

def wyjdz():
    master.destroy()
    try:
        server_socket.shutdown(socket.SHUT_WR)
        server_socket.close()
    except:
        pass

def zmianaFazy():
    sendValue("FAZA 1\n")

def zmianaAmplitudy(x):
    msg = "AMP " + str(amp.get()) + "\n"
    checkDelay(msg)

def zmianaCzestotliwosci(x):
    msg = "FREQ " + str(freq.get()) + "\n"
    checkDelay(msg)

def dzwiek():
    sendValue("FALA \n")

master = tk.Tk()
master.title("WHat")
master.resizable(False, False)

# Sekcja filtrów
filtrFrame = tk.Frame(master)
filtrFrame.grid(row = 0, column= 0, sticky ="nsew")

filterUi = tk.Frame(filtrFrame)
filterUi.grid(row = 0, column = 0, sticky = "nsew")
filterUi.grid_rowconfigure(0, weight = 1)

org_img = Image.open("C:\\Users\\majab\\testing\\Tempometr\\logo.png")
res_img = org_img.resize((70,30), Image.Resampling.LANCZOS)
img = ImageTk.PhotoImage(res_img)

nazwaUi = tk.Canvas(filterUi, width=70)
nazwaUi.grid(row = 0, column= 0, sticky="nsew")

#nazwaUi.img = tk.PhotoImage(file = "C:\\Users\\majab\\testing\\Tempometr\\logo.png")
nazwaUi.create_image(0, 0, anchor = "nw", image = img)
nazwaUi.img = img

metronom_B = Metronom(filtrFrame, socket = server_socket)
metronom_B.grid(row = 0, column = 1, sticky = "nw")

#Sekcja oscylatora A
oscyAFrame = tk.Frame(master)
oscyAFrame.grid(row = 0, column= 1, sticky="nse")

dzwiek_B = tk.Button(oscyAFrame, text = "ON", command = dzwiek)
dzwiek_B.pack()

# tworzenie wykresu
fig = figure.Figure(figsize=(8,4), dpi = 70)
plot1 = fig.add_subplot(111)
plot1.axes.get_xaxis().set_visible(False)
plot1.axes.get_yaxis().set_visible(False)

y_data = []
(line,) = plot1.plot(y_data, "#B01A99")
wykres = backend.FigureCanvasTkAgg(fig, master = oscyAFrame)
wykres.get_tk_widget().pack(in_=oscyAFrame)
wykres.draw()

def nowe_dane(y):
    x = range(len(y))
    line.set_data(x, y)
    for coll in plot1.collections:
        coll.remove()
    plot1.fill_between(x, y, alpha = 0.5 , color = "#B01A99")
    plot1.relim()
    plot1.autoscale_view()
    wykres.draw()

def receive_data():
    global y_data
    while True:
        try:
            dane = server_socket.recv(100000).decode().strip()
            if dane:
                y = przetwarzanieDanych(dane)
                print(len(y))
                master.after(0, lambda: nowe_dane(y))
        except BlockingIOError:
            pass
        except OSError:
            break  # zamknięto socket

amp = tk.DoubleVar()
amplituda_S = tk.Scale(oscyAFrame, from_ = 0, to = 2, resolution = 0.01, orient = "horizontal", label = "amplituda", variable=amp, command= zmianaAmplitudy)
amplituda_S.pack()

freq = tk.DoubleVar()
czestotliwosc_S = tk.Scale(oscyAFrame, label = "czestotliwosc", from_ = 50, to = 700, resolution= 2, orient = "horizontal", variable=freq, command = zmianaCzestotliwosci) 
czestotliwosc_S.pack()

# Sekcja oscylatora B
oscyBFrame = tk.Frame(master)
oscyBFrame.grid(row = 0, column= 2, sticky="nsew")
quit_B = tk.Button(oscyBFrame, text="wyjdz", command = wyjdz)
quit_B.pack()

threading.Thread(target=receive_data, daemon=True).start()

master.mainloop()
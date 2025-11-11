from CanvasButton import CanvasButton
import tkinter as tk

class Metronom:
    def __init__(self, master, width = 50, height = 30, text_on = "ON", text_off = "OFF", bg = "lightgray", activebg = "gray", socket = None):
        self.master = master
        self.width = width
        self.height = height
        self.text_on = text_on
        self.text_off = text_off
        self.bg = bg
        self.activebg = activebg
        self.socket = socket
        self.maxTempo = 300
        self.minTempo = 30
        self.tempo = 140
        self.text_var = tk.StringVar()
        self.text_var.set(self.tempo)


        # Tworznie uk³adu przyciskow dla kontrolii metronomu
        self.mainFrame = tk.Frame(self.master)
        #self.mainFrame.grid()

        self.vcmd = (self.master.register(self.only_numbers), "%P")

        self.leftFrame = tk.Frame(self.mainFrame)
        self.leftFrame.grid(row = 0, column = 0, sticky = "nsew")
        self.leftFrame.grid_rowconfigure(0, weight=1)

        self.onOffButton = CanvasButton(self.leftFrame, command = self.wlacz, command_off = self.wlacz, height = self.height, text = self.text_on, text_off = self.text_off, bg = self.bg, activebg = self.activebg, press = True)
        self.wpisanie = tk.Entry(self.leftFrame, textvariable=self.text_var, font = ("Ariel", 12), justify = "center", width = 8, validate="key", validatecommand=self.vcmd)

        self.onOffButton.grid(row = 0, column = 0, sticky = "nsew")
        self.wpisanie.grid(row = 0, column = 1, sticky = "nsew")
        self.wpisanie.bind("<Return>", self.pokaz)

        self.rightFrame = tk.Frame(self.mainFrame)
        self.rightFrame.grid(row = 0, column= 1, sticky="nsew")
        self.rightFrame.grid_columnconfigure(0, weight=1)

        self.plusButton = CanvasButton(self.rightFrame, text = "+", height = self.height // 2, command = lambda : self.zmienTempo(1))
        self.minusButton = CanvasButton(self.rightFrame, text = "-", height = self.height // 2, command = lambda : self.zmienTempo(-1))

        self.plusButton.grid(row = 0, column= 0, sticky="nsew")
        self.minusButton.grid(row = 1, column = 0, sticky="nsew")

    def wlacz(self):
        msg = "METROON \n"
        self.socket.sendall(msg.encode())

    def pokaz(self, event = None):
        if self.text_var.get() == "":
            self.tempo = self.minTempo
        else :
            try:
                if int(self.text_var.get()) <= self.minTempo:
                    self.tempo = self.minTempo
                elif int(self.text_var.get()) >= self.maxTempo:
                    self.tempo = self.maxTempo
                else :
                    self.tempo = int(self.text_var.get())
            except:
                pass
        self.sendTempo()
        self.text_var.set(self.tempo)
        self.master.focus()

    def only_numbers(self, P):
        return P.isdigit() or P == ""

    def zmienTempo(self, x):
        self.tempo += x
        self.text_var.set(self.tempo)
        self.sendTempo()

    def sendTempo(self):
        msg = f"METRO {self.tempo}\n"
        self.socket.sendall(msg.encode())

    def grid(self, **kwargs):
        self.mainFrame.grid(**kwargs)

    def pack(self, **kwargs):
        self.mainFrame.pack(**kwargs)
    
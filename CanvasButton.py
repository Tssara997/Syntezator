import tkinter as tk

class CanvasButton:
  def __init__(self, master, height =60 , width = 25, text = "", text_off = "", command = None, command_off = None, bg = "lightgray", activebg = "gray", press = False, **kwargs):
    self.master = master
    self.height = height
    self.width = width
    self.text_on = text
    self.text_off = text_off
    self.bg = bg
    self.activebg = activebg
    self.press = press
    self.command_on = command
    self.command_off = command_off
    self.active = True

    self.canvas = tk.Canvas(self.master, width=self.width, height=self.height, highlightthickness=0)
    self.canvas.configure(bg=self.bg)

    self.rect = self.canvas.create_rectangle(0, 0, self.width, self.height, fill=self.bg, outline= "white")
    self.text = self.canvas.create_text(self.width//2, self.height//2, text = self.text_on)

    self.canvas.bind("<Button-1>", self.on_click)
    self.canvas.bind("<ButtonRelease-1>", self.on_release)
    self.canvas.bind("<Enter>", self.on_hover)
    self.canvas.bind("<Leave>", self.of_hover )
  
  def on_click(self, event = None):
    self.canvas.itemconfig(self.rect, fill = self.activebg, outline = "black")
    if self.press:
      self.active = not self.active
      if self.active:
        self.canvas.itemconfig(self.text, text = self.text_on)
        if self.command_on : self.command_on()
      if not self.active:
        self.canvas.itemconfig(self.text, text = self.text_off)
        if self.command_off : self.command_off()
    else :
      self.active = True
      if self.command_on : self.command_on()

  def on_release(self, event = None):
    if self.active:
      self.canvas.itemconfig(self.rect, fill = self.bg, outline = "white")

  def on_hover(self, event = None):
    # self.canvas.itemconfig(self.rect, fill = self.activebg)
    pass
    
  def of_hover(self, event = None):
    if self.active:
      self.canvas.itemconfig(self.rect, fill = self.bg)

  def grid(self, **kwargs):
    self.canvas.grid(**kwargs)

  def pack(self, **kwargs):
    self.canvas.pack(**kwargs)

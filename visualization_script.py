import serial
import numpy as np
import matplotlib.pyplot as plt
import re
from matplotlib.animation import FuncAnimation

# --- Configuration ---
PORT = 'COM3'   # Change to your serial port (e.g., /dev/ttyACM0 on Linux)
BAUD = 115200
WIDTH, HEIGHT = 32, 24

# Choose a colormap that’s common for thermal visualization:
# Options: 'inferno', 'plasma', 'turbo', 'jet'
COLORMAP = 'inferno'

# --- Setup Serial Connection ---
ser = serial.Serial(PORT, BAUD, timeout=2)

# --- Visualization Setup ---
plt.style.use('dark_background')
fig, ax = plt.subplots()
im = ax.imshow(np.zeros((HEIGHT, WIDTH)), cmap=COLORMAP, vmin=20, vmax=40)
plt.colorbar(im, ax=ax, label='Temperature (°C)')
ax.set_title("MLX90640 Thermal Camera")
ax.axis('off')

# --- Helper Function to Read Frame from Serial ---
def read_frame():
    lines = []
    while True:
        line = ser.readline().decode(errors='ignore').strip()
        if not line:
            continue
        if "===" in line:
            lines = []  # new frame starting
        elif re.match(r"^-?\d+\.\d+.*", line):  # numeric line
            lines.append(line)
            if len(lines) >= HEIGHT:
                try:
                    data = []
                    for l in lines:
                        nums = re.findall(r"-?\d+\.\d+", l)
                        data.extend([float(n) for n in nums])
                    if len(data) == WIDTH * HEIGHT:
                        return np.array(data).reshape((HEIGHT, WIDTH))
                except Exception:
                    pass

# --- Animation Update Function ---
def update(_):
    frame = read_frame()
    if frame is not None:
        im.set_data(frame)
        im.set_clim(vmin=np.min(frame), vmax=np.max(frame))
    return [im]

ani = FuncAnimation(fig, update, interval=200, blit=True)
plt.show()

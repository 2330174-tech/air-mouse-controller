# Air Mouse Controller

A simple ESP32-to-PC air-mouse prototype. The ESP32 sends mouse movement and button state over UDP; the Python receiver on the computer applies the movement and clicks through `pynput`.

## Files

- `mouse_controller.py` — Python UDP receiver for the computer.
- `mouse.c` — ESP32 Arduino sketch source. It is C++ despite the `.c` filename; rename it to `mouse.ino` to open it directly in the Arduino IDE.

## Requirements

- Python 3
- An ESP32 (the sketch can be adapted for ESP8266)
- Both the ESP32 and computer connected to the same trusted Wi-Fi network
- Left and right buttons connected to the GPIO pins configured in the sketch

## Setup

1. Install the Python dependency:

   ```bash
   python -m pip install pynput
   ```

2. Find the computer's local-network IPv4 address and set it in `mouse.c` as `rx_ip`. The default `192.168.1.100` is only an example.
3. Set `YOUR_WIFI_SSID` and `YOUR_WIFI_PASSWORD` in `mouse.c`.
4. Open the sketch in the Arduino IDE (rename `mouse.c` to `mouse.ino` first), select the correct board and port, then upload it.
5. Start the receiver on the computer:

   ```bash
   python mouse_controller.py
   ```

6. Allow Python to receive UDP traffic on port `4210` in the computer's firewall, limited to your trusted local network where possible.

The receiver expects packets in `X,Y,L,R` format: horizontal movement, vertical movement, left-button state, and right-button state. Button state `1` means pressed and `0` means released. It also accepts the older `X,Y` format. The sketch currently sends `dx = 0` and `dy = 0`; connect your motion sensor or joystick and update those values to enable cursor movement.

## Configuration and troubleshooting

- `UDP_PORT` in `mouse_controller.py` and `udp_port` in the sketch must match.
- The sketch's `rx_ip` must be the computer's current LAN address. Configure a DHCP reservation if that address changes.
- Adjust `DEADZONE` in `mouse_controller.py` to tune jitter filtering.
- Depending on sensor orientation, invert or swap the axes in the `mouse.move(...)` call.
- On macOS, grant the terminal/Python process Accessibility permission if `pynput` cannot control the mouse. On Linux, ensure the active desktop session supports `pynput`.

## Network safety

This prototype uses unauthenticated UDP. The Python receiver binds to all network interfaces, and any device able to reach UDP port `4210` may be able to move or click the mouse. Use it only on a trusted network, do not forward the port or expose it to the internet, and restrict inbound firewall access to the ESP32 or trusted LAN. Do not use it on a network where untrusted devices can connect.

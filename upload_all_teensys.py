import os
import subprocess
import glob

def after_build(source, target, env):
    # Teensy wird als HID-Gerät erkannt, z.B. /dev/hidraw* oder COM-Port unter Windows
    # Hier Beispiel für Windows: Teensy Loader sucht nach "hid#VID_16C0&PID_0483"
    teensy_ports = glob.glob(r'\\?\hid#vid_16c0&pid_0483*')
    if not teensy_ports:
        print("Keine Teensy-Geräte gefunden.")
        return

    firmware_path = env['TARGET_BUILD_BIN']
    for port in teensy_ports:
        print(f"Upload auf Teensy: {port}")
        # Teensy Loader CLI muss installiert sein
        subprocess.run([
            "teensy_loader_cli",
            "-mmcu=TEENSY41",  # Passe ggf. an dein Modell an
            "-w",
            "-v",
            "-s",
            firmware_path
        ])

# PlatformIO Hook
Import("env")
env.AddPostAction("build", after_build)

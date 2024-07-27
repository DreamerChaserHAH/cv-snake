import subprocess
import pyautogui
import time

process = subprocess.Popen(['build/cv-snake'])

def is_process_running(process):
   return process.poll() is None

while not is_process_running(process):
    time.sleep(0.1)

time.sleep(1)
pyautogui.keyDown('a')
time.sleep(1)
pyautogui.keyDown('s')
time.sleep(1)
pyautogui.keyDown('d')
time.sleep(1)
pyautogui.keyDown('w')
time.sleep(1)

process.terminate()
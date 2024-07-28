import subprocess
import pyautogui
import pygetwindow as gw
import time
import re
import cv2
import pytesseract

process = subprocess.Popen(['build/cv-snake'])

def is_process_running(process):
   return process.poll() is None

while not is_process_running(process):
    time.sleep(0.1)   # Get the PID of the subprocess

    
# Get the PID of the subprocess
pid = process.pid
    
# Use AppleScript to get the title and bounds of the window with the specific PID
apple_script = f'''
    tell application "System Events"
        set frontApp to first application process whose unix id is {pid}
        if exists (1st window of frontApp) then
            set w to 1st window of frontApp
            return {{name of w, position of w, size of w}}
        else
            return {{}}
        end if
    end tell
    '''
    
try:
    result = subprocess.check_output(['osascript', '-e', apple_script]).decode('utf-8').strip()
    if result:
        info_list=result.split(",")
        print(info_list)
    else:
        print(f"No window found for PID {pid}")
except subprocess.CalledProcessError:
    print(f"Could not get window info for PID {pid}")

    
title=info_list[0]
x=int(info_list[1])
y=int(info_list[2])
width=int(info_list[3])
height=int(info_list[4])
screenshot = pyautogui.screenshot(region=(x, y,int(3*width/4), int(height/3.5)))
        
# Save the screenshot
filename = f"{title.replace(' ', '_')}_screenshot.png"
screenshot.save(filename)
print(f"Screenshot saved as {filename}")

# Load the image
image = cv2.imread(filename)

# Resize the image to make the text larger
resized_image = cv2.resize(image, None, fx=3, fy=3, interpolation=cv2.INTER_CUBIC)

# Convert the image to grayscale
gray = cv2.cvtColor(resized_image, cv2.COLOR_BGR2GRAY)

# Invert the grayscale image
inverted_gray = cv2.bitwise_not(gray)

# Apply thresholding
_, thresh = cv2.threshold(inverted_gray, 100, 255, cv2.THRESH_BINARY)

# Use Tesseract to extract text from the ROI
score_text = pytesseract.image_to_string(thresh, config='--psm 6 digits')

# Post-process the extracted text to filter out non-digit characters and clean the result
cleaned_score_text = ''.join(filter(str.isdigit, score_text))

# Display the processed ROI for debugging
cv2.imshow('Processed ROI', thresh)
cv2.waitKey(0)
cv2.destroyAllWindows()

print(f'Extracted Score: {cleaned_score_text}')


#time.sleep(1)
#pyautogui.keyDown('a')
#time.sleep(1)
#pyautogui.keyDown('s')
#time.sleep(1)
#pyautogui.keyDown('d')
#time.sleep(1)
#pyautogui.keyDown('w')
#time.sleep(1)

process.terminate()
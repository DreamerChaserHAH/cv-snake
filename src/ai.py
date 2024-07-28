import subprocess
import time
import re
import cv2
import pytesseract
import pyautogui

class GameWindow:
    def __init__(self, command):
        self.process = subprocess.Popen(command)
        self.pid = self.process.pid
        self.title = None
        self.x = None
        self.y = None
        self.width = None
        self.height = None

    def is_process_running(self):
        return self.process.poll() is None

    def get_window_info(self):
        apple_script = f'''
            tell application "System Events"
                set frontApp to first application process whose unix id is {self.pid}
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
                info_list = result.split(",")
                self.title = info_list[0]
                self.x = int(info_list[1])
                self.y = int(info_list[2])
                self.width = int(info_list[3])
                self.height = int(info_list[4])
                return True
            else:
                print(f"No window found for PID {self.pid}")
                return False
        except subprocess.CalledProcessError:
            print(f"Could not get window info for PID {self.pid}")
            return False

    def capture_screenshot(self, filename):
        screenshot = pyautogui.screenshot(region=(self.x, self.y, int(3 * self.width / 4), int(self.height / 3.5)))
        screenshot.save(filename)
        print(f"Screenshot saved as {filename}")

    def terminate_process(self):
        self.process.terminate()

def preprocess_image(filename):
    image = cv2.imread(filename)
    resized_image = cv2.resize(image, None, fx=3, fy=3, interpolation=cv2.INTER_CUBIC)
    gray = cv2.cvtColor(resized_image, cv2.COLOR_BGR2GRAY)
    inverted_gray = cv2.bitwise_not(gray)
    _, thresh = cv2.threshold(inverted_gray, 100, 255, cv2.THRESH_BINARY)
    return thresh

def extract_text_from_image(image):
    score_text = pytesseract.image_to_string(image, config='--psm 6 digits')
    cleaned_score_text = ''.join(filter(str.isdigit, score_text))
    return cleaned_score_text

def main():
    command = ['build/cv-snake']
    game_window = GameWindow(command)
    
    while not game_window.is_process_running():
        time.sleep(0.1)

    if game_window.get_window_info():
        screenshot_filename = f"{game_window.title.replace(' ', '_')}_screenshot.png"
        game_window.capture_screenshot(screenshot_filename)
        
        processed_image = preprocess_image(screenshot_filename)
        
        cv2.imshow('Processed ROI', processed_image)
        cv2.waitKey(0)
        cv2.destroyAllWindows()
        
        extracted_score = extract_text_from_image(processed_image)
        print(f'Extracted Score: {extracted_score}')

    game_window.terminate_process()

if __name__ == "__main__":
    main()

import pyautogui
pyautogui.PAUSE = 0.005
with open('result.txt') as f:
    lines = f.readlines()
    y = 0
    for line in lines:
        x = 0
        for letter in line:
            pyautogui.moveTo(680 + (50*x), 300 + (50*y))
            pyautogui.click();           
            if(letter == "B"):
                pyautogui.click()
            x = x + 1
        y = y + 1

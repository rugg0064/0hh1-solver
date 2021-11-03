import pyautogui
pyautogui.PAUSE = 0.00
im = pyautogui.screenshot()

for y in range(0,12):
    for x in range(0,12):
        px = pyautogui.pixel( 680 + (50*x), 300 + (50*y) )
        pyautogui.moveTo(680 + (50*x), 300 + (50*y))
        if(px == (0x2a,0x2a,0x2a)):
            toWrite = "-"
        elif(px[2] > px[1]):
            pyautogui.click()
        else:
            pyautogui.click()
            pyautogui.click()
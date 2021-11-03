import pyautogui
pyautogui.PAUSE = 0.00
im = pyautogui.screenshot()

with open('../input.txt', 'w') as f:
    for y in range(0,12):
        for x in range(0,12):
            px = pyautogui.pixel( 680 + (50*x), 300 + (50*y) )
            toWrite = ""
            if(px == (0x2a,0x2a,0x2a)):
                toWrite = "-"
            elif(px[2] > px[1]):
                toWrite = "B"
            else:
                toWrite = "R"
            print(toWrite, end="")
            f.write(toWrite)
        print();
        f.write("\n")
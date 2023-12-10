from PyQt5.uic import loadUi
from PyQt5.QtWidgets import QApplication
import random

def is_valid_input(start, end):
    return start <= end

def create():
    try:
        ok = False
        nbProcess = int(windows.nbProcess.text())
        startTA = int(windows.startTA.text())
        endTA = int(windows.endTA.text())
        startTE = int(windows.startTE.text())
        endTE = int(windows.endTE.text())
        startPR = int(windows.startPR.text())
        endPR = int(windows.endPR.text())
        # Generate random processes
        processes = []
        for i in range(1, nbProcess + 1):
            ta = random.randint(startTA, endTA)
            te = random.randint(startTE, endTE)
            pr = random.randint(startPR, endPR)
            processes.append(f'P{i}:{ta}:{te}:{pr}')

            # Write processes to process.txt
            with open('process.txt', 'w') as file:
                file.write('\n'.join(processes))
                windows.errMsg.setText("")
                windows.msg.setText("created successfully!")

    except Exception as e:
        windows.errMsg.setText(f"Invalid Input! Check Again!")

app=QApplication([])
windows=loadUi("ProcessGeneratorInterface.ui")
windows.show()
windows.btn.clicked.connect(create)
app.exec()
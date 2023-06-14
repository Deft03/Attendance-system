#RUN CODE: python3 execute_rfid
#!/usr/bin/env python
import time
import RPi.GPIO as GPIO
from mfrc522 import SimpleMFRC522
import mysql.connector

db = mysql.connector.connect(
  host="localhost",
  user="sangiot",
  passwd="123",
  database="finalExam"
)

cursor = db.cursor()
reader = SimpleMFRC522()

try:
  while True:
    print('----Place Card to------')
    id, text = reader.read()
    print("ur id: ",str(id))
    
    value1 = str(id)
    cursor.execute("UPDATE checkedForm set rfid_uid='%s' WHERE id = 1 " % (value1,))
    db.commit()

    time.sleep(2)
finally:
  GPIO.cleanup()

 
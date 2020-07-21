import serial                                
ArduinoUnoSerial = serial.Serial('com5',9600)                                                   
print (ArduinoUnoSerial.readline())                       
print ("You have new message from Arduino")
while 1:      
    var = input()                                                
    if (var == '1'):                                                     
        ArduinoUnoSerial.write('1')                           
        print ("LED turned ON")         
        time.sleep(1)          
    if (var == '0'):   
        ArduinoUnoSerial.write('0')           
        print ("LED turned OFF")         
        time.sleep(1)
    if (var == 'fine and you'):     
        ArduinoUnoSerial.write('0')   
        print ("I'm fine too,Are you Ready to !!!")         
        print ("Type 1 to turn ON LED and 0 to turn OFF LED")         
        time.sleep(1)
from pyArduino import *
import matplotlib.pyplot as plt
#import numpy as np

ts = 0.1 # Tiempo de muestreo
tf = 20 # Tiempo de simulacion
t = np.arange(0,tf+ts,ts) # Array de tiempo
N = len(t) # Numero de muestras

######################## Comunicacion Serial ###############

port = 'COM10'  # Com ESP32
baudRate = 115200 # Velocidad de transmisión en Baudios de la ESP32

arduino = serialArduino(port,baudRate,2)# Objeto serial

arduino.readSerialStart() # Inicia lectura de datos

######################### Señales #####################

pv  = np.zeros(N) # Variable de proceso (Pv) - Donde se almacenará la lectura de la velocidad en rad/s
cv  = np.zeros(N) # Variable de control (Cv)
sp = np.zeros(N) # Variable de deseada (Sp) - Set point

######################### Setpoint Escalon ################
##Inicialmente será un escalón de cero y después se convierte en un escalón de 8 rad/s
""" for k in range(N):
     if k*ts > 3:
          sp[k] = 8    
     else:
          sp[k] = 0  """
          
###################### Setpoint Trayectoria ################
sp = 4*np.cos(0.5*t)+6

########################## Loop ########################

for k in range(N):

     start_time = time.time() # Tiempo actual
     
     arduino.sendData([sp[k]]) # Enviar Sp (debe ser una lista)
     
     pv[k] = arduino.rawData[0] # Recibir Pv 
     cv[k] = arduino.rawData[1] # Recibir Cv
     
     elapsed_time = time.time() - start_time # Tiempo transcurrido
     
     time.sleep(ts-elapsed_time) # Esperar hasta completar el tiempo de muestreo
     
     
arduino.sendData([0]) # Detener motor     
arduino.close() # Cerrar puerto serial
     
###################### Mostrar figuras ######################
plt.figure()     
plt.plot(t,sp,label='Sp')     
plt.plot(t,pv,label='Pv')
plt.legend(loc='upper left')

plt.figure() 
plt.plot(t,cv,label='Cv')
plt.legend(loc='upper left')

plt.show()


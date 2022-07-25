from pyArduino import *
import matplotlib.pyplot as plt

ts = 0.1 # Tiempo de muestreo
tf = 10  # Tiempo de simulacion
t = np.arange(0,tf+ts,ts) # Array de tiempo
N = len(t) # Numero de muestras

######################## Comunicacion Serial ###############

port = 'COM10'  # Com Arduino
baudRate = 115200 # Velocidad de transmisión en Baudios de la ESP32

arduino = serialArduino(port,baudRate)# Objeto serial

arduino.readSerialStart() # Inicia lectura de datos

######################### Señales #####################

pv  = np.zeros(N) # Variable de proceso (Pv) - Donde se almacenará la lectura de la velocidad en rad/s
cv  = np.zeros(N) # Variable de control (Cv)

########################## Loop ########################

for k in range(N):

     start_time = time.time() # Tiempo actual en ese instante

     # Escalon 
     if k*ts   > 3:  # Escalon a los 3 segundos
          cv[k] = 40  # Valor escalon del 0 al 100% (40%)
     else:
          cv[k] = 0;
     
     arduino.sendData([cv[k]]) # Enviar Cv (debe ser una lista), si se necesita enviar otro dato, en el parametro se agraga una coma
     
     pv[k] = arduino.rawData[0] # Recibir Pv en porcentaje
          
     elapsed_time = time.time() - start_time # Tiempo transcurrido
     
     time.sleep(ts-elapsed_time) # Esperar hasta completar el tiempo de muestreo
     
     
arduino.sendData([0]) # Detener motor     
arduino.close() # Cerrar puerto serial

######################## Guardar señales ###########################
with open('firstResponse.npy', 'wb') as f:
     np.save(f,cv)
     np.save(f,pv)
     np.save(f,t)
     np.save(f,ts)
     
###################### Mostrar figuras ######################
     
plt.plot(t,pv,label='Pv')
plt.plot(t,cv,label='Cv')
plt.legend(loc='upper left')
plt.show()

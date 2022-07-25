from pyRobotics import *
import numpy as np

ts = 0.1
tf = 10
t = np.arange(0,tf+ts,ts)
N = len(t)

hx = 0.5*np.cos(0.6*t)
hy = 0.5*np.sin(0.6*t)

phi = 45*(np.pi/180.0)*np.ones(N)

path = "stl"
color = ["yellow","black","gray","gray","white","blue"]

uniciclo = robotics(path,color)

xmin = -2
xmax= 2
ymin = -2
ymax =2
zmin = 0
zmax = 2
bounds = [xmin,xmax,ymin,ymax,zmin,zmax]
uniciclo.configureScene(bounds)

uniciclo.initTrajectory(hx,hy)

escala = 5
uniciclo.initRobot(hx,hy,phi,escala)

uniciclo.startSimulation(1,ts)




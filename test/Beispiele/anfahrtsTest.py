import turtle
import keyboard
import math

robotSize = 20
ballSize =  10

# --- keep window open until Space is pressed ---
screen = turtle.Screen()
screen.listen()                     # start listening for keyboard events

t = turtle.Turtle()
t.hideturtle()
t.up()

bX = screen.canvwidth/2
bY = screen.canvheight/2
rX = bX + int(input("right"))
rY = bY + int(input("up"))

initAngle = 90

keyboard.wait("space")

def drawNewPos():
    screen.clear()
    screen.register_shape()
    t.goto(bX, bY)
    t.circle(ballSize)
    t.goto(rX, rY)
    t.circle(robotSize)

def useNewPos():
    dX = rX-bX
    dY = rY-bY
    dist = math.sqrt(dX*dX+dY*dY)
    angle = initAngle/dist
    angle -= math.degrees(math.atan(dX/dY))
    rX += math.sin(math.radians(angle))
    rY += math.cos(math.radians(angle))


useNewPos()
print(rX)
print(rY)

turtle.done()   # hand control to the event loop

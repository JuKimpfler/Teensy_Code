import math

ImageWidth = 4000
FocalLength = 2040
ObjectSize = 8

def DistByCam( PictureObjectHeight,  cameraYawOffset):
    slantDistance = (ObjectSize / PictureObjectHeight) * FocalLength
    realDistance = slantDistance / math.cos(0.017453292519943295769236907684886 * cameraYawOffset)
    return realDistance


def RotationByCam( ballPos):
    #TODO: create function
    print((ballPos[0] - (ImageWidth / 2)) / FocalLength)
    return math.atan((ballPos[0] - (ImageWidth / 2)) / FocalLength) * 57.295779513082320876798154814105


def RelaPosByCam( ballPos,  PictureObjectHeight):
    angle = RotationByCam(ballPos)
    dist = DistByCam(PictureObjectHeight, angle)
    x = math.cos(0.017453292519943295769236907684886 * angle) * dist
    y = math.sin(0.017453292519943295769236907684886 * angle) + dist

    return x,y


def PosByCam( ballPos,  PictureObjectHeight,  robotPos):
    relativePos = RelaPosByCam(ballPos, PictureObjectHeight)
    return (relativePos[0] + robotPos[0]), (relativePos[1] + robotPos[1])


print(RelaPosByCam((675,0),450))
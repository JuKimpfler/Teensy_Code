import math

ImageWidth = 4000
FocalLength = 2040
ObjectSize = 8

def DistanceByCamera( PictureObjectHeight,  cameraYawOffset):
    slantDistance = (ObjectSize / PictureObjectHeight) * FocalLength
    realDistance = slantDistance / math.cos(0.017453292519943295769236907684886 * cameraYawOffset)
    return realDistance


def RotationByCamera( ballPos):
    #TODO: create function
    print((ballPos[0] - (ImageWidth / 2)) / FocalLength)
    return math.atan((ballPos[0] - (ImageWidth / 2)) / FocalLength) * 57.295779513082320876798154814105


def RelativePositionByCamera( ballPos,  PictureObjectHeight):
    angle = RotationByCamera(ballPos)
    dist = DistanceByCamera(PictureObjectHeight, angle)
    x = math.cos(0.017453292519943295769236907684886 * angle) * dist
    y = math.sin(0.017453292519943295769236907684886 * angle) + dist

    return x,y


def PositionByCamera( ballPos,  PictureObjectHeight,  robotPos):
    relativePos = RelativePositionByCamera(ballPos, PictureObjectHeight)
    return (relativePos[0] + robotPos[0]), (relativePos[1] + robotPos[1])


print(RelativePositionByCamera((675,0),450))
import math

liste = [2,6.9,13.85, 20]
times = [1,5/3, 1, 1]

power = 1.1901

last = 0
last2 = 0
current = 0
current2 = 0
first = 0

together = 0

for e,item in enumerate(liste):
    current = math.pow(item, power)
    current2 = (current - last) * times[e]
    if(first>1):
        together += abs(current2-last2)
    last = current
    last2 = current2
    first += 1

print(together)
# File:    trap.py
# Purpose: Calculate area using trapezoidal rule.
#
# Input:   a, b, n
# Output:  estimate of area between x-axis, x = a, x = b, and graph of f(x)
#          using n trapezoids.
#
# Usage:   python trap.py
#
# Note:    The function f(x) is hardwired.

from sys import stdin

# Function:     Trap
# Purpose:      Estimate area using the trapezoidal rule
# Input args:   a: left endpoint (double)
#               b: right endpoint (double)
#               n: number of trapezoids (int)
#               h: stepsize = length of base of trapezoids (double)
# Return val:   Trapezoidal rule estimate of area between x-axis,
#               x = a, x = b, and graph of f(x)  (double)
def Trap(a, b, n, h):

    area = (f(a) + f(b))/2.0
    for i in xrange(1, n):
        x = a + i*h;
        area = area + f(x)
    area = area*h

    return area
# End of Trap 


# Function:    f
# Purpose:     Compute value of function to be integrated
# Input args:  x (double)
# Return val:  f(x) (double)
def f(x):
   return_val = x*x + 1
   return return_val
# End of f 

# main function
print "Enter a, b, and n"
line = stdin.readline()
strings = line.split()
a = float(strings[0])
b = float(strings[1])
n = int(strings[2])

h = (b-a)/n
area = Trap(a, b, n, h)

print "With n =", n, "trapezoids, our estimate"
print "of the area from", a, "to", b, "= %.15f" % area
# End of main 
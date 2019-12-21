-- Derek Banas tutorial on lua

name = "Derek"
io.write("Size of string ", #name, "\n")
name = 4

io.write("My name is ", name, "\n")
print(type(name))

-- basic math operations
io.write("5 + 3 = ", 5+3, "\n")
io.write("5 - 3 = ", 5-3, "\n")
io.write("5 * 3 = ", 5*3, "\n")
io.write("5 / 3 = ", 5/3, "\n")
io.write("5.2 % 3 = ", 5%3, "\n")

-- Math functions: floor, ceil, max, min, sin, cos, tan,
-- asin, acos, exp, log, log10, pow, sqrt, random, math.randomseed

-- math library functions
io.write("floor(2.345) : ", math.floor(2.345), "\n")
io.write("ceil(2.345) : ", math.ceil(2.345), "\n")
io.write("max(2, 3) : ", math.max(2, 3), "\n")
io.write("min(2, 3) : ", math.min(2, 3), "\n")
io.write("pow(8, 2) : ", 8^2, "\n")
io.write("sqrt(64) : ", math.sqrt(64), "\n")

-- random functions
io.write("math.random() : ", math.random(), "\n")
io.write("math.random() : ", math.random(10), "\n")
io.write("math.random() : ", math.random(5, 100), "\n")
math.randomseed(os.time())

-- relational operators: > < >= <= == ~=
-- logical operators: and or not

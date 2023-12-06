# TODO
# checking the input
while True:
    height = input("Height: ")
    if height.isdigit() == False:
        continue
    if (int(height) <= 8 and int(height) >= 1):
        break
height = int(height)
# printing the pyramid
for i in range(1, height + 1):
    k = " " * (height-i) + "#"*i + "  " + "#" * i
    print(k)

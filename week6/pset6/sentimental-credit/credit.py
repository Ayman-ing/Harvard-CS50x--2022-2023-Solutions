# TODO
num = int(input("number: "))
k = num
sum1 = 0
sum2 = 0
lenth = 0
# looking for the lenth
while (k != 0):
    k = k // 10
    lenth = lenth + 1
    if (k < 100 and k > 9):
        firstnum = k // 10
        secondnum = k % 10
# calculating the sums
k = num
while (k != 0):
    digit1 = k % 10
    k = k // 10
    sum1 = sum1 + digit1
    digit2 = (k % 10) * 2
    if (digit2 > 9):
        sum2 = sum2 + digit2 % 10 + digit2 // 10
    else:
        sum2 = sum2 + digit2
    k = k // 10
sum = sum1 + sum2
# checking the card
if (sum % 10 != 0):
    print("INVALID")
else:
    if (lenth == 15 and firstnum == 3 and (secondnum == 4 or secondnum == 7)):
        print("AMEX")
    elif (lenth == 16 and ((secondnum + firstnum*10) in [51, 52, 53, 54, 55])):
        print("MASTERCARD")
    elif ((lenth == 13 or lenth == 16) and firstnum == 4):
        print("VISA")
    else:
        print("INVALID")


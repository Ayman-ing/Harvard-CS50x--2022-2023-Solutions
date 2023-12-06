# TODO
text = input("Text: ")
letters = 0
words = 1
sentences = 0
# iterating the text
for i in text:
    if i.isalpha() == True:
        letters += 1
    elif i == " ":
        words += 1
    elif i == "." or i == "!" or i == "?":
        sentences += 1
# calculating the grade
L = (letters*100)/(words)
S = (sentences*100)/(words)
index = 0.0588 * L - 0.296 * S - 15.8
index = round(index)
# printing the grade
if index < 1:
    print("Before Grade 1")
elif index >= 16:
    print("Grade 16+")
else:
    print("Grade ", index)
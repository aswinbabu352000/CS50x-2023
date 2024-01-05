# Get a string input
# For word in string, see if it equals to HELLO, if yes print 0, if no 100
# If first word has a h in it then 20, else 100
from cs50 import get_string
import string

def main():
    greeting = get_string("Greeting: ").lower().strip()

    word = ""

    for i in range(len(greeting)):
        if greeting[i].isspace()  or greeting[i] in string.punctuation:
            break
        else:
            word += greeting[i]


    if 'hello' in greeting:
        print("$0")
    else:
        if 'h' == word[0]:
            print("$20")
        else:
            print("$100")

main()
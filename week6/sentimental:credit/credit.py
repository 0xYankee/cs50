# TODO
import re
import sys


def main():
    card = get_number()
    if is_valid(card):
        # Identify first few digits of card and match with corresponding
        identifier_2 = re.match(r"(\d{2})", card)
        identifier_3 = re.match(r"(\d{3})", card)
        identifier_6 = re.match(r"(\d{6})", card)
        id2 = identifier_2.group(1)
        id3 = identifier_3.group(1)
        id6 = identifier_6.group(1)
        # Print corresponding card names with matching identifiers
        if id2 == "37":
            print("AMEX")
        elif id2 in ["51", "52", "55"]:
            print("MASTERCARD")
        elif id3 == "222":
            print("MASTERCARD")
        elif id2 in ["40", "41", "42"]:
            print("VISA")
        elif id6 == "499999":
            print("VISA")
        else:
            print("INVALID")
            sys.exit(0)
    else:
        print("INVALID")
        sys.exit(0)


# Prompt user for card number
def get_number():
    card = input("Number: ")
    if len(card) > 12 and len(card) < 17:
        return card
    else:
        print("INVALID")
        sys.exit(1)


# Check if card number is valid length and using algorithm
def is_valid(card):
    length = len(card)
    total_sum = 0
    alternate = False
    # Using the algorithm invented by Hans Peter Luhn of IBM
    while (length > 0):
        if alternate == False:
            total_sum += int(card[length - 1])
            length -= 1
            alternate = True
        elif alternate == True:
            i = int(card[length - 1]) * 2
            if i > 9:
                i = 1 + (i % 10)
            total_sum += i
            length -= 1
            alternate = False
    # Check if result does not return a remainder
    if total_sum % 10 == 0:
        return True
    else:
        return False


main()
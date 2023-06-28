# TODO
# Prompt user for a height (integer)
# Print a pyramid of '#' with corresponding height


def main():
    height = get_height()
    # Print left hashes
    for i in range(height):
        i += 1
        print(" " * (height - i), end="")
        print("#" * i, end="")
        print("  ", end="")
        print("#" * i)


def get_height():
    while (True):
        try:
            i = int(input("Height: "))
            if i > 0 and i < 9:
                return i
        except ValueError:
            print("Not an integer.")


main()
import csv
import sys


def main():

    # TODO: Check for command-line usage
    if len(sys.argv) != 3:
        sys.exit("Usage: python dna.py data.csv sequence.txt")

    # TODO: Read database file into a variable
    list = []
    with open(sys.argv[1], "r") as database:
        reader = csv.DictReader(database)
        for row in reader:
            for key, value in row.items():
                if key != "name":
                    row[key] = int(value)
            list.append(row)

    # TODO: Read DNA sequence file into a variable
    with open(sys.argv[2], "r") as file:
        for str in file:
            sequence = str

    # TODO: Find longest match of each STR in DNA sequence
    count = {}
    total_sequence = 0
    keys = list[0].keys()
    for key in keys:
        if key == "name":
            count[key] = 0
        else:
            count[key] = longest_match(sequence, key)
            total_sequence += 1

    # TODO: Check database for matching profiles
    count_match = {}
    for i in range(len(list)):
        count_match[list[i]["name"]] = 0
        for key in keys:
            if count[key] == list[i][key]:
                count_match[list[i]["name"]] += 1

    for name, count_value in count_match.items():
        if count_value == total_sequence:
            print(name)
            sys.exit(0)

    print("No match")
    sys.exit(0)


def longest_match(sequence, subsequence):
    """Returns length of longest run of subsequence in sequence."""

    # Initialize variables
    longest_run = 0
    subsequence_length = len(subsequence)
    sequence_length = len(sequence)

    # Check each character in sequence for most consecutive runs of subsequence
    for i in range(sequence_length):

        # Initialize count of consecutive runs
        count = 0

        # Check for a subsequence match in a "substring" (a subset of characters) within sequence
        # If a match, move substring to next potential match in sequence
        # Continue moving substring and checking for matches until out of consecutive matches
        while True:

            # Adjust substring start and end
            start = i + count * subsequence_length
            end = start + subsequence_length

            # If there is a match in the substring
            if sequence[start:end] == subsequence:
                count += 1

            # If there is no match in the substring
            else:
                break

        # Update most consecutive matches found
        longest_run = max(longest_run, count)

    # After checking for runs at each character in seqeuence, return longest run found
    return longest_run


main()

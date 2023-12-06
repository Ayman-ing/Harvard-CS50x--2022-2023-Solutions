import csv
import sys


def main():

    # TODO: Check for command-line usage
    if len(sys.argv) != 3:
        print("arguments incorrect")
    else:

        filename1 = sys.argv[1]
        filename2 = sys.argv[2]
        # TODO: Read database file into a variable
        with open(filename1) as database:
            STR = csv.DictReader(database)
            str_list = []
            for firstrow in STR:
                str_list += firstrow
                break

        # TODO: Read DNA sequence file into a variable
        with open(filename2) as DNA:
            DNA_ch = DNA.read()

        # TODO: Find longest match of each STR in DNA sequence
        longest_seq = {}
        for i in range(1, len(str_list)):
            longest_seq[str_list[i]] = longest_match(DNA_ch, str_list[i])

        # TODO: Check database for matching profiles
        with open(filename1) as database:
            STR = csv.DictReader(database)
            for row in STR:
                i = 1
                while (i < len(str_list)):
                    if int(row[str_list[i]]) != longest_seq[str_list[i]]:
                        break
                    i += 1
                if i == len(str_list):
                    print(row["name"])
                    return
        print("No match")

    return


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

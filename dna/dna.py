import csv
import sys


def main():
    # Check for command-line usage
    if not len(sys.argv) == 3:
        print("Usage: python dna.py data.csv sequence.txt")
        return
    # Read database file into a variable
    database_file = open(sys.argv[1])
    database = csv.DictReader(database_file)

    # Read DNA sequence file into a variable
    sequence_file = open(sys.argv[2])
    sequence = str(sequence_file.read())

    # Find longest match of each STR in DNA sequence
    # create a dict called values to store the longest match for each str (from list called strs)
    strs = database.fieldnames
    strs = strs[1 : len(strs)]

    compare = {}
    # itterate over each str in the data add it as a key with the value comming from the longest_match function into compare_dict
    for str_name in strs:
        compare[str_name] = longest_match(sequence, str_name)

    # Check database for matching profiles

    for row in database:
        counter = 0
        for str_name in strs:
            if compare[str_name] == int(row[str_name]):
                counter += 1
        if counter == len(strs):
            print(row["name"])

            database_file.close()
            sequence_file.close()
            return

    print("No match")

    database_file.close()
    sequence_file.close()
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

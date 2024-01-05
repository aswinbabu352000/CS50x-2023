import csv

with open("phonebook.csv", "a")

    name = input("Name: ")
    number = input("Number: ")

    writer = csv.DictWriter(file, fieldname = ["name", "number"])
    writer.writerow(["name": name, "number": number])

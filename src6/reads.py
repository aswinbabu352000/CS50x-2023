import csv

books = []

#Add books to your shelf by reading from books.csv
with open("test.csv") as file:
    file_reader = csv.DictReader(file)
    for book in file_reader:
        print(book["title"])
import csv
import sqlite3

# open file
csv_file = open("students.csv")
# read into object
reader1 = csv.DictReader(csv_file)
# connect to db
conn = sqlite3.connect("roster.db")
c = conn.cursor()

# execute CREATION of tables
sql_file = open("schema.sql")
sql_command = sql_file.read()
c.executescript(sql_command)

houses, heads = set(), set()

for row1 in reader1:
    # inserts student names in the student table
    c.execute("INSERT INTO students (student_name) VALUES (?)", (row1["student_name"],))

    # inserts houses and heads in a the house and head lists
    houses.add(row1["house"])
    heads.add(row1["head"])

# inserts house name and head from the two lists
for house, head in zip(houses, heads):
    c.execute("INSERT INTO houses (house_name, head) VALUES (?, ?)", (house, head))

csv_file.close()
# rereading the csv file into a new DictReader
csv_file2 = open("students.csv")
reader2 = csv.DictReader(csv_file2)
for row2 in reader2:
    c.execute(
        "INSERT INTO house_assignment (student_id, house_id) VALUES ((SELECT id FROM students WHERE student_name = ?),(SELECT id FROM houses WHERE house_name = ?))",
        (row2["student_name"], row2["house"]),
    )


conn.commit()
conn.close()
csv_file2.close()
sql_file.close()

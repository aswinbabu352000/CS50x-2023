s = input("Do you agree? ")

s = s.lower()
if s.lower() in ("Y", "y", "yes"):
    print("Agreed")
if s.lower() in ("N", "n", "no"):
    print("Not agreed")
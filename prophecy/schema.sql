CREATE TABLE students (
    id INTEGER,
    student_name TEXT,
    PRIMARY KEY(id)
);

CREATE TABLE houses (
    id INTEGER,
    house_name TEXT,
    head TEXT,
    PRIMARY KEY(id)
);

CREATE TABLE house_assignment (
    student_id INTEGER,
    house_id INTEGER,
    FOREIGN KEY (student_id) REFERENCES student(id),
    FOREIGN KEY (house_id) REFERENCES house(id)
);

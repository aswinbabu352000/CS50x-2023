SELECT title
/* Movies of bradly cooper */
FROM (  SELECT title , movie_id FROM movies
        JOIN stars ON movies.id = stars.movie_id
        WHERE stars.person_id = (SELECT id FROM people WHERE name = "Bradley Cooper"))
        AS bradly_cooper
/* Movies of jennifer lawrence */
JOIN (  SELECT movie_id FROM movies
        JOIN stars ON movies.id = stars.movie_id
        WHERE stars.person_id = (SELECT id FROM people WHERE name = "Jennifer Lawrence"))
        AS jennifer_lawrence
ON bradly_cooper.movie_id = jennifer_lawrence.movie_id;

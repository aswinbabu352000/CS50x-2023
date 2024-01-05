SELECT name
/* Kevin Bacon Movies*/
FROM(   SELECT id, title
        FROM movies
        JOIN stars ON movies.id = stars.movie_id
        WHERE stars.person_id = (SELECT id FROM people WHERE name = "Kevin Bacon")
) AS kevin_bacon_movies

JOIN stars ON kevin_bacon_movies.id = stars.movie_id
JOIN people ON stars.person_id = people.id
WHERE name != "Kevin Bacon";
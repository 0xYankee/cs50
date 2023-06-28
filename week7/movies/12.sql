SELECT title
FROM movies AS m
JOIN stars AS s, people AS p
ON m.id = s.movie_id
AND s.person_id = p.id
WHERE p.name = 'Johnny Depp'
AND m.id IN (
    SELECT movie_id
    FROM stars
    WHERE person_id IN (
        SELECT id
        FROM people
        WHERE name = 'Helena Bonham Carter'
    )
)
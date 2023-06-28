SELECT DISTINCT name
FROM people AS p
JOIN movies AS m, stars AS s
ON p.id = s.person_id
AND s.movie_id = m.id
WHERE m.id IN (
    SELECT movie_id
    FROM stars
    WHERE person_id IN (
        SELECT id
        FROM people
        WHERE name = 'Kevin Bacon'
        AND birth = '1958'
    )
)
AND name IS NOT 'Kevin Bacon'
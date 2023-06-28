SELECT name
FROM people AS p
JOIN directors AS d, movies AS m, ratings AS r
ON p.id = d.person_id
AND d.movie_id = m.id
AND m.id = r.movie_id
WHERE rating >= '9.0'
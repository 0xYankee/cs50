SELECT DISTINCT name
FROM people AS p
JOIN stars AS s, movies AS m
ON m.id = s.movie_id
AND s.person_id = p.id
WHERE m.year = '2004'
ORDER BY birth
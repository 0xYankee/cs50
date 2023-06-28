SELECT title
FROM movies AS m
JOIN people AS p, ratings AS r, stars AS s
ON m.id = r.movie_id
AND m.id = s.movie_id
AND s.person_id = p.id
WHERE p.name = 'Chadwick Boseman'
ORDER BY r.rating DESC
LIMIT 5
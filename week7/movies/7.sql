SELECT title, rating
FROM movies
JOIN ratings
ON id = movie_id
WHERE year = '2010'
AND rating IS NOT '0'
ORDER BY rating DESC, title
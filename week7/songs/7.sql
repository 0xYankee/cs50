WITH person AS (
SELECT id AS person_id
FROM artists
WHERE name = 'Drake'
)

SELECT AVG(energy)
FROM songs
JOIN person
WHERE artist_id = person_id
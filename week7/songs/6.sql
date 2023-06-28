WITH person AS (
SELECT id AS person_id
FROM artists
WHERE name = 'Post Malone'
)

SELECT name
FROM songs
JOIN person
WHERE artist_id = person_id
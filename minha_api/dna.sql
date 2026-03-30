CREATE TABLE dna(
    id INTEGER PRIMARY KEY AUTO_INCREMENT,
    sequence TEXT UNIQUE,
    is_simian INTEGER
)


SELECT * FROM dna
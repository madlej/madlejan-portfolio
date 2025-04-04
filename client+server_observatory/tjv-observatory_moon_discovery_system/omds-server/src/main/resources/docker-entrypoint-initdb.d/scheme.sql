CREATE TABLE IF NOT EXISTS planet (
    name_planet VARCHAR(255) NOT NULL PRIMARY KEY,
    type_planet VARCHAR(255) NOT NULL,
    mass INTEGER
    );

CREATE TABLE IF NOT EXISTS astronomer (
    astronomer_id BIGINT NOT NULL PRIMARY KEY,
    real_name VARCHAR(255) NOT NULL,
    email VARCHAR(255) NOT NULL
    );

CREATE TABLE IF NOT EXISTS moon (
    moon_id BIGINT NOT NULL PRIMARY KEY,
    discovery_date DATE NOT NULL,
    name_moon VARCHAR(255),
    diameter INTEGER,
    planet_name_planet VARCHAR(255),
    CONSTRAINT fk_planet FOREIGN KEY (planet_name_planet) REFERENCES planet(name_planet)
    );

CREATE TABLE IF NOT EXISTS moon_astronomers (
    moon_moon_id BIGINT,
    PRIMARY KEY (moon_moon_id, astronomers_astronomer_id),
    astronomers_astronomer_id BIGINT,
    CONSTRAINT fk_moon FOREIGN KEY (moon_moon_id) REFERENCES moon(moon_id) ON DELETE CASCADE,
    CONSTRAINT fk_astronomer FOREIGN KEY (astronomers_astronomer_id) REFERENCES astronomer(astronomer_id)
    );


INSERT INTO planet (name_planet, type_planet) VALUES ('Earth', 'Terrastrial');
INSERT INTO planet (name_planet, type_planet) VALUES ('Jupiter', 'Gas');

INSERT INTO astronomer (astronomer_id, real_name, email) VALUES ('22', 'Jan Madle', 'jan.madle@moonObs.cz');
INSERT INTO astronomer (astronomer_id, real_name, email) VALUES ('134', 'Daniela Novakova', 'daniela.novakova@moonObs.cz');

INSERT INTO moon (moon_id, discovery_date, name_moon, diameter, planet_name_planet) VALUES (231, '1930-02-18', 'Europa', null, 'Jupiter');
INSERT INTO moon (moon_id, discovery_date, name_moon, diameter, planet_name_planet) VALUES (232311, '1990-01-01', 'Thesis', null, 'Earth');
INSERT INTO moon (moon_id, discovery_date, name_moon, diameter, planet_name_planet) VALUES (1, '1910-01-01', 'Thesis', null, 'Earth');
INSERT INTO moon (moon_id, discovery_date, name_moon, diameter, planet_name_planet) VALUES (2, '2004-06-01', 'Thesis', null, 'Jupiter');
INSERT INTO moon (moon_id, discovery_date, name_moon, diameter, planet_name_planet) VALUES (3, '1971-12-12', 'Thesis', null, 'Earth');
INSERT INTO moon (moon_id, discovery_date, name_moon, diameter, planet_name_planet) VALUES (4, '1990-01-09', 'Thesis', null, 'Jupiter');

INSERT INTO moon_astronomers (moon_moon_id, astronomers_astronomer_id) VALUES (231, 22);
INSERT INTO moon_astronomers (moon_moon_id, astronomers_astronomer_id) VALUES (1, 22);
INSERT INTO moon_astronomers (moon_moon_id, astronomers_astronomer_id) VALUES (2, 22);
INSERT INTO moon_astronomers (moon_moon_id, astronomers_astronomer_id) VALUES (3, 22);
INSERT INTO moon_astronomers (moon_moon_id, astronomers_astronomer_id) VALUES (4, 22);
INSERT INTO moon_astronomers (moon_moon_id, astronomers_astronomer_id) VALUES (232311, 134);

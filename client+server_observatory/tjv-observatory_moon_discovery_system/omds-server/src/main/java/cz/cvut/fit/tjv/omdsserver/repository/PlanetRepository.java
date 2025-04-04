package cz.cvut.fit.tjv.omdsserver.repository;

import cz.cvut.fit.tjv.omdsserver.domain.Planet;
import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.data.repository.CrudRepository;

public interface PlanetRepository extends CrudRepository<Planet, String> {
}
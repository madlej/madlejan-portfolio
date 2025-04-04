package cz.cvut.fit.tjv.omdsserver.repository;

import cz.cvut.fit.tjv.omdsserver.domain.Astronomer;
import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.data.repository.CrudRepository;

public interface AstronomerRepository extends CrudRepository<Astronomer, Long> {
}
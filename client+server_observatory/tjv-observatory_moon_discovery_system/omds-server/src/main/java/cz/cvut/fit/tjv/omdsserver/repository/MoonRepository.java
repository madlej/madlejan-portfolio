package cz.cvut.fit.tjv.omdsserver.repository;

import cz.cvut.fit.tjv.omdsserver.domain.Moon;
import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.data.jpa.repository.Query;
import org.springframework.data.repository.CrudRepository;

import java.time.LocalDate;
import java.util.List;

public interface MoonRepository extends CrudRepository<Moon, Long> {

    @Query("SELECT m FROM Moon m JOIN m.astronomers a WHERE a.astronomerId = :astronomerid AND m.discoveryDate > :discoveryDate")
    List<Moon> getMoonsDiscoverAfterDate(Long astronomerid, LocalDate discoveryDate);
}


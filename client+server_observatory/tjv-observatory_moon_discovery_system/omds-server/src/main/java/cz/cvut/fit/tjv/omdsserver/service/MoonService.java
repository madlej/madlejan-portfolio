package cz.cvut.fit.tjv.omdsserver.service;

import cz.cvut.fit.tjv.omdsserver.controller.dto.MoonDto;
import cz.cvut.fit.tjv.omdsserver.domain.Moon;
import jakarta.persistence.EntityNotFoundException;

import java.time.LocalDate;
import java.util.List;

public interface MoonService extends CrudService<Moon, Long> {
    public Moon addMoonToAstronomerAndPlanet(Long astronomerid, Moon moon) throws EntityNotFoundException;

    public Moon addAstronomerToMoon(Long moonid, Long astronomerid) throws EntityNotFoundException;

    public void removeAstronomerFromMoon(Long moonid, Long astronomerid) throws EntityNotFoundException;

    public List<Moon> getMoonsDiscoverAfterDate(Long astronomerid, LocalDate discoveryDate) throws EntityNotFoundException;

}

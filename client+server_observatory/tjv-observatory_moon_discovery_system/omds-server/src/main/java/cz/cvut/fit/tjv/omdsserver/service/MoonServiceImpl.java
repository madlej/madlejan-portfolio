package cz.cvut.fit.tjv.omdsserver.service;

import cz.cvut.fit.tjv.omdsserver.domain.Astronomer;
import cz.cvut.fit.tjv.omdsserver.domain.Moon;
import cz.cvut.fit.tjv.omdsserver.domain.Planet;
import cz.cvut.fit.tjv.omdsserver.repository.AstronomerRepository;
import cz.cvut.fit.tjv.omdsserver.repository.MoonRepository;
import cz.cvut.fit.tjv.omdsserver.repository.PlanetRepository;
import jakarta.persistence.EntityNotFoundException;
import org.springframework.data.repository.CrudRepository;
import org.springframework.stereotype.Component;
import org.springframework.stereotype.Service;

import java.time.LocalDate;
import java.util.List;

@Service
public class MoonServiceImpl extends CrudServiceImpl<Moon, Long> implements MoonService {

    private final PlanetRepository planetRepository;
    MoonRepository moonRepository;
    AstronomerRepository astronomerRepository;

    public MoonServiceImpl(MoonRepository moonRepository, AstronomerRepository astronomerRepository, PlanetRepository planetRepository) {
        this.moonRepository = moonRepository;
        this.astronomerRepository = astronomerRepository;
        this.planetRepository = planetRepository;
    }

    @Override
    public CrudRepository<Moon, Long> getCrudRepository() {return moonRepository;}

    @Override
    public Moon addMoonToAstronomerAndPlanet(Long astronomerid, Moon moon) throws EntityNotFoundException {

        Astronomer astronomer = astronomerRepository.findById(astronomerid).orElseThrow(() -> new IllegalArgumentException("Astronomer with id " + astronomerid + " does not exist"));
        Planet planet = planetRepository.findById(moon.getPlanet().getId()).orElseThrow(() -> new IllegalArgumentException("Planet with id " + moon.getPlanet().getId() + " does not exist"));

        Moon createMoon = moonRepository.save(moon);
        astronomer.getMoons().add(createMoon);
        astronomerRepository.save(astronomer);

        planet.getMoons().add(createMoon);
        planetRepository.save(planet);

        return createMoon;
    }

    @Override
    public Moon addAstronomerToMoon(Long moonid, Long astronomerid){
        Moon moon = moonRepository.findById(moonid).orElseThrow(() -> new IllegalArgumentException("Moon with id " + moonid + " does not exist"));
        Astronomer astronomer = astronomerRepository.findById(astronomerid).orElseThrow(() -> new IllegalArgumentException("Astronomer with id " + astronomerid + " does not exist"));

        moon.getAstronomers().add(astronomer);
        moonRepository.save(moon);

        return moon;
    }

    @Override
    public void removeAstronomerFromMoon(Long moonid, Long astronomerid){
        Moon moon = moonRepository.findById(moonid).orElseThrow(() -> new IllegalArgumentException("Moon with id " + moonid + " does not exist"));
        Astronomer astronomer = astronomerRepository.findById(astronomerid).orElseThrow(() -> new IllegalArgumentException("Astronomer with id " + astronomerid + " does not exist"));

        moon.getAstronomers().remove(astronomer);
        moonRepository.save(moon);

        astronomer.getMoons().remove(moon);
        astronomerRepository.save(astronomer);
    }

    @Override
    public List<Moon> getMoonsDiscoverAfterDate(Long astronomerid, LocalDate discoveryDate){
        return moonRepository.getMoonsDiscoverAfterDate(astronomerid, discoveryDate);
    };
}

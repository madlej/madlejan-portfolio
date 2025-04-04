package cz.cvut.fit.tjv.omdsserver.service;

import cz.cvut.fit.tjv.omdsserver.domain.Planet;
import cz.cvut.fit.tjv.omdsserver.repository.PlanetRepository;
import org.springframework.data.repository.CrudRepository;
import org.springframework.stereotype.Service;

@Service
public class PlanetServiceImpl extends CrudServiceImpl<Planet, String> implements PlanetService{

    PlanetRepository planetRepository;

    public PlanetServiceImpl(PlanetRepository planetRepository) {
        this.planetRepository = planetRepository;
    }

    @Override
    public CrudRepository<Planet, String> getCrudRepository() {return planetRepository;}

}

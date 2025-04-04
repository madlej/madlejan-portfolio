package cz.cvut.fit.tjv.omdsserver.controller.converter;

import cz.cvut.fit.tjv.omdsserver.controller.dto.MoonDto;
import cz.cvut.fit.tjv.omdsserver.domain.Astronomer;
import cz.cvut.fit.tjv.omdsserver.domain.Moon;
import cz.cvut.fit.tjv.omdsserver.service.AstronomerService;
import cz.cvut.fit.tjv.omdsserver.service.PlanetService;
import jakarta.persistence.EntityNotFoundException;
import org.springframework.stereotype.Component;

import java.util.Optional;

@Component
public class MoonDtoConverter implements converterDto<MoonDto, Moon> {

    private final PlanetService planetService;
    private final AstronomerService astronomerService;

    public MoonDtoConverter(PlanetService planetService, AstronomerService astronomerService) {
        this.planetService = planetService;
        this.astronomerService = astronomerService;
    }

    @Override
    public MoonDto entityToDto(Moon entity) {
        MoonDto dto = new MoonDto();
        dto.setMoonId(entity.getMoonId());
        dto.setDiscoveryDate(entity.getDiscoveryDate());
        dto.setNameMoon(entity.getNameMoon());
        dto.setDiameter(entity.getDiameter());
        dto.setAstronomersIds(entity.getAstronomers().stream().map(Astronomer::getAstronomerId).toList());
        dto.setPlanetId(entity.getPlanet().getNamePlanet());
        return dto;
    }

    @Override
    public Moon dtoToEntity(MoonDto dto) {
        Moon entity = new Moon();
        entity.setMoonId(dto.getMoonId());
        entity.setDiscoveryDate(dto.getDiscoveryDate());
        entity.setNameMoon(dto.getNameMoon());
        entity.setDiameter(dto.getDiameter());
//        entity.setAstronomers(dto.getAstronomersIds().stream().map(astronomerService::findById).flatMap(Optional::stream).toList());
        entity.setAstronomers(dto.getAstronomersIds().stream().map(id -> astronomerService.findById(id).orElseThrow(() -> new EntityNotFoundException("Astronomer with id " + id + " not found!"))).toList());
        entity.setPlanet(planetService.findById(dto.getPlanetId()).orElseThrow(() -> new EntityNotFoundException("Planet with id " + dto.getPlanetId() + " not found!")));
        return entity;
    }

}

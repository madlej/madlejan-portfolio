package cz.cvut.fit.tjv.omdsserver.controller.converter;

import cz.cvut.fit.tjv.omdsserver.controller.dto.PlanetDto;
import cz.cvut.fit.tjv.omdsserver.domain.Moon;
import cz.cvut.fit.tjv.omdsserver.domain.Planet;
import cz.cvut.fit.tjv.omdsserver.service.MoonService;
import org.springframework.stereotype.Component;

import java.util.Optional;

@Component
public class PlanetDtoConverter implements converterDto<PlanetDto, Planet> {

    private final MoonService moonService;

    public PlanetDtoConverter(MoonService moonService) {
        this.moonService = moonService;
    }

    @Override
    public PlanetDto entityToDto(Planet entity) {
        PlanetDto dto = new PlanetDto();
        dto.setNamePlanet(entity.getNamePlanet());
        dto.setTypePlanet(entity.getTypePlanet());
        dto.setMass(entity.getMass());
        dto.setMoonsIds(entity.getMoons().stream().map(Moon::getMoonId).toList());
        return dto;
    }

    @Override
    public Planet dtoToEntity(PlanetDto dto) {
        Planet entity = new Planet();
        entity.setNamePlanet(dto.getNamePlanet());
        entity.setTypePlanet(dto.getTypePlanet());
        entity.setMass(dto.getMass());
        entity.setMoons(dto.getMoonsIds().stream().map(moonService::findById).flatMap(Optional::stream).toList());
        return entity;
    }
}

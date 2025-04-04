package cz.cvut.fit.tjv.omdsserver.controller.converter;

import cz.cvut.fit.tjv.omdsserver.controller.dto.AstronomerDto;
import cz.cvut.fit.tjv.omdsserver.domain.Astronomer;
import cz.cvut.fit.tjv.omdsserver.domain.Moon;
import cz.cvut.fit.tjv.omdsserver.service.AstronomerService;
import cz.cvut.fit.tjv.omdsserver.service.MoonService;
import org.springframework.stereotype.Component;
import java.util.Optional;


import java.util.stream.Collectors;
import java.util.stream.StreamSupport;

@Component
public class AstronomerDtoConverter implements converterDto<AstronomerDto, Astronomer> {

    private final MoonService moonService;

    public AstronomerDtoConverter(MoonService moonService) {
        this.moonService = moonService;
    }


    @Override
    public AstronomerDto entityToDto(Astronomer entity) {
        AstronomerDto dto = new AstronomerDto();
        dto.setAstronomerId(entity.getAstronomerId());
        dto.setRealName(entity.getRealName());
        dto.setEmail(entity.getEmail());
        dto.setMoonsIds(entity.getMoons().stream().map(Moon::getMoonId).toList());
        return dto;
    }

    @Override
    public Astronomer dtoToEntity(AstronomerDto dto) {
        Astronomer entity = new Astronomer();
        entity.setAstronomerId(dto.getAstronomerId());
        entity.setRealName(dto.getRealName());
        entity.setEmail(dto.getEmail());
        entity.setMoons(dto.getMoonsIds().stream().map(moonService::findById).flatMap(Optional::stream).toList());
        return entity;
    }

}

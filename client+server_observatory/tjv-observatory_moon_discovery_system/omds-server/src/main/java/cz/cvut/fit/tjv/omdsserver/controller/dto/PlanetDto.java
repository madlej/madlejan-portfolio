package cz.cvut.fit.tjv.omdsserver.controller.dto;

import cz.cvut.fit.tjv.omdsserver.domain.Moon;
import jakarta.persistence.Column;
import jakarta.persistence.OneToMany;
import lombok.Getter;
import lombok.Setter;

import java.util.LinkedList;
import java.util.List;

@Getter
@Setter
public class PlanetDto {
    private String namePlanet;
    private String typePlanet;
    private Integer mass;
    private List<Long> moonsIds;
}

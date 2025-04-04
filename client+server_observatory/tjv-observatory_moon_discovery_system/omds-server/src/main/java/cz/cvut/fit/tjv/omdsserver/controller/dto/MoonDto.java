package cz.cvut.fit.tjv.omdsserver.controller.dto;

import cz.cvut.fit.tjv.omdsserver.domain.Astronomer;
import jakarta.persistence.Column;
import lombok.Getter;
import lombok.Setter;

import java.time.LocalDate;
import java.util.LinkedList;
import java.util.List;

@Getter
@Setter
public class MoonDto {
    private Long moonId;
    private LocalDate discoveryDate;
    private String nameMoon;
    private Integer diameter;
    private List<Long> astronomersIds;
    private String planetId;
}

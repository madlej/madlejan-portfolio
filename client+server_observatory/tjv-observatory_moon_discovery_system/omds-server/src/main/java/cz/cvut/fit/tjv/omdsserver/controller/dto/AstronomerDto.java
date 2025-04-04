package cz.cvut.fit.tjv.omdsserver.controller.dto;

import lombok.Getter;
import lombok.Setter;

import java.util.List;

@Getter
@Setter
public class AstronomerDto {
        private Long astronomerId;
        private String realName;
        private String email;
        private List<Long> moonsIds;
}

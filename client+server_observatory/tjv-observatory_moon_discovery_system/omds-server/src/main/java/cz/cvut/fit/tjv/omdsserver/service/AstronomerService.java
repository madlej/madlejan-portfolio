package cz.cvut.fit.tjv.omdsserver.service;

import cz.cvut.fit.tjv.omdsserver.domain.Astronomer;
import cz.cvut.fit.tjv.omdsserver.domain.Moon;
import jakarta.persistence.EntityNotFoundException;
import org.springframework.format.annotation.DateTimeFormat;
import org.springframework.web.bind.annotation.RequestParam;

import java.time.LocalDate;
import java.util.List;

public interface AstronomerService extends CrudService<Astronomer, Long> {

    public void deleteAstronomer(Long id) throws EntityNotFoundException;
}

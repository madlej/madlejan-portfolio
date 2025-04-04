package cz.cvut.fit.tjv.omdsserver.service;

import cz.cvut.fit.tjv.omdsserver.domain.Astronomer;
import cz.cvut.fit.tjv.omdsserver.controller.dto.AstronomerDto;
import cz.cvut.fit.tjv.omdsserver.domain.Moon;
import cz.cvut.fit.tjv.omdsserver.repository.AstronomerRepository;
import cz.cvut.fit.tjv.omdsserver.repository.MoonRepository;
import jakarta.persistence.EntityNotFoundException;
import org.springframework.data.repository.CrudRepository;
import org.springframework.stereotype.Service;

import java.time.LocalDate;
import java.util.List;

@Service
public class AstronomerServiceImp extends CrudServiceImpl<Astronomer, Long> implements AstronomerService{
    private AstronomerRepository astronomerRepository;
    private MoonRepository moonRepository;

    public AstronomerServiceImp(AstronomerRepository astronomerRepository, MoonRepository moonRepository) {
        this.astronomerRepository = astronomerRepository;
        this.moonRepository = moonRepository;
    }

    @Override
    protected CrudRepository<Astronomer, Long> getCrudRepository() {return astronomerRepository;};

    @Override
    public void deleteAstronomer(Long astronomerid) {
        Astronomer astronomer = astronomerRepository.findById(astronomerid).orElseThrow(() -> new EntityNotFoundException("Astronomer with id " + astronomerid + " does not exist"));

        for(Moon moon : astronomer.getMoons()) {
            moon.getAstronomers().remove(astronomer);
            moonRepository.save(moon);
        }

        astronomerRepository.deleteById(astronomerid);
    }


}

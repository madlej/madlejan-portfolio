package cz.cvut.fit.tjv.omdsserver.service;

import cz.cvut.fit.tjv.omdsserver.domain.Astronomer;
import cz.cvut.fit.tjv.omdsserver.domain.Moon;
import cz.cvut.fit.tjv.omdsserver.repository.AstronomerRepository;
import cz.cvut.fit.tjv.omdsserver.repository.MoonRepository;
import jakarta.persistence.EntityNotFoundException;
import org.assertj.core.api.Assertions;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;
import org.mockito.InjectMocks;
import org.mockito.Mock;
import org.mockito.Mockito;
import org.mockito.MockitoAnnotations;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.boot.test.context.SpringBootTest;

import java.util.ArrayList;
import java.util.HashSet;
import java.util.Optional;

import static org.junit.jupiter.api.Assertions.assertThrows;
import static org.mockito.Mockito.*;

@SpringBootTest
public class AstronomerServiceImplTest {
    @InjectMocks
    private AstronomerServiceImp astronomerService;

    @Mock
    private AstronomerRepository astronomerRepository;

    @Mock
    private MoonRepository moonRepository;

    @BeforeEach
    void setUp() {
        MockitoAnnotations.openMocks(this);
    }

    @Test
    void deleteAstronomer_successful() {
        Astronomer astronomer = new Astronomer();
        astronomer.setAstronomerId(1L);
        astronomer.setRealName("John Doe");
        astronomer.setMoons(new ArrayList<>());

        Moon moon = new Moon();
        moon.setMoonId(2L);
        moon.setNameMoon("Luna");
        moon.setAstronomers(new ArrayList<>());
        moon.getAstronomers().add(astronomer);

        astronomer.getMoons().add(moon);

        Mockito.when(astronomerRepository.findById(1L)).thenReturn(Optional.of(astronomer));
        Mockito.when(moonRepository.findById(2L)).thenReturn(Optional.of(moon));

        astronomerService.deleteAstronomer(1L);

        Mockito.verify(astronomerRepository, times(1)).deleteById(1L);
        Assertions.assertThat(moon.getAstronomers()).doesNotContain(astronomer);
        Mockito.verify(moonRepository, times(1)).save(moon);
    }

    @Test
    void deleteAstronomer_notFound() {
        Long invalidAstronomerId = 999L;

        Mockito.when(astronomerRepository.findById(invalidAstronomerId)).thenReturn(Optional.empty());

        Exception exception = assertThrows(EntityNotFoundException.class, () -> {
            astronomerService.deleteAstronomer(invalidAstronomerId);
        });

        Assertions.assertThat(exception.getMessage()).isEqualTo("Astronomer with id 999 does not exist");
        Mockito.verify(astronomerRepository, never()).deleteById(anyLong());
    }
}


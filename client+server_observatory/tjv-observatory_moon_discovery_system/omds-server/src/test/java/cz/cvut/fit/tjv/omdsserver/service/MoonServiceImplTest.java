package cz.cvut.fit.tjv.omdsserver.service;

import cz.cvut.fit.tjv.omdsserver.domain.Astronomer;
import cz.cvut.fit.tjv.omdsserver.domain.Moon;
import cz.cvut.fit.tjv.omdsserver.repository.AstronomerRepository;
import cz.cvut.fit.tjv.omdsserver.repository.MoonRepository;
import org.junit.jupiter.api.Assertions;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;
import org.mockito.InjectMocks;
import org.mockito.Mock;
import org.mockito.Mockito;
import org.mockito.MockitoAnnotations;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.boot.test.context.SpringBootTest;

import java.util.Optional;

import static org.junit.jupiter.api.Assertions.*;
import static org.mockito.Mockito.*;

@SpringBootTest
public class MoonServiceImplTest {
    //--- UNIT TEST ---
    @InjectMocks
    private MoonServiceImpl moonService;
    @Mock
    private MoonRepository moonRepository;
    @Mock
    private AstronomerRepository astronomerRepository;

    @BeforeEach
    void setUp() {
        MockitoAnnotations.openMocks(this);
    }

    @Test
    void addAstronomerToMoon_successful() {
        Long moonid = 1L;
        Long astronomerid = 2L;

        Moon mockMoon = new Moon();
        mockMoon.setMoonId(moonid);

        Astronomer mockAstronomer = new Astronomer();
        mockAstronomer.setAstronomerId(astronomerid);

        Mockito.when(moonRepository.findById(moonid)).thenReturn(Optional.of(mockMoon));
        Mockito.when(astronomerRepository.findById(astronomerid)).thenReturn(Optional.of(mockAstronomer));

        Moon updatedMoon = moonService.addAstronomerToMoon(moonid, astronomerid);

        Assertions.assertNotNull(updatedMoon);
        Assertions.assertTrue(updatedMoon.getAstronomers().contains(mockAstronomer));

        Mockito.verify(moonRepository).findById(moonid);
        Mockito.verify(astronomerRepository).findById(astronomerid);
        Mockito.verify(moonRepository).save(mockMoon);
    }

    @Test
    void addAstronomerToMoon_moonNotFound() {
        Long moonid = 1L;
        Long astronomerid = 2L;

        Mockito.when(moonRepository.findById(moonid)).thenReturn(Optional.empty());

        Exception exception = assertThrows(IllegalArgumentException.class, () -> {
            moonService.addAstronomerToMoon(moonid, astronomerid);
        });
        Assertions.assertEquals("Moon with id " + moonid + " does not exist", exception.getMessage());

        Mockito.verify(astronomerRepository, never()).findById(anyLong());
    }

    @Test
    void addAstronomerToMoon_astronomerNotFound() {
        Long moonid = 1L;
        Long astronomerid = 2L;

        Moon mockMoon = new Moon();
        mockMoon.setMoonId(moonid);

        Mockito.when(moonRepository.findById(moonid)).thenReturn(Optional.of(mockMoon));
        Mockito.when(astronomerRepository.findById(astronomerid)).thenReturn(Optional.empty());

        Exception exception = assertThrows(IllegalArgumentException.class, () -> {
            moonService.addAstronomerToMoon(moonid, astronomerid);
        });
        Assertions.assertEquals("Astronomer with id " + astronomerid + " does not exist", exception.getMessage());

        Mockito.verify(moonRepository).findById(moonid);
        Mockito.verify(astronomerRepository).findById(astronomerid);
        Mockito.verify(moonRepository, never()).save(any(Moon.class));
    }


}

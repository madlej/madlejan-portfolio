package cz.cvut.fit.tjv.omdsserver.controller;

import cz.cvut.fit.tjv.omdsserver.controller.converter.converterDto;
import cz.cvut.fit.tjv.omdsserver.controller.dto.PlanetDto;
import cz.cvut.fit.tjv.omdsserver.domain.Planet;
import cz.cvut.fit.tjv.omdsserver.service.PlanetService;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;
import org.mockito.Mockito;
import org.mockito.MockitoAnnotations;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.boot.test.autoconfigure.web.servlet.WebMvcTest;
import org.springframework.boot.test.mock.mockito.MockBean;
import org.springframework.http.MediaType;
import org.springframework.test.web.servlet.MockMvc;
import org.springframework.test.web.servlet.request.MockMvcRequestBuilders;

import static org.mockito.ArgumentMatchers.any;
import static org.mockito.Mockito.when;
import static org.springframework.test.web.servlet.result.MockMvcResultMatchers.jsonPath;
import static org.springframework.test.web.servlet.result.MockMvcResultMatchers.status;

@WebMvcTest(PlanetController.class)
public class PlanetControllerTest {

    @Autowired
    private MockMvc mockMvc;
    @MockBean
    private PlanetService planetService;
    @MockBean
    private converterDto<PlanetDto, Planet> planetDtoConverter;
    @BeforeEach
    void setUp() {
        MockitoAnnotations.openMocks(this);
    }

    @Test
    void createPlanet_successful() throws Exception {
        PlanetDto planetDto = new PlanetDto();
        planetDto.setNamePlanet("Earth");

        Planet planet = new Planet();
        planet.setNamePlanet("Earth");

        Mockito.when(planetService.findById("Earth")).thenReturn(java.util.Optional.empty());
        Mockito.when(planetDtoConverter.dtoToEntity(any(PlanetDto.class))).thenReturn(planet);
        Mockito.when(planetService.create(any(Planet.class))).thenReturn(planet);
        Mockito.when(planetDtoConverter.entityToDto(any(Planet.class))).thenReturn(planetDto);

        mockMvc.perform(MockMvcRequestBuilders.post("/planets").contentType(MediaType.APPLICATION_JSON).content("{\"namePlanet\" : \"Earth\"}"))
                .andExpect(status().isOk()).andExpect(jsonPath("$.namePlanet").value("Earth"));
    }

    @Test
    void createPlanet_conflict() throws Exception {
        PlanetDto planetDto = new PlanetDto();
        planetDto.setNamePlanet("Earth");

        Planet planet = new Planet();
        planet.setNamePlanet("Earth");

        when(planetService.findById("Earth")).thenReturn(java.util.Optional.of(planet));

        mockMvc.perform(MockMvcRequestBuilders.post("/planets").contentType(MediaType.APPLICATION_JSON)
                        .content("{\"namePlanet\" : \"Earth\"}")).andExpect(status().isConflict());
    }
}

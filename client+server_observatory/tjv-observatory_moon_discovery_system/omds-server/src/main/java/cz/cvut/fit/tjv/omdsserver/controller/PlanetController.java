package cz.cvut.fit.tjv.omdsserver.controller;

import cz.cvut.fit.tjv.omdsserver.controller.converter.converterDto;
import cz.cvut.fit.tjv.omdsserver.controller.dto.PlanetDto;
import cz.cvut.fit.tjv.omdsserver.domain.Planet;
import cz.cvut.fit.tjv.omdsserver.repository.PlanetRepository;
import cz.cvut.fit.tjv.omdsserver.service.PlanetService;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.responses.ApiResponse;
import io.swagger.v3.oas.annotations.responses.ApiResponses;
import jakarta.persistence.EntityNotFoundException;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.dao.DataIntegrityViolationException;
import org.springframework.web.bind.annotation.*;

import java.util.List;
import java.util.stream.Collectors;
import java.util.stream.StreamSupport;

@RestController
@CrossOrigin
@RequestMapping("planets")
public class PlanetController {

    private final PlanetService planetService;
    private final converterDto<PlanetDto, Planet> planetDtoConverter;

    @Autowired
    public PlanetController(PlanetService planetService, converterDto<PlanetDto, Planet> planetDtoConverter) {
        this.planetService = planetService;
        this.planetDtoConverter = planetDtoConverter;
    }

    @PostMapping
    @Operation(summary = "Create a new planet")
    @ApiResponses({
            @ApiResponse(responseCode = "200", description = "Successfully created new planet"),
            @ApiResponse(responseCode = "409", description = "Planet with this id already exists")
    })
    public PlanetDto createPlanet(@RequestBody PlanetDto planetDto) {
        if(planetService.findById(planetDto.getNamePlanet()).isPresent()) throw new DataIntegrityViolationException("Planet with id " + planetDto.getNamePlanet() + " already exists");
        Planet planet = planetDtoConverter.dtoToEntity(planetDto);
        Planet createdPlanet = planetService.create(planet);
        return planetDtoConverter.entityToDto(createdPlanet);
    }

    @GetMapping("/{planetid}")
    @Operation(summary = "Get a planet by ID")
    @ApiResponses({
            @ApiResponse(responseCode = "200", description = "Successfully get planet"),
            @ApiResponse(responseCode = "404", description = "Planet with this id does not exists")
    })
    public PlanetDto getPlanet(@PathVariable String planetid) {
        return planetService.findById(planetid).map(planetDtoConverter::entityToDto).orElseThrow(() -> new EntityNotFoundException("Astronomer with id " + planetid + " does not exist"));
    }

    @GetMapping
    @Operation(summary = "Get all planets")
    @ApiResponses({
            @ApiResponse(responseCode = "200", description = "Successfully get all planets"),
    })
    public Iterable<PlanetDto> getAllPlanets() {
        return StreamSupport.stream(planetService.findAll().spliterator(), false).map(planetDtoConverter::entityToDto).collect(Collectors.toList());
    }

    @PutMapping("/{planetid}")
    @Operation(summary = "Update a planet by ID")
    @ApiResponses({
            @ApiResponse(responseCode = "200", description = "Successfully updated planet"),
            @ApiResponse(responseCode = "404", description = "Planet with this id does not exists")
    })
    public PlanetDto updatePlanet(@PathVariable String planetid, @RequestBody PlanetDto planetDto) {
        if(planetService.findById(planetid).isEmpty()) throw new EntityNotFoundException("Planet with id " + planetid + " does not exist");
        Planet planetToUpdate = planetDtoConverter.dtoToEntity(planetDto);
        Planet updatedPlanet = planetService.update(planetid, planetToUpdate);
        return planetDtoConverter.entityToDto(updatedPlanet);
    }

    @DeleteMapping("/{planetid}")
    @Operation(summary = "Delete existing planet by ID")
    @ApiResponses({
            @ApiResponse(responseCode = "200", description = "Successfully deleted planet"),
            @ApiResponse(responseCode = "404", description = "Planet with this id does not exists")
    })
    public void deletePlanet(@PathVariable String planetid) {
        if(planetService.findById(planetid).isEmpty()) throw new EntityNotFoundException("Planet with id " + planetid + " does not exist");
        planetService.deleteById(planetid);
    }



}

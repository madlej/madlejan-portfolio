package cz.cvut.fit.tjv.omdsserver.controller;

import cz.cvut.fit.tjv.omdsserver.controller.converter.converterDto;
import cz.cvut.fit.tjv.omdsserver.controller.dto.AstronomerDto;
import cz.cvut.fit.tjv.omdsserver.controller.dto.MoonDto;
import cz.cvut.fit.tjv.omdsserver.controller.dto.PlanetDto;
import cz.cvut.fit.tjv.omdsserver.domain.Astronomer;
import cz.cvut.fit.tjv.omdsserver.domain.Moon;
import cz.cvut.fit.tjv.omdsserver.domain.Planet;
import cz.cvut.fit.tjv.omdsserver.service.AstronomerService;
import cz.cvut.fit.tjv.omdsserver.service.MoonService;
import cz.cvut.fit.tjv.omdsserver.service.PlanetService;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.responses.ApiResponse;
import io.swagger.v3.oas.annotations.responses.ApiResponses;
import jakarta.persistence.EntityNotFoundException;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.dao.DataIntegrityViolationException;
import org.springframework.web.bind.annotation.*;

import java.util.stream.Collectors;
import java.util.stream.StreamSupport;

@RestController
@CrossOrigin
@RequestMapping("moons")
public class MoonController {

    private final MoonService moonService;
    private final converterDto<MoonDto, Moon> moonDtoConverter;

    @Autowired
    public MoonController(MoonService moonService, converterDto<MoonDto, Moon> moonDtoConverter) {
        this.moonService = moonService;
        this.moonDtoConverter = moonDtoConverter;
    }

    @PostMapping
    @Operation(summary = "Create a new moon")
    @ApiResponses({
            @ApiResponse(responseCode = "200", description = "Successfully created new moon"),
            @ApiResponse(responseCode = "409", description = "Moon with this id already exists")
    })
    public MoonDto createMoon(@RequestBody MoonDto moonDto) {
        if(moonService.findById(moonDto.getMoonId()).isPresent()) throw new DataIntegrityViolationException("Moon with id " + moonDto.getMoonId() + " already exists");
        Moon moon = moonDtoConverter.dtoToEntity(moonDto);
        Moon createdMoon = moonService.create(moon);
        return moonDtoConverter.entityToDto(createdMoon);
    }

    @GetMapping("/{moonid}")
    @Operation(summary = "Get a moon by ID")
    @ApiResponses({
            @ApiResponse(responseCode = "200", description = "Successfully get moon"),
            @ApiResponse(responseCode = "404", description = "Moon with this id does not exists")
    })
    public MoonDto getMoon(@PathVariable Long moonid) {
        return moonService.findById(moonid).map(moonDtoConverter::entityToDto).orElseThrow(() -> new EntityNotFoundException("Moon with id " + moonid + " does not exist"));
    }

    @GetMapping
    @Operation(summary = "Get all moons")
    @ApiResponses({
            @ApiResponse(responseCode = "200", description = "Successfully get all moons"),
    })
    public Iterable<MoonDto> getAllMoons() {
        return StreamSupport.stream(moonService.findAll().spliterator(), false).map(moonDtoConverter::entityToDto)
                .collect(Collectors.toList());
    }

    @PutMapping("/{moonid}")
    @Operation(summary = "Update moon by ID")
    @ApiResponses({
            @ApiResponse(responseCode = "200", description = "Successfully updated moon"),
            @ApiResponse(responseCode = "404", description = "Moon with this id does not exists")
    })
    public MoonDto updateMoon(@PathVariable Long moonid, @RequestBody MoonDto moonDto) {
        if(moonService.findById(moonid).isEmpty()) throw new EntityNotFoundException("Moon with id " + moonid + " does not exist");
        Moon moonToUpdate = moonDtoConverter.dtoToEntity(moonDto);
        Moon updatedMoon = moonService.update(moonid, moonToUpdate);
        return moonDtoConverter.entityToDto(updatedMoon);
    }

    @DeleteMapping("/{moonid}")
    @Operation(summary = "Delete existing moon by ID")
    @ApiResponses({
            @ApiResponse(responseCode = "200", description = "Successfully deleted moon"),
            @ApiResponse(responseCode = "404", description = "Moon with this id does not exists")
    })
    public void deleteMoon(@PathVariable Long moonid) {
        if(moonService.findById(moonid).isEmpty()) throw new EntityNotFoundException("Moon with id " + moonid + " does not exist");
        moonService.deleteById(moonid);
    }

    @PostMapping("/{moonid}/astronomers/{astronomerid}")
    @Operation(summary = "Create association (moon discovered by astronomer) between moon and astronomer")
    @ApiResponses({
            @ApiResponse(responseCode = "200", description = "Successfully created association between moon and astronomer"),
    })
    public MoonDto addAstronomerToMoon(@PathVariable Long moonid, @PathVariable Long astronomerid) {
        Moon moon = moonService.addAstronomerToMoon(moonid, astronomerid);
        return moonDtoConverter.entityToDto(moon);
    }

    @DeleteMapping("/{moonid}/astronomers/{astronomerid}")
    @Operation(summary = "Delete association (moon discovered by astronomer) between moon and astronomer")
    @ApiResponses({
            @ApiResponse(responseCode = "200", description = "Successfully removed astronomer from moon"),
    })
    public void removeAstronomerFromMoon(@PathVariable Long moonid, @PathVariable Long astronomerid) {
        moonService.removeAstronomerFromMoon(moonid, astronomerid);
    }
}

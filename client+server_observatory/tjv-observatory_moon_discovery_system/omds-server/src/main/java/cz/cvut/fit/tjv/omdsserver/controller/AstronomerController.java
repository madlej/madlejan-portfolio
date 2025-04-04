package cz.cvut.fit.tjv.omdsserver.controller;

import cz.cvut.fit.tjv.omdsserver.controller.converter.converterDto;
import cz.cvut.fit.tjv.omdsserver.controller.dto.MoonDto;
import cz.cvut.fit.tjv.omdsserver.controller.dto.PlanetDto;
import cz.cvut.fit.tjv.omdsserver.domain.Astronomer;
import cz.cvut.fit.tjv.omdsserver.controller.dto.AstronomerDto;
import cz.cvut.fit.tjv.omdsserver.domain.Moon;
import cz.cvut.fit.tjv.omdsserver.domain.Planet;
import cz.cvut.fit.tjv.omdsserver.service.AstronomerService;
import cz.cvut.fit.tjv.omdsserver.service.AstronomerServiceImp;
import cz.cvut.fit.tjv.omdsserver.service.MoonService;
import cz.cvut.fit.tjv.omdsserver.service.PlanetService;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.responses.ApiResponse;
import io.swagger.v3.oas.annotations.responses.ApiResponses;
import jakarta.persistence.EntityNotFoundException;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.dao.DataIntegrityViolationException;
import org.springframework.format.annotation.DateTimeFormat;
import org.springframework.web.bind.annotation.*;

import java.time.LocalDate;
import java.util.List;
import java.util.stream.Collectors;
import java.util.stream.StreamSupport;

@RestController
@CrossOrigin
@RequestMapping("astronomers")
public class AstronomerController {

    private final AstronomerService astronomerService;
    private final MoonService moonService;
    private final converterDto<AstronomerDto, Astronomer> astronomerDtoConverter;
    private final converterDto<MoonDto, Moon> moonDtoConverter;

    @Autowired
    public AstronomerController(AstronomerService astronomerService, MoonService moonService, converterDto<AstronomerDto, Astronomer> astronomerDtoConverter, converterDto<MoonDto, Moon> moonDtoConverter) {
        this.astronomerService = astronomerService;
        this.moonService = moonService;
        this.astronomerDtoConverter = astronomerDtoConverter;
        this.moonDtoConverter = moonDtoConverter;
    }

    @PostMapping
    @Operation(summary = "Create a new astronomer")
    @ApiResponses({
            @ApiResponse(responseCode = "200", description = "Successfully created new astronomer"),
            @ApiResponse(responseCode = "409", description = "Astronomer with this id already exists")
    })
    public AstronomerDto createAstronomer(@RequestBody AstronomerDto astronomerDto) {
        if (astronomerService.findById(astronomerDto.getAstronomerId()).isPresent()) throw new DataIntegrityViolationException("Astronomer with id " + astronomerDto.getAstronomerId() + " already exists");
        Astronomer astronomer = astronomerDtoConverter.dtoToEntity(astronomerDto);
        Astronomer createdAstronomer = astronomerService.create(astronomer);
        return astronomerDtoConverter.entityToDto(createdAstronomer);
    }

    @GetMapping("/{astronomerid}")
    @Operation(summary = "Get an astronomer by ID")
    @ApiResponses({
            @ApiResponse(responseCode = "200", description = "Successfully get the astronomer"),
            @ApiResponse(responseCode = "404", description = "Astronomer with this id does not exists")
    })
    public AstronomerDto getAstronomer(@PathVariable Long astronomerid) {
        return astronomerService.findById(astronomerid).map(astronomerDtoConverter::entityToDto).orElseThrow(() -> new EntityNotFoundException("Astronomer with id " + astronomerid + " does not exist"));
    }

    @GetMapping
    @Operation(summary = "Get all astronomers")
    @ApiResponses({
            @ApiResponse(responseCode = "200", description = "Successfully get all astronomer"),
    })
    public Iterable<AstronomerDto> getAllAstronomers() {
        return StreamSupport.stream(astronomerService.findAll().spliterator(), false).map(astronomerDtoConverter::entityToDto).collect(Collectors.toList());
    }

    @PutMapping("/{astronomerid}")
    @Operation(summary = "Update an astronomer by ID")
    @ApiResponses({
            @ApiResponse(responseCode = "200", description = "Astronomer successfully updated"),
            @ApiResponse(responseCode = "404", description = "Astronomer with this id does not exists")
    })
    public AstronomerDto updateAstronomer(@PathVariable Long astronomerid, @RequestBody AstronomerDto astronomerDto) {
        if (astronomerService.findById(astronomerid).isEmpty())throw new EntityNotFoundException("Astronomer with id " + astronomerid + " does not exist");
        Astronomer astronomerToUpdate = astronomerDtoConverter.dtoToEntity(astronomerDto);
        Astronomer updatedAstronomer = astronomerService.update(astronomerid, astronomerToUpdate);
        return astronomerDtoConverter.entityToDto(updatedAstronomer);
    }

    @DeleteMapping("/{astronomerid}")
    @Operation(summary = "Delete an astronomer by ID")
    @ApiResponses({
            @ApiResponse(responseCode = "200", description = "Successfully deleted astronomer"),
            @ApiResponse(responseCode = "404", description = "Astronomer with this id does not exists")
    })
    public void deleteAstronomer(@PathVariable Long astronomerid) {
        if(astronomerService.findById(astronomerid).isEmpty()) throw new EntityNotFoundException("Astronomer with id " + astronomerid + " does not exist");
        astronomerService.deleteAstronomer(astronomerid);
    }

    @PostMapping("/{astronomerid}/moons")
    @Operation(summary = "Add a moon to an astronomer and planet")
    @ApiResponses({
            @ApiResponse(responseCode = "200", description = "Successfully created new moon to astronomer"),
    })
    public MoonDto addMoonToAstronomerAndPlanet(@PathVariable Long astronomerid, @RequestBody MoonDto moonDto) {
        Moon createdMoon = moonService.addMoonToAstronomerAndPlanet(astronomerid, moonDtoConverter.dtoToEntity(moonDto));
        return moonDtoConverter.entityToDto(createdMoon);
    }

    @GetMapping("/{astronomerid}/discoveryDate")
    @Operation(summary = "Get all moons discovered after a specific date by astronomer")
    @ApiResponses({
            @ApiResponse(responseCode = "200", description = "Successfully retrieved moons that were discovered before date"),
    })
    public List<MoonDto> getMoonsDiscoverAfterDate(@PathVariable Long astronomerid, @RequestParam @DateTimeFormat(iso = DateTimeFormat.ISO.DATE) LocalDate discoveryDate) {
        List<Moon> moons = moonService.getMoonsDiscoverAfterDate(astronomerid, discoveryDate);
        return moons.stream().map(moonDtoConverter::entityToDto).collect(Collectors.toList());
    }



}


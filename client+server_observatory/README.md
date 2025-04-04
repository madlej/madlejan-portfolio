# observatory_Moon_Discovery_System

The application is designed to record newly discovered moons in our planetary system. It allows astronomers to log celestial objects along with the planet they orbit. Additionally, it stores information about the astronomer (or group of astronomers) who made the discovery, along with other relevant details for the observatory.

Advanced query example:
A query that returns all moons discovered after a specified date by a specific astronomer.

## Deploy

### Server: omds-server
Databaze (docker): 
```docker compose up```

Run server: 
```./gradlew bootRun```
Documentation (swagger): http://localhost:8080/swagger-ui/index.html


### Client: omds-client
Run client: 
```npm run dev```

Application will run on: http://localhost:3000/


***


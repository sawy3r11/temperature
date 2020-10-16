package pl.dk.temperature.controller;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.HttpStatus;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.*;
import pl.dk.temperature.dto.AddTemperatureReading;
import pl.dk.temperature.dto.TemperatureReadingDTO;
import pl.dk.temperature.service.TemperatureService;

import java.time.LocalDateTime;
import java.util.List;
import java.util.Optional;

@RestController
@RequestMapping("/api/sensor/temperature")
public class TemperatureController {
    private TemperatureService temperatureService;

    @PostMapping("{sensorId}")
    public HttpStatus addReading(@PathVariable Long sensorId, @RequestBody AddTemperatureReading addTemperatureReading){
        System.out.println(String.format("Time: %s | READING: %s", LocalDateTime.now(), addTemperatureReading.toString()));
        this.temperatureService.add(sensorId, addTemperatureReading);
        return HttpStatus.CREATED;
    }

    @GetMapping("{sensorId}")
    public ResponseEntity<List<TemperatureReadingDTO>> getAll(@PathVariable Long sensorId){
        Optional<List<TemperatureReadingDTO>> optional = this.temperatureService.getAll(sensorId);
        if(optional.isPresent()){
            return new ResponseEntity<>(optional.get(), HttpStatus.OK);
        }else{
            return new ResponseEntity<>(HttpStatus.NOT_FOUND);
        }

    }

    @Autowired
    public void setTemperatureService(TemperatureService temperatureService) {
        this.temperatureService = temperatureService;
    }

}

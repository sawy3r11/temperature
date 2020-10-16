package pl.dk.temperature.service;

import org.springframework.stereotype.Service;
import pl.dk.temperature.dto.AddTemperatureReading;
import pl.dk.temperature.dto.TemperatureReadingDTO;

import java.time.LocalDateTime;
import java.util.*;

@Service
public class TemperatureService {
    private static int MAX_LIST_SIZE = 100;

    private Map<Long, List<TemperatureReadingDTO>> map;

    public TemperatureService(){
        map = new HashMap<>();
    }

    public void add(Long sensorId, AddTemperatureReading addTemperatureReading){
        List<TemperatureReadingDTO> list = this.getReadings(sensorId);
        TemperatureReadingDTO temperatureReading = this.map(LocalDateTime.now(), addTemperatureReading);
        this.addElementToList(list, temperatureReading);
    }

    public Optional<List<TemperatureReadingDTO>> getAll(Long sensorId){
        if (sensorId == null || !this.map.containsKey(sensorId)) {
            return Optional.empty();
        }else{
            return Optional.of(this.map.get(sensorId));
        }
    }

    private void addElementToList(List<TemperatureReadingDTO> list, TemperatureReadingDTO element){
        if(list.size()>MAX_LIST_SIZE){
            list.remove(MAX_LIST_SIZE-1);
        }
        list.add(0, element);
    }

    private List<TemperatureReadingDTO> getReadings(Long sensorId){
        if(!map.containsKey(sensorId)){
            map.put(sensorId, new ArrayList<>());
        }
        List<TemperatureReadingDTO> list = map.get(sensorId);
        return list;
    }

    private TemperatureReadingDTO map(LocalDateTime time, AddTemperatureReading addTemperatureReading){
        return TemperatureReadingDTO
                .builder()
                .t1(addTemperatureReading.getT1())
                .t2(addTemperatureReading.getT2())
                .t3(addTemperatureReading.getT3())
                .t4(addTemperatureReading.getT4())
                .time(time)
                .build();
    }

}

package pl.dk.temperature.dto;

import lombok.AllArgsConstructor;
import lombok.Builder;
import lombok.Data;
import lombok.NoArgsConstructor;
import org.springframework.format.annotation.DateTimeFormat;

import java.time.LocalDateTime;

@Data
@NoArgsConstructor
@AllArgsConstructor
@Builder
public class TemperatureReadingDTO {
    private LocalDateTime time;
    private Long sensorId;
    private Double t1;
    private Double t2;
    private Double t3;
    private Double t4;
}

package pl.dk.temperature.dto;

import lombok.*;

@Data
@Builder
@NoArgsConstructor
@AllArgsConstructor
@ToString
public class AddTemperatureReading {
    private Long sensorId;
    private Double t1;
    private Double t2;
    private Double t3;
    private Double t4;
}

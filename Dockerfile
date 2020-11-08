FROM openjdk:8
COPY ./target/temperature-0.0.1-SNAPSHOT.jar /usr/src/temperature/temperature.jar
WORKDIR /usr/src/temperature
CMD ["java", "-jar", "temperature.jar"]

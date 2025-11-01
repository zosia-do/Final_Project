 document.addEventListener("DOMContentLoaded", function(){
     const IO = new AdafruitIO(username, key);
    const temperatureFeed = "temperature";
    const humidityFeed = "humidity";
    const co2Feed = "co2";

    const temperatureDisplay = document.getElementById("temp-value");
    const humidityDisplay = document.getElementById("humidity-value");
    const co2Display = document.getElementById("co2-value");

    function updateTemperature() {
        IO.getData(temperatureFeed, function({ json }) {
            if (Array.isArray(json) && json.length > 0) {
                // Use the latest value (first item)
                let latest = json[0];
                let tempValue = latest.value;
                temperatureDisplay.textContent = `${tempValue} °C`;
            } else {
                temperatureDisplay.textContent = "-- °C";
            }
        });
    }

    updateTemperature();
    setInterval(updateTemperature, 60000);

});


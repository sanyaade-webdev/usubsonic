import QtQuick 1.1
import nobdy 0.1

   SubsonicBrowser {
    width: 800
    height: 480

    NobdyStream {
        id: vehicleState
        request: VehicleData.VehicleState

        onValueChanged: {
            if(vehicleState.value === 0)
                player.pause();
            else if(vehicleState.value === 1)
                player.play();
        }
    }
}
r

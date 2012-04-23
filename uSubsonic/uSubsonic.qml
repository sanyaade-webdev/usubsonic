import QtQuick 1.1
import org.subsonic 1.0
import QtMultimediaKit 1.1
import Qt.labs.particles 1.0

Rectangle {
    id: container
    width: 800
    height: 480
    color: "black"

    property variant artists: subsonic.index;
    property variant songs: subsonic.songs;

    SubsonicModel {
        id: subsonic

        Component.onCompleted: {
            subsonic.connect();
        }
    }

    Audio {
         id: player
         property variant nowPlaying
         property int index
         function playSong( i )
         {
             index = i;

             if(index > songs.count)
             {
                 console.log("ERROR: song index out of range")
                 return;
             }

             var song = songs[index];
             player.source = subsonic.streamUrl(song);
             player.nowPlaying = song;
             player.play();
         }

         onStatusChanged: {
             if(player.status == Audio.EndOfMedia) {
                 index ++;
                 player.playSong(index)
             }
         }
    }

    Flickable {
        flickableDirection: Flickable.HorizontalFlick
        interactive: true
        width: parent.width
        anchors.top: parent.top
        anchors.bottom: bottomToolbar.top
        anchors.bottomMargin: 5
        contentWidth: musicList.width + nowplaying.width

        ListView {
            id: musicList

            width: container.width
            height: parent.height
            model: container.songs
            property bool songs: false
            spacing: 1

            Binding {
                target: musicList
                property: "model"
                value: container.songs
                when: musicList.songs
            }

            Binding {
                target: musicList
                property: "model"
                value: container.artists
                when: !musicList.songs
            }

            delegate: Rectangle {
                width: parent.width
                height: 50
                color: "lightgrey"

                Row {
                    x: 5
                    height: parent.height
                    spacing: 5
                    Image {
                        visible: modelData.coverArt !== undefined
                        height: parent.height
                        sourceSize.height: height
                        fillMode: Image.PreserveAspectFit
                        source: modelData.coverArt ? subsonic.albumArtUrl(modelData):""
                    }

                    Text {

                        height: parent.height
                        verticalAlignment: Text.AlignVCenter
                        font.pixelSize: 30
                        text: modelData.name ? modelData.name : modelData.title
                    }
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        if(modelData.name || modelData.isDir) {
                            subsonic.getMusicObjectsForId(modelData.id)
                        }
                        else if(!modelData.isVideo) {
                            player.playSong(model.index)
                        }

                        musicList.songs = true;
                    }
                }
            }
        }

        Item {
            id: nowplaying
            anchors.left: musicList.right
            height: parent.height
            width: container.width

            Particles {
                y: 0
                width: parent.width
                height: parent.height
                source: "star.png"
                lifeSpan: 15000
                lifeSpanDeviation: 5000
                count: 100 * (player.position / player.duration)
                angle: 70
                angleDeviation: 36
                velocity: 30
                velocityDeviation: 10
                ParticleMotionWander {
                    xvariance: 30
                    pace: 100
                }
            }

            Row {
                Image {

                    fillMode: Image.PreserveAspectFit
                    source: subsonic.albumArtUrl(player.nowPlaying);
                    sourceSize.width: nowplaying.width / 3
                    width: nowplaying.width / 3
                }

                Column {

                    Text {
                        text: player.nowPlaying.title
                        font.pixelSize: 30
                        color: "white"
                    }

                    Text {
                        text: "Artist: " + player.nowPlaying.artist
                        color: "white"
                    }

                    Text {
                        text: "Album: " + player.nowPlaying.album
                        color: "white"
                    }

                    Text {
                        text: "Genre: " + player.nowPlaying.genre
                        color: "white"
                    }

                    Text {
                        text: "Bitrate: " + player.nowPlaying.bitRate
                        color: "white"
                    }



                }
            }
        }

    }

    Image {
        id: bottomToolbar
        anchors.bottom: parent.bottom
        height: 50
        width: parent.width
        source: "bottom_bar.png"

        Text {
            text: player.nowPlaying.title + " - " + player.nowPlaying.artist + " - "  + player.nowPlaying.album
            y: 5
            anchors.horizontalCenter: bottomToolbar.horizontalCenter
            color: "white"
        }

        Row {
            height: parent.height
            x: 5
            spacing: 5

            Rectangle {
                height: 35
                width: 35
                radius: 5
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 5
                color: "white"
                Image {
                    anchors.fill: parent
                    source: "back.png"
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: musicList.songs = false
                }
            }

            Image {
                id: prevButton

                source: "media_prev.png"
                anchors.verticalCenter: playPauseButton.verticalCenter

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        player.playSong(player.index-1)
                    }
                }
            }

            Image {
                id: playPauseButton

                source: player.paused || !player.playing ? "media_play.png":"pause.png"
                y: 5

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        if(!player.paused)
                            player.pause();
                        else player.play();
                    }
                }
            }

            Image {
                id: nextButton

                source: "media_next.png"
                anchors.verticalCenter: playPauseButton.verticalCenter

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        player.playSong(player.index+1)
                    }
                }
            }


            BorderImage {
                id: progressBar
                border.left:5
                border.right:5
                horizontalTileMode: BorderImage.Stretch
                anchors.verticalCenter: parent.verticalCenter
                width: bottomToolbar.width - 230
                source: "progress_border.png"
                clip: true

                Image {
                    id: bufferProgress
                    anchors.verticalCenter: parent.verticalCenter
                    width: (parent.width) * player.bufferProgress
                    source: "progress_filler.png"
                    opacity: 0.25
                    x: 3

                    Behavior on width {
                        NumberAnimation { duration: 500 }
                    }

                }

                Image {
                    id: progress
                    anchors.verticalCenter: parent.verticalCenter
                    x: 3
                    width: (parent.width) * (player.position / player.duration)
                    source: "progress_filler.png"

                    Behavior on width {
                        NumberAnimation { duration: 500 }
                    }
                }
            }

            Rectangle {
                height: 35
                width: 35
                radius: 5
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 5
                color: "white"
                Image {
                    anchors.fill: parent
                    source: "configure.png"
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: toolBox.visible = true
                }
            }

        }
    }

    Item {
        id: toolBox
        anchors.fill: parent
        visible: false

        Rectangle {
            id: fog;
            anchors.fill: parent;
            opacity: 0.25;
            MouseArea {
                anchors.fill: parent;
                onClicked:  {
                    subsonic.connect();
                    toolBox.visible = false;
                }
            }
        }

        Rectangle {
            color: "darkgrey"
            width: parent.width / 2
            height: parent.height / 2
            anchors.centerIn: parent

            Grid {
                columns: 2
                anchors.fill: parent
                spacing: 10

                Text {
                    color: "white"
                    font.pixelSize: 20
                    text: "Server url"
                }

                TextEntry {
                    id: serverTextEntry
                    text: subsonic.serverUrl
                    width: parent.width / 2
                    height: 50
                    onTextChanged: {
                        console.log("setting username: " + serverTextEntry.text);
                        subsonic.serverUrl = serverTextEntry.text
                    }
                }

                Text {
                    color: "white"
                    font.pixelSize: 20
                    text: "Username"
                }

                TextEntry {
                    id: usernameTextEntry
                    width: parent.width / 2
                    height: 50
                    text: subsonic.username

                    onTextChanged: {
                        console.log("setting username: " + usernameTextEntry.text);
                        subsonic.username = usernameTextEntry.text
                    }
                }

                Text {
                    color: "white"
                    font.pixelSize: 20
                    text: "Password"
                }

                TextEntry {
                    id: passwordTextEntry
                    width: parent.width / 2
                    height: 50
                    text: subsonic.password
                    onTextChanged: {
                        console.log("setting password")
                        subsonic.password = passwordTextEntry.text
                    }
                }
            }
        }
    }

}

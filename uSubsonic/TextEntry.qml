import QtQuick 1.0

Rectangle {
	id: container
	radius: 10
	color: "white"
	border.color: "gray"
	border.width: 1

	property alias text: input.text

	signal accepted

	TextInput {
		id: input
		font.pixelSize: parent.height / 2
		anchors.fill: parent
		anchors.margins: 5

		onAccepted: container.accepted;
	}

}

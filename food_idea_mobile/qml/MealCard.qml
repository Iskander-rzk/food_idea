import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.14
import QtQuick.Controls.Material 2.14

Frame {
    id: root
    property var meal
    signal replaceRequested(int mealType)
    signal openRequested(int id)
    signal favoriteRequested(int id)
    padding: 0

    background: Rectangle {
        radius: 20
        color: "#FFFFFF"
        border.color: "#E7EBE2"
        border.width: 1
    }

    ColumnLayout {
        width: parent.width
        spacing: 0

        Rectangle {
            Layout.fillWidth: true
            height: 38
            color: "#EAF2E7"
            radius: 20
            Label {
                anchors.left: parent.left
                anchors.leftMargin: 15
                anchors.verticalCenter: parent.verticalCenter
                text: "✦  " + (root.meal.typeName || "Приём пищи")
                color: "#477451"
                font.pixelSize: 13
                font.bold: true
            }
        }

        Image {
            Layout.fillWidth: true
            Layout.preferredHeight: root.meal.imagePath ? Math.min(root.width * 0.48, 190) : 0
            visible: !!root.meal.imagePath
            source: root.meal.imagePath || ""
            fillMode: Image.PreserveAspectCrop
            asynchronous: true
            cache: true
        }

        ColumnLayout {
            Layout.fillWidth: true
            Layout.margins: 15
            spacing: 10

            Label {
                Layout.fillWidth: true
                text: root.meal.name || "Блюдо не найдено"
                wrapMode: Text.Wrap
                font.pixelSize: 18
                font.bold: true
                color: "#20352A"
            }

            Flow {
                Layout.fillWidth: true
                spacing: 6
                Repeater {
                    model: [
                        Number(root.meal.calories || 0).toFixed(0) + " ккал",
                        "Б " + Number(root.meal.protein || 0).toFixed(1) + " г",
                        "Ж " + Number(root.meal.fat || 0).toFixed(1) + " г",
                        "У " + Number(root.meal.carbs || 0).toFixed(1) + " г"
                    ]
                    delegate: Rectangle {
                        width: metric.implicitWidth + 16
                        height: 27
                        radius: 9
                        color: "#F3F5EF"
                        Label {
                            id: metric
                            anchors.centerIn: parent
                            text: modelData
                            color: "#5C6B5D"
                            font.pixelSize: 11
                        }
                    }
                }
            }

            Label {
                Layout.fillWidth: true
                visible: root.meal.id >= 0 && !!root.meal.missingString
                text: root.meal.missingString === "всё есть"
                      ? "✓ Все ингредиенты есть"
                      : (root.meal.missingString ? "Не хватает: " + root.meal.missingString : "")
                wrapMode: Text.Wrap
                color: root.meal.missingString === "всё есть" ? "#438052" : "#A9793C"
                font.pixelSize: 12
            }

            RowLayout {
                Layout.fillWidth: true
                spacing: 8
                Button {
                    Layout.fillWidth: true
                    Layout.preferredHeight: 43
                    text: "Подробнее"
                    enabled: root.meal.id >= 0
                    onClicked: root.openRequested(root.meal.id)
                    background: Rectangle {
                        radius: height / 2
                        color: parent.down ? "#386A47" : "#4F805B"
                        opacity: parent.enabled ? 1 : 0.5
                    }
                    contentItem: Label {
                        text: parent.text
                        color: "#FFFFFF"
                        font.pixelSize: 14
                        font.bold: true
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }
                }
                Button {
                    Layout.preferredWidth: 45
                    Layout.preferredHeight: 43
                    text: root.meal.favorite ? "♥" : "♡"
                    enabled: root.meal.id >= 0
                    onClicked: root.favoriteRequested(root.meal.id)
                    background: Rectangle {
                        radius: height / 2
                        color: root.meal.favorite ? "#FBE4E7" : "#E7F0E4"
                    }
                    contentItem: Label {
                        text: parent.text
                        color: root.meal.favorite ? "#CC5260" : "#3F7950"
                        font.pixelSize: 24
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }
                }
                Button {
                    Layout.preferredWidth: 48
                    Layout.preferredHeight: 43
                    text: "↻"
                    enabled: root.meal.id >= 0
                    onClicked: root.replaceRequested(root.meal.mealType)
                    background: Rectangle {
                        radius: height / 2
                        color: parent.down ? "#386A47" : "#4F805B"
                        opacity: parent.enabled ? 1 : 0.5
                    }
                    contentItem: Label {
                        text: parent.text
                        color: "#FFFFFF"
                        font.pixelSize: 22
                        font.bold: true
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }
                }
            }
        }
    }
}

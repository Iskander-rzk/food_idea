import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.14
import QtQuick.Controls.Material 2.14

Frame {
    id: root

    property var meal
    signal replaceRequested(int mealType)
    signal openRequested(int id)

    padding: 0
    background: Rectangle {
        radius: 18
        color: "#ffffff"
        border.width: 1
        border.color: "#e3e8ee"
    }

    ColumnLayout {
        width: parent.width
        spacing: 0

        Rectangle {
            Layout.fillWidth: true
            height: 46
            radius: 18
            color: "#eff9ef"

            Label {
                anchors.fill: parent
                anchors.leftMargin: 16
                verticalAlignment: Text.AlignVCenter
                text: root.meal.typeName || "Приём пищи"
                font.pixelSize: 16
                font.bold: true
                color: "#214221"
            }
        }

        Image {
            Layout.fillWidth: true
            Layout.preferredHeight: root.meal.imagePath ? 165 : 0
            visible: !!root.meal.imagePath
            source: root.meal.imagePath || ""
            fillMode: Image.PreserveAspectCrop
            asynchronous: true
        }

        ColumnLayout {
            Layout.fillWidth: true
            spacing: 8
            Layout.margins: 16

            Label {
                Layout.fillWidth: true
                text: root.meal.name || "Блюдо не найдено"
                wrapMode: Text.Wrap
                font.pixelSize: 20
                font.bold: true
                color: "#172018"
            }

            Label {
                Layout.fillWidth: true
                visible: root.meal.id >= 0
                text: qsTr("%1 ккал  •  Б %2 г  •  Ж %3 г  •  У %4 г")
                      .arg(Number(root.meal.calories).toFixed(0))
                      .arg(Number(root.meal.protein).toFixed(1))
                      .arg(Number(root.meal.fat).toFixed(1))
                      .arg(Number(root.meal.carbs).toFixed(1))
                wrapMode: Text.Wrap
                font.pixelSize: 13
                color: "#657267"
            }

            Label {
                Layout.fillWidth: true
                visible: root.meal.id >= 0
                text: root.meal.missingString ? qsTr("Не хватает: %1").arg(root.meal.missingString) : ""
                wrapMode: Text.Wrap
                font.pixelSize: 13
                color: root.meal.missingString === "всё есть" ? "#2d7a32" : "#a36616"
            }

            Label {
                Layout.fillWidth: true
                visible: root.meal.id >= 0 && !!root.meal.recipe
                text: root.meal.recipe || ""
                maximumLineCount: 3
                elide: Text.ElideRight
                wrapMode: Text.Wrap
                font.pixelSize: 14
                color: "#374238"
            }

            RowLayout {
                Layout.fillWidth: true
                spacing: 8

                Button {
                    Layout.fillWidth: true
                    text: "Подробнее"
                    enabled: root.meal.id >= 0
                    onClicked: root.openRequested(root.meal.id)
                }

                Button {
                    text: "Заменить"
                    enabled: root.meal.id >= 0
                    onClicked: root.replaceRequested(root.meal.mealType)
                }
            }
        }
    }
}

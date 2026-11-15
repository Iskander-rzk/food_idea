import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.14
import QtQuick.Controls.Material 2.14

Frame {
    id: root

    property var recipe
    property bool showAddButton: false
    signal openRequested(int id)
    signal addRequested(int id)

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

        Image {
            Layout.fillWidth: true
            Layout.preferredHeight: root.recipe.imagePath ? 165 : 0
            visible: !!root.recipe.imagePath
            source: root.recipe.imagePath || ""
            fillMode: Image.PreserveAspectCrop
            asynchronous: true
        }

        ColumnLayout {
            Layout.fillWidth: true
            spacing: 8
            Layout.margins: 16

            Label {
                Layout.fillWidth: true
                text: root.recipe.typeName + "  •  " + root.recipe.name
                wrapMode: Text.Wrap
                font.pixelSize: 18
                font.bold: true
                color: "#172018"
            }

            Label {
                Layout.fillWidth: true
                text: qsTr("%1 ккал  •  Б %2 г  •  Ж %3 г  •  У %4 г")
                      .arg(Number(root.recipe.calories).toFixed(0))
                      .arg(Number(root.recipe.protein).toFixed(1))
                      .arg(Number(root.recipe.fat).toFixed(1))
                      .arg(Number(root.recipe.carbs).toFixed(1))
                wrapMode: Text.Wrap
                font.pixelSize: 13
                color: "#657267"
            }

            Label {
                Layout.fillWidth: true
                visible: root.recipe.missingString !== undefined
                text: root.recipe.missingString === "всё есть"
                      ? "✓ Все ингредиенты есть"
                      : qsTr("Не хватает: %1").arg(root.recipe.missingString)
                wrapMode: Text.Wrap
                font.pixelSize: 13
                color: root.recipe.missingString === "всё есть" ? "#2d7a32" : "#a36616"
            }

            Label {
                Layout.fillWidth: true
                text: root.recipe.ingredients && root.recipe.ingredients.length
                      ? qsTr("Ингредиенты: %1").arg(root.recipe.ingredients.join(", "))
                      : "Ингредиенты не указаны"
                wrapMode: Text.Wrap
                font.pixelSize: 13
                color: "#59645a"
            }

            RowLayout {
                Layout.fillWidth: true
                Button {
                    Layout.fillWidth: true
                    text: "Подробнее"
                    onClicked: root.openRequested(root.recipe.id)
                }
                Button {
                    visible: root.showAddButton
                    text: "В меню"
                    onClicked: root.addRequested(root.recipe.id)
                }
            }
        }
    }
}

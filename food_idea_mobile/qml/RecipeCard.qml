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

        Image {
            Layout.fillWidth: true
            Layout.preferredHeight: root.recipe.imagePath ? Math.min(root.width * 0.48, 190) : 0
            visible: !!root.recipe.imagePath
            source: root.recipe.imagePath || ""
            fillMode: Image.PreserveAspectCrop
            asynchronous: true
            cache: true
        }

        ColumnLayout {
            Layout.fillWidth: true
            Layout.margins: 15
            spacing: 10

            RowLayout {
                Layout.fillWidth: true
                spacing: 6
                Label {
                    Layout.fillWidth: true
                    text: (root.recipe.typeName || "Рецепт") + "  ·  " + (root.recipe.name || "")
                    wrapMode: Text.Wrap
                    font.pixelSize: 17
                    font.bold: true
                    color: "#20352A"
                }
                Button {
                    Layout.preferredWidth: 40
                    Layout.preferredHeight: 40
                    text: root.recipe.favorite ? "♥" : "♡"
                    font.pixelSize: 26
                    onClicked: root.favoriteRequested(root.recipe.id)
                    background: Rectangle { radius: 20; color: root.recipe.favorite ? "#FCE9EA" : "#F3F5EF" }
                    contentItem: Label {
                        text: parent.text
                        color: root.recipe.favorite ? "#D35062" : "#758675"
                        font.pixelSize: 26
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }
                }
            }

            Flow {
                Layout.fillWidth: true
                spacing: 6
                Repeater {
                    model: [
                        Number(root.recipe.calories || 0).toFixed(0) + " ккал",
                        "Б " + Number(root.recipe.protein || 0).toFixed(1) + " г",
                        "Ж " + Number(root.recipe.fat || 0).toFixed(1) + " г",
                        "У " + Number(root.recipe.carbs || 0).toFixed(1) + " г"
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
                visible: !!root.recipe.excluded
                text: "Не подходит: содержит исключённый продукт"
                color: "#B94350"
                wrapMode: Text.Wrap
                font.pixelSize: 12
            }

            Label {
                Layout.fillWidth: true
                visible: root.recipe.missingString !== undefined
                text: root.recipe.missingString === "всё есть"
                      ? "✓ Все ингредиенты есть"
                      : "Не хватает: " + root.recipe.missingString
                wrapMode: Text.Wrap
                font.pixelSize: 12
                color: root.recipe.missingString === "всё есть" ? "#438052" : "#A9793C"
            }

            Label {
                Layout.fillWidth: true
                text: root.recipe.ingredients && root.recipe.ingredients.length
                      ? "Ингредиенты: " + root.recipe.ingredients.join(", ")
                      : "Ингредиенты не указаны"
                wrapMode: Text.Wrap
                font.pixelSize: 12
                color: "#788579"
                maximumLineCount: 2
                elide: Text.ElideRight
            }

            RowLayout {
                Layout.fillWidth: true
                spacing: 8
                Button {
                    Layout.fillWidth: true
                    Layout.preferredHeight: 42
                    text: "Подробнее"
                    onClicked: root.openRequested(root.recipe.id)
                    background: Rectangle {
                        radius: height / 2
                        color: parent.down ? "#386A47" : "#4F805B"
                    }
                    contentItem: Label {
                        text: parent.text
                        color: "#FFFFFF"
                        font.pixelSize: 13
                        font.bold: true
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }
                }
                Button {
                    visible: root.showAddButton
                    Layout.fillWidth: true
                    Layout.preferredHeight: 42
                    text: root.recipe.excluded ? "Исключено" : "+ В меню"
                    enabled: !root.recipe.excluded
                    onClicked: root.addRequested(root.recipe.id)
                    background: Rectangle {
                        radius: height / 2
                        color: parent.down ? "#386A47" : "#4F805B"
                    }
                    contentItem: Label {
                        text: parent.text
                        color: "#FFFFFF"
                        font.pixelSize: 13
                        font.bold: true
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }
                }
            }
        }
    }
}

import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Controls.Material 2.14
import QtQuick.Layouts 1.14
import QtQuick.Dialogs 1.3 as QuickDialogs

ApplicationWindow {
    id: window
    visible: true
    width: 420
    height: 860
    minimumWidth: 320
    minimumHeight: 600
    title: "Food Idea"

    Material.theme: Material.Light
    Material.accent: "#55a95a"

    property int currentTab: 0
    property var selectedRecipe: ({})

    background: Rectangle { color: "#f6f8f6" }

    Connections {
        target: foodController
        onMessageChanged: {
            toast.text = message
            toast.open()
        }
    }

    function openRecipe(id) {
        selectedRecipe = foodController.recipeById(id)
        recipeDialog.open()
    }

    Dialog {
        id: recipeDialog
        modal: true
        anchors.centerIn: parent
        width: Math.min(window.width - 24, 520)
        height: Math.min(window.height - 48, 720)
        title: selectedRecipe.name || "Рецепт"
        standardButtons: Dialog.Close

        ScrollView {
            anchors.fill: parent
            clip: true

            ColumnLayout {
                width: recipeDialog.width - 36
                spacing: 12

                Image {
                    Layout.fillWidth: true
                    Layout.preferredHeight: selectedRecipe.imagePath ? 190 : 0
                    visible: !!selectedRecipe.imagePath
                    source: selectedRecipe.imagePath || ""
                    fillMode: Image.PreserveAspectCrop
                    asynchronous: true
                }

                Label {
                    Layout.fillWidth: true
                    text: selectedRecipe.typeName || ""
                    font.pixelSize: 14
                    color: "#657267"
                }

                Label {
                    Layout.fillWidth: true
                    text: selectedRecipe.name || ""
                    wrapMode: Text.Wrap
                    font.pixelSize: 24
                    font.bold: true
                    color: "#172018"
                }

                Label {
                    Layout.fillWidth: true
                    text: selectedRecipe.id >= 0
                          ? qsTr("%1 ккал  •  Б %2 г  •  Ж %3 г  •  У %4 г")
                            .arg(Number(selectedRecipe.calories).toFixed(0))
                            .arg(Number(selectedRecipe.protein).toFixed(1))
                            .arg(Number(selectedRecipe.fat).toFixed(1))
                            .arg(Number(selectedRecipe.carbs).toFixed(1))
                          : ""
                    wrapMode: Text.Wrap
                    color: "#3f493f"
                }

                Label {
                    Layout.fillWidth: true
                    text: selectedRecipe.ingredients && selectedRecipe.ingredients.length
                          ? qsTr("Ингредиенты\n%1").arg(selectedRecipe.ingredients.join("\n"))
                          : "Ингредиенты не указаны"
                    wrapMode: Text.Wrap
                    font.pixelSize: 15
                    color: "#374238"
                }

                Label {
                    Layout.fillWidth: true
                    text: selectedRecipe.recipe || "Рецепт не указан"
                    wrapMode: Text.Wrap
                    font.pixelSize: 16
                    color: "#172018"
                }
            }
        }
    }

    Dialog {
        id: addDishDialog
        modal: true
        anchors.centerIn: parent
        width: Math.min(window.width - 24, 520)
        height: Math.min(window.height - 36, 820)
        title: "Новый рецепт"
        standardButtons: Dialog.Cancel

        contentItem: ScrollView {
            clip: true

            ColumnLayout {
                width: addDishDialog.width - 36
                spacing: 12

                TextField {
                    id: nameField
                    Layout.fillWidth: true
                    placeholderText: "Название блюда"
                }

                ComboBox {
                    id: typeField
                    Layout.fillWidth: true
                    model: ["Завтрак", "Обед", "Ужин", "Перекус"]
                }

                TextArea {
                    id: recipeField
                    Layout.fillWidth: true
                    implicitHeight: 130
                    placeholderText: "Рецепт пошагово"
                    wrapMode: TextEdit.Wrap
                }

                RowLayout {
                    Layout.fillWidth: true
                    TextField { id: calField; Layout.fillWidth: true; placeholderText: "Ккал"; inputMethodHints: Qt.ImhFormattedNumbersOnly }
                    TextField { id: proteinField; Layout.fillWidth: true; placeholderText: "Белки"; inputMethodHints: Qt.ImhFormattedNumbersOnly }
                }

                RowLayout {
                    Layout.fillWidth: true
                    TextField { id: fatField; Layout.fillWidth: true; placeholderText: "Жиры"; inputMethodHints: Qt.ImhFormattedNumbersOnly }
                    TextField { id: carbsField; Layout.fillWidth: true; placeholderText: "Углеводы"; inputMethodHints: Qt.ImhFormattedNumbersOnly }
                }

                TextField {
                    id: ingredientsField
                    Layout.fillWidth: true
                    placeholderText: "Ингредиенты через запятую"
                }

                RowLayout {
                    Layout.fillWidth: true
                    Label {
                        Layout.fillWidth: true
                        text: imageField.text ? "Фото выбрано" : "Фото не выбрано"
                        elide: Text.ElideMiddle
                    }
                    Button {
                        text: "Выбрать фото"
                        onClicked: imageDialog.open()
                    }
                }

                Button {
                    Layout.fillWidth: true
                    text: "Добавить рецепт"
                    enabled: nameField.text.trim().length > 0
                    onClicked: {
                        const ok = foodController.addDish(
                            nameField.text,
                            typeField.currentIndex,
                            recipeField.text,
                            Number(calField.text.replace(",", ".")) || 0,
                            Number(proteinField.text.replace(",", ".")) || 0,
                            Number(fatField.text.replace(",", ".")) || 0,
                            Number(carbsField.text.replace(",", ".")) || 0,
                            ingredientsField.text,
                            imageField.text
                        )
                        if (ok) {
                            nameField.clear()
                            recipeField.clear()
                            calField.clear()
                            proteinField.clear()
                            fatField.clear()
                            carbsField.clear()
                            ingredientsField.clear()
                            imageField.text = ""
                            addDishDialog.close()
                            currentTab = 1
                        }
                    }
                }
            }
        }
    }

    TextField {
        id: imageField
        visible: false
    }

    QuickDialogs.FileDialog {
        id: imageDialog
        title: "Выберите фото блюда"
        nameFilters: ["Изображения (*.png *.jpg *.jpeg *.webp)"]
        onAccepted: imageField.text = fileUrl.toString()
    }

    Popup {
        id: toast
        y: parent.height - bottomBar.height - height - 12
        x: (parent.width - width) / 2
        width: Math.min(parent.width - 32, 360)
        padding: 12
        closePolicy: Popup.NoAutoClose

        background: Rectangle {
            radius: 12
            color: "#263328"
        }

        contentItem: Label {
            id: toastText
            text: toast.text
            color: "white"
            wrapMode: Text.Wrap
            horizontalAlignment: Text.AlignHCenter
        }

        property string text: ""

        Timer {
            id: toastTimer
            interval: 2200
            onTriggered: toast.close()
        }

        onOpened: toastTimer.restart()
    }

    StackLayout {
        anchors.top: header.bottom
        anchors.bottom: bottomBar.top
        anchors.left: parent.left
        anchors.right: parent.right
        currentIndex: window.currentTab

        Flickable {
            clip: true
            contentWidth: width
            contentHeight: dayColumn.implicitHeight + 24

            ColumnLayout {
                id: dayColumn
                x: 12
                y: 12
                width: parent.width - 24
                spacing: 12

                RowLayout {
                    Layout.fillWidth: true
                    Label {
                        Layout.fillWidth: true
                        text: "Меню на день"
                        font.pixelSize: 26
                        font.bold: true
                        color: "#172018"
                    }
                    Button {
                        text: "Обновить"
                        onClicked: foodController.generateDay()
                    }
                }

                Label {
                    Layout.fillWidth: true
                    text: "Подборка учитывает продукты в вашем списке и избегает недавних повторов."
                    wrapMode: Text.Wrap
                    font.pixelSize: 13
                    color: "#657267"
                }

                Repeater {
                    model: foodController.today
                    delegate: MealCard {
                        Layout.fillWidth: true
                        meal: modelData
                        onReplaceRequested: function(type) { foodController.replaceMeal(type) }
                        onOpenRequested: function(id) { openRecipe(id) }
                    }
                }
            }
        }

        ColumnLayout {
            spacing: 0

            ToolBar {
                Layout.fillWidth: true
                RowLayout {
                    anchors.fill: parent
                    anchors.margins: 8
                    Label {
                        Layout.fillWidth: true
                        text: "Все рецепты"
                        font.pixelSize: 24
                        font.bold: true
                    }
                    Button {
                        text: "+ Свой рецепт"
                        onClicked: addDishDialog.open()
                    }
                }
            }

            ScrollView {
                Layout.fillWidth: true
                Layout.fillHeight: true
                clip: true

                ColumnLayout {
                    width: parent.width - 24
                    x: 12
                    y: 12
                    spacing: 12

                    Label {
                        Layout.fillWidth: true
                        text: qsTr("Доступно рецептов: %1").arg(foodController.recipeCount)
                        color: "#657267"
                    }

                    Repeater {
                        model: foodController.allRecipes
                        delegate: RecipeCard {
                            Layout.fillWidth: true
                            recipe: modelData
                            showAddButton: true
                            onOpenRequested: function(id) { openRecipe(id) }
                            onAddRequested: function(id) { foodController.addToDayMenu(id) }
                        }
                    }
                }
            }
        }

        ColumnLayout {
            spacing: 0

            ToolBar {
                Layout.fillWidth: true
                RowLayout {
                    anchors.fill: parent
                    anchors.margins: 8
                    Label {
                        Layout.fillWidth: true
                        text: "Мои продукты"
                        font.pixelSize: 24
                        font.bold: true
                    }
                    Label {
                        text: qsTr("%1 шт.").arg(foodController.products.length)
                        color: "#657267"
                    }
                }
            }

            ScrollView {
                Layout.fillWidth: true
                Layout.fillHeight: true
                clip: true

                ColumnLayout {
                    width: parent.width - 24
                    x: 12
                    y: 12
                    spacing: 12

                    RowLayout {
                        Layout.fillWidth: true
                        TextField {
                            id: productField
                            Layout.fillWidth: true
                            placeholderText: "Например: яйца, сыр, помидор"
                            onAccepted: {
                                foodController.addProduct(text)
                                clear()
                            }
                        }
                        Button {
                            text: "+"
                            onClicked: {
                                foodController.addProduct(productField.text)
                                productField.clear()
                            }
                        }
                    }

                    Label {
                        Layout.fillWidth: true
                        text: foodController.products.length
                              ? "Ваши продукты:"
                              : "Добавьте продукты, чтобы получать более точные рекомендации."
                        wrapMode: Text.Wrap
                        color: "#657267"
                    }

                    ListView {
                        Layout.fillWidth: true
                        Layout.preferredHeight: Math.max(0, Math.min(contentHeight, 260))
                        model: foodController.products
                        clip: true
                        delegate: ItemDelegate {
                            width: ListView.view.width
                            text: modelData
                            onClicked: foodController.removeProduct(index)
                            ToolTip.visible: hovered
                            ToolTip.text: "Нажмите, чтобы удалить"
                        }
                    }

                    Rectangle {
                        Layout.fillWidth: true
                        height: 1
                        color: "#dfe5df"
                    }

                    Label {
                        Layout.fillWidth: true
                        text: "Подходящие рецепты"
                        font.pixelSize: 20
                        font.bold: true
                        color: "#172018"
                    }

                    Label {
                        Layout.fillWidth: true
                        visible: foodController.products.length === 0
                        text: "После добавления продуктов здесь появятся блюда, для которых не хватает не более двух ингредиентов."
                        wrapMode: Text.Wrap
                        color: "#657267"
                    }

                    Repeater {
                        model: foodController.myRecipes
                        delegate: RecipeCard {
                            Layout.fillWidth: true
                            recipe: modelData
                            showAddButton: true
                            onOpenRequested: function(id) { openRecipe(id) }
                            onAddRequested: function(id) { foodController.addToDayMenu(id) }
                        }
                    }

                    Label {
                        Layout.fillWidth: true
                        visible: foodController.products.length > 0 && foodController.myRecipes.length === 0
                        text: "Подходящих рецептов пока нет. Добавьте ещё несколько продуктов."
                        wrapMode: Text.Wrap
                        horizontalAlignment: Text.AlignHCenter
                        color: "#657267"
                    }
                }
            }
        }
    }

    ToolBar {
        id: header
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        height: 64

        RowLayout {
            anchors.fill: parent
            anchors.leftMargin: 16
            anchors.rightMargin: 16

            Label {
                text: "Food Idea"
                font.pixelSize: 24
                font.bold: true
                color: "#172018"
            }

            Label {
                Layout.fillWidth: true
                text: "что приготовить сегодня?"
                font.pixelSize: 13
                color: "#657267"
                elide: Text.ElideRight
            }
        }
    }

    TabBar {
        id: bottomBar
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        height: 72
        currentIndex: window.currentTab
        onCurrentIndexChanged: window.currentTab = currentIndex

        TabButton { text: "🍽\nМеню"; font.pixelSize: 13 }
        TabButton { text: "📖\nРецепты"; font.pixelSize: 13 }
        TabButton { text: "🥕\nПродукты"; font.pixelSize: 13 }
    }
}

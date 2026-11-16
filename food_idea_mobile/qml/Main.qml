import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Controls.Material 2.14
import QtQuick.Layouts 1.14
import QtQuick.Dialogs 1.3 as QuickDialogs

ApplicationWindow {
    id: window
    visible: true
    width: 390
    height: 844
    minimumWidth: 320
    minimumHeight: 560
    title: "Food Idea"

    Material.theme: Material.Light
    Material.accent: "#4F805B"
    Material.primary: "#4F805B"
    Material.foreground: "#20352A"

    property int currentTab: 0
    property var selectedRecipe: ({})

    background: Rectangle { color: "#F5F6F0" }

    Connections {
        target: foodController
        onMessageChanged: {
            toast.text = message
            toast.open()
        }
        onDataChanged: {
            if (recipeDialog.visible && selectedRecipe.id !== undefined)
                selectedRecipe = foodController.recipeById(selectedRecipe.id)
        }
    }

    function openRecipe(id) {
        selectedRecipe = foodController.recipeById(id)
        recipeDialog.open()
    }

    Dialog {
        id: recipeDialog
        modal: true
        x: (window.width - width) / 2
        y: (window.height - height) / 2
        width: Math.min(window.width, 520)
        height: window.height
        padding: 0
        title: ""
        closePolicy: Popup.CloseOnEscape

        background: Rectangle {
            color: "#F5F6F0"
            radius: 0
        }

        contentItem: Item {
            ColumnLayout {
                anchors.fill: parent
                spacing: 0

                Rectangle {
                    Layout.fillWidth: true
                    height: 58
                    color: "#FFFFFF"
                    RowLayout {
                        anchors.fill: parent
                        anchors.leftMargin: 16
                        anchors.rightMargin: 18
                        spacing: 12

                        ToolButton {
                            text: "‹"
                            font.pixelSize: 32
                            Layout.preferredWidth: 38
                            Layout.preferredHeight: 44
                            onClicked: recipeDialog.close()
                            contentItem: Label {
                                text: "‹"
                                color: "#315D3D"
                                font.pixelSize: 34
                                horizontalAlignment: Text.AlignHCenter
                                verticalAlignment: Text.AlignVCenter
                            }
                        }

                        Label {
                            Layout.fillWidth: true
                            text: "РЕЦЕПТ"
                            color: "#718074"
                            font.pixelSize: 12
                            font.bold: true
                            font.letterSpacing: 1.4
                        }

                        ToolButton {
                            text: "×"
                            font.pixelSize: 26
                            onClicked: recipeDialog.close()
                            contentItem: Label {
                                text: "×"
                                color: "#526354"
                                font.pixelSize: 27
                                horizontalAlignment: Text.AlignHCenter
                                verticalAlignment: Text.AlignVCenter
                            }
                        }
                    }
                }

                ScrollView {
                    id: recipeScroll
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    clip: true
                    contentWidth: width
                    ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
                    ScrollBar.vertical.policy: ScrollBar.AlwaysOff

                    ColumnLayout {
                        width: recipeScroll.availableWidth
                        spacing: 0

                        Image {
                            Layout.fillWidth: true
                            Layout.preferredHeight: selectedRecipe.imagePath ? Math.min(recipeScroll.width * 0.72, 290) : 0
                            visible: !!selectedRecipe.imagePath
                            source: selectedRecipe.imagePath || ""
                            fillMode: Image.PreserveAspectCrop
                            asynchronous: true
                            cache: true
                        }

                        Rectangle {
                            Layout.fillWidth: true
                            Layout.preferredHeight: 150
                            visible: !selectedRecipe.imagePath
                            color: "#E6EDE2"
                            Label {
                                anchors.centerIn: parent
                                text: "🥗"
                                font.pixelSize: 58
                            }
                        }

                        ColumnLayout {
                            Layout.fillWidth: true
                            Layout.leftMargin: 20
                            Layout.rightMargin: 20
                            Layout.topMargin: 22
                            Layout.bottomMargin: 28
                            spacing: 17

                            Label {
                                Layout.fillWidth: true
                                text: (selectedRecipe.typeName || "Блюдо").toUpperCase()
                                color: "#4F805B"
                                font.pixelSize: 11
                                font.bold: true
                                font.letterSpacing: 1.3
                            }

                            Label {
                                Layout.fillWidth: true
                                text: selectedRecipe.name || "Название блюда"
                                wrapMode: Text.Wrap
                                font.pixelSize: 27
                                font.bold: true
                                color: "#20352A"
                            }

                            Label {
                                Layout.fillWidth: true
                                text: "Пищевая ценность на порцию"
                                color: "#718074"
                                font.pixelSize: 13
                            }

                            GridLayout {
                                Layout.fillWidth: true
                                columns: 2
                                rowSpacing: 9
                                columnSpacing: 9

                                Repeater {
                                    model: [
                                        {title: "КАЛОРИЙНОСТЬ", value: Number(selectedRecipe.calories || 0).toFixed(0) + " ккал", accent: true},
                                        {title: "БЕЛКИ", value: Number(selectedRecipe.protein || 0).toFixed(1) + " г", accent: false},
                                        {title: "ЖИРЫ", value: Number(selectedRecipe.fat || 0).toFixed(1) + " г", accent: false},
                                        {title: "УГЛЕВОДЫ", value: Number(selectedRecipe.carbs || 0).toFixed(1) + " г", accent: false}
                                    ]
                                    delegate: Rectangle {
                                        Layout.fillWidth: true
                                        Layout.preferredHeight: 75
                                        radius: 15
                                        color: modelData.accent ? "#E4EFE2" : "#FFFFFF"
                                        border.color: "#E7EBE2"
                                        Column {
                                            anchors.left: parent.left
                                            anchors.leftMargin: 13
                                            anchors.verticalCenter: parent.verticalCenter
                                            spacing: 5
                                            Label {
                                                text: modelData.title
                                                color: "#788579"
                                                font.pixelSize: 9
                                                font.bold: true
                                            }
                                            Label {
                                                text: modelData.value
                                                color: "#20352A"
                                                font.pixelSize: 17
                                                font.bold: true
                                            }
                                        }
                                    }
                                }
                            }

                            Rectangle {
                                Layout.fillWidth: true
                                height: 1
                                color: "#E4E9E0"
                            }

                            Label {
                                Layout.fillWidth: true
                                text: "Ингредиенты"
                                color: "#20352A"
                                font.pixelSize: 20
                                font.bold: true
                            }

                            Label {
                                Layout.fillWidth: true
                                text: selectedRecipe.ingredients && selectedRecipe.ingredients.length
                                      ? "•  " + selectedRecipe.ingredients.join("\n•  ")
                                      : "Ингредиенты не указаны"
                                wrapMode: Text.Wrap
                                lineHeight: 1.35
                                color: "#526354"
                                font.pixelSize: 14
                            }

                            Rectangle {
                                Layout.fillWidth: true
                                height: 1
                                color: "#E4E9E0"
                            }

                            Label {
                                Layout.fillWidth: true
                                text: "Как приготовить"
                                color: "#20352A"
                                font.pixelSize: 20
                                font.bold: true
                            }

                            Label {
                                Layout.fillWidth: true
                                text: selectedRecipe.recipe || "Рецепт приготовления пока не добавлен."
                                wrapMode: Text.Wrap
                                lineHeight: 1.4
                                color: "#526354"
                                font.pixelSize: 14
                            }

                            RowLayout {
                                Layout.fillWidth: true
                                spacing: 9
                                Button {
                                    Layout.fillWidth: true
                                    text: foodController.isFavorite(selectedRecipe.id) ? "♥ В избранном" : "♡ В избранное"
                                    onClicked: foodController.toggleFavorite(selectedRecipe.id)
                                    background: Rectangle { radius: 18; color: "#E5F0E4" }
                                    contentItem: Label {
                                        text: parent.text
                                        color: "#3F7950"
                                        horizontalAlignment: Text.AlignHCenter
                                        verticalAlignment: Text.AlignVCenter
                                    }
                                }
                                Button {
                                    Layout.fillWidth: true
                                    text: "В меню"
                                    enabled: !foodController.isExcluded(selectedRecipe.id)
                                    onClicked: { foodController.addToDayMenu(selectedRecipe.id); recipeDialog.close() }
                                    Material.background: "#4F805B"
                                    Material.foreground: "#FFFFFF"
                                }
                            }
                            Label {
                                Layout.fillWidth: true
                                visible: foodController.isExcluded(selectedRecipe.id)
                                text: "Блюдо содержит исключённый ингредиент и не может быть добавлено в меню."
                                wrapMode: Text.Wrap
                                color: "#B94350"
                                font.pixelSize: 12
                            }
                            Button {
                                Layout.fillWidth: true
                                Layout.preferredHeight: 50
                                text: "Понятно"
                                onClicked: recipeDialog.close()
                                Material.background: "#4F805B"
                                Material.foreground: "#FFFFFF"
                            }
                        }
                    }
                }
            }
        }
    }

    Dialog {
        id: addDishDialog
        modal: true
        anchors.centerIn: parent
        width: Math.min(window.width - 20, 500)
        height: Math.min(window.height - 24, 820)
        title: "Новый рецепт"
        standardButtons: Dialog.Cancel

        contentItem: ScrollView {
            clip: true
            contentWidth: width
            ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
            ScrollBar.vertical.policy: ScrollBar.AlwaysOff

            ColumnLayout {
                width: Math.max(0, parent.width - 24)
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
            flickableDirection: Flickable.VerticalFlick
            contentWidth: width
            contentHeight: dayColumn.implicitHeight + 24

            ColumnLayout {
                id: dayColumn
                x: 12
                y: 12
                width: Math.max(0, parent.width - 24)
                spacing: 12

                RowLayout {
                    Layout.fillWidth: true
                    Label {
                        Layout.fillWidth: true
                        text: "Меню на день"
                        font.pixelSize: 25
                        font.bold: true
                        color: "#20352A"
                    }
                    Button {
                        id: refreshMenuButton
                        Layout.preferredHeight: 42
                        Layout.preferredWidth: 118
                        text: "↻ Обновить"
                        font.pixelSize: 12
                        font.bold: true
                        onClicked: foodController.generateDay()
                        background: Rectangle {
                            radius: height / 2
                            color: refreshMenuButton.down ? "#386A47" : "#4F805B"
                            opacity: refreshMenuButton.enabled ? 1 : 0.55
                        }
                        contentItem: Label {
                            text: refreshMenuButton.text
                            color: "#FFFFFF"
                            font.pixelSize: 12
                            font.bold: true
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                        }
                    }
                }

                Label {
                    Layout.fillWidth: true
                    text: "Подборка учитывает продукты в вашем списке и избегает недавних повторов."
                    wrapMode: Text.Wrap
                    font.pixelSize: 13
                    color: "#788579"
                }

                Rectangle {
                    Layout.fillWidth: true
                    Layout.preferredHeight: 104
                    radius: 18
                    color: "#E6F0E3"
                    ColumnLayout {
                        anchors.fill: parent
                        anchors.margins: 14
                        spacing: 5
                        Label {
                            text: "Дневной рацион · КБЖУ"
                            font.bold: true
                            font.pixelSize: 14
                            color: "#315B3C"
                        }
                        Label {
                            Layout.fillWidth: true
                            text: Math.round(foodController.dailyTotals.calories) + " / " + Math.round(foodController.nutritionGoals.calories) + " ккал"
                            font.pixelSize: 20
                            font.bold: true
                            color: "#20352A"
                        }
                        Label {
                            Layout.fillWidth: true
                            text: "Белки " + Math.round(foodController.dailyTotals.protein) + " / " + foodController.nutritionGoals.protein +
                                  "  ·  Жиры " + Math.round(foodController.dailyTotals.fat) + " / " + foodController.nutritionGoals.fat +
                                  "  ·  Углеводы " + Math.round(foodController.dailyTotals.carbs) + " / " + foodController.nutritionGoals.carbs
                            wrapMode: Text.Wrap
                            font.pixelSize: 11
                            color: "#52705B"
                        }
                    }
                }

                Button {
                    Layout.fillWidth: true
                    text: "Добавить недостающие продукты в покупки"
                    onClicked: foodController.addMissingToShopping()
                    enabled: foodController.missingForToday.length > 0
                    background: Rectangle { radius: height / 2; color: "#E2EEE0" }
                    contentItem: Label {
                        text: parent.text
                        color: "#3B7449"
                        font.pixelSize: 12
                        font.bold: true
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }
                }

                Repeater {
                    model: foodController.today
                    delegate: MealCard {
                        Layout.fillWidth: true
                        meal: modelData
                        onReplaceRequested: function(type) { foodController.replaceMeal(type) }
                        onOpenRequested: function(id) { openRecipe(id) }
                        onFavoriteRequested: function(id) { foodController.toggleFavorite(id) }
                    }
                }
            }
        }

        ColumnLayout {
            spacing: 0

            ToolBar {
                Layout.fillWidth: true
                Layout.preferredHeight: 112
                background: Rectangle { color: "#F5F6F0" }

                ColumnLayout {
                    anchors.fill: parent
                    anchors.leftMargin: 16
                    anchors.rightMargin: 16
                    anchors.topMargin: 8
                    anchors.bottomMargin: 6
                    spacing: 8

                    Button {
                        id: addRecipeTopButton
                        Layout.alignment: Qt.AlignHCenter
                        Layout.preferredWidth: Math.min(280, window.width - 32)
                        Layout.preferredHeight: 46
                        text: "Добавить свой рецепт"
                        font.pixelSize: 14
                        font.bold: true
                        onClicked: addDishDialog.open()
                        background: Rectangle {
                            radius: height / 2
                            color: addRecipeTopButton.down ? "#386A47" : "#4F805B"
                        }
                        contentItem: Label {
                            text: addRecipeTopButton.text
                            color: "#FFFFFF"
                            font.pixelSize: 14
                            font.bold: true
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                        }
                    }

                    Label {
                        Layout.fillWidth: true
                        text: "Все рецепты"
                        color: "#20352A"
                        font.pixelSize: 21
                        font.bold: true
                    }
                }
            }

            Flickable {
                id: recipesFlickable
                Layout.fillWidth: true
                Layout.fillHeight: true
                clip: true
                contentWidth: width
                contentHeight: recipeListColumn.implicitHeight + 32
                flickableDirection: Flickable.VerticalFlick
                boundsBehavior: Flickable.StopAtBounds
                ScrollBar.vertical: ScrollBar { policy: ScrollBar.AlwaysOff }

                ColumnLayout {
                    id: recipeListColumn
                    width: Math.max(0, recipesFlickable.width - 24)
                    x: 12
                    y: 12
                    spacing: 12

                    Label {
                        Layout.fillWidth: true
                        text: qsTr("Доступно рецептов: %1").arg(foodController.recipeCount)
                        color: "#788579"
                    }

                    Repeater {
                        model: foodController.allRecipes
                        delegate: RecipeCard {
                            Layout.fillWidth: true
                            recipe: modelData
                            showAddButton: true
                            onOpenRequested: function(id) { openRecipe(id) }
                            onAddRequested: function(id) { foodController.addToDayMenu(id) }
                            onFavoriteRequested: function(id) { foodController.toggleFavorite(id) }
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
                        font.pixelSize: 22
                        font.bold: true
                    }
                    Label {
                        text: qsTr("%1 шт.").arg(foodController.products.length)
                        color: "#788579"
                    }
                }
            }

            ScrollView {
                Layout.fillWidth: true
                Layout.fillHeight: true
                clip: true
                contentWidth: width
                ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
                ScrollBar.vertical.policy: ScrollBar.AlwaysOff

                ColumnLayout {
                    width: Math.max(0, parent.width - 24)
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
                        color: "#788579"
                    }

                    ListView {
                        Layout.fillWidth: true
                        Layout.preferredHeight: Math.max(0, Math.min(contentHeight, 260))
                        model: foodController.products
                        clip: true
                        spacing: 5
                        delegate: Rectangle {
                            id: productRow
                            width: ListView.view.width
                            height: 54
                            radius: 13
                            property bool highlighted: rowHover.containsMouse || removeProductButton.hovered || removeProductButton.down
                            color: highlighted ? "#FCE5E5" : "#FFFFFF"
                            border.width: 1
                            border.color: highlighted ? "#E98C8C" : "#E7EBE2"

                            MouseArea {
                                id: rowHover
                                anchors.fill: parent
                                acceptedButtons: Qt.NoButton
                                hoverEnabled: true
                            }

                            RowLayout {
                                anchors.fill: parent
                                anchors.leftMargin: 14
                                anchors.rightMargin: 7
                                spacing: 7

                                Label {
                                    Layout.fillWidth: true
                                    text: modelData
                                    color: productRow.highlighted ? "#A52F38" : "#20352A"
                                    font.pixelSize: 14
                                    elide: Text.ElideRight
                                    verticalAlignment: Text.AlignVCenter
                                }

                                Button {
                                    id: removeProductButton
                                    Layout.preferredWidth: 42
                                    Layout.preferredHeight: 42
                                    hoverEnabled: true
                                    onClicked: foodController.removeProduct(index)
                                    background: Rectangle {
                                        radius: 12
                                        color: removeProductButton.down || removeProductButton.hovered ? "#F7C8C8" : "#FDECEC"
                                    }
                                    contentItem: Item {
                                        Image {
                                            anchors.centerIn: parent
                                            source: "qrc:/qml/trash_icon.png"
                                            width: 20
                                            height: 20
                                            fillMode: Image.PreserveAspectFit
                                        }
                                    }
                                    ToolTip.visible: hovered
                                    ToolTip.text: "Удалить продукт"
                                }
                            }
                        }
                    }

                    Rectangle {
                        Layout.fillWidth: true
                        height: 1
                        color: "#E4E9E0"
                    }

                    Label {
                        Layout.fillWidth: true
                        text: "Исключённые продукты"
                        font.pixelSize: 20
                        font.bold: true
                        color: "#20352A"
                    }
                    Label {
                        Layout.fillWidth: true
                        text: "Эти ингредиенты не будут использоваться при подборе блюд."
                        wrapMode: Text.Wrap
                        font.pixelSize: 12
                        color: "#788579"
                    }
                    TextField {
                        id: excludedField
                        Layout.fillWidth: true
                        placeholderText: "Например: арахис, грибы"
                        onAccepted: {
                            foodController.addExcludedProduct(text, allergySwitch.checked)
                            clear()
                        }
                    }
                    RowLayout {
                        Layout.fillWidth: true
                        spacing: 6
                        CheckBox {
                            id: allergySwitch
                            text: "Аллергия"
                            Layout.fillWidth: true
                        }
                        Button {
                            text: "Исключить"
                            onClicked: {
                                foodController.addExcludedProduct(excludedField.text, allergySwitch.checked)
                                excludedField.clear()
                            }
                            Material.background: "#4F805B"
                            Material.foreground: "#FFFFFF"
                        }
                    }
                    Label {
                        Layout.fillWidth: true
                        text: "При аллергии обязательно проверяйте состав продукта и следы аллергенов: автоматический фильтр не гарантирует безопасность."
                        wrapMode: Text.Wrap
                        font.pixelSize: 11
                        color: "#AD6C43"
                    }
                    Repeater {
                        model: foodController.excludedProducts
                        delegate: Rectangle {
                            Layout.fillWidth: true
                            Layout.preferredHeight: 50
                            radius: 13
                            color: "#FFF1F0"
                            RowLayout {
                                anchors.fill: parent
                                anchors.leftMargin: 12
                                anchors.rightMargin: 7
                                spacing: 5
                                Label {
                                    Layout.fillWidth: true
                                    text: modelData.name + (modelData.allergy ? " · Аллергия" : " · Не нравится")
                                    color: "#973C45"
                                    elide: Text.ElideRight
                                    font.pixelSize: 13
                                }
                                Button {
                                    Layout.preferredWidth: 43
                                    Layout.preferredHeight: 40
                                    onClicked: foodController.removeExcludedProduct(index)
                                    background: Rectangle { radius: 12; color: "#F9DEDE" }
                                    contentItem: Item {
                                        Image {
                                            anchors.centerIn: parent
                                            source: "qrc:/qml/trash_icon.png"
                                            width: 20
                                            height: 20
                                            fillMode: Image.PreserveAspectFit
                                        }
                                    }
                                }
                            }
                        }
                    }

                    Rectangle {
                        Layout.fillWidth: true
                        height: 1
                        color: "#E4E9E0"
                    }

                    Label {
                        Layout.fillWidth: true
                        text: "Подходящие рецепты"
                        font.pixelSize: 20
                        font.bold: true
                        color: "#20352A"
                    }

                    Label {
                        Layout.fillWidth: true
                        visible: foodController.products.length === 0
                        text: "После добавления продуктов здесь появятся блюда, для которых не хватает не более двух ингредиентов."
                        wrapMode: Text.Wrap
                        color: "#788579"
                    }

                    Repeater {
                        model: foodController.myRecipes
                        delegate: RecipeCard {
                            Layout.fillWidth: true
                            recipe: modelData
                            showAddButton: true
                            onOpenRequested: function(id) { openRecipe(id) }
                            onAddRequested: function(id) { foodController.addToDayMenu(id) }
                            onFavoriteRequested: function(id) { foodController.toggleFavorite(id) }
                        }
                    }

                    Label {
                        Layout.fillWidth: true
                        visible: foodController.products.length > 0 && foodController.myRecipes.length === 0
                        text: "Подходящих рецептов пока нет. Добавьте ещё несколько продуктов."
                        wrapMode: Text.Wrap
                        horizontalAlignment: Text.AlignHCenter
                        color: "#788579"
                    }
                }
            }
        }

        // Personal page: favorites, shopping, nutrition goals.
        ScrollView {
            clip: true
            contentWidth: width
            ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
            ScrollBar.vertical.policy: ScrollBar.AlwaysOff

            ColumnLayout {
                x: 14
                y: 16
                width: Math.max(0, parent.width - 28)
                spacing: 12

                Label {
                    Layout.fillWidth: true
                    text: "Моё питание"
                    font.pixelSize: 24
                    font.bold: true
                    color: "#20352A"
                }
                Label {
                    Layout.fillWidth: true
                    text: "Цели по КБЖУ, покупки и любимые блюда"
                    color: "#788579"
                    font.pixelSize: 12
                    wrapMode: Text.Wrap
                }
                Rectangle {
                    Layout.fillWidth: true
                    implicitHeight: goalsContent.implicitHeight + 30
                    radius: 20
                    color: "#FFFFFF"
                    border.color: "#E7EBE2"
                    ColumnLayout {
                        id: goalsContent
                        x: 14
                        y: 14
                        width: parent.width - 28
                        spacing: 10
                        Label { text: "Мои цели"; color: "#20352A"; font.bold: true; font.pixelSize: 19 }
                        Label { Layout.fillWidth: true; text: "Цели используются при автоматическом подборе меню."; wrapMode: Text.Wrap; color: "#788579"; font.pixelSize: 12 }
                        GridLayout {
                            Layout.fillWidth: true
                            columns: 2
                            columnSpacing: 12
                            rowSpacing: 10

                            ColumnLayout {
                                Layout.fillWidth: true
                                spacing: 4
                                Label { text: "Калории, ккал"; color: "#526354"; font.pixelSize: 12 }
                                TextField {
                                    id: goalCalories
                                    Layout.fillWidth: true
                                    placeholderText: "Введите калории"
                                    text: String(foodController.nutritionGoals.calories)
                                    inputMethodHints: Qt.ImhFormattedNumbersOnly
                                }
                            }
                            ColumnLayout {
                                Layout.fillWidth: true
                                spacing: 4
                                Label { text: "Белки, г"; color: "#526354"; font.pixelSize: 12 }
                                TextField {
                                    id: goalProtein
                                    Layout.fillWidth: true
                                    placeholderText: "Введите белки"
                                    text: String(foodController.nutritionGoals.protein)
                                    inputMethodHints: Qt.ImhFormattedNumbersOnly
                                }
                            }
                            ColumnLayout {
                                Layout.fillWidth: true
                                spacing: 4
                                Label { text: "Жиры, г"; color: "#526354"; font.pixelSize: 12 }
                                TextField {
                                    id: goalFat
                                    Layout.fillWidth: true
                                    placeholderText: "Введите жиры"
                                    text: String(foodController.nutritionGoals.fat)
                                    inputMethodHints: Qt.ImhFormattedNumbersOnly
                                }
                            }
                            ColumnLayout {
                                Layout.fillWidth: true
                                spacing: 4
                                Label { text: "Углеводы, г"; color: "#526354"; font.pixelSize: 12 }
                                TextField {
                                    id: goalCarbs
                                    Layout.fillWidth: true
                                    placeholderText: "Введите углеводы"
                                    text: String(foodController.nutritionGoals.carbs)
                                    inputMethodHints: Qt.ImhFormattedNumbersOnly
                                }
                            }
                        }
                        Button {
                            Layout.fillWidth: true
                            text: "Сохранить цели"
                            onClicked: foodController.setNutritionGoals(Number(goalCalories.text.replace(",", ".")),
                                                                            Number(goalProtein.text.replace(",", ".")),
                                                                            Number(goalFat.text.replace(",", ".")),
                                                                            Number(goalCarbs.text.replace(",", ".")))
                            background: Rectangle { radius: height / 2; color: "#4F805B" }
                            contentItem: Label { text: parent.text; color: "white"; font.bold: true; horizontalAlignment: Text.AlignHCenter; verticalAlignment: Text.AlignVCenter }
                        }
                    }
                }

                Label { Layout.fillWidth: true; text: "Список покупок"; font.pixelSize: 20; font.bold: true; color: "#20352A" }
                RowLayout {
                    Layout.fillWidth: true
                    TextField {
                        id: shoppingField
                        Layout.fillWidth: true
                        placeholderText: "Добавить продукт"
                        onAccepted: { foodController.addShoppingItem(text); clear() }
                    }
                    Button {
                        text: "+"
                        onClicked: { foodController.addShoppingItem(shoppingField.text); shoppingField.clear() }
                        Material.background: "#4F805B"
                        Material.foreground: "white"
                    }
                }
                Button {
                    Layout.fillWidth: true
                    text: "Добавить недостающие из меню"
                    onClicked: foodController.addMissingToShopping()
                    Material.background: "#E3EEDF"
                    Material.foreground: "#3F7950"
                }
                Label {
                    Layout.fillWidth: true
                    visible: foodController.shoppingList.length === 0
                    text: "Список пуст. Добавьте продукт или соберите недостающие из меню."
                    color: "#788579"
                    wrapMode: Text.Wrap
                    font.pixelSize: 12
                }
                Repeater {
                    model: foodController.shoppingList
                    delegate: Rectangle {
                        Layout.fillWidth: true
                        Layout.preferredHeight: 53
                        radius: 13
                        color: "#FFFFFF"
                        border.color: "#E7EBE2"
                        RowLayout {
                            anchors.fill: parent
                            anchors.leftMargin: 8
                            anchors.rightMargin: 7
                            CheckBox {
                                checked: modelData.done
                                onClicked: foodController.toggleShoppingItem(index)
                            }
                            Label {
                                Layout.fillWidth: true
                                text: modelData.name
                                elide: Text.ElideRight
                                font.pixelSize: 14
                                color: modelData.done ? "#909D91" : "#20352A"
                                font.strikeout: modelData.done
                            }
                            Button {
                                Layout.preferredWidth: 41
                                Layout.preferredHeight: 41
                                onClicked: foodController.removeShoppingItem(index)
                                background: Rectangle { radius: 12; color: "#FCE8E8" }
                                contentItem: Item {
                                    anchors.fill: parent
                                    Image {
                                        anchors.centerIn: parent
                                        source: "qrc:/qml/trash_icon.png"
                                        width: 20
                                        height: 20
                                        fillMode: Image.PreserveAspectFit
                                    }
                                }
                            }
                        }
                    }
                }

                Label { Layout.fillWidth: true; text: "Избранные рецепты"; font.pixelSize: 20; font.bold: true; color: "#20352A" }
                Label {
                    Layout.fillWidth: true
                    visible: foodController.favoriteRecipes.length === 0
                    text: "Нажмите ♡ на любом рецепте, чтобы добавить его сюда."
                    color: "#788579"
                    wrapMode: Text.Wrap
                    font.pixelSize: 12
                }
                Repeater {
                    model: foodController.favoriteRecipes
                    delegate: RecipeCard {
                        Layout.fillWidth: true
                        recipe: modelData
                        showAddButton: true
                        onOpenRequested: function(id) { openRecipe(id) }
                        onAddRequested: function(id) { foodController.addToDayMenu(id) }
                        onFavoriteRequested: function(id) { foodController.toggleFavorite(id) }
                    }
                }
                Item { Layout.preferredHeight: 20 }
            }
        }
    }

    ToolBar {
        id: header
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        height: 70
        background: Rectangle { color: "#FFFFFF" }

        RowLayout {
            anchors.fill: parent
            anchors.leftMargin: 19
            anchors.rightMargin: 19
            spacing: 11

            Rectangle {
                Layout.preferredWidth: 39
                Layout.preferredHeight: 39
                radius: 13
                color: "#E5EFE2"
                Label {
                    anchors.centerIn: parent
                    text: "✦"
                    color: "#4F805B"
                    font.pixelSize: 23
                }
            }

            ColumnLayout {
                Layout.fillWidth: true
                spacing: 0
                Label {
                    text: "Food Idea"
                    font.pixelSize: 21
                    font.bold: true
                    color: "#20352A"
                }
                Label {
                    text: "вкусно и сбалансированно"
                    font.pixelSize: 11
                    color: "#849084"
                }
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
        background: Rectangle {
            color: "#FFFFFF"
            border.color: "#E7EBE2"
            border.width: 1
        }
        onCurrentIndexChanged: window.currentTab = currentIndex

        TabButton {
            text: "⌂\nМеню"
            font.pixelSize: 12
            contentItem: Label {
                text: parent.text
                color: parent.checked ? "#3F7950" : "#879287"
                font.pixelSize: 12
                font.bold: parent.checked
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }
            background: Rectangle {
                color: parent.checked ? "#EDF4EA" : "transparent"
                radius: 13
                anchors.fill: parent
                anchors.margins: 5
            }
        }
        TabButton {
            text: "▤\nРецепты"
            font.pixelSize: 12
            contentItem: Label {
                text: parent.text
                color: parent.checked ? "#3F7950" : "#879287"
                font.pixelSize: 12
                font.bold: parent.checked
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }
            background: Rectangle {
                color: parent.checked ? "#EDF4EA" : "transparent"
                radius: 13
                anchors.fill: parent
                anchors.margins: 5
            }
        }
        TabButton {
            text: "＋\nПродукты"
            font.pixelSize: 12
            contentItem: Label {
                text: parent.text
                color: parent.checked ? "#3F7950" : "#879287"
                font.pixelSize: 12
                font.bold: parent.checked
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }
            background: Rectangle {
                color: parent.checked ? "#EDF4EA" : "transparent"
                radius: 13
                anchors.fill: parent
                anchors.margins: 5
            }
        }
        TabButton {
            text: "♡\nМоё"
            font.pixelSize: 12
            contentItem: Label {
                text: parent.text
                color: parent.checked ? "#3F7950" : "#879287"
                font.pixelSize: 12
                font.bold: parent.checked
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }
            background: Rectangle {
                color: parent.checked ? "#EDF4EA" : "transparent"
                radius: 13
                anchors.fill: parent
                anchors.margins: 5
            }
        }
    }
}

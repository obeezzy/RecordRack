import "../../rrui" as RRUi
import Fluid.Controls 1.0 as FluidControls
import QtCharts 2.3 as QCharts
import QtQuick 2.12
import QtQuick.Controls 2.12 as QQC2
import QtQuick.Controls.Material 2.3
import QtQuick.Layouts 1.3 as QQLayouts

RRUi.HomeCard {
    id: mostSoldProductCard

    cardTitle: qsTr("Top sold products today")
    subtitle: qsTr("Top 5")
    bottomPadding: 4

    QQLayouts.RowLayout {
        height: 300

        anchors {
            left: parent.left
            right: parent.right
        }

        QCharts.ChartView {
            id: chartView

            QQLayouts.Layout.preferredWidth: 250
            QQLayouts.Layout.preferredHeight: 250
            QQLayouts.Layout.maximumWidth: 251
            QQLayouts.Layout.maximumHeight: 251
            legend.visible: false
            antialiasing: true
            animationOptions: QCharts.ChartView.SeriesAnimations
            backgroundRoundness: 0
            backgroundColor: "#eeeeee"

            margins {
                top: 0
                bottom: 0
                left: 0
                right: 0
            }

            QCharts.PieSeries {
                id: pieSeries

                property var colorModel: []

                function explodeSlice(index) {
                    for (var i = 0; i < pieSeries.count; ++i) {
                        pieSeries.at(i).exploded = index === i;
                    }
                }

                name: qsTr("Top sold")
                opacity: 0.6
                useOpenGL: true
                Component.onCompleted: {
                    var model = [];
                    for (var i = 0; i < pieSeries.count; ++i) model.push(pieSeries.at(i).color)
                    pieSeries.colorModel = model;
                    pieSeries.at(0).exploded = true;
                }

                QCharts.VPieModelMapper {
                    model: mostSoldProductCard.model
                    labelsColumn: 0
                    valuesColumn: 1
                }

            }

        }

        RRUi.ChartTableView {
            QQLayouts.Layout.fillWidth: true
            QQLayouts.Layout.alignment: Qt.AlignHCenter
            height: contentHeight
            model: mostSoldProductCard.model
            interactive: false
            headerData: [{
                "width": 20,
                "type": "color"
            }, {
                "title": qsTr("Product"),
                "width": 130,
                "role": "product"
            }, {
                "title": qsTr("Total quantity"),
                "width": 100,
                "horizontalAlignment": Qt.AlignHCenter,
                "role": "total_quantity"
            }]
            colorModel: pieSeries.colorModel
            onRowEntered: pieSeries.explodeSlice(row)
        }

    }

}

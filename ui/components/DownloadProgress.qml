import QtQuick
import QtQuick.Controls

import hex_launcher

Item{
    id: dlProgress

    property int numOfUpdates
    property bool isUpdating: false
    property int updateNum

    ProgressBar{
        id: progress
        anchors{
            top: parent.top
            left: parent.left
            right: parent.right
        }

        from: 0


        Connections{
            target: GlobalModels.updater

            function onDownloadingBaseMod(){
                dlText.text = "downloading retro rewind"
            }

            function onUpdateFileSize(size){
                progress.to = size
            }

            function onBytesDownloaded(downloaded){
                progress.value = downloaded
            }

            function onUnzipFile(){
                if(dlProgress.isUpdating){
                    dlText.text = "installing update"
                }
                else{
                    dlText.text = "installing base mod"
                }
                loading.running = true
                loading.visible = true
                progress.visible = false
            }

            function onFetchingUpdates() {
                dlText.text = "getting updates"
            }

            function onNumOfUpdates(num: int) {
                dlProgress.numOfUpdates = num
            }

            function onInstallingUpdate(index: int){
                dlText.text = "installing update " + index + " / " + dlProgress.numOfUpdates
            }

            function onModInstallFinished(){
                dlText.text = "retro rewind installed and updated"
            }

        }

    }

    Text{
        id: dlText
        anchors{
            verticalCenter: parent.verticalCenter
            horizontalCenter: parent.horizontalCenter
        }

        color: "white"
        
    }

    BusyIndicator{
        id: loading
        height: 50
        width: 50
        anchors{
            right: dlText.left
            verticalCenter: dlText.verticalCenter
            rightMargin: 10
        }

        visible: false
    }

}
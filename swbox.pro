#-------------------------------------------------
#
# Project created by QtCreator 2016-04-06T17:14:13
#
#-------------------------------------------------

QT  += core gui widgets
QT  += network sql xml charts serialport
QT  += qml quick quickwidgets

static{
    QTPLUGIN+=qtvirtualkeyboardplugin
    QT+=svg
}

TARGET = swbox
TEMPLATE = app

DEFINES += AMG             #OEM
#DEFINES += XDP_II          #OEM
#DEFINES += OHV             #OEM
#DEFINES += ZDIT            #OEM
#DEFINES += NO_OEM           #OEM

DEFINES += MULTI_CHANNEL        #多通道

TRANSLATIONS += trans/en.ts

#编译优化选项
#QMAKE_CXXFLAGS += -g
#QMAKE_CXXFLAGS_RELEASE += -O        # Release -O
#QMAKE_CXXFLAGS_RELEASE += -O1       # Release -O1
#QMAKE_CXXFLAGS_RELEASE -= -O2       # Release -O2
#QMAKE_CXXFLAGS_RELEASE += -O3       # Release -O3


SOURCES += \
    IO/Data/fifodata.cpp \
    IO/Data/recwave.cpp \
    IO/SqlCfg/sqlcfg.cpp \
    IO/Key/keydetect.cpp \
    IO/Com/rdb/thread.c \
    IO/Com/rdb/rdb.c \
    IO/Com/rdb/ipcs.c \
    main.cpp \
    Gui/Common/common.cpp \
    Gui/Common/barchart.cpp \
    Gui/Function/tevwidget.cpp \
    Gui/Function/hfctwidget.cpp \
    Gui/Function/faultlocation.cpp \
    Gui/Function/aawidget.cpp \
    IO/Other/battery.cpp \
    Gui/Options/systeminfo.cpp \
    Gui/Options/recwavemanage.cpp \
    Gui/Options/options.cpp \
    Gui/Options/factoryreset.cpp \
    Gui/Options/debugset.cpp \
    Gui/Common/recwaveform.cpp \
    Gui/Function/aewidget.cpp \
    Gui/mainwindow.cpp \
    IO/Data/reccontrol.cpp \
    IO/Data/fifocontrol.cpp \
    Gui/Qml/quickview.cpp \
    IO/Other/CPU/xadc_core.c \
    IO/Other/CPU/cpustatus.cpp \
    Gui/Common/prpsscene.cpp \
    Gui/Function/uhfwidget.cpp \
    IO/Other/buzzer.cpp \
    Gui/Function/Asset/assetview.cpp \
    Gui/Function/Asset/assetmodel.cpp \
    Gui/Function/Asset/assetsql.cpp \
    Gui/Function/Asset/assetwidget.cpp \
    Gui/Function/Asset/assetfilemanagement.cpp \
    IO/Other/cpu.c \
    Gui/Common/basewidget.cpp \
    Gui/Function/channelwidget.cpp \
    IO/Com/socket.cpp \
    IO/Com/rdb/gpio_oper.c \
    Gui/Common/wifipassword.cpp \
    IO/File/filetools.cpp \
    IO/File/logtools.cpp \
    IO/File/report.cpp \
    Algorithm/Bp/bp.cpp \
    Algorithm/Bp/bpcable.cpp \
    Algorithm/Wavelet/wavelet.cpp \
    Algorithm/compute.cpp \
    Algorithm/fft.cpp \
    Algorithm/fir.cpp \
    IO/Sync/syncthread.cpp \
    IO/File/spacecontrol.cpp \
    Gui/Common/wifi.cpp \
    IO/Com/tcpsocket.cpp \
    Gui/Charts/prpdchart.cpp \
    Gui/Charts/historicchart.cpp \
    Gui/Charts/basechart.cpp \
    Gui/Charts/prpschart.cpp \
    Gui/Charts/histogramchart.cpp \
    Gui/Charts/spectrachart.cpp \
    Gui/Charts/tfchart.cpp \
    Gui/Charts/flychart.cpp \
    Gui/Functions/hchannelfunction.cpp \
    Gui/Functions/basechannlfunction.cpp \
    Gui/Functions/lchannelfunction.cpp \
    Gui/Common/contextmenu.cpp \
    Gui/Function/settingmunu.cpp \
    Gui/Charts/camerachart.cpp \
    Gui/Options/optionwidget.cpp \
    Gui/Options/Option/optionbasic.cpp \
    Gui/Options/Option/optionexpert.cpp \
    Gui/Options/Option/optionstorage.cpp \
    Gui/Options/Option/optionwifi.cpp \
    IO/File/testtools.cpp \
    IO/Com/modbussync.cpp \
    IO/Com/modbus.cpp \
    Gui/mainfunction.cpp \
    Thread/Camera/cameradata.cpp \
    Thread/Camera/cameradecode.cpp \
    Thread/Camera/camerasocket1.cpp \
    Thread/Camera/camerasocket.cpp \
    Thread/Sync/synccompute.cpp \
    Thread/Sync/syncsocket.cpp \
    Thread/Sync/gpsinfo.cpp


HEADERS  += \
    IO/Data/fifodata.h \
    IO/Data/zynq.h \
    IO/Data/data.h \
    IO/Data/recwave.h \
    IO/Key/key.h \
    IO/Key/keydetect.h \
    IO/SqlCfg/sqlcfg.h \    
    IO/Com/rdb/thread.h \
    IO/Com/rdb/rdb.h \
    IO/Com/rdb/point_table.h \
    IO/Com/rdb/ipcs.h \
    IO/Com/rdb/data_types.h \
    IO/Com/rdb/comm_head.h \
    Gui/Common/common.h \
    Gui/Common/barchart.h \
    Gui/Function/tevwidget.h \
    Gui/Function/hfctwidget.h \
    Gui/Function/faultlocation.h \
    Gui/Function/aawidget.h \
    IO/Other/battery.h \
    Gui/Options/systeminfo.h \
    Gui/Options/recwavemanage.h \
    Gui/Options/options.h \
    Gui/Options/factoryreset.h \
    Gui/Options/debugset.h \
    Gui/Common/recwaveform.h \
    Gui/Function/aewidget.h \
    Gui/mainwindow.h \
    IO/Data/reccontrol.h \
    IO/Data/fifocontrol.h \
    Gui/Qml/quickview.h \
    IO/Other/CPU/xadc_core_if.h \
    IO/Other/CPU/xadc_core.h \
    IO/Other/CPU/types.h \
    IO/Other/CPU/events.h \
    IO/Other/CPU/cpustatus.h \
    Gui/Common/prpsscene.h \
    Gui/Function/uhfwidget.h \
    IO/Other/buzzer.h \
    Gui/Function/Asset/assetview.h \
    Gui/Function/Asset/assetmodel.h \
    Gui/Function/Asset/assetsql.h \
    Gui/Function/Asset/assetwidget.h \
    Gui/Function/Asset/assetfilemanagement.h \
    IO/Other/cpu.h \
    Gui/Common/basewidget.h \
    Gui/Function/channelwidget.h \
    IO/Com/socket.h \
    IO/Com/rdb/global_define.h \
    IO/Com/rdb/gpio_oper.h \
    Gui/Common/wifipassword.h \
    IO/File/filetools.h \
    IO/File/logtools.h \
    IO/File/report.h \
    Algorithm/Bp/bp.h \
    Algorithm/Bp/bpcable.h \
    Algorithm/Wavelet/wavelet.h \
    Algorithm/compute.h \
    Algorithm/fft.h \
    Algorithm/fir.h \
    IO/Sync/syncthread.h \
    IO/File/spacecontrol.h \
    Gui/Common/wifi.h \
    IO/Com/tcpsocket.h \
    Gui/Charts/prpdchart.h \
    Gui/Charts/historicchart.h \
    Gui/Charts/basechart.h \
    Gui/Charts/prpschart.h \
    Gui/Charts/histogramchart.h \
    Gui/Charts/spectrachart.h \
    Gui/Charts/tfchart.h \
    Gui/Charts/flychart.h \
    Gui/Functions/hchannelfunction.h \
    Gui/Functions/basechannlfunction.h \
    Gui/Functions/lchannelfunction.h \
    Gui/Common/contextmenu.h \
    Gui/Function/settingmunu.h \
    Gui/Charts/camerachart.h \
    Gui/Options/optionwidget.h \
    Gui/Options/Option/optionbasic.h \
    Gui/Options/Option/optionexpert.h \
    Gui/Options/Option/optionstorage.h \
    Gui/Options/Option/optionwifi.h \
    IO/File/testtools.h \
    IO/Com/modbussync.h \
    IO/Com/modbus.h \
    Gui/mainfunction.h \
    Thread/Camera/cameradata.h \
    Thread/Camera/cameradecode.h \
    Thread/Camera/camerasocket1.h \
    Thread/Camera/camerasocket.h \
    Thread/Sync/synccompute.h \
    Thread/Sync/syncsocket.h \
    Thread/Sync/gpsinfo.h

FORMS += \
    Gui/Function/tevwidget.ui \
    Gui/Function/hfctwidget.ui \
    Gui/Function/faultlocation.ui \
    Gui/Function/aawidget.ui \
    Gui/Function/aewidget.ui \
    Gui/Options/voiceplayer.ui \
    Gui/Options/systeminfo.ui \
    Gui/Options/optionui.ui \
    Gui/Options/debugui.ui \
    Gui/mainwindow.ui \
    Gui/Function/uhfwidget.ui \
    Gui/Function/Asset/assetwidget.ui \
    Gui/Options/apinfo.ui \
    Gui/Options/Option/optionbasic.ui \
    Gui/Options/Option/optionexpert.ui \
    Gui/Options/Option/optionstorage.ui \
    Gui/Options/Option/optionwifi.ui \
    Gui/Options/optionwidget.ui


RESOURCES += \
    resource/resource.qrc

DISTFILES += \
    BOOT.bin \
    trans/en.ts \
    resource/Busy.qml \
    resource/Input.qml \
    resource/MyInputPanel.qml \
    resource/MyKeyboard.qml

################################################################################

#qwt & sqlite3
INCLUDEPATH += \
    /usr/local/qwt-6.1.3/include \
    /usr/local/sqlite3/include
LIBS += \
    -L"/usr/local/qwt-6.1.3/lib" -lqwt \
    -L"/usr/local/sqlite3/lib" -lsqlite3

#Linux线程
unix:!macx: LIBS += -L$$PWD/../../../../pub/toolchain/arm-xilinx-linux-gnueabi-4.9.2/arm-xilinx-linux-gnueabi/libc/usr/lib/ -lpthread
INCLUDEPATH += $$PWD/../../../../pub/toolchain/arm-xilinx-linux-gnueabi-4.9.2/arm-xilinx-linux-gnueabi/libc/usr/include
DEPENDPATH += $$PWD/../../../../pub/toolchain/arm-xilinx-linux-gnueabi-4.9.2/arm-xilinx-linux-gnueabi/libc/usr/include
unix:!macx: PRE_TARGETDEPS += $$PWD/../../../../pub/toolchain/arm-xilinx-linux-gnueabi-4.9.2/arm-xilinx-linux-gnueabi/libc/usr/lib/libpthread.a

#FFT-NE10
unix:!macx: LIBS += -L$$PWD/lib/ -lNE10
INCLUDEPATH += $$PWD/lib/NE10
DEPENDPATH += $$PWD/lib/NE10
unix:!macx: PRE_TARGETDEPS += $$PWD/lib/libNE10.a

#小波变换-SP++3.0
unix:!macx: LIBS += -L$$PWD/../../../disk1/lib/SP++3.0/lib/ -lfftw3
INCLUDEPATH += $$PWD/../../../disk1/lib/SP++3.0/include
DEPENDPATH += $$PWD/../../../disk1/lib/SP++3.0/include
unix:!macx: PRE_TARGETDEPS += $$PWD/../../../disk1/lib/SP++3.0/lib/libfftw3.a

#视频解码-ffmpeg4.0.2
#INCLUDEPATH += /usr/local/ffmpeg/include
#LIBS += /usr/local/ffmpeg/lib/avcodec.a\
#        /usr/local/ffmpeg/lib/avdevice.a\
#        /usr/local/ffmpeg/lib/avfilter.a\
#        /usr/local/ffmpeg/lib/avformat.a\
#        /usr/local/ffmpeg/lib/avutil.a\
#        /usr/local/ffmpeg/lib/postproc.a\
#        /usr/local/ffmpeg/lib/swresample.a\
#        /usr/local/ffmpeg/lib/swscale.a
INCLUDEPATH += $$/usr/local/ffmpeg/include
DEPENDPATH += $$/usr/local/ffmpeg/include

LIBS += -L$$/usr/local/ffmpeg/lib/ -lavcodec\
        -L$$/usr/local/ffmpeg/lib/ -lavdevice\
        -L$$/usr/local/ffmpeg/lib/ -lavfilter\
        -L$$/usr/local/ffmpeg/lib/ -lavformat\
        -L$$/usr/local/ffmpeg/lib/ -lavutil\
        -L$$/usr/local/ffmpeg/lib/ -lpostproc\
        -L$$/usr/local/ffmpeg/lib/ -lswresample\
        -L$$/usr/local/ffmpeg/lib/ -lswscale






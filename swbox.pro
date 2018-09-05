#-------------------------------------------------
#
# Project created by QtCreator 2016-04-06T17:14:13
#
#-------------------------------------------------

QT  += core gui widgets network
QT  += serialport sql qml quick quickwidgets xml
#QT += opengl

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

#DEFINES += TEST_LAB


TRANSLATIONS += trans/en.ts

#if(contains(DEFINES,ARM)) {
#    message( "Configuring for arm build..." )
#} else {
#    message( "Configuring for pc build..." )
#}

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
    IO/Other/logtools.cpp \
    IO/Other/filetools.cpp \
    IO/Data/reccontrol.cpp \
    IO/Data/fifocontrol.cpp \
    Gui/Qml/quickview.cpp \
    IO/Other/CPU/xadc_core.c \
    IO/Other/CPU/cpustatus.cpp \
    Gui/Common/prpsscene.cpp \
    Gui/Common/fft.cpp \
    Gui/Function/uhfwidget.cpp \
    IO/Other/buzzer.cpp \
    Gui/Function/Asset/assetview.cpp \
    Gui/Function/Asset/assetmodel.cpp \
    Gui/Function/Asset/assetsql.cpp \
    Gui/Function/Asset/assetwidget.cpp \
    Gui/Common/compute.cpp \
    IO/Other/report.cpp \
    Gui/Function/Asset/assetfilemanagement.cpp \
    Gui/Common/fir.cpp \
    IO/Other/cpu.c \
    Gui/Common/basewidget.cpp \
    Gui/Function/channelwidget.cpp \
    IO/Com/socket.cpp \
    IO/Com/rdb/gpio_oper.c \
    Gui/Common/wifipassword.cpp \
    Gui/Algorithm/Bp/bp.cpp \
    Gui/Algorithm/Bp/bpcable.cpp \
    Gui/Algorithm/Wavelet/wavelet.cpp


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
    IO/Other/logtools.h \
    IO/Other/filetools.h \
    IO/Data/reccontrol.h \
    IO/Data/fifocontrol.h \
    Gui/Qml/quickview.h \
    IO/Other/CPU/xadc_core_if.h \
    IO/Other/CPU/xadc_core.h \
    IO/Other/CPU/types.h \
    IO/Other/CPU/events.h \
    IO/Other/CPU/cpustatus.h \
    Gui/Common/prpsscene.h \
    Gui/Common/fft.h \
    Gui/Function/uhfwidget.h \
    IO/Other/buzzer.h \
    Gui/Function/Asset/assetview.h \
    Gui/Function/Asset/assetmodel.h \
    Gui/Function/Asset/assetsql.h \
    Gui/Function/Asset/assetwidget.h \
    Gui/Common/compute.h \
    IO/Other/report.h \
    Gui/Function/Asset/assetfilemanagement.h \
    Gui/Common/fir.h \
    IO/Other/cpu.h \
    Gui/Common/basewidget.h \
    Gui/Function/channelwidget.h \
    IO/Com/socket.h \
    IO/Com/rdb/global_define.h \
    IO/Com/rdb/gpio_oper.h \
    Gui/Common/wifipassword.h \
    Gui/Algorithm/Bp/bp.h \
    Gui/Algorithm/Bp/bpcable.h \
    Gui/Algorithm/Wavelet/wavelet.h

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
    Gui/Options/apinfo.ui


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

#qwt
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

#NE10
unix:!macx: LIBS += -L$$PWD/lib/ -lNE10
INCLUDEPATH += $$PWD/lib/NE10
DEPENDPATH += $$PWD/lib/NE10
unix:!macx: PRE_TARGETDEPS += $$PWD/lib/libNE10.a

#小波变换
unix:!macx: LIBS += -L$$PWD/../../../disk1/lib/SP++3.0/lib/ -lfftw3
INCLUDEPATH += $$PWD/../../../disk1/lib/SP++3.0/include
DEPENDPATH += $$PWD/../../../disk1/lib/SP++3.0/include
unix:!macx: PRE_TARGETDEPS += $$PWD/../../../disk1/lib/SP++3.0/lib/libfftw3.a






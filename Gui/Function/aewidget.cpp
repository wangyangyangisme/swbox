﻿#include "aewidget.h"
#include "ui_aewidget.h"
#include <QLineEdit>

#define SETTING_NUM 9           //设置菜单条目数
#define VALUE_MAX 60

AEWidget::AEWidget(G_PARA *data, CURRENT_KEY_VALUE *val, MODE mode, int menu_index, QWidget *parent) :
    ChannelWidget(data, val, mode, menu_index, parent),
    ui(new Ui::AEWidget)
{
    ui->setupUi(this);
    this->resize(CHANNEL_X, CHANNEL_Y);
    this->move(3, 3);
    this->setStyleSheet("AEWidget {border-image: url(:/widgetphoto/bk/bk2.png);}");
    Common::set_comboBox_style(ui->comboBox);

    recWaveForm->raise();

    reload(-1);
    temp_db = 0;
    fly_number = 0;

    //图形设置
    chart_ini();

    timer_fly = new QTimer(this);
    timer_fly->setInterval(20000);
    connect(timer_fly, SIGNAL(timeout()), this, SLOT(fly_Reset()));   //每20秒刷新一次数据状态

    reload(menu_index);
}

AEWidget::~AEWidget()
{
    delete ui;
}

void AEWidget::fresh_1000ms()
{
    fresh(true);
    d_BarChart->fresh();
    plot_Barchart->replot();
}

void AEWidget::fresh_100ms()
{
    fresh(false);
    //为一个工频周期选取32个点，用于fft变换
    ae_fftlist.clear();
    for (int i = 0; i < ae_datalist.count(); ++i) {
        if(i%25 == 0){
            ae_fftlist.append(ae_datalist.at(i));
        }
    }

    pulse_100ms = Common::calc_pulse_list(ae_datalist,aeultra_sql->fpga_threshold);

//    qDebug()<<"pulse_100ms"<<pulse_100ms;
//    qDebug()<<"ae_datalist"<<ae_datalist;
//    qDebug()<<"ae_timelist"<<ae_timelist;

    //特征指数图
    int space;      //相邻两次脉冲的间隔时间
    int temp;
    if(pulse_100ms.count() > 1){
        for (int i = 1; i < pulse_100ms.count(); ++i) {
            temp = pulse_100ms.at(i).x() - pulse_100ms.at(i-1).x();
            if(temp > 20){
                //                 qDebug()<<temp;
            }
            space = (pulse_100ms.at(i).x() - pulse_100ms.at(i-1).x() ) * 320000.0 / 128 / 100000;
            //            qDebug()<<space;
            if(space >= 3 && space <= 60){
                histogram_map[space]++;
            }
        }
    }
    histogram_data.clear();

    for(int i=0;i<60;i++){
        QwtInterval interval( i * 0.1 , (i+1) * 0.1 );
        interval.setBorderFlags( QwtInterval::ExcludeMaximum );
        histogram_data.append( QwtIntervalSample( histogram_map[i], interval ) );
    }

    d_histogram->setData(new QwtIntervalSeriesData( histogram_data ));

    plot_Histogram->replot();




    //频谱图
    do_Spectra_compute();


    //PRPD
    int x,y, time;
    double _y;

    for(int i=0; i<pulse_100ms.count(); i++){
        //        x = Common::time_to_phase(pulse_100ms.at(i).x() );              //时标
        time = pulse_100ms.at(i).x() * 320000 / 128 + ae_timelist.first();  //时标
        x = Common::time_to_phase(time );              //时标(待定)
        x = (x/2)*2;            //去掉奇数项
        _y = Common::physical_value(pulse_100ms.at(i).y(),mode);         //强度
        y = (int)20*log(qAbs(_y) );


        if(x<360 && x>=0 && y<=60 &&y>=-60){
            QwtPoint3D p0(x,y,map[x][y+60]);
            map[x][y+60]++;
            QwtPoint3D p1(x,y,map[x][y+60]);
            if(map[x][y+60]>1){
                int n = prpd_samples.indexOf(p0);
                prpd_samples[n] = p1;
            }
            else{
                prpd_samples.append(p1);
            }
        }
        else{
            //            qDebug()<<QPointF(x,y) << "\t"<< pulse_100ms.at(i);
        }
        time /= 200000;
        if(time > 200){
            time -= 200;
        }
        fly_samples.append(QwtPoint3D(time, y, 1));
    }


    d_PRPD->setSamples(prpd_samples);
    plot_PRPD->replot();
    d_fly->setSamples(fly_samples);
    plot_fly->replot();
    ae_timelist.clear();

}

void AEWidget::reload(int index)
{
    //基本sql内容的初始化
    sql_para = *sqlcfg->get_para();
    if(mode == AE1){
        aeultra_sql = &sql_para.ae1_sql;
        ae_pulse_data = &data->recv_para_envelope1;
    }
    else if(mode == AE2){
        aeultra_sql = &sql_para.ae2_sql;
        ae_pulse_data = &data->recv_para_envelope2;
    }

    //构造函数中计时器不启动
    if(index == menu_index){
        if(!timer_100ms->isActive()){
            timer_100ms->start();
        }
        if(!timer_1000ms->isActive()){
            timer_1000ms->start();
        }
        if(!timer_fly->isActive()){
            timer_fly->start();
        }
        if(mode == AE1){
            data->set_send_para (sp_vol_l1, aeultra_sql->vol);
            data->set_send_para (sp_aa_record_play, 0);        //耳机送1通道
        }
        else if(mode == AE2){
            data->set_send_para (sp_vol_l2, aeultra_sql->vol);
            data->set_send_para (sp_aa_record_play, 2);        //耳机送2通道
        }
        data->set_send_para(sp_auto_rec, 0);        //关闭自动录波
        ae_pulse_data->readComplete = 1;        //读取完成标志
        fresh_setting();
    }
}

void AEWidget::do_key_up_down(int d)
{
    key_val->grade.val1 = 1;
    Common::change_index(key_val->grade.val2, d, SETTING_NUM, 1);
}

void AEWidget::do_key_left_right(int d)
{
    QList<int> list;
    switch (key_val->grade.val2) {
    case 1:
        aeultra_sql->mode = !aeultra_sql->mode;
        break;
    case 2:
        list << BASIC << PRPD << FLY << Exponent << Spectra;
        Common::change_index(aeultra_sql->chart, d, list);
        break;
    case 3:
        if( (aeultra_sql->gain<9.95 && d>0) || (aeultra_sql->gain<10.15 && d<0) ){
            Common::change_index(aeultra_sql->gain, d * 0.1, 100, 0.1 );
        }
        else{
            Common::change_index(aeultra_sql->gain, d * 10, 100, 0.1 );
        }
        break;
    case 4:
        Common::change_index(aeultra_sql->vol, d, VOL_MAX, VOL_MIN );
        break;
    case 5:
        Common::change_index(aeultra_sql->low, d, aeultra_sql->high, 0 );
        break;
    case 6:
        Common::change_index(aeultra_sql->high, d, 60, aeultra_sql->low );
        break;
    case 7:
        aeultra_sql->fpga_threshold += Common::code_value(1,mode) * d;
        break;
    case 8:
        Common::change_index(aeultra_sql->time, d, TIME_MAX, TIME_MIN );
        break;
    default:
        break;
    }
}

void AEWidget::do_key_ok()
{
    sqlcfg->sql_save(&sql_para);
    reload(menu_index);
    switch (key_val->grade.val2) {
    case 8:
        key_val->grade.val1 = 1;        //为了锁住主界面，防止左右键切换通道
        emit startRecWave(mode, aeultra_sql->time);        //发送录波信号
        emit show_indicator(true);
//        isBusy = true;
        return;
    case 9:
        maxReset(ui->label_max);
        PRPDReset();
        fly_Reset();
        break;
    default:
        break;
    }
    ChannelWidget::do_key_ok();
}

void AEWidget::chart_ini()
{
    //barchart
    plot_Barchart = new QwtPlot(ui->widget);
    Common::set_barchart_style(plot_Barchart, VALUE_MAX);
    plot_Barchart->resize(200, 140);
    d_BarChart = new BarChart(plot_Barchart, &db, &aeultra_sql->high, &aeultra_sql->low);

    //PRPD
    plot_PRPD = new QwtPlot(ui->widget);
    plot_PRPD->resize(200, 140);
    d_PRPD = new QwtPlotSpectroCurve;
    Common::set_PRPD_style(plot_PRPD,d_PRPD,VALUE_MAX,PRPD_single);
    PRPDReset();

    //飞行图
    plot_fly = new QwtPlot(ui->widget);
    plot_fly->resize(200, 140);
    d_fly = new QwtPlotSpectroCurve;
    Common::set_fly_style(plot_fly,d_fly,VALUE_MAX);
    fly_Reset();

    //特征指数
    plot_Histogram = new QwtPlot(ui->widget);
    plot_Histogram->resize(200, 140);
    d_histogram = new QwtPlotHistogram;
    Common::set_histogram_style(plot_Histogram,d_histogram,0,6,0,100,"");

    //频谱图
    plot_Spectra = new QwtPlot(ui->widget);
    plot_Spectra->resize(200, 140);
    d_Spectra = new QwtPlotHistogram;
    Common::set_Spectra_style(plot_Spectra,d_Spectra,0,4,0,30,"");

}

void AEWidget::PRPDReset()
{
    for(int i=0;i<360;i++){
        for(int j=0;j<121;j++){
            map[i][j]=0;
        }
    }
    emit send_PRPD_data(prpd_samples);
    prpd_samples.clear();

    for (int i = 0; i < 60; ++i) {
        histogram_map[i]=0;
    }
    histogram_data.clear();
}

void AEWidget::save_channel()
{
    PRPDReset();
    ChannelWidget::save_channel();
}

void AEWidget::fly_Reset()
{
    fly_samples.clear();
}

void AEWidget::fresh_Histogram()
{
    histogram_data.clear();

    int tmp;

    for(int j=0;j<121;j++){
        tmp = 0;
        for(int i=0;i<360;i++){
            tmp += map[i][j];
        }
        QwtInterval interval( j - 60.0 , j - 59.0 );
        interval.setBorderFlags( QwtInterval::ExcludeMaximum );
        histogram_data.append( QwtIntervalSample( tmp, interval ) );
    }

    d_histogram->setData(new QwtIntervalSeriesData( histogram_data ));

    plot_Histogram->replot();
}

void AEWidget::fresh(bool f)
{
    int offset;
    double val,val_db;

    Compute::calc_aa_value(data,mode,aeultra_sql,&val, &val_db, &offset);

    if(db < int(val_db)){
        db = int(val_db);      //每秒的最大值
    }

    //记录并显示最大值
    if (max_db <val_db ) {
        max_db = val_db;
        ui->label_max->setText(tr("最大值: ") + QString::number(max_db));
    }

    if(f){  //直接显示（1s一次）
        int v_50Hz,v_100Hz;
        Compute::compute_f_value(ae_fftlist,fft,v_50Hz,v_100Hz);
        ui->label_50Hz->setText(QString("%1mV").arg(v_50Hz));
        ui->label_100Hz->setText(QString("%1mV").arg(v_100Hz));
        ui->progressBar_50Hz->setValue(v_50Hz);
        ui->progressBar_100Hz->setValue(v_100Hz);

        ui->label_val->setText(QString::number(val_db, 'f', 1));
        temp_db = val_db;
        //彩色显示
        if ( val_db >= aeultra_sql->high) {
            ui->label_val->setStyleSheet("QLabel {font-family:WenQuanYi Micro Hei;font-size:60px;color:red}");
            emit beep(menu_index, 2);        //蜂鸣器报警
        } else if (val_db >= aeultra_sql->low) {
            ui->label_val->setStyleSheet("QLabel {font-family:WenQuanYi Micro Hei;font-size:60px;color:yellow}");
            emit beep(menu_index, 1);
        } else {
            ui->label_val->setStyleSheet("QLabel {font-family:WenQuanYi Micro Hei;font-size:60px;color:green}");
        }

        int is_current = 0;
        if((int)key_val->grade.val0 == menu_index){
            is_current = 1;
        }

        if(mode == AE1){
            Common::rdb_set_yc_value(AE1_amplitude_yc,val_db,is_current);
//            Common::rdb_set_yc_value(AE1_severity,0,is_current);
//            Common::rdb_set_yc_value(AE1_gain,aeultra_sql->gain,is_current);
//            Common::rdb_set_yc_value(AE1_biased,aeultra_sql->offset,is_current);
            Common::rdb_set_yc_value(AE1_noise_biased_adv_yc,offset,is_current);
        }
        else if(mode == AE2){
            Common::rdb_set_yc_value(AE2_amplitude_yc,val_db,is_current);
//            Common::rdb_set_yc_value(AE2_severity,0,is_current);
//            Common::rdb_set_yc_value(AE2_gain,aeultra_sql->gain,is_current);
//            Common::rdb_set_yc_value(AE2_biased,aeultra_sql->offset,is_current);
            Common::rdb_set_yc_value(AE2_noise_biased_adv_yc,offset,is_current);
        }
        emit send_log_data(val_db,0,0,is_current,"NOISE");
    }
    else{   //条件显示
        if(qAbs(val_db-temp_db ) >= aeultra_sql->step){
            ui->label_val->setText(QString::number(val_db, 'f', 1));
            if ( val_db > aeultra_sql->high) {
                ui->label_val->setStyleSheet("QLabel {font-family:WenQuanYi Micro Hei;font-size:60px;color:red}");
                emit beep(menu_index, 2);        //蜂鸣器报警
            } else if (val_db >= aeultra_sql->low) {
                ui->label_val->setStyleSheet("QLabel {font-family:WenQuanYi Micro Hei;font-size:60px;color:yellow}");
                emit beep(menu_index, 1);
            } else {
                ui->label_val->setStyleSheet("QLabel {font-family:WenQuanYi Micro Hei;font-size:60px;color:green}");
            }
        }
    }
}

void AEWidget::fresh_setting()
{
    if (aeultra_sql->mode == single) {
        ui->comboBox->setItemText(0,tr("检测模式\t[单次]"));
        timer_100ms->setSingleShot(true);
        timer_1000ms->setSingleShot(true);
    } else {
        ui->comboBox->setItemText(0,tr("检测模式\t[连续]"));
        timer_100ms->setSingleShot(false);
        timer_1000ms->setSingleShot(false);
    }

    plot_Barchart->hide();
    plot_PRPD->hide();
    plot_fly->hide();
    plot_Histogram->hide();
    plot_Spectra->hide();

    switch (aeultra_sql->chart) {
    case BASIC:
        plot_Barchart->show();
        ui->comboBox->setItemText(1,tr("图形显示 \t[时序图]"));
        break;
    case PRPD:
        plot_PRPD->show();
        ui->comboBox->setItemText(1,tr("图形显示\t[PRPD]"));
        break;
    case FLY:
        plot_fly->show();
        ui->comboBox->setItemText(1,tr("图形显示 \t[飞行图]"));
        break;
    case Exponent:
        plot_Histogram->show();
        ui->comboBox->setItemText(1,tr("图形显示 [特征指数]"));
        break;
    case Spectra:
        plot_Spectra->show();
        ui->comboBox->setItemText(1,tr("图形显示  \t[频谱图]"));
        break;
    default:
        break;
    }

    ui->comboBox->setItemText(2,tr("增益调节\t[×%1]").arg(QString::number(aeultra_sql->gain, 'f', 1)) );
    ui->comboBox->setItemText(3,tr("音量调节\t[×%1]").arg(QString::number(aeultra_sql->vol)));
    ui->comboBox->setItemText(4,tr("黄色报警阈值\t[%1]dB").arg(QString::number(aeultra_sql->low)));
    ui->comboBox->setItemText(5,tr("红色报警阈值\t[%1]dB").arg(QString::number(aeultra_sql->high)));
    ui->comboBox->setItemText(6,tr("脉冲触发\t[%1]mV").arg(QString::number((int)Common::physical_value(aeultra_sql->fpga_threshold,mode) )));
    ui->comboBox->setItemText(7,tr("连续录波\t[%1]s").arg(aeultra_sql->time));

    ui->comboBox->setCurrentIndex(key_val->grade.val2-1);

    if (key_val->grade.val2 && key_val->grade.val0 == menu_index && key_val->grade.val5 == 0 && isBusy != true) {
        ui->comboBox->showPopup();
    }
    else{
        ui->comboBox->hidePopup();
    }

    ui->comboBox->lineEdit()->setText(tr(" 参 数 设 置"));
    emit fresh_parent();
}

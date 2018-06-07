﻿#include "basewidget.h"

BaseWidget::BaseWidget(G_PARA *data, CURRENT_KEY_VALUE *val, MODE mode, int menu_index, QWidget *parent) : QFrame(parent)
{
    this->data = data;
    this->key_val = val;
    this->mode = mode;
    this->menu_index = menu_index;

    this->db = 0;
    this->max_db = 0;
    this->db_last1 = 0;
    this->manual = false;
    this->token = 0;
    this->isBusy = false;

    timer_1000ms = new QTimer(this);
    timer_1000ms->setInterval(1000);
    connect(timer_1000ms, SIGNAL(timeout()), this, SLOT(fresh_1000ms()));

    timer_1ms = new QTimer(this);
    timer_1ms->setInterval(2);         //1ms读取一次数据
    connect(timer_1ms, SIGNAL(timeout()), this, SLOT(add_token()));

    timer_100ms = new QTimer(this);
    timer_100ms->setInterval(100);
    connect(timer_100ms, SIGNAL(timeout()), this, SLOT(fresh_100ms()));

    timer_freeze = new QTimer(this);      //timer3设置了一个界面手动退出后的锁定期,便于操作
    timer_freeze->setInterval(FREEZE_TIME);      //5秒内不出现新录波界面
    timer_freeze->setSingleShot(true);

    timer_rec_close_delay= new QTimer(this);          //timer_rec_close_delay用于延迟关闭录波系统，为节能启用
    timer_rec_close_delay->setInterval(2000);
    timer_rec_close_delay->setSingleShot(true);
    connect(timer_rec_close_delay, SIGNAL(timeout()), this, SLOT(close_rec()));

    recWaveForm = new RecWaveForm(menu_index,this);
    connect(this, SIGNAL(send_key(quint8)), recWaveForm, SLOT(trans_key(quint8)));
    connect(recWaveForm,SIGNAL(fresh_parent()),this,SIGNAL(fresh_parent()));
    connect(recWaveForm, SIGNAL(fresh_parent()), timer_freeze, SLOT(start()) );

    logtools = new LogTools(mode);      //日志保存模块
    connect(this,SIGNAL(send_log_data(double,int,double,int)),logtools,SLOT(dealLog(double,int,double,int)));
    connect(this,SIGNAL(send_PRPD_data(QVector<QwtPoint3D>)),logtools,SLOT(dealRPRDLog(QVector<QwtPoint3D>)));

    fft = new FFT;

}

void BaseWidget::change_log_dir()
{
    logtools->change_current_asset_dir();
}

void BaseWidget::showWaveData(VectorList buf, MODE mod)
{
    if(key_val->grade.val0 == menu_index){       //在超声界面，可以显示
        isBusy = false;
        emit show_indicator(false);
        key_val->grade.val1 = 1;        //为了锁住主界面，防止左右键切换通道
        key_val->grade.val5 = 1;
        emit fresh_parent();
        recWaveForm->working(key_val,buf,mod);
    }
    fresh_setting();
}

void BaseWidget::save_channel()
{

}

void BaseWidget::fresh_1000ms()
{
    qDebug()<<"base";
}

void BaseWidget::fresh_100ms()
{

}

void BaseWidget::fresh_1ms()
{

}

void BaseWidget::trans_key(quint8 key_code)
{
    if (key_val == NULL || key_val->grade.val0 != menu_index) {
        return;
    }

    if(isBusy){
        return;
    }

    if(key_val->grade.val5 != 0){
        emit send_key(key_code);
        return;
    }

    switch (key_code) {
    case KEY_OK:
        do_key_ok();
        break;
    case KEY_CANCEL:
        do_key_cancel();
        break;
    case KEY_UP:
        do_key_up_down(-1);
        break;
    case KEY_DOWN:
        do_key_up_down(1);
        break;
    case KEY_LEFT:
        do_key_left_right(-1);
        break;
    case KEY_RIGHT:
        do_key_left_right(1);
        break;
    default:
        break;
    }
    fresh_setting();
}

//这里可以根据不同模式设置不同的令牌增长速度
void BaseWidget::add_token()
{
//    qDebug()<<"token="<<token;
    if(token < TOKEN_MAX){
        token += 5;
    }
}

void BaseWidget::close_rec()
{
//    qDebug()<<"close_rec";
    data->set_send_para (sp_rec_on, 0);
}

void BaseWidget::do_key_ok()
{
    key_val->grade.val1 = 0;
    key_val->grade.val2 = 0;
}

void BaseWidget::do_key_cancel()
{
    key_val->grade.val1 = 0;
    key_val->grade.val2 = 0;
}

void BaseWidget::maxReset(QLabel *label)
{
    max_db = 0;
    label->setText(tr("最大值: ") + QString::number(max_db));
}

void BaseWidget::add_ae_data()
{
    for (int i = 0; i < 128; ++i) {
        ae_datalist.append(ae_pulse_data->data[i+2]);
    }

    ae_timelist.append(ae_pulse_data->time/* + ae_timelist.last()*/);

    ae_pulse_data->readComplete = 1;        //读取完成标志
}

void BaseWidget::do_Spectra_compute()
{
    if(ae_datalist.count() > 2048){
//        if(mode == AE2){
//            qDebug()<<"AE2:"<<ae_datalist.count()<<'\t'<<Common::avrage(ae_datalist);
//        }
//        else if(mode == AA1){
//            qDebug()<<"AA1:"<<ae_datalist.count()<<'\t'<<Common::avrage(ae_datalist);
//        }

        QVector<qint32> fft_result = fft->fft2048(ae_datalist.mid(0,2048));
        for (int i = 0; i <41 ; ++i) {
            Spectra_map[i] = Common::avrage(fft_result.mid(i*5 + 1, (i+1)*5) );
        }

        Spectra_data.clear();
        for(int i=0;i<40;i++){
            QwtInterval interval( 0.1*(i + 0.2) , 0.1*(i + 0.8) );
            interval.setBorderFlags( QwtInterval::ExcludeMaximum );
            Spectra_data.append( QwtIntervalSample( Spectra_map[i], interval ) );
        }

        d_Spectra->setData(new QwtIntervalSeriesData( Spectra_data ));
        plot_Spectra->replot();
        ae_datalist.clear();
    }
}




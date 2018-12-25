﻿#include "compute.h"
#include "Gui/Common/common.h"

Compute::Compute(QObject *parent) : QObject(parent)
{

}

QPoint Compute::trans_data(int x, int y, MODE mode)
{
    if(mode != TEV1 && mode != TEV2)
        return QPoint();

    switch (mode) {
    case TEV1:
        y = sqlcfg->get_para()->tev1_sql.gain * H_C_FACTOR * (y - sqlcfg->get_para()->tev1_sql.fpga_zero) ; //注意，脉冲计算里，忽略了噪声偏置的影响
        break;
    case TEV2:
        y = sqlcfg->get_para()->tev2_sql.gain * H_C_FACTOR * (y - sqlcfg->get_para()->tev2_sql.fpga_zero) ; //注意，脉冲计算里，忽略了噪声偏置的影响
        break;
    default:
        break;
    }
    //取DB值
    if(y>=1){
        y = ((double)20) * log10(y);
    }
    else if(y<=-1){
        y = -((double)20) * log10(-y);
    }
    else{
        y = 0;
    }
    x = Common::time_to_phase(x);
    return QPoint(x,y);
}

QVector<PC_DATA> Compute::compute_pc_1ms(QVector<int> list, int x_origin, double gain, int threshold)
{
    QVector<PC_DATA> pclist_1ms;

    if(list.length() < 2 )
        return pclist_1ms;

    PC_DATA pc_data;
    int first=0,last=0;
    for (int i = 0; i < list.length() - 1; ++i) {
        if(list.at(i) * list.at(i+1) <= 0){
            last = i+1;
            if(last-first > 1){         //至少有3个点
                QVector<int> list_1node = list.mid(first,last-first+1);

                //                if(max_abs(list_1node.mid(1,list_1node.count()-2)).y() > threshold ){
                QPoint P = max_abs(list_1node.mid(1,list_1node.count()-2));
                if(P.y() > threshold ){
                    //                    qDebug()<<"max value:"<< P << "\tthreshold"<<threshold<<list_1node;
                    pc_data = compute_pc_1node(list_1node, x_origin, gain);  //计算一个脉冲的pC值
                    if(qAbs(pc_data.pc_value) > 10){            //认为值严格为0的点是无意义点(可加入更严格的筛选条件)
                        pclist_1ms.append( pc_data );
                    }
                }
                first = i;
            }
        }
    }

    //    if(pclist_1ms.isEmpty()  && max_abs(list).y() > threshold){           //序列没有子脉冲，则把整个序列看做一个脉冲
    //        pclist_1ms.append(compute_pc_1node(list, x_origin, gain) );
    //    }

    return pclist_1ms;
}

PC_DATA Compute::compute_pc_1node(QVector<int> list, int x_origin, double gain)
{
    PC_DATA pc_data;
    pc_data.pc_value = 0;

    if(list.length() < 2 )
        return pc_data;

    double t1=0, t2=0, s=0;                 //前后中面积
    int first = 0, last = list.length()-1;  //前中,后中分割点
    double first_zero = first, last_zero = last, peak = first;     //第一个零点,第二个零点,峰值点

    if(list.at(0) * list.at(1) <= 0 && list.at(0) != list.at(1)){       //判断符号变化,且不同时为0
        t1 = triangle(list.at(1), list.at(0));
        first_zero = list.at(0) / (list.at(0) - list.at(1));
        first = 1;
    }

    if(list.at(last-1) * list.at(last) <= 0 && list.at(last-1) != list.at(last)){
        t2 = triangle(list.at(last-1), list.at(last));
        last_zero = last - 1 + list.at(last-1) / (list.at(last-1) - list.at(last));
        last = list.length() - 2;
    }

    s = simpson(list.mid(first,last - first + 1));

    //求峰值位置
    peak = first;
    for (int i = 0; i < last - first + 1; ++i) {
        if( qAbs(list.at(first + i))  > qAbs( list.at(peak)) ) {
            peak = first + i;
        }
    }

    //    qDebug()<<"t1="<<t1<<"\ts="<<s<<"\tt2="<<t2<<"\t"<<list << "simpson :"<<list.mid(first,last - first + 1);

    pc_data.pc_value = gain * H_C_FACTOR * (t1 + s + t2);
    pc_data.phase = x_origin;

    pc_data.rise_time = 10 * (peak - first_zero);       //单位为ns
    pc_data.fall_time = 10 * (last_zero - peak);        //单位为ns
    return pc_data;
}

//list是已标准化的脉冲序列（如何标准化，以后探讨）
//list长度至少为2
//函数实现用复化辛普生公式求积分
double Compute::simpson(QVector<int> list)
{
    if(list.length() < 2 )
        return 0;

    if(list.length() == 2)
        return (list.at(0) + list.at(1)) / 2.0 ;      //梯形公式

    int n = (list.length()-1) / 2 ;     //n至少为1
    double S = list.at(0) - list.at(2*n);
    for (int i = 1; i <= n; ++i) {
        S += 4*list.at(2*i-1) + 2*list.at(2*i);
    }
    if(list.length() % 2 == 1){     //如果长度为奇数，正好使用复化辛普生公式，否则补上一个梯形公式做结尾
        return S / 3.0;
    }
    else{
        return (list.at(2*n) + list.last() )/2.0 + S/3.0 ;
    }
}

//返回序列首（尾）遇到符号变化时，三角形的面积
//d1为靠近序列内部的数据点，d2为序列首（尾）部数据点
double Compute::triangle(double d1, double d2)
{
    return d1*d1 / (d1 - d2) / 2.0;
}

//求一组测量值的50Hz频率分量和100Hz频率分量
void Compute::compute_f_value(QVector<int> list, FFT *fft, int &v_50Hz, int &v_100Hz)
{
    QVector<double> fft_50Hz, fft_100Hz;
    for (int i = 0; i < list.count() / 32; ++i) {
        list = fft->fft32(list.mid(i*32,32));
        fft_50Hz.append(list.at(1));
        fft_100Hz.append(list.at(2));
    }
    v_50Hz = Common::avrage(fft_50Hz);
    v_100Hz = Common::avrage(fft_100Hz);
}

//寻找一个数组的最大值点
//返回值为(最大值序号,最大值)
QPoint Compute::max(QVector<int> list)
{
    QPoint P;
    if(!list.isEmpty()){
        int m = list.at(0), max_i=0;
        for (int i = 0; i < list.count(); ++i) {
            if(m < list.at(i)){
                m = list.at(i);
                max_i = i;
            }
        }
        P.setX(max_i);
        P.setY(m);
    }
    return P;
}

//寻找一个数组的最小值点
//返回值为(最小值序号,最小值)
QPoint Compute::min(QVector<int> list)
{
    QPoint P;
    if(!list.isEmpty()){
        int m = list.at(0), min_i=0;
        for (int i = 0; i < list.count(); ++i) {
            if(m > list.at(i)){
                m = list.at(i);
                min_i = i;
            }
        }
        P.setX(min_i);
        P.setY(m);
    }
    return P;
}

//寻找一个数组的绝对值最大点
//返回值为(绝对值最大点序号,最大绝对值)
QPoint Compute::max_abs(QVector<int> list)
{
    QPoint P;
    if(!list.isEmpty()){
        int m = qAbs(list.at(0) ), max_i=0;
        for (int i = 0; i < list.count(); ++i) {
            if(m < qAbs(list.at(i) ) ){
                m = qAbs(list.at(i) );
                max_i = i;
            }
        }
        P.setX(max_i);
        P.setY(m);
    }
    return P;
}

QVector<int> Compute::sim_sin(int amp, int f, int n)
{
    QVector<int> list;
    for (int i = 0; i < n; ++i) {
        list.append( amp * sin(i*f*2*3.1416/100)  );
    }
    return list;
}

QVector<int> Compute::sim_pulse(int amp, int n)
{
    QVector<int> list;
    if(n<40){
        qDebug()<<"sim_pulse input points should greater than 40!";
        return list;
    }

    list.fill(0,n);
    for (int i = 0; i < 20; ++i) {
        list[20+i] = amp * (20-i) / 20;
    }
    return list;
}

double Compute::l_channel_modify(double val)
{
    if(val > 609.5368972){
        return val * 5.446758047;
    }
    else if(val > 568.8529308){
        return val * interpolation(val, 568.8529308, 609.5368972, 5.273770842, 5.446758047);
    }
    else if(val > 489.7788194){
        return val * interpolation(val, 489.7788194, 568.8529308, 5.104344862, 5.273770842);
    }
    else if(val > 402.7170343){
        return val * interpolation(val, 402.7170343, 489.7788194, 4.966266211, 5.104344862);
    }
    else if(val > 312.6079367){
        return val * interpolation(val, 312.6079367, 402.7170343, 4.798342665, 4.966266211);
    }
    else if(val > 216.2718524){
        return val * interpolation(val, 216.2718524, 312.6079367, 4.623810214, 4.798342665);
    }
    else if(val > 114.8153621){
        return val * interpolation(val, 114.8153621, 216.2718524, 4.35481795, 4.623810214);
    }
    else if(val > 93.32543008){
        return val * interpolation(val, 93.32543008, 114.8153621, 4.286077221, 4.35481795);
    }
    else if(val > 71.61434102){
        return val * interpolation(val, 71.61434102, 93.32543008, 4.189105083, 4.286077221);
    }
    else if(val > 48.97788194){
        return val * interpolation(val, 48.97788194, 71.61434102, 4.083475889, 4.189105083);
    }
    else if(val > 25.11886432){
        return val * interpolation(val, 25.11886432, 48.97788194, 3.981071706, 4.083475889);
    }
    else if(val > 13.03166778){
        return val * interpolation(val, 13.03166778, 25.11886432, 3.836807447, 3.981071706);
    }
    else if(val > 10.59253725){
        return val * interpolation(val, 10.59253725, 13.03166778, 3.776243505, 3.836807447);
    }
    else if(val > 8.413951416){
        return val * interpolation(val, 8.413951416, 10.59253725, 3.565506682, 3.776243505);
    }
    else if(val > 3.090295433){
        return val * interpolation(val, 3.090295433, 8.413951416, 3.235936569, 3.565506682);
    }
    else if(val > 1.9498446){
        return val * interpolation(val, 1.9498446, 3.090295433, 2.56430692, 3.235936569);
    }
    else if(val > 1.548816619){
        return val * interpolation(val, 1.548816619, 1.9498446, 1.936962687, 2.56430692);
    }
    else if(val > 1.096478196){
        return val * interpolation(val, 1.096478196, 1.548816619, 1.266681721, 1.936962687);
    }
    else{
        return val;
    }
}

double Compute::interpolation(double x, double x1, double x2, double f1, double f2)
{
    return (f1 + (f2 - f1) * (x - x1) / (x2 - x1));
}

void Compute::calc_aa_value(G_PARA *data, MODE mode, L_CHANNEL_SQL *x_sql, double *aa_val, double *aa_db, int *offset)
{
    int d;

    if(mode == AA1 || mode == AE1){
        d = ((int)data->recv_para_normal.ldata0_max - (int)data->recv_para_normal.ldata0_min) / 2 ;      //最大值-最小值=幅值
    }
    else {
        d = ((int)data->recv_para_normal.ldata1_max - (int)data->recv_para_normal.ldata1_min) / 2 ;      //最大值-最小值=幅值
    }

    double factor = AA_FACTOR;
    if(mode == AE1){
        factor = sqlcfg->ae1_factor();
    }
    else if(mode == AE2){
        factor = sqlcfg->ae2_factor();
    }

    * offset = ( d - 1 / x_sql->gain / factor ) / 100;
    * aa_val = l_channel_modify( (d - x_sql->offset * 100) * x_sql->gain * factor );
    if(* aa_val < 0.1){         //保证结果有值，最小是-20dB
        * aa_val = 0.1;
    }
    * aa_db = 20 * log10 (* aa_val);

}

















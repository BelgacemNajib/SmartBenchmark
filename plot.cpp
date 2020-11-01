#include <iostream>
#include <algorithm>
#include <execution>
#include <omp.h>
#include <vector>
#include <QFile>
#include <QVector>
#include <QHBoxLayout>
#include <QWidget>
#include <QDateTime>
#include <QSysInfo>
#include "plot.h"

plot::plot(QWidget *parent): QMainWindow(parent)
{
    setFixedSize(600,300);
    const QString filename  = "benchmark_" + QDateTime::currentDateTime().toString("yyyyMMddhhmmss")  + ".txt";
    file = new QFile(filename);

}

void plot::plotCurves(bool writeInFile){
    prepareCurves();
    if(writeInFile)
        writeFile();
    show();
}

void plot::setMaxSizeVector(const unsigned size){
    maxSizeVector = size;
}

void plot::setMinSizeVector(const unsigned size){
    minSizeVector = size;
}

void plot::writeFile() {

    if (!file->open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append ))
        return;
    std::string headMsg = "";
    std::string dataMsg = "";

    // head of file
    for(auto it = data.begin() ; it != data.end(); it++)
    {
        headMsg += it->first + "\t";
    }
    headMsg += "\n";


    for( unsigned i = 0;i< data.begin()->second.size(); i++)
    {
        for(auto it = data.begin() ; it != data.end(); it++)
        {
            dataMsg += std::to_string(int(it->second[i].value)) + "\t";
        }

        dataMsg += "\n";
    }


    file->write(headMsg.c_str(), headMsg.size());
    file->write(dataMsg.c_str(), dataMsg.size());
    file->close();

}

void plot::computeBenchmarks(std::vector<int> &v){


    auto now = std::chrono::high_resolution_clock::now;
    std::chrono::high_resolution_clock::time_point start, end;
    unsigned size = v.size();

    // using indexing
    start = now();
    for(unsigned i = 0;i < size;i++){
        v[i] = 1;
    }
    end = now();
    data["Index"].push_back(QCPGraphData(size,duration(end - start)));

    // using iterator
    start = now();
    for(auto i = v.begin();i !=  v.end();i++){
        *i  = 1; // just a simple instruction
    }
    end = now();
    data["Iterator"].push_back(QCPGraphData(size,duration(end - start)));

    // using std for_ech execution parallel
    start = now();
    std::for_each(std::execution::par,v.begin(),v.end(),[](int& i){i=1;});
    end = now();
    data["ForEachPar"].push_back(QCPGraphData(size,duration(end - start)));

    // using std for_ech execution sequentiel
    start = now();
    std::for_each(v.begin(),v.end(),[](int& i){i=2;});
    end = now();
    data["ForEachSeq"].push_back(QCPGraphData(size,duration(end - start)));

    // using openmp
    start = now();
#pragma omp parallel
#pragma omp for
    for(unsigned i = 0;i < size;i++){
        v[i] = 1;
    }
    end = now();
    data["OpenMP"].push_back(QCPGraphData(size,duration(end - start)));
}

int plot::duration(std::chrono::duration<long long, std::ratio<1, 1000000000> > t){
    return std::chrono::duration_cast<std::chrono::milliseconds>(t).count();
}

void plot::prepareCurves(){
    QCustomPlot* customPlot = new QCustomPlot(this);
    setCentralWidget(customPlot);

    auto dataIter = data.begin();
    unsigned minValue = 10000;
    unsigned maxValue = 0;
    for (int gi=0; gi<data.size(); ++gi)
    {
        customPlot->addGraph();
        QColor color(20+200/4.0*gi,70*(1.6-gi/4.0), 150, 150);
        customPlot->graph()->setLineStyle(QCPGraph::lsLine);
        customPlot->graph()->setPen(QPen(color.lighter(200)));
        customPlot->graph()->setBrush(QBrush(color));
        customPlot->graph()->setName(dataIter->first.c_str());

        // data
        customPlot->graph()->data()->set(dataIter->second);

        // find min and max usefull for axis limits
        for (auto it = dataIter->second.begin();
             it != dataIter->second.end(); it++)
        {
            if(it->value > maxValue) maxValue = it->value;
            if(it->value < minValue) minValue = it->value;

        }
        dataIter++;
    }

    // set a more compact font size for bottom and left axis tick labels:
    customPlot->xAxis->setTickLabelFont(QFont(QFont().family(), 8));
    customPlot->yAxis->setTickLabelFont(QFont(QFont().family(), 8));

    // set axis labels:
    customPlot->xAxis->setLabel("size of vector");
    customPlot->yAxis->setLabel("time (ms)");

    // make top and right axes visible but without ticks and labels:
    customPlot->xAxis2->setVisible(true);
    customPlot->xAxis2->setTicks(false);
    customPlot->xAxis2->setTickLabels(false);

    customPlot->yAxis2->setTicks(false);
    customPlot->yAxis2->setVisible(true);
    customPlot->yAxis2->setTickLabels(false);

    customPlot->yAxis->setRange(minValue, maxValue);
    customPlot->xAxis->setRange(minSizeVector, maxSizeVector);

    // show legend with slightly transparent background brush:
    customPlot->legend->setVisible(true);
    customPlot->legend->setBrush(QColor(255, 255, 255, 150));

}

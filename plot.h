#pragma once
#include <chrono>
#include <QMainWindow>
#include "qcustomplot/qcustomplot.h" // for QCPGraphData


class plot:public QMainWindow
{
private:

    // prepare data
    std::unordered_map<std::string,QVector<QCPGraphData>> data;
    QFile* file;
    unsigned maxSizeVector,minSizeVector;

    int duration(std::chrono::duration<long long, std::ratio<1,1000000000> > t);
    void prepareCurves();
    void writeFile();

public:
    explicit plot(QWidget *parent = nullptr);

    void plotCurves(bool writeInFile = true);

    void setMaxSizeVector(const unsigned size);
    void setMinSizeVector(const unsigned size);

    void computeBenchmarks(std::vector<int>& v);

};

#include <iostream>
#include <fstream>
#include <QApplication>
#include "plot.h"

int main(int argc, char *argv[])
{

    QApplication a(argc,argv);

    plot p;
    std::vector<int> v;

    unsigned maxSizeVector = 1000000;
    unsigned minSizeVector = 20000;

    auto archTitle = "Benchmarks using "
            + QSysInfo::prettyProductName() + " on "
            + QSysInfo::currentCpuArchitecture();
    p.setWindowTitle(archTitle);

    p.setMaxSizeVector(maxSizeVector);// used to delimit graphs
    p.setMinSizeVector(minSizeVector);

    for(unsigned size = minSizeVector; size <= maxSizeVector; size += 20000){
        for(unsigned i = 0;i < size;i++) {
            v.push_back(i);
        }
        p.computeBenchmarks(v);
        v.clear();
    }
     p.plotCurves();

    return a.exec();

}


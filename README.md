# SmartBenchmark
Benchmarks to compare several  ways to loop std vector as function of its size
## approch
The STL library, more precisely the std::chrono is used to compute timespend of each of the following method to loop std::vector
1. using table like indexing v[i]
2. using vector iterator
3. using std::for_each in sequentiel
4. using std::for_each in parallel
5. using openmp parallel for

For various iteration on vector size, QT (qcustomplot) is used to plot curves of timespend as function of vector size 


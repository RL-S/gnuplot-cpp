#include "gnuplot.h"
#include <chrono>
#include <thread>


int main(){
    GnuplotPipe gp;
    gp.sendLine("plot [-pi/2:pi] cos(x),-(sin(x) > sin(x+1) ? sin(x) : sin(x+1))");
    gp.sendEndOfData();
    int duration {2};
    std::cout << "Sleeping for " << duration << "s, then plotting next graph.\n";
    std::this_thread::sleep_for(std::chrono::seconds(duration));
    gp.sendLine("plot [-pi/2:pi] -(sin(x) > sin(x+1) ? sin(x+1) : sin(x))");
    return 0;
}

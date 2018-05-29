#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <cstdio>
#include <algorithm>
#include <iterator>
#include <string>
#include "CoffeeShop.h"

template <class Container>
void split(const std::string& str, Container& cont)
{
    std::istringstream iss(str);
    std::copy(std::istream_iterator<std::string>(iss),
         std::istream_iterator<std::string>(),
         std::back_inserter(cont));
}

int main(int argc, char * argv[]) {
    if(argc != 3) {
        std::cout << "Run the code with the following command: ./project1 [input_file] [output_file]" << std::endl;
        return 1;
    }
    std::ifstream infile(argv[1]);
    std::string line;
    // process first line
    std::getline(infile, line);
    //number of cashiers
    int N = std::stoi(line);
    //process second line
    std::getline(infile, line);
    //number of orders
    int M = std::stoi(line);
    //create vector of orders to save orders from input file
    std::vector<Order> orders;
    orders.reserve(M);
    //add orders from input file to the vector
    for(int i=0; i<M; i++) {
        getline(infile, line);
        std::vector<std::string> words;
        split(line,words);
        Order newOrder(std::stod(words[0]), std::stod(words[1]), std::stod(words[2]), std::stod(words[3]));
        orders.push_back(newOrder);
    }

    infile.close();

    //create Coffee Shop for our simulation
    CoffeeShop myShop(N);
    //run simulation for the first model
    myShop.runSimulation1(orders);
    //print the results to the output file
    freopen(argv[2], "w", stdout);
    double totalRunTime = myShop.getCurrentTime();
    printf("%.2lf\n", totalRunTime);
    printf("%d\n", myShop.getMaxCashQueLength()) ;
    printf("%d\n", myShop.getMaxBarQueLength());
    //utilization of the cashiers
    for(int i=0; i<N; i++) {
        printf("%.2lf\n", myShop.getCashiers()[i].busyTime / totalRunTime);
    }
    //utilization of the baristas
    for(int i=0, n= N/3; i< n; i++) {
        printf("%.2lf\n", myShop.getBaristas()[i].busyTime / totalRunTime );
    }
    //turnaround times of the orders
    for(int i=0; i<M; i++) {
    	printf("%.2lf\n", orders[i].turnaroundTime );
    }

    //reset orders' turnaround times
    for(int i=0; i<M; i++) {
    	orders[i].turnaroundTime = 0;
    }

    //run simulation for the second model
    myShop.runSimulation2(orders);
    double totalRunTime2 = myShop.getCurrentTime();
    printf("\n%.2lf", totalRunTime2);
    printf("\n%d", myShop.getMaxCashQueLength()) ;
    for(int i=0,n=N/3; i<n; i++) {
    	printf("\n%d", myShop.getMaxBarQueLength2()[i]);
    }
    //utilization of the cashiers
    for(int i=0; i<N; i++) {
        printf("\n%.2lf", myShop.getCashiers()[i].busyTime / totalRunTime2);
    }
    //utilization of the baristas
    for(int i=0, n= N/3; i< n; i++) {
        printf("\n%.2lf", myShop.getBaristas()[i].busyTime / totalRunTime2 );
    }
    //turnaround times of the orders
    for(int i=0; i<M; i++) {
    	printf("\n%.2lf", orders[i].turnaroundTime );
    }

    fclose(stdout);
    return 0;
}
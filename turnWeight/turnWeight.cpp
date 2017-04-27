#include <iostream>
#include <vector>
#include <math.h>
#include <string.h>
#include <stdio.h>
using namespace std;

// orientation
// E =4 ; S =3 ; N = 2; W =1
#define LEFT        4
#define FORWARD     3
#define RIGHT       2
#define BACK        1


struct car
{
    int E; // 4
    int S;  //3
    int N;  //2
    int W;  //1
};

void print_current_orientation(car car)
{
    cout << car.E << " points to East \n";
    cout << car.S << " points to South \n";
    cout << car.N << " points to North \n";
    cout << car.W << " points to West \n";
}

int update_orientation(car &car, int turn)
{
    if ( car.E == turn)
    {
        car.E = FORWARD;
        car.S = RIGHT;
        car.W = BACK;
        car.N = LEFT;
        return 1;
    }
    if ( car.S == turn)
    {
        car.E = LEFT;
        car.S = FORWARD;
        car.W = RIGHT;
        car.N = BACK;
        return 1;
    }
    if ( car.N == turn)
    {
        car.E = RIGHT;
        car.S = BACK;
        car.W = LEFT;
        car.N = FORWARD;
        return 1;
    }
    if ( car.W == turn)
    {
        car.E = BACK;
        car.S = LEFT;
        car.W = FORWARD;
        car.N = RIGHT;
        return 1;
    }
    return 0;
}
// call when sensor reads for turn.
int make_move()
{
    
    
}
int main(void)
{
    //init
    car car1;
    
    car1.S = FORWARD;
    car1.E = LEFT;
    car1.W = RIGHT;
    car1.N = BACK;
    
    update_orientation(car1,LEFT);
    update_orientation(car1,RIGHT);
    update_orientation(car1,LEFT);
    
    print_current_orientation(car1);
    return 0;
}

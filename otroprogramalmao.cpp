#include <iostream>
#include <random>
const int ARRAY_SIZE = 90;
typedef short tvec[ARRAY_SIZE];


int random(short max)
{
    std::default_random_engine gen;
    std::uniform_int_distribution<int> dist(0,max);
    return dist(gen);
}

void GenVec( tvec v, short n ) {
    for (int i = 0; i < ARRAY_SIZE; i++){
        v[i] = random(ARRAY_SIZE) + 1;
    }
}

int main() {
    tvec p,q,r;
    GenVec(p, ARRAY_SIZE);
}
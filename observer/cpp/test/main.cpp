#include <stdlib.h>
#include <time.h>

#include "hp.h"
#include "text.h"
#include "bar.h"
#include "num.h"

int main(int argc, char* argv[]) {
    hp h;
    text t(&h);
    bar b(&h);
    num n(&h);

    srand(time(NULL));
    for(int i=0; i<10; ++i) {
        std::cout << std::endl << i+1 << ":" << std::endl;
        int health = rand()%101;
        h.set_health(health);
    }

    return 0;
}

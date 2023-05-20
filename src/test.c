#include "socketUtil.h"

int main(){
    char *json = showtimeToJSON();
    printf("%s", json);
    return 0;
}
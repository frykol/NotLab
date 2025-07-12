#include <iostream>
#include "vector.h"
#include "matrix.h"
#include "lu_decomposition.h"
#include "inverse.h"

int main(void){
    notlab::MatrixF first = notlab::MatrixF::fromList({{1,2,3},{3,2,2},{1,6,12}}, "MacierzA");
    notlab::MatrixF second = notlab::MatrixF::fromList({{1,1,1},{6,12,1},{5,6,1}}, "MacierzB");

    notlab::MatrixF add = first + second;
    notlab::MatrixF sub = first - second;
    notlab::MatrixF mult = first * second;
    notlab::MatrixF div = first/first;

    add.printAll();
    sub.printAll();
    mult.printAll();
    div.printAll();

    div.setName("division matrix");

    div = 3 * div;

    div.printAll();

    return 0;
}
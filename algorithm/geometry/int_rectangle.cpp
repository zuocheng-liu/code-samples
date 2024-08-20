#include <iostream>
#include <cmath>

using namespace std;

int main() {
    int a = 4;
    int b = 6;

    double diagonal = sqrt(a * a + b * b);
    double half_width = a / (2 * sqrt(2));
    double half_height = b / (2 * sqrt(2));

    int rows = ceil(half_height);
    int cols = ceil(half_width);

    int points = (rows + 1) * (cols + 1);

    cout << "Points: " << points << endl;

    return 0;
}

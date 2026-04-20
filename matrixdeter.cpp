#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstdlib>
#include <cmath>
#include <new>
#include <type_traits>

using namespace std;

const int MAX_MATRIX_ROWCOL_SIZE = 12;
int SET_WIDTH = 8;

// namespace for matrix
namespace MatrixDetHieng
{
    template<class> class det;

    //  matrix class
    template<class T>
    class Matrix
    {
    protected:
        T* arr;
        size_t row, col;

    public:
        // constructor
        Matrix(T* inputArr, size_t r, size_t c)
        {
            if(r < 1 || c < 1 || r > MAX_MATRIX_ROWCOL_SIZE)
            {
                cout << "Invalid matrix size\n";
                exit(-1);
            }

            arr = new(nothrow) T[r*c];
            if(!arr)
            {
                cout << "Memory allocation failed\n";
                exit(-1);
            }

            row = r;
            col = c;

            for(int i = 0; i < r*c; i++)
                arr[i] = inputArr[i];
        }

        // Destructor
        ~Matrix()
        {
            delete[] arr;
        }

        // Print matrix in | | format
        void printWithBars(ostream& out) const
        {
            for(int i = 0; i < row; i++)
            {
                out << "| ";
                for(int j = 0; j < col; j++)
                {
                    out << setw(SET_WIDTH) << arr[i*col + j] << " ";
                }
                out << "|\n";
            }
        }

        T* getArr() { return arr; }
        int getSize() { return row; }
    };

    //  SQUARE MATRIX 
    template<class T>
    class sqMatrix : public Matrix<T>
    {
    public:
        sqMatrix(T* inputArr, size_t size)
            : Matrix<T>(inputArr, size, size)
        {
            if(size < 2)
            {
                cout << "Matrix too small\n";
                exit(-1);
            }
        }
    };

    //  DET CLASS 
    template<class T>
    class det
    {
    private:

        // recursive determinant function
        T determinantP(T* arr, int size, ostream& out, bool showProcess)
        {
            // base (2x2)
            if(size == 2)
            {
                return arr[0]*arr[3] - arr[1]*arr[2];
            }

            T result = 0;

            for(int col = 0; col < size; col++)
            {
                // create minor matrix
                T* minor = new(nothrow) T[(size-1)*(size-1)];
                if(!minor)
                {
                    cout << "Memory error\n";
                    exit(-1);
                }

                int idx = 0;

                for(int i = 1; i < size; i++)
                {
                    for(int j = 0; j < size; j++)
                    {
                        if(j == col) continue;
                        minor[idx++] = arr[i*size + j];
                    }
                }

                T sign = (col % 2 == 0) ? 1 : -1;

                // process
                if(showProcess)
                {
                    out << "\n" << arr[col] << " *\n";

                    for(int i = 0; i < size-1; i++)
                    {
                        out << "| ";
                        for(int j = 0; j < size-1; j++)
                        {
                            out << setw(SET_WIDTH) << minor[i*(size-1)+j] << " ";
                        }
                        out << "|\n";
                    }
                }

                result += sign * arr[col] * determinantP(minor, size-1, out, showProcess);

                delete[] minor;
            }

            return result;
        }

    public:

        // main determinant function
        T determinant(sqMatrix<T>& m, bool showProcess, ostream& out)
        {
            int size = m.getSize();
            T* arr = m.getArr();

            out << "Matrix:\n";
            m.printWithBars(out);

            T result = determinantP(arr, size, out, showProcess);

            out << "\nDeterminant is: " << result << endl;

            return result;
        }
    };
}

// ================= MAIN =================
using namespace MatrixDetRin;

int main()
{
    int size = 3;

    cout << "Enter 9 values for a 3x3 matrix:\n";

    double arr[9];

    for(int i = 0; i < 9; i++)
    {
        cin >> arr[i];
    }

    sqMatrix<double> mat(arr, size);
    det<double> d;

    // console output
    d.determinant(mat, true, cout);

    // file output
    ofstream fout("output.txt");
    if(fout)
    {
        d.determinant(mat, true, fout);
        fout.close();
    }

    return 0;
}

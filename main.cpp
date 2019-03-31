#include <iostream>
#include <cstdio>
#include <cstring>
#include <vector>
#include <cassert>

const int MAX_NUMBER = 25000000;
const int ABC_SIZE = 256;

void LSDSort( int*& Arr, const int& k )
{
    int* tempArr = new int[k];
    std::vector<int> lstack( 1500 );
    std::vector<int> rstack( 1500 );
    std::vector<int> bytestack( 1500 );
    int Count[ABC_SIZE];
    lstack[1] = 0;
    rstack[1] = k;
    bytestack[1] = 4;
    int stackSize = 1;

    int left;
    int right;
    int byte;
    int temp;
    int i;
    int j;
    int bitMove;

    while( stackSize != 0 ) {

        left = lstack[stackSize];
        right = rstack[stackSize];
        byte = bytestack[stackSize];
        bitMove = byte * 8 - 8;
        --stackSize;

        if( right - left < 25 ) {
            for( i = left + 1; i < right; ++i ) {
                temp = Arr[i];
                j = i;
                for( ; j > 0; --j ) {
                    if( temp < Arr[j - 1] ) {
                        Arr[j] = Arr[j - 1];
                    } else {
                        break;
                    }
                }
                Arr[j] = temp;
            }
        } else {
            std::memset( &Count[0], 0, sizeof( int ) * ABC_SIZE );
            for( i = left; i < right; ++i ) {
                ++Count[( Arr[i] >> bitMove ) & 255];
            }
            Count[ABC_SIZE - 1] = right - left - Count[ABC_SIZE - 1];
            for( i = ABC_SIZE - 2; i >= 0; --i ) {
                Count[i] = Count[i + 1] - Count[i];
            }
            if( byte > 1 ) {
                for( i = 1; i < ABC_SIZE; ++i ) {
                    if( Count[i] - Count[i - 1] > 1 ) {
                        ++stackSize;
                        lstack[stackSize] = Count[i - 1] + left;
                        rstack[stackSize] = Count[i] + left;
                        bytestack[stackSize] = byte - 1;
                    }
                }
                if( Count[ABC_SIZE - 1] + left < right - 1 ) {
                    ++stackSize;
                    lstack[stackSize] = Count[ABC_SIZE - 1] + left;
                    rstack[stackSize] = right;
                    bytestack[stackSize] = byte - 1;
                }
            }
            for( i = left; i < right; ++i ) {
                tempArr[Count[( Arr[i] >> bitMove ) & 255]++] = Arr[i];
            }

            if( k == right - left ) {
                std::swap( Arr, tempArr );
            } else {
                std::memcpy( &Arr[left], tempArr, sizeof( int ) * ( right - left ) );
            }
        }
    }
    delete[] tempArr;
}


int main()
{
    int* Arr = new int[MAX_NUMBER];

    FILE* input;
    FILE* output;
    input = fopen( "input.txt", "r" );
    output = fopen( "output.txt", "w" );

    char buf[1000000];
    size_t n;
    int x = 0;
    size_t i;
    int k = 0;
    while( ( n = fread( buf, 1, sizeof( buf ), input ) ) > 0 ) {
        for( i = 0; i < n; ++i ) {
            if( '0' <= buf[i] && buf[i] <= '9' ) {
                x = x * 10 + buf[i] - '0';
            } else {
                Arr[k++] = x;
                x = 0;
            }
        }
    }

    LSDSort( Arr, k );

    int out;
    int amountZero;
    size_t tmp = 0;
    for( i = 9; i < k; i += 10 ) {
        out = 0;
        amountZero = 0;
        for( ; Arr[i] > 0; Arr[i] /= 10, ++amountZero ) {
            out = out * 10 + Arr[i] % 10;
        }
        for( ; out > 0; out /= 10, --amountZero ) {
            buf[tmp++] = out % 10 + '0';
        }
        for( ; amountZero > 0; --amountZero ) {
            buf[tmp++] = '0';
        }
        buf[tmp++] = ' ';
        if( tmp > 900000 ) {
            fwrite( buf, 1, tmp, output );
            tmp = 0;
        }
    }
    fwrite( buf, 1, tmp, output );

    fclose( input );
    fclose( output );
    delete[] Arr;

    return 0;
}
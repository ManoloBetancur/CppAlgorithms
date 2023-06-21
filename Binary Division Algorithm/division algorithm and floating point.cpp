#include <iostream>
#include <bitset>
#include <tuple>
#define N 8

/*
    Restoring division algorithm and implementation of fixed 
*/

using std::cout;
using std::endl;
using std::tuple;
using std::bitset;



bitset<N> Addition(const bitset<N> binary1, const bitset<N> binary2) {
    std::bitset<N> sum;
    bool carry = false;
    for (int i = 0; i < N; ++i) {
        bool bit1 = binary1[i];
        bool bit2 = binary2[i];
        
        // Calculate the current sum bit without considering carry-in or carry-out
        sum[i] = bit1 ^ bit2 ^ carry;

        // Calculate the carry for the next bit
        carry = (bit1 & bit2) | (bit1 & carry) | (bit2 & carry);
        
    }
    
    return sum;
}



bitset<N> convert_twos_complement( bitset<N> negative_number)
{
    int complement = ~(negative_number.to_ulong())+1;
    return bitset<N>(complement);
}

tuple<bitset<N>,bitset<N>> restoring_division(bitset<N> Q,bitset<N> M)
{
    bitset<N> A(0);
    
    for (int i = N-1; i >= 0; i--)
    {
        A<<=1;
        int MSB_Q = Q.test(N-1);
        A.set(0,MSB_Q);
        Q<<=1;
        A = Addition(A,convert_twos_complement(M));
        int MSB_A = A.test(N-1);
        if (MSB_A == 1)
        {
            Q.set(0,0);
            //Restore A
            A = Addition(A,M);
        }
        else
        {
            Q.set(0,1);
        }
    }

    tuple<bitset<N>,bitset<N>> result(Q,A);

    return result;


}

double calculate_fractional_part(int remainder, bitset<N> div)
{
    double fractionals_8bits[8] = {0.1,0.01,0.001,0.0001,0.00001,0.000001,0.0000001,0.00000001};
    //8 bit presicion
    tuple<bitset<N>,bitset<N>> temp;
    bitset<N> dividend = remainder*10;

    double result = 0.0;
    for (int i = 0; i < N; i++)
    {
        temp = restoring_division(dividend,div);
        auto [Q,R] = temp;

        int int_part = Q.to_ulong();

        result += fractionals_8bits[i]*int_part;

        dividend = R.to_ulong()*10;


    }

    return result;
    
}


int main() {


    
    //bitset<N> divided(0b11111111); 
    //bitset<N> divisor(0b00011111); 

    bitset<N> divided(151); 
    bitset<N> divisor(33); 
    

    tuple<bitset<N>,bitset<N>> result = restoring_division(divided,divisor);

    auto [Q,R] = result;

    int significand = Q.to_ulong();
    int remainder = R.to_ulong();

    cout<<divided<<"÷"<<divisor<<" = "<<endl;
    cout<<divided.to_ulong()<<"÷"<<divisor.to_ulong()<<" = "<<endl;
    cout<<"##RESULT###"<<endl;
    cout<<"Quotient: "<<Q<<" DEC: "<<significand<<endl;
    cout<<"Reminder: "<<R<<" DEC: "<<remainder<<endl;

    double fractional_part = calculate_fractional_part(remainder, divisor);

    cout<<"Float number is "<< double(significand)+fractional_part << endl;

    cout<<"Significand : "<<double(significand)<<endl;
    cout<<"Fractional : "<<fractional_part<<endl;

    
    return 0;


}
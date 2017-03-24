#include "BigIntLib.hpp"
#include "MatrixLib.hpp"
#include "VectorLib.hpp"
#include "RationalNumberLib.hpp"
#include "ErrorsCatchers.hpp"
#include <iostream>

int main(int argc, const char * argv[]){
    try {
        const RationalNumber r1("256","512"), r2=1/r1;
        RationalNumber r3,r4(r2);
        BigInt r5="9999999999999999999999999999999999999999999999999999999999999999999", r6=3, r7;

        FILE *out = fopen("out.txt", "wt");
        FILE *in = fopen("output.txt", "rt");

        Matrix m1(in), m2;

        m2 = m1*m1;
        m2.write(out);

        fclose(out);
        fclose(in);

        r3=r1*r2;
        
        r7=r5/r6;
        r4.makeCanonical();
        
        r1.print();
        printf("\n");
        r3.print();
        printf("\n");
        r7.print();
        printf("\n");

        
    } catch (DenominatorIsZeroERROR) {
        
    }
    catch (DividerIsZeroERROR) {
        
    }
    catch (MemoryAllocatingERROR) {
        
    }
    catch (TypesConvertingERROR) {
        
    }
    catch (MatrixSizeERROR) {
        
    }
    catch (VectorSizeERROR) {
        std::cout << "Ошибка";
    }
    return 0;
}


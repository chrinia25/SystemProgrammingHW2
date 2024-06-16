#include <stdio.h>
#include "qrCode_C.h"

int main(){
    int ans;
    ans = read_QR();
    printf("%d\n",ans);
    
    return 0;
}

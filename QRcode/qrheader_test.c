#include <stdio.h>
#include "qrCode_C.h"

int main(){
    int ans;
    ans=qrCodedetect();
    printf("%d\n",ans);
}
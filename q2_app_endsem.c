// Online C compiler to run C program online
#include <stdio.h>
#include <limits.h>
int main() {

    int maxElement = INT_MIN;
    int arr[]={27,32,101,132,13,77,51,99};
    int n = sizeof(arr)/sizeof(arr[0]);
    for(int i=0;i<n;i++){
        if(maxElement<arr[i]) maxElement = arr[i];
    }
    int maxPrime = -1;
    for(int i=0;i<n;i++){
        int flag = 1;
        for(int j=2;j*j<maxElement;j++){
            if(arr[i]!=j){
                if(arr[i]%j==0){
                    flag = 0;
                    break;
                }
            }
        }
        if(flag==1){
            if(maxPrime<arr[i]) maxPrime = arr[i];
        }
    }
    printf("%d",maxPrime);

    return 0;
}

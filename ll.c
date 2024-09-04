#include<stdio.h>
struct node{
	long long data;
	int next;
};
int main(){
	struct node arr_ll[10];
	int d=11;
	for(int i=0;i<10;i++){
		arr_ll[i].data=d++;
		arr_ll[i].next=i+1;
	}
	arr_ll[9].next=-1;
	for(int i=0;i<10;i++){
		printf("%lld  %d",arr_ll[i].data,arr_ll[i].next);
		printf("\n");

	}
	for(int i=5;i<10;i++){
		arr_ll[i-1].data=arr_ll[i].data;
		arr_ll[i-1].next=i;
	}
	long long target=0;
	scanf("%lld",&target);
	arr_ll[9].data=target;
	for(int i=0;i<10;i++){
		printf("%lld %d",arr_ll[i].data,arr_ll[i].next);
		printf("\n");
	}

	return 0;
}

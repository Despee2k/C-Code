#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

typedef struct{
	int num;
}Octet;

int getNewSubnetMask(int host);
int getNB(Octet *o, int nsm);
int getNB_prefix(int prefix, int nsm);
int getDelta (int nsm);
void formula1(Octet *o, int place, int delta);
void formula2(Octet *o, int place, int delta);
void formula3(Octet *o, int place, int delta);

int main(void){
	int i = 0;
	int hosts;
	int placement;
	int prefix = 0;
	int netbits;
	char ipAddress[16];
	char y_n[4];
	Octet *octet;
	octet = (Octet *) malloc (4 * sizeof(Octet));
	
	printf("Enter IP Address: ");
	scanf("%[^\n]s", ipAddress);
	
	char *token = strtok(ipAddress, ".");
	
	while (token != NULL){
		octet[i].num = atoi(token);
		token = strtok(NULL, ".");
		i++;
	}
	
	printf("Enter number of Hosts: ");
	scanf("%d", &hosts);
	
	int result = getNewSubnetMask(hosts);
	
	printf("With prefix? [Yes/No]: ");
	scanf(" %s", y_n);
	
	if (tolower(y_n[0]) == 'y'){
		printf("Prefix: ");
		scanf("%d", &prefix);
		netbits = getNB_prefix(prefix, result);
	}
	
	printf("Placement (Nth): ");
	scanf("%d", &placement);
	
	int delta = getDelta(result);
	if(prefix == 0) netbits = getNB(octet, result);
	
	if (netbits > 1 && netbits < 9) formula1(octet, placement, delta);
	else if (netbits > 8 && netbits < 17) formula2(octet, placement, delta);
	else formula3(octet, placement, delta);
	
	while (1){
		printf("\nEnter more placements? [Yes/No]: ");
		scanf(" %s", y_n);
		
		if (tolower(y_n[0]) == 'y'){
			printf("Placement (Nth): ");
			scanf("%d", &placement);
		
			if (netbits > 1 && netbits < 9) formula1(octet, placement, delta);
			else if (netbits > 8 && netbits < 17) formula2(octet, placement, delta);
			else formula3(octet, placement, delta);
		}
		else break;
	}
	free(octet);
	
	return 0;
}

int getNewSubnetMask(int host){
	int n = 1;
	int count = 0;
	while(n - 2 < host){
		n *= 2;
		count++;
	}
	
	int result = 32 - count;
	return result;
}

int getNB(Octet *o, int nsm){
	int result;
	if (o[0].num < 127) 
		result = nsm - 8;
	else if (o[0].num > 127 && o[0].num < 192) 
		result = nsm - 16;
	else result = nsm - 24;
	
	return result;
}

int getNB_prefix(int prefix, int nsm){
	int result;
	result = nsm - prefix;
	
	return result;
}

int getDelta(int nsm){
	int result;
	int delta = 1;
	int i;
	
	if (nsm >= 24) result = nsm - 24;
	else if (nsm >= 16) result = nsm - 16;
	else if (nsm >= 8) result = nsm - 8;
	
	for (i = 0; i < 8 - result; i++) delta *= 2;
	
	return delta;
}

void formula1(Octet *o, int place, int delta){
	double result;
	
	result = delta * place;
	
	if((int) result > 256) {
		formula2(o, place, delta);
		return;
	}
	
	o[3].num = result;
	printf("S%d = %d.%d.%d.%d\n", place, o[0].num, o[1].num, o[2].num, o[3].num);
}

void formula2(Octet *o, int place, int delta){
	double result;
	
	result = delta * place / 256.0;
	
	if((int) result > 256) {
		formula3(o, place, delta); 
		return;
	}
	
	o[2].num = (int) result;
	
	result = result - (int) result;
	result *= 256;
	
	o[3].num = result;
	printf("S%d = %d.%d.%d.%d\n", place, o[0].num, o[1].num, o[2].num, o[3].num);
}

void formula3(Octet *o, int place, int delta){
	double result;
	
	result = delta * place / 65536.0;
	
	o[1].num = (int) result;
	
	result = result - (int) result;
	result *= 256;
	
	o[2].num = (int) result;
	
	result = result - (int) result;
	result *= 256;
	
	o[3].num = (int) result;
	printf("S%d = %d.%d.%d.%d\n", place, o[0].num, o[1].num, o[2].num, o[3].num);
}

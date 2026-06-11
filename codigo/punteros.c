#include <stdio.h>

int main() {
	int x = 10;
	void* p = &x;        // p apunta a un int, pero como void* no sabe qué tipo es
	printf("%d\n", *(int*)p);  // casteamos p a int* y lo desreferenciamos
	return 0;
}
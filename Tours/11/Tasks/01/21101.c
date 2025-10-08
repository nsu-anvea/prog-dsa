#include <stdio.h>
#define uc unsigned char

int main() {
	freopen("input.txt", "r", stdin);
	freopen("output.txt", "w", stdout);

    int alphabet[256] = {0};

    uc letter;
	while (scanf("%c", &letter) != EOF)
        alphabet[letter]++;

	for (int i = 0; i < 256; i++)
		if (alphabet[i])
			printf("%d : %d\n", i, alphabet[i]);

	return 0;
}
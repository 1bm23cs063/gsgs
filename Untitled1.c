#include <stdio.h>
#include <string.h>
#include <ctype.h>

void preprocess(char* str) {
    int i, j = 0;
    char temp[strlen(str)];
    for (i = 0; str[i]; i++) {
        if (isalpha(str[i])) {
            temp[j++] = toupper(str[i]);
        }
    }
    temp[j] = '\0';
    strcpy(str, temp);
}

void caesar(char* text, int shift, int encrypt) {
    if (!encrypt) shift = 26 - (shift % 26);
    printf("%s (Caesar): ", encrypt ? "Encrypted" : "Decrypted");
    for (int i = 0; text[i]; i++) {
        if (isupper(text[i])) {
            char ch = ((text[i] - 'A' + shift) % 26) + 'A';
            printf("%c", ch);
        } else {
            printf("%c", text[i]); // Retain spaces or other non-alphabet characters
        }
    }
    printf("\n");
}

void vigenere(char* text, char* key, int encrypt) {
    preprocess(key);
    int keyLen = strlen(key);
    printf("%s (Vigenère): ", encrypt ? "Encrypted" : "Decrypted");
    for (int i = 0; text[i]; i++) {
        if (isalpha(text[i])) {
            int k = key[i % keyLen] - 'A';
            if (!encrypt) k = 26 - k;  // Reverse the direction for decryption
            char ch = ((text[i] - 'A') + (encrypt ? k : -k) + 26) % 26 + 'A';
            printf("%c", ch);
        } else {
            printf("%c", text[i]); // Retain spaces or other non-alphabet characters
        }
    }
    printf("\n");
}

void createPlayfairMatrix(char key[], char matrix[5][5]) {
    int seen[26] = {0}, i, k = 0;
    preprocess(key);
    seen['J' - 'A'] = 1; // 'I' and 'J' are treated as the same
    for (i = 0; i < strlen(key); i++) {
        int ch = key[i] - 'A';
        if (!seen[ch]) {
            matrix[k / 5][k % 5] = key[i];
            seen[ch] = 1;
            k++;
        }
    }
    for (i = 0; i < 26; i++) {
        if (!seen[i]) {
            matrix[k / 5][k % 5] = (i == ('I' - 'A')) ? 'I' : (i + 'A');
            seen[i] = 1;
            k++;
        }
    }
}

void findPosition(char matrix[5][5], char ch, int* row, int* col) {
    if (ch == 'J') ch = 'I'; // Treat 'J' as 'I'
    for (int i = 0; i < 5; i++)
        for (int j = 0; j < 5; j++)
            if (matrix[i][j] == ch) {
                *row = i;
                *col = j;
                return;
            }
}

void playfair(char* text, char* key, int encrypt) {
    preprocess(text);
    char matrix[5][5];
    createPlayfairMatrix(key, matrix);
    char newText[200];
    int i, j = 0;
    for (i = 0; text[i]; i++) {
        newText[j++] = text[i];
        if (text[i] == text[i + 1]) {
            newText[j++] = 'X';
        }
    }
    newText[j] = '\0';
    if (j % 2 != 0) newText[j++] = 'X';
    newText[j] = '\0';
    printf("%s (Playfair): ", encrypt ? "Encrypted" : "Decrypted");
    for (i = 0; i < j; i += 2) {
        int r1, c1, r2, c2;
        findPosition(matrix, newText[i], &r1, &c1);
        findPosition(matrix, newText[i + 1], &r2, &c2);
        if (r1 == r2) {
            printf("%c%c",
                   matrix[r1][(c1 + (encrypt ? 1 : 4)) % 5],
                   matrix[r2][(c2 + (encrypt ? 1 : 4)) % 5]);
        } else if (c1 == c2) {
            printf("%c%c",
                   matrix[(r1 + (encrypt ? 1 : 4)) % 5][c1],
                   matrix[(r2 + (encrypt ? 1 : 4)) % 5][c2]);
        } else {
            printf("%c%c", matrix[r1][c2], matrix[r2][c1]);
        }
    }
    printf("\n");
}

void autokey(char* text, char* key, int encrypt) {
    preprocess(key);
    int textLen = strlen(text);
    char fullKey[200];
    if (encrypt) {
        strcpy(fullKey, key);
        strncat(fullKey, text, textLen - strlen(key));
    } else {
        strcpy(fullKey, key);
    }
    printf("%s (Autokey): ", encrypt ? "Encrypted" : "Decrypted");
    for (int i = 0; i < textLen; i++) {
        int keyVal = fullKey[i] - 'A';
        int textVal = text[i] - 'A';
        char ch;
        if (encrypt) {
            ch = ((textVal + keyVal) % 26) + 'A';
        } else {
            ch = ((textVal - keyVal + 26) % 26) + 'A';
            fullKey[i + strlen(key)] = ch;  // Update the key stream for decryption
        }
        printf("%c", ch);
    }
    printf("\n");
}

int main() {
    char text[200], key[100];
    int choice, shift, mode;

    while (1) {
        printf("\nChoose a cipher:\n");
        printf("1. Caesar Cipher\n");
        printf("2. Vigenère Cipher\n");
        printf("3. Playfair Cipher\n");
        printf("4. Autokey Cipher\n");
        printf("5. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);
        getchar();

        if (choice == 5) break;

        printf("Enter text: ");
        fgets(text, sizeof(text), stdin);
        text[strcspn(text, "\n")] = 0;
        preprocess(text);

        printf("\nChoose operation:\n1. Encrypt\n2. Decrypt\nEnter choice: ");
        scanf("%d", &mode);
        getchar();
        int encrypt = (mode == 1);

        switch (choice) {
            case 1:
                printf("Enter shift value: ");
                scanf("%d", &shift);
                getchar();
                caesar(text, shift, encrypt);
                break;
            case 2:
                printf("Enter keyword: ");
                fgets(key, sizeof(key), stdin);
                key[strcspn(key, "\n")] = 0;
                vigenere(text, key, encrypt);
                break;
            case 3:
                printf("Enter keyword: ");
                fgets(key, sizeof(key), stdin);
                key[strcspn(key, "\n")] = 0;
                playfair(text, key, encrypt);
                break;
            case 4:
                printf("Enter keyword: ");
                fgets(key, sizeof(key), stdin);
                key[strcspn(key, "\n")] = 0;
                autokey(text, key, encrypt);
                break;
            default:
                printf("Invalid choice.\n");
        }
    }

    printf("Exiting program.\n");
    return 0;
}


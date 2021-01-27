/*
 * g.c
 * Copyright (C) 2021 ctf <ctf@ctf-VirtualBox>
 *
 * Distributed under terms of the MIT license.
 */

#include<cstdio>
#include<cmath>
#include<cstring>

int sbox_inv[257] = {143, 15, 224, 141, 216, 191, 213, 98, 182, 91, 198, 113, 156, 43, 115, 68, 127, 134, 94, 38, 4, 186, 107, 220, 23, 160, 237, 207, 211, 149, 77, 250, 151, 210, 222, 79, 22, 214, 35, 25, 42, 6, 223, 230, 132, 65, 235, 117, 209, 53, 24, 169, 28, 171, 69, 99, 102, 184, 239, 215, 130, 32, 0, 100, 155, 44, 7, 165, 174, 59, 176, 118, 193, 76, 123, 12, 125, 63, 231, 80, 37, 88, 74, 122, 97, 95, 49, 200, 60, 144, 108, 219, 197, 187, 89, 17, 131, 52, 236, 120, 51, 18, 217, 110, 67, 13, 183, 136, 180, 39, 255, 179, 61, 181, 218, 240, 8, 1, 103, 163, 27, 172, 116, 212, 46, 153, 129, 246, 192, 243, 175, 146, 105, 66, 154, 248, 106, 81, 137, 62, 34, 226, 188, 11, 126, 201, 167, 166, 75, 247, 36, 147, 10, 251, 55, 128, 253, 82, 16, 138, 72, 111, 92, 85, 158, 90, 21, 190, 254, 73, 145, 101, 203, 124, 164, 19, 56, 70, 9, 3, 83, 228, 30, 139, 64, 31, 47, 152, 202, 194, 26, 20, 195, 196, 241, 2, 225, 54, 142, 57, 242, 119, 157, 177, 199, 112, 168, 206, 227, 221, 5, 178, 238, 232, 48, 135, 233, 96, 208, 148, 121, 109, 162, 161, 204, 33, 205, 244, 249, 78, 150, 87, 234, 93, 133, 50, 45, 104, 189, 173, 185, 86, 29, 170, 71, 229, 40, 159, 84, 245, 252, 140, 41, 58, 14, 114}
;
int cntk7k1[257][257] = {0};
int cntk4k0[257][257] = {0};
int cntk5[257] = {0};
int cntk2[257] = {0};
int cntk6[257] = {0};
int cntk3[257] = {0};


int bit(int n, int k) {
    return (n & (1 << k)) != 0;
}

int getk7k1k4k0() {
    FILE* f = fopen("data", "rb");
    
    for (int i = 0; i < 65536; i++) {
        unsigned char p[8], c[8];
        fread(&p, 8, 1, f);
        fread(&c, 8, 1, f);
        for (int a = 0; a < 256; a++) {
            for (int b = 0; b < 256; b++) {
                int u4b7 = sbox_inv[(int)c[7-7] ^ a];
                int u4b1 = sbox_inv[(int)c[7-1] ^ b];
                int u4b4 = sbox_inv[(int)c[7-4] ^ a];
                int u4b0 = sbox_inv[(int)c[7-0] ^ b];
                if (bit(p[7-7], 0) ^ bit(p[7-7], 3) ^ bit(p[7-7], 4) ^ bit(u4b7, 0) ^ bit(u4b7, 4) ^ bit(u4b1, 0) ^ bit(u4b1, 4)) {
                    cntk7k1[a][b]++;
                }
                if (bit(p[7-7], 0) ^ bit(p[7-7], 2) ^ bit(p[7-7], 3) ^ bit(p[7-7], 4) ^ bit(p[7-7], 5)  ^ bit(u4b4, 0) ^ bit(u4b4, 4) ^ bit(u4b0, 0) ^ bit(u4b0, 4)) {
                    cntk4k0[a][b]++;
                }
            }
        }
    }
    for (int a = 0; a < 256; a++) {
        for (int b = 0; b < 256; b++) {
            int bias = abs(cntk7k1[a][b] - 32768);
            if (bias > 1000){
                printf("(k7, k1) = (%d, %d), bias = %d\n", a, b, bias);
            }
        }
    }
    for (int a = 0; a < 256; a++) {
        for (int b = 0; b < 256; b++) {
            int bias = abs(cntk4k0[a][b] - 32768);
            if (bias > 1000){
                printf("(k4, k0) = (%d, %d), bias = %d\n", a, b, bias);
            }
        }
    }

    return 0;
}

int getk5k2(){
    int k7 = 130;
    int k1 = 194;
    int k4 = 65;
    int k0 = 40;

    FILE* f = fopen("data", "rb");

    for (int i = 0; i < 65536; i++) {
        unsigned char p[8], c[8];
        fread(&p, 8, 1, f);
        fread(&c, 8, 1, f);
        for (int a = 0; a < 256; a++) {
            int u4b5 = sbox_inv[(int)c[7-5] ^ a];
            int u4b2 = sbox_inv[(int)c[7-2] ^ a];
            int u4b7 = sbox_inv[(int)c[7-7] ^ k7];
            int u4b1 = sbox_inv[(int)c[7-1] ^ k1];
            int u4b4 = sbox_inv[(int)c[7-4] ^ k4];
            int u4b0 = sbox_inv[(int)c[7-0] ^ k0];
            if (bit(p[7-7], 1) ^ bit(p[7-7], 2) ^ bit(p[7-7], 3) ^ bit(p[7-7], 4) ^ bit(p[7-7], 6)  ^ bit(u4b0, 0) ^ bit(u4b1, 0) ^ bit(u4b2, 0) ^ bit(u4b7, 0) ^ bit(u4b0, 4) ^ bit(u4b1, 4) ^ bit(u4b2, 4) ^ bit(u4b7, 4)) {
                cntk2[a]++;
            }
            if (bit(p[7-7], 1) ^ bit(p[7-7], 6) ^ bit(p[7-7], 7) ^ bit(u4b1, 0) ^ bit(u4b4, 0) ^ bit(u4b5, 0) ^ bit(u4b7, 0) ^ bit(u4b1, 4) ^ bit(u4b4, 4) ^ bit(u4b5, 4) ^ bit(u4b7, 4)) {
                cntk5[a]++;            
            }
        }
    }
    for (int a = 0; a < 256; a++) {
        int bias = abs(cntk2[a] - 32768);
        if (bias > 1000){
            printf("(k2) = (%d), bias = %d\n", a, bias);
        }
    }
    for (int a = 0; a < 256; a++) {
        int bias = abs(cntk5[a] - 32768);
        if (bias > 1000){
            printf("(k5) = (%d), bias = %d\n", a, bias);
        }
    }

    return 0;
}


int getk3k6(){
    int k7 = 130;
    int k1 = 194;
    int k4 = 65;
    int k0 = 40;
    int k5 = 150;
    int k2 = 239;

    FILE* f = fopen("data", "rb");

    for (int i = 0; i < 65536; i++) {
        unsigned char p[8], c[8];
        fread(&p, 8, 1, f);
        fread(&c, 8, 1, f);
        for (int a = 0; a < 256; a++) {
            int u4b6 = sbox_inv[(int)c[7-6] ^ a];
            int u4b3 = sbox_inv[(int)c[7-3] ^ a];
            int u4b7 = sbox_inv[(int)c[7-7] ^ k7];
            int u4b1 = sbox_inv[(int)c[7-1] ^ k1];
            int u4b4 = sbox_inv[(int)c[7-4] ^ k4];
            int u4b0 = sbox_inv[(int)c[7-0] ^ k0];
            int u4b5 = sbox_inv[(int)c[7-5] ^ k5];
            int u4b2 = sbox_inv[(int)c[7-2] ^ k2];
            if (bit(p[7-7], 1) ^ bit(p[7-7], 2) ^ bit(p[7-7], 3) ^ bit(p[7-7], 4) ^ bit(p[7-7], 6) ^ bit(p[7-7], 7) ^ bit(u4b0, 0) ^ bit(u4b2, 0) ^ bit(u4b4, 0) ^ bit(u4b5, 0) ^ bit(u4b6, 0) ^ bit(u4b0, 4) ^ bit(u4b2, 4) ^ bit(u4b4, 4) ^ bit(u4b5, 4) ^ bit(u4b6, 4)) {
                cntk6[a]++;
            }
            if (bit(p[7-7], 1) ^ bit(p[7-7], 2) ^ bit(p[7-7], 4) ^ bit(p[7-7], 7) ^ bit(u4b0, 0) ^ bit(u4b1, 0) ^ bit(u4b2, 0) ^ bit(u4b3, 0) ^ bit(u4b4, 0) ^ bit(u4b0, 4) ^ bit(u4b1, 4) ^ bit(u4b2, 4) ^ bit(u4b3, 4) ^ bit(u4b4, 4)){
                cntk3[a]++;  
            }
        }
    }
    for (int a = 0; a < 256; a++) {
        int bias = abs(cntk3[a] - 32768);
        if (bias > 1000){
            printf("(k3) = (%d), bias = %d\n", a, bias);
        }
    }
    for (int a = 0; a < 256; a++) {
        int bias = abs(cntk6[a] - 32768);
        if (bias > 1000){
            printf("(k6) = (%d), bias = %d\n", a, bias);
        }
    }

    return 0;
}

int main(){
    int k[8] = {40, 194, 239, 235, 65, 150, 167, 130};
    for (int i = 0; i < 8; ++i) {
        printf("K%d:%d\n", i, k[i]);
    }
    return 0;
}


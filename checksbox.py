#! /usr/bin/env python3
# -*- coding: utf-8 -*-
# vim:fenc=utf-8
#
# Copyright © 2021 ctf <ctf@ctf-VirtualBox>
#
# Distributed under terms of the MIT license.

sbox = [62, 117, 195, 179, 20, 210, 41, 66, 116, 178, 152, 143, 75, 105, 254, 1, 158, 95, 101, 175, 191, 166, 36, 24, 50, 39, 190, 120, 52, 242, 182, 185, 61, 225, 140, 38, 150, 80, 19, 109, 246, 252, 40, 13, 65, 236, 124, 186, 214, 86, 235, 100, 97, 49, 197, 154, 176, 199, 253, 69, 88, 112, 139, 77, 184, 45, 133, 104, 15, 54, 177, 244, 160, 169, 82, 148, 73, 30, 229, 35, 79, 137, 157, 180, 248, 163, 241, 231, 81, 94, 165, 9, 162, 233, 18, 85, 217, 84, 7, 55, 63, 171, 56, 118, 237, 132, 136, 22, 90, 221, 103, 161, 205, 11, 255, 14, 122, 47, 71, 201, 99, 220, 83, 74, 173, 76, 144, 16, 155, 126, 60, 96, 44, 234, 17, 215, 107, 138, 159, 183, 251, 3, 198, 0, 89, 170, 131, 151, 219, 29, 230, 32, 187, 125, 134, 64, 12, 202, 164, 247, 25, 223, 222, 119, 174, 67, 147, 146, 206, 51, 243, 53, 121, 239, 68, 130, 70, 203, 211, 111, 108, 113, 8, 106, 57, 240, 21, 93, 142, 238, 167, 5, 128, 72, 189, 192, 193, 92, 10, 204, 87, 145, 188, 172, 224, 226, 207, 27, 218, 48, 33, 28, 123, 6, 37, 59, 4, 102, 114, 91, 23, 209, 34, 42, 2, 196, 141, 208, 181, 245, 43, 78, 213, 216, 232, 46, 98, 26, 212, 58, 115, 194, 200, 129, 227, 249, 127, 149, 135, 228, 31, 153, 250, 156, 168, 110]

def not_pow_of_2(x):
    return x&(x-1)

def bit(x, i):
    return (x>>i)&1

#目的：找出输入位和输出位的某种线性关系，使其近似于sbox

evl = 40
#假如(128+evl)/256概率可以确定线性关系，则认为此函数与sbox近似

for i in range(256):#sbox输入位对应参数
    for j in range(256):#sbox输出位对应参数
        if not_pow_of_2(i) and not_pow_of_2(j):
            continue
        
        bias = 0
        for x in range(256):
            res = (bin(x&i).count('1') + bin(sbox[x]&j).count('1')) % 2
            bias += res

        if abs(bias-128) > evl:
            tmp = ''
            for k in range(8):
                if bit(i, k):
                    tmp += 'X'+str(k)+'+'
            for k in range(8):
                if bit(j, k):
                    tmp += 'Y'+str(k)+'+'
            tmp = tmp.strip('+')
            print(tmp)



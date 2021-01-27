#! /usr/bin/env python3
# -*- coding: utf-8 -*-
# vim:fenc=utf-8
#
# Copyright © 2021 ctf <ctf@ctf-VirtualBox>
#
# Distributed under terms of the MIT license.

ptable = [
    0, 8, 16, 24, 32, 40, 48, 56, 
    1, 9, 17, 25, 33, 41, 49, 57, 
    2, 10, 18, 26, 34, 42, 50, 58, 
    3, 11, 19, 27, 35, 43, 51, 59, 
    4, 12, 20, 28, 36, 44, 52, 60, 
    5, 13, 21, 29, 37, 45, 53, 61, 
    6, 14, 22, 30, 38, 46, 54, 62, 
    7, 15, 23, 31, 39, 47, 55, 63
]

sbox = [62, 117, 195, 179, 20, 210, 41, 66, 116, 178, 152, 143, 75, 105, 254, 1, 158, 95, 101, 175, 191, 166, 36, 24, 50, 39, 190, 120, 52, 242, 182, 185, 61, 225, 140, 38, 150, 80, 19, 109, 246, 252, 40, 13, 65, 236, 124, 186, 214, 86, 235, 100, 97, 49, 197, 154, 176, 199, 253, 69, 88, 112, 139, 77, 184, 45, 133, 104, 15, 54, 177, 244, 160, 169, 82, 148, 73, 30, 229, 35, 79, 137, 157, 180, 248, 163, 241, 231, 81, 94, 165, 9, 162, 233, 18, 85, 217, 84, 7, 55, 63, 171, 56, 118, 237, 132, 136, 22, 90, 221, 103, 161, 205, 11, 255, 14, 122, 47, 71, 201, 99, 220, 83, 74, 173, 76, 144, 16, 155, 126, 60, 96, 44, 234, 17, 215, 107, 138, 159, 183, 251, 3, 198, 0, 89, 170, 131, 151, 219, 29, 230, 32, 187, 125, 134, 64, 12, 202, 164, 247, 25, 223, 222, 119, 174, 67, 147, 146, 206, 51, 243, 53, 121, 239, 68, 130, 70, 203, 211, 111, 108, 113, 8, 106, 57, 240, 21, 93, 142, 238, 167, 5, 128, 72, 189, 192, 193, 92, 10, 204, 87, 145, 188, 172, 224, 226, 207, 27, 218, 48, 33, 28, 123, 6, 37, 59, 4, 102, 114, 91, 23, 209, 34, 42, 2, 196, 141, 208, 181, 245, 43, 78, 213, 216, 232, 46, 98, 26, 212, 58, 115, 194, 200, 129, 227, 249, 127, 149, 135, 228, 31, 153, 250, 156, 168, 110]

def not_pow_of_2(x):
    return x&(x-1)

def bit(x, i):
    return (x>>i)&1

def log2(x):
    for i in reversed(range(8)):
        if x == (1<<i):
            return i
    return -1

def mypremutation(pos):
    for it in range(64):
        if ptable[it] == pos:
            return it
    return -1

#目的：找出输入位和输出位的某种线性关系，使其近似于sbox操作

evl = 40
#假如(128+evl)/256概率可以确定线性关系，则认为此函数与sbox近似

xpath = {}#从单位输入映射到多位输出
ypath = {}#从多位输入映射到单位输出

def prep_path():
    for i in range(1, 256):#sbox输入位对应参数
        for j in range(1, 256):#sbox输出位对应参数
            if not_pow_of_2(i) and not_pow_of_2(j):
                continue
            bias = 0
            for x in range(256):
                res = (bin(x&i).count('1') + bin(sbox[x]&j).count('1')) % 2
                bias += res

            if abs(bias-128) > evl:#记录近似线性关系
                if not_pow_of_2(i):
                    tmp = []
                    for k in range(8):
                        if bit(i, k):
                            tmp.append(k)
                    ypath[log2(j)] = tmp

                else:
                    tmp = []
                    for k in range(8):
                        if bit(j, k):
                            tmp.append(k)
                    xpath[log2(i)] = tmp
    print('xpath:', xpath)
    print('ypath', ypath)

def permutations(poss):#获得对每个在集合中的bit位置进行permutaion后的bit位置集合
    return [mypremutation(pos) for pos in poss]

def expands(poss):#获得对每个在集合中的bit位置进行sbox后的bit位置集合
    nextposs = [] 
    buk = [0 for i in range(64)]
    for pos in poss:
        j = pos % 8
        if j not in xpath.keys():
            return False, []
        for i in xpath[j]:
            buk[pos - j + i] += 1
    for i in range(64):
        if buk[i]&1:
            nextposs.append(i)
    return True, nextposs 

def forshow(poss):
    return [(x//8, x%8) for x in poss]

def findpath():#进行rd-1轮扩散，找到某个结果与输入之间的线性关系式
    for sy, sxs in ypath.items():
        for st in range(8):
            poss = [st * 8 + sy]
            for rd in range(2):
                poss = permutations(poss)
                flag, poss = expands(poss)
                if not flag:
                    break
            else:
                cnt = 0
                tmp = ''
                poss = permutations(poss)
                for x in sxs:
                    tmp += 'M[%d, %d] + '%(st, x)
                for pos in poss:
                    cnt += 1
                    tmp += 'T4[%d, %d] + '%(pos // 8, pos % 8)
                tmp = tmp.strip('+ ')
                if cnt <= 6:
                    print(tmp)

prep_path()
findpath()


def print_matrix(matrix):
    for i in matrix:
        for elem in i:
            print(elem, end=" ")
        print()
#1
def f(x):
    return x**2
def trap_int(a,b,n):
    h,array,i = (b-a)/n,[],a
    integral=(f(a)+f(b))/2
    for i in range(n):
        integral+=f(a+i*h)
    return integral*h
print(trap_int(0,1,1000))
#2
from random import *
def is_magic_square(a):
    m=15
    for i in a:
        if sum(i)!=m:
            return 0
    if a[0][2]+a[1][2]+a[2][2]==a[0][1]+a[1][1]+a[2][1]==a[0][0]+a[1][0]+a[2][0]==a[0][0]+a[1][1]+a[2][2]== \
            a[0][2]+a[1][1]+a[2][0]==m:
        return 1
    else:
        return 0
def rand_square():
    rand,a= list(range(1, 10)),[]
    shuffle(rand)
    for i in range(0, 9, 3):
        a.append(rand[i:i + 3])
    return a
n = 3
a = rand_square()
while not(is_magic_square(a)):
    a = rand_square()
else:
    print_matrix(a)
#3
def distance(a,b):
    arr=[]
    for i in range(n):
        arr.append(((b[0][0] - a[i][0]) ** 2 + (b[0][1] - a[i][1]) ** 2)**0.5)
    for i in range(n):
        if arr[i] == min(arr):
            return a[i]
n=int(input("Введите количество сокровищ: "))
a,b=[[0]*2 for i in range(n)],[[0]*2]
for i in range(n):
    for j in range(2):
        a[i][j] = int(input(f"Введите координату сокровища с позицией {i,j}: "))
b[0][0],b[0][1] = int(input("Введите координату Сани с позицией (0,0): ")),int(input("Введите координату Сани с позицией (0,0): "))
print(distance(a,b))
#4
def new_dish(name,ing,price):
    menu.append([name,ing.split(','),int(price)])
menu = [["Пицца Маргарита", ["мука", "томаты", "сыр", "базилик"], 10],
["Салат Цезарь", ["салат", "курица", "сыр", "сухарики"], 8],["Суп Томатный", ["томаты", "лук", "морковь", "картофель"], 6]]
inp = int(input('''1. Отобразить меню ресторана.
2. Найти блюдо по названию и отобразить его ингредиенты и цену. (Вы вводите название блюда).
3. Добавить новое блюдо в меню.
4. Изменить цену блюда (Вы вводите название и новую цену).
Что вы бы хотели? '''))
if inp == 1:
    print_matrix(menu)
elif inp == 2:
    for i in range(len(menu)):
        if input("Какое блюдо вы хотите? ") == menu[i][0]:
            print(menu[i][1],menu[i][2])
elif inp == 3:
    new_dish(input("Название нового блюда: "),input("Его ингридиенты: "),input("Прайс: "))
    print("Теперь меню выглядит так: ")
    print_matrix(menu)
elif inp == 4:
    for i in range(len(menu)):
        if input("Цену на какое блюдо вы хотите изменить? ") == menu[i][0]:
            menu[i][2]=int(input("Введите новую цену: "))
            print("Теперь меню выглядит так: ")
            print_matrix(menu)
            break
#5
from random import *
n,m=int(input("Кол-во строк:")),int(input("Кол-во столбцов:"))
matrix_1,matrix_2=[[0]*m for i in range(n)],[[0]*n for i in range(m)]
for i in range(n):
    for j in range(m):
        matrix_1[i][j]=randint(1,10**2)
"""
matrix_2=[[0]*n for i in range(m)]
matrix_1=[[11,12,13,14],[21,22,23,24],[31,32,33,34]]
"""
for i in range(n):
    for j in range(m):
        matrix_2[j][i]=matrix_1[i][j]
print_matrix(matrix_1)
print()
print_matrix(matrix_2)
#6
n,c=int(input("Введите количество строк и столбцов: ")),0
matrix_1=[[0]*n for i in range(n)]
for i in range(n):
     for j in range(n):
         matrix_1[i][j] = int(input(f"Введите элемент {i,j} матрицы {n}x{n}: "))
c=matrix_1[0][0]
matrix_1[0][0]=matrix_1[-1][0]
matrix_1[-1][0]=c
c=matrix_1[0][-1]
matrix_1[0][-1]=matrix_1[-1][-1]
matrix_1[-1][-1]=c
print_matrix(matrix_1)
#7
n,m=int(input("Кол-во рядов: ")),int(input("Кол-во мест: "))
matrix_1=[[0]*m for i in range(n)]
for i in range(n):
    for j in range(m):
        matrix_1[i][j] = int(input(f"Введите {j+1} место {i+1} ряда: "))
k=int(input("Введите количество билетов: "))
nulls_count=''
for i in range(n):
    for j in range(m):
        nulls_count+=''.join(str(matrix_1[i][j]))
    if "0"*k in nulls_count:
        print(f"Места есть, ближайший ряд {i+1}")
        break
else:
    print("Мест нет")

#8
n,m=int(input("Введите количество строк: ")),int(input("Введите количество столбцов: "))
matrix=[[0]*m for i in range(n)]
for i in range(n):
    for j in range(m):
        matrix[0][j] = 1
        matrix[i][0] = 1
        matrix[i][j] = matrix[i-1][j]+matrix[i][j-1]
for i in matrix:
    print(" ".join(f"{elem:6}" for elem in i))
#9
from random import *
matrix=[["."]*4 for i in range(4)]
matrix_ships=[0]*4
coordinates=[(i,j) for i in range(2) for j in range(4)]
shuffle(coordinates)
bomb=coordinates[0]
coordinates.remove(coordinates[0])
n,c,b=6,0,0
for i in range(4):
    a = randint(0, n)
    matrix_ships[i]=coordinates[n]
    coordinates.remove(coordinates[n])
    n-=1
print_matrix(matrix_ships)
print(bomb)
while c<4:
    print_matrix(matrix)
    z=input("Введите координаты: ").split(",")
    for i in range(4):
        if int(z[0]) == matrix_ships[i][0] and int(z[1]) == matrix_ships[i][1]:
            matrix[int(z[0])][int(z[1])]="X"
            c+=1
        elif int(z[0])==bomb[0] and int(z[1])==bomb[1]:
            print("BOOM!")
            c,b=4,1
            break
else:
    if b!=1:
        print("Вы выиграли!")


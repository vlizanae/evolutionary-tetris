import random

ancho = input('Ingrese el ancho: ')
alto = input('Ingrese el alto: ')
n_piezas = input('Ingrese número de piezas: ')
t_piezas = input('Ingrese tipos de piezas: ')

t_piezas_list = list(map(int, t_piezas.split()))

f = open(str(ancho)+'_'+str(alto)+'_'+str(n_piezas)+'.txt', 'w+')

f.write(str(ancho)+' '+str(alto)+'\n')
f.write(str(n_piezas)+'\n')
f.write(str(t_piezas)+'\n')

secuencia = ''

for i in range(int(n_piezas)):
	if(i==(int(n_piezas)-1)):
		secuencia = secuencia + str(random.choice(t_piezas_list))
		continue
	secuencia = secuencia + str(random.choice(t_piezas_list)) + ' '

f.write(secuencia)

f.close()
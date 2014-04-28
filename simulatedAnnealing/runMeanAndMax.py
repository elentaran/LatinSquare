import numpy
import os
import sys
import time
if len(sys.argv)!=3:
	print "error 2 arguments needed"
	sys.exit(1)

os.system("make")

sommeTemps = 0.
somme = 0.
nbRuns = 100
for i in range(0,nbRuns):
	print "Run "+str(i)
	#filename = "record/"+sys.argv[1]+"_"+sys.argv[2]+".best"
	#os.system("rm "+filename)
	temps1 = time.time()
	os.system("./ontest > TEMP")
	temps2 = time.time()
	#os.system("cat "+filename+" > TEMP")
	fichier = open('TEMP','r')
	scorec = fichier.readline()
	scorec = fichier.readline()
	scorec = fichier.readline()
	fichier.close()
	tmp = str(scorec)
	tmp2 = tmp.split()
	somme += float(tmp2[2])
	sommeTemps += float(temps2) - float(temps1)

print "moyenne : "+str(somme/nbRuns)+" en: "+str(sommeTemps/nbRuns)

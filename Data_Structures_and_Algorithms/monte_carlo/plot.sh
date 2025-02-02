#!/bin/bash
pi_Real=3.141592653589793
make clean && make monte_carlo # Rebuild the sincos executable .
for i in {1..1000}
do
./monte_carlo -n $i > data

done

./monte_carlo -n 16384 -r $RANDOM > data.1 #creating random seeds for 5 sets of data
./monte_carlo -n 16384 -r $RANDOM > data.2
./monte_carlo -n 16384 -r $RANDOM > data.3
./monte_carlo -n 16384 -r $RANDOM > data.4
./monte_carlo -n 16384 -r $RANDOM > data.5

awk '{ print $5 }' data > new # format so that new contains "circle" points
awk '{ print $3 " " $4 }' data > newdata #format so x and y are in file newdata
awk '{ print $2 " " $1 "         " '$pi_Real' }' data.1 > pi_Fake1.dat #formating so that file contains pi,space,iterations,and the real pi for these 5 sets of data we randomized earlier
awk '{ print $2 " " $1 "         " '$pi_Real' }' data.2 > pi_Fake2.dat
awk '{ print $2 " " $1 "         " '$pi_Real' }' data.3 > pi_Fake3.dat
awk '{ print $2 " " $1 "         " '$pi_Real' }' data.4 > pi_Fake4.dat
awk '{ print $2 " " $1 "         " '$pi_Real' }' data.5 > pi_Fake5.dat

tail -n +2 new >> new1 #deletes first line then appends the rest of the data in file 
tail -n +2 pi_Fake1 >> pi_Fake1.dat
tail -n +2 pi_Fake2 >> pi_Fake2.dat	
tail -n +2 pi_Fake3 >> pi_Fake3.dat
tail -n +2 _Fake4 >> pi_Fake4.dat
tail -n +2 pi_Fake5 >> pit_Fake5.dat

j=0
d=1
l=1
tail -n +2 newdata > newerdata.$j
while read new1; #inspiration was linked list typeish but this is probably the most unoptimized 
	do    	
	
	if [[ "$new1" -eq 1 ]]; 
	then
		
	 	head -n 1 newerdata.$j >> blue.dat1
	 	tail -n +2 newerdata.$j > newerdata.$d  
	elif [[ "$new1" -eq 0 ]] 
	then
		
		head -n 1 newerdata.$j >> red.dat1
		tail -n +2 newerdata.$j > newerdata.$d
	fi
	(( d = d + 1 ))
	(( j = j + 1 ))
done < new1
#sort blue and red data points to decrease data duplication
sort -u blue.dat1 > blue.dat
sort -u red.dat1 > red.dat

gnuplot <<END
	set terminal pdf
	set output "Plot1.pdf"
	set multiplot
	
	set yrange[0:1]
	set xrange[0:1]
	plot "red.dat" notitle pt 7 ps 0.5 lc rgb "red" 
	plot "blue.dat" notitle pt 7 ps 0.5 lc rgb "blue" 
	plot [x=0:1] sqrt(1-x**2) notitle lc rgb "black"
	unset multiplot
	set terminal pdf
	set output "Plot2.pdf" 
	set multiplot
	set logscale x
	set xrange[1:16384]
	set yrange[-1:1]
	set title "Monte Carlo Error Estimation"
	set ylabel "Error"
	
	
	
	plot "pi_Fake1.dat" using 2:(column(3)-column(1)) with linespoints ls 1 pt -1 lc rgb "blue" notitle
	plot "pi_Fake2.dat" using 2:(column(3)-column(1)) with linespoints ls 1 pt -1 lc rgb "red" notitle  
	plot "pi_Fake3.dat" using 2:(column(3)-column(1)) with linespoints ls 1 pt -1 lc rgb "green" notitle 
	plot "pi_Fake4.dat" using 2:(column(3)-column(1))  with linespoints ls 1 pt -1 notitle
	plot "pi_Fake5.dat" using 2:(column(3)-column(1)) with linespoints ls 1 pt -1 lc rgb "yellow" notitle 
	
	unset multiplot
END







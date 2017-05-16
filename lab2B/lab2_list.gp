#! /usr/bin/gnuplot
#
# purpose:
#	 generate data reduction graphs for the multi-threaded list project
#
# input: lab2_list.csv
#	1. test name
#	2. # threads
#	3. # iterations per thread
#	4. # lists
#	5. # operations performed (threads x iterations x (ins + lookup + delete))
#	6. run time (ns)
#	7. run time per operation (ns)
#
# output:
#	lab2_list-1.png ... throughput of synchronized lists
#	lab2_list-2.png ... threads and iterations that run (un-protected) w/o failure
#	lab2_list-3.png ... threads and iterations that run (protected) w/o failure
#	lab2_list-4.png ... cost per operation vs number of threads
#
# Note:
#	Managing data is simplified by keeping all of the results in a single
#	file.  But this means that the individual graphing commands have to
#	grep to select only the data they want.


# general plot parameters
set terminal png
set datafile separator ","

# PLOT 1: Throughput of synchronized lists
set title "Plot 1: Throughput of Synchronized Lists"
set xlabel "# Threads"
set xrange[0:30]
set ylabel "Throughput (operations / sec)"
set logscale y 10
set output 'lab2b_1.png'

plot \
     "< grep 'list-none-m,[0-9]*,1000,1,' lab2_list.csv" using ($2):(1000000000/($7)) \
	title 'Mutex' with linespoints lc rgb 'red', \
     "< grep 'list-none-s,[0-9]*,1000,1,' lab2_list.csv" using ($2):(1000000000/($7)) \
	title 'Spin-Lock' with linespoints lc rgb 'green'

# PLOT 2: Mean time per mutex wait and mean time per operation
set title "Plot 2: Per-operation Times for Mutex Protected List Operations"
set xlabel "# Threads"
set xrange [0:30]
set ylabel "Average Time Per Operations (ns)"
set logscale y 10
set output 'lab2b_2.png'

plot \
     "< grep 'list-none-m,[0-9]*,1000,1,' lab2_list.csv" using ($2):($8) \
	title 'Mutex Wait' with linespoints lc rgb 'green', \
     "< grep 'list-none-m,[0-9]*,1000,1,' lab2_list.csv" using ($2):($7) \
	title 'Completion Time' with linespoints lc rgb 'red'

# PLOT 3: Successful Iterations when Partitioned Sublists
set title "Plot 3: Synchronization with Partitioned Sublists"
set xlabel "Yields"
set xrange [0:4]
set ylabel "Successful Iterations"
set logscale y 10
set output 'lab2b_3.png'
plot \
   "< grep 'list-id-none' lab2_list.csv" using (1):($3) \
	with points lc rgb "red" title "No Synchronization", \
   "< grep 'list-id-s' lab2_list.csv" using (2):($3) \
	with points lc rgb "green" title "Mutex", \
   "< grep 'list-id-m' lab2_list.csv" using (3):($3) \
	with points lc rgb "blue" title "Spin", \

 # "no valid points" is possible if even a single iteration can't run
 #

 # unset the kinky x axis
 unset xtics
 set xtics

# set title "List-4: Scalability of synchronization mechanisms"
# set xlabel "Threads"
# set logscale x 2
# unset xrange
# set xrange [0.75:]
# set ylabel "Length-adjusted cost per operation(ns)"
# set logscale y
# set output 'lab2_list-4.png'
# set key left top
# plot \
#      "< grep -e 'list-none-m,[0-9]*,1000,' lab2_list.csv" using ($2):($7)/(4*($3)) \
# 	title '(adjusted) list w/mutex' with linespoints lc rgb 'blue', \
#      "< grep -e 'list-none-s,[0-9]*,1000,' lab2_list.csv" using ($2):($7)/(4*($3)) \
# 	title '(adjusted) list w/spin-lock' with linespoints lc rgb 'green'

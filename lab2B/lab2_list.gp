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
     "< grep 'list-none-m' lab2_list.csv" using ($2):(1000000000/($7)) \
	title 'Mutex' with linespoints lc rgb 'red', \
     "< grep 'list-none-s' lab2_list.csv" using ($2):(1000000000/($7)) \
	title 'Spin-Lock' with linespoints lc rgb 'green'

# PLOT 2: Mean time per mutex wait and mean time per operation
set title "List-2: Per-operation Times for Mutex Protected List Operations"
set xlabel "# Threads"
set xrange [0:30]
set ylabel "Average Time Per Operations (ns)"
set logscale y 10
set output 'lab2b-2.png'

plot \
     "< grep list-none-m lab2_list.csv" using ($2):($8) \
	title 'Mutex Wait' with points lc rgb 'green', \
     "< grep list-none-m lab2_list.csv" using ($2):($7) \
	title 'Completion Time' with points lc rgb 'red'

# set title "List-3: Protected Iterations that run without failure"
# unset logscale x
# set xrange [0:5]
# set xlabel "Yields"
# set xtics("" 0, "yield=i" 1, "yield=d" 2, "yield=il" 3, "yield=dl" 4, "" 5)
# set ylabel "successful iterations"
# set logscale y 10
# set output 'lab2_list-3.png'
# plot \
#     "< grep 'list-i-none,12,' lab2_list.csv" using (1):($3) \
# 	with points lc rgb "red" title "unprotected, T=12", \
#     "< grep 'list-d-none,12,' lab2_list.csv" using (2):($3) \
# 	with points lc rgb "red" title "", \
#     "< grep 'list-il-none,12,' lab2_list.csv" using (3):($3) \
# 	with points lc rgb "red" title "", \
#     "< grep 'list-dl-none,12,' lab2_list.csv" using (4):($3) \
# 	with points lc rgb "red" title "", \
#     "< grep 'list-i-m,12,' lab2_list.csv" using (1):($3) \
# 	with points lc rgb "green" title "Mutex, T=12", \
#     "< grep 'list-d-m,12,' lab2_list.csv" using (2):($3) \
# 	with points lc rgb "green" title "", \
#     "< grep 'list-il-m,12,' lab2_list.csv" using (3):($3) \
# 	with points lc rgb "green" title "", \
#     "< grep 'list-dl-m,12,' lab2_list.csv" using (4):($3) \
# 	with points lc rgb "green" title "", \
#     "< grep 'list-i-s,12,' lab2_list.csv" using (1):($3) \
# 	with points lc rgb "blue" title "Spin-Lock, T=12", \
#     "< grep 'list-d-s,12,' lab2_list.csv" using (2):($3) \
# 	with points lc rgb "blue" title "", \
#     "< grep 'list-il-s,12,' lab2_list.csv" using (3):($3) \
# 	with points lc rgb "blue" title "", \
#     "< grep 'list-dl-s,12,' lab2_list.csv" using (4):($3) \
# 	with points lc rgb "blue" title ""
# #
# # "no valid points" is possible if even a single iteration can't run
# #
#
# # unset the kinky x axis
# unset xtics
# set xtics
#
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

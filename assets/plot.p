# Output Settings
set terminal png size 800,300
set output 'out.png'
# Labels, Title and Data
set key inside bottom right
set xlabel 'Number of Iterations'
set ylabel 'Time(s)'
set title 'Iteration Increase Comparison'
plot "data.txt" using 1:2 title 'No threads' with linespoints,\
"data.txt" using 1:3 title "1 Thread" with linespoints,\
"data.txt" using 1:4 title "2 Thread" with linespoints,\
"data.txt" using 1:5 title "3 Thread" with linespoints,\
"data.txt" using 1:6 title "4 Thread" with linespoints
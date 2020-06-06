set terminal svg 
set output 'lab5.svg' 
set title 'Wyniki' 
set xlabel 'n' 
set ylabel 't(n) [s]' 
set y2label 'przyspieszenie - cpu/gpu'
set y2tics border nomirror
set style line 2 \
    linecolor rgb '#ff0000' \
    linetype 1 linewidth 2 \
    pointtype 7 pointsize 0.8
set style line 1 \
    linecolor rgb '#00ff00' \
    linetype 1 linewidth 2 \
    pointtype 7 pointsize 0.8
set style line 3 \
    linecolor rgb '#0000ff' \
    linetype 1 linewidth 1 
plot 'file' using 1:($2/1000) title "cpu" with linespoints linestyle 1, 'file' using 1:($3/1000) title "gpu" with linespoints linestyle 2, 'file' using 1:($2/$3) title "przyspieszenie" with lines axes x1y2 linestyle 3 

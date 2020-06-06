count=100
file='results.txt'
>file
while [ $count -lt 2001 ]
do   
    echo -n $count >> file
    echo -n ' ' >>file
    $PWD/lab5 $count >> file
    count=$[$count+100]
done

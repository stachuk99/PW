count=50
file='results.txt'
>file
while [ $count -lt 1201 ]
do   
    echo -n $count >> file
    echo -n ' ' >>file
    $PWD/lab6 $count >> file
    count=$[$count+50]
done

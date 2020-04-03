import time
import threading
import random
import numpy as np
from multiprocessing import Pool
import matplotlib.pyplot as plt


def do_nothing():
    return 0


def thread_time_measure():
    start = time.time()
    for i in range(0, 10000):
        t = threading.Thread(target=do_nothing)
        t.start()
    end = time.time()
    print(end - start)
    start = time.time()
    for i in range(0, 10000):
        do_nothing()
    end = time.time()
    print(end - start)


def gnp_line(i):
    for j in range(i+1, n):
        x = (random.random() <= p)
        a[i][j] = x
        a[j][i] = x
    return 0


def parallel_gnp():
    for i in range(n-1):
        t = threading.Thread(target=gnp_line, args=(i,))
        t.start()
        threads_list.append(t)
    for t in threads_list:
        t.join()
    return 0


def sequence_gnp():
    for row in range(n - 1):
        for j in range(row + 1, n):
            x = (random.random() <= p)
            a[row][j] = x
            a[j][row] = x
    return 0


def pool_gnp():
    pool = Pool()
    pool.map(gnp_line, range(n))
    pool.close()
    pool.join()
    return 0


# thread_time_measure()
n = 1000
p = 0.3
a = np.zeros((n, n), dtype=bool)
threads_list = []
if __name__ == '__main__':
    parallelTime = []
    sequentialTime = []
    parallel_gnp()
    for i in range(0, 20000, 1000):
        a = np.zeros((i, i), dtype=bool)
        n=i
        start = time.time()
        parallel_gnp()
        end = time.time()
        print("In parallel: ", end - start)
        parallelTime.append(end-start)

        start = time.time()
        sequence_gnp()
        end = time.time()
        print("sequentially: ", end - start)
        sequentialTime.append(end - start)

    plt.plot(range(1, len(sequentialTime) + 1), sequentialTime, parallelTime)
    plt.xlabel('N')
    plt.ylabel('Time')
    plt.show()
    #start = time.time()
    #pool_gnp()
    #end = time.time()
    #print("pool version: ", end - start)

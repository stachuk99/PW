import time
import threading
import random
import numpy as np
import multiprocessing
import matplotlib.pyplot as plt

lock = threading.Lock()
total_sum = np.int64(0)


#  the task is to sum a table in parallel
def exercise2_2():
    size = 5000000
    step = int(size / 8)  # to make it easier I assumed that
    # size of table is always divisible by amount of threads
    values = np.random.randint(1000, size=size)
    thread_list = []
    start = time.time()
    for i in range(0, size, step):
        t = threading.Thread(target=summing, args=(values, i, step))
        t.start()
        thread_list.append(t)
    for t in thread_list:
        t.join()
    end = time.time()
    print("parallel time: ", end - start)
    checksum = np.int64(0)
    start = time.time()
    for value in values:
        checksum += value
    end = time.time()
    print("sequential time: ", end - start)
    print("sum =     ", total_sum, "\nchecksum= ", checksum)


def summing(values, pointer, n):
    part_sum = np.int64(0)
    for i in range(pointer, pointer+n):
        part_sum += values[i]
    lock.acquire()      # documentation says that addition is not thread safety
    # so i used lock but it work well without it :(
    global total_sum
    total_sum += part_sum
    lock.release()


# this exercise is almost same, the only difference is that instead of using
# threads synchronization I used append() which is atomic operation
def exercise2_3():
    size = 5000000
    step = int(size / 8)  # to make it easier I assumed that
    # size of table is always divisible by amount of threads
    values = np.random.randint(1000, size=size)
    thread_list = []
    sum_list = []
    summed_list = np.int64(0)
    start = time.time()
    for i in range(0, size, step):
        t = threading.Thread(target=atomic_summing, args=(values, i, step, sum_list))
        t.start()
        thread_list.append(t)
    for t in thread_list:
        t.join()
    for v in sum_list:
        summed_list += v
    end = time.time()
    print("parallel time: ", end - start)
    checksum = np.int64(0)
    start = time.time()
    for value in values:
        checksum += value
    end = time.time()
    print("sequential time: ", end - start)
    print("sum =     ", summed_list, "\nchecksum= ", checksum)


def atomic_summing(values, pointer, n, sum_list):
    part_sum = np.int64(0)
    for i in range(pointer, pointer+n):
        part_sum += values[i]
    sum_list.append(part_sum)


if __name__ == '__main__':
    exercise2_2()
    exercise2_3()


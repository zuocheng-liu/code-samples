# coding: utf-8
from concurrent.futures import ThreadPoolExecutor, as_completed
from threading import Lock
import time
count = 0
exp_count = 0
test_dict = {}
run = False
#run = True
lock = Lock() 

def spider(compute_id = 0):
    try :
        global count
        global exp_count
        global test_dict
        global run
        global lock
        base = (compute_id + 1) * 10 * 1000000
        while True :
            if not run :
                continue
            for i in range(0, 1000000) :
                count += 1
                #lock.acquire()
                exp_count += 1
                #lock.release()
                test_dict[base + i] = exp_count
            break
    except Exception as e :
        print(e)
 
with ThreadPoolExecutor(max_workers=4) as t:  # 创建一个最大容纳数量为5的线程池
    future_list = []
    for i in range(0, 4) :
       future_list.append(t.submit(spider, i)) 
    time.sleep(0.1)
    print ("start to compute, curr count is:", count, " exp_count:", exp_count)
    run = True
    for future in as_completed(future_list):
        print("some thread done, count:", count , " exp_count:", exp_count)
        pass
    print("all done, count" , count , " exp_count:", exp_count)
    print("test dict len:", len(test_dict))


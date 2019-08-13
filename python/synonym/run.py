from multiprocessing import Process, Pool
from digger import Digger
import time

digger = Digger(8, 0.8)
digger.gCorpus("middel")
digger.prepare("data/query")

def digger_process(step, start):
    res = []
    try:
        for corp in digger.corps[start:min(start+step, len(digger.corps)+1)]:
            for que in digger.querys:
                if digger.bf.contains(corp):
                    break
                
                if digger.similarity(que, corp):
                    res.append((corp, que))
                    digger.bf.add(corp)   
    except Exception as err:
        print(err)       

    return res

def main():
    results = []
    now = time.time()
    step_len = len(digger.corps) // digger.pro_num + 1
    pool = Pool(digger.pro_num)
    for i in range(digger.pro_num):
        results.append(pool.apply_async(digger_process, args=(step_len, step_len*i)))

    pool.close()
    pool.join()

    with open("same_ques", encoding="utf8", mode="w") as outf:
        for result in results:
            for corp, que in result.get():
                outf.write(f"{corp}\t{que}\n")
    outf.close()

    print(f"time cost - {time.time()-now}")

if __name__ == "__main__":
    main()    
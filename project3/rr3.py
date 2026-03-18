import sys
import heapq as hq
from itertools import count

MAX_PRIORITY = int(10) #Convert high processes to low ones
MAX_SKIP = int(100)

class Proc:
    def __init__(s, name: str, priority: int, arrival: int, total: int, block_interval: int):
        s.name = name.strip('\n')
        s.priority = int(priority)
        s.arrival = int(arrival)
        s.total = int(total)
        s.block_interval = int(block_interval)

        s.finish = -1
        s.runtime = 0

        s.blocking = 0
        s.interval_left = s.block_interval

        s.complete = False

    def __lt__ (s, o):
        if s.priority == o.priority:
            return ord(s.name[0]) < ord(o.name[0])
        return s.priority < o.priority
        pass

    def end(s, t):
        s.t_finish = t
        s.complete = True

    def run(s, quantum, block_duration) -> int:
        if s.runtime + quantum > s.total:
            quantum = s.total - s.runtime #Adjust time for end of process

        if s.block_interval > 0:
            if quantum < s.interval_left:
                s.interval_left -= quantum
            else:
                quantum = s.interval_left #Set step to remaining time
                s.blocking = block_duration + quantum #Quantum
                s.interval_left = s.block_interval #Reset time to blocking
        s.runtime += quantum
        return quantum
                
    def update_blocking(s, quantum) -> int:
        s.blocking -= quantum
        return s.blocking
    
    def turnaround(s) -> int:
        return s.finish - s.arrival
    
    def stats(s) -> str:
        return f"{s.name}, S:{s.start}, F:{s.finish}, T:{s.turnaround()}"

    @staticmethod
    def header() -> str: 
        return "Task, Priority, Arrival Time, Total Time, Block Interval"

    def __str__(s): 
        return f"{s.name}, {s.priority}, {s.arrival}, {s.total}, {s.block_interval}"

def round_robin(procs: list[Proc], time_slice: int, block_duration: int) -> None:
    time = 0
    logs = []
    turnarounds = []

    num_procs = len(procs)

    procs = sorted(procs, key=lambda x: x.arrival, reverse=False)
    print(Proc.header())
    for p in procs:
        print(p)

    q_temp: list[(int, int, Proc)] = []
    q_arrival = []
    q_blocked = []
    q_ready = []

    hq.heapify(q_temp)
    hq.heapify(q_arrival)
    hq.heapify(q_blocked)
    hq.heapify(q_ready)

    c_arrival = count()

    while True:
        if len(procs) + len(q_arrival) + len(q_blocked) + len(q_ready) == 0:
            break
        
        #Add new processes
        offset = 0
        for i in range(len(procs)):
            p = procs[i - offset]
            if p.arrival <= time:
                hq.heappush(q_arrival, (MAX_PRIORITY - p.priority, next(c_arrival) - num_procs, procs.pop(0)))
                offset += 1
        
        #Push Arrivals to Ready
        while q_arrival:
            priority, _, p = hq.heappop(q_arrival)
            hq.heappush(q_ready, (priority, _, p))

        #Handle ready processes
        quantum = time_slice
        name = ""
        endchar = ""
        if q_ready:
            priority, _, p = hq.heappop(q_ready)
            name = p.name
            quantum = p.run(quantum, block_duration)
            if p.runtime >= p.total:
                p.finish = time + quantum
                turnarounds.append(p.turnaround())
                endchar = "T"
            elif p.blocking > 0:
                hq.heappush(q_blocked, (priority, time + quantum, p))
                endchar = "B"
            else:
                endchar = "P"
                hq.heappush(q_ready, (priority, time + quantum, p))
        else:
            name = "(IDLE)"
            endchar = "I"
            if q_blocked:
                min_quantum = MAX_SKIP
                #logs.append(f"\tBlocked Search MAX={MAX_SKIP}")
                for _, _, p in q_blocked:
                    temp = p.update_blocking(0)
                    logs.append(f"\t\t{p.name} has {temp}")
                    if temp < min_quantum:
                        min_quantum = temp
                quantum = min_quantum #+ prev_quantum #Add temp as we have added it already
                #logs.append(f"\tNEW={quantum}")
        #Handle blocked proccess
        while q_blocked:
            priority, _, p = hq.heappop(q_blocked)
            p.update_blocking(quantum)
            if p.blocking < 1:
                hq.heappush(q_ready, (priority, _, p))
            else:
                hq.heappush(q_temp, (priority, _, p))
        while q_temp:
            priority, _, p = hq.heappop(q_temp)
            hq.heappush(q_blocked, (priority, _, p))
        

        logs.append(f"{time}\t{name}\t{quantum}\t{endchar}")
        time += quantum
        prev_quantum = quantum

    for l in logs:
        print(l)
    for t in turnarounds:
        print(f"T:{t}")
    print(f"AVG={sum(turnarounds)/(len(turnarounds))}")

def parse_inputs() -> list:
    procs = []
    for line in sys.stdin:
        split = line.split(" ")
        if (split[0]) == "#":
            continue
        if len(split) != 5:
            print('Parsing error bad length')
        procs.append(Proc(*split[:5]))
    return procs

if __name__ == "__main__":
    time_slice = 10
    block_duration = 20
    if len(sys.argv) == 3:
        time_slice = int(sys.argv[1])
        block_duration = int(sys.argv[2])
    procs = parse_inputs()
    round_robin(procs, time_slice, block_duration)
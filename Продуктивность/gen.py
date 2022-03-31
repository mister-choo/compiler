import argparse
import numpy as np
from datetime import datetime, timedelta

fmtstr = '%d.%m.%y'
def date(d): return datetime.strptime(d, fmtstr)


def gen_time_diap(start, end):
    return str(int(start)).format(0)+":00-"+str(int(end)).format(0)+":00"


def gen_timetable_from_list(list_like):
    a = list_like.copy()
    b = list_like.copy()

    b.pop(0)
    b.append(0)
    result = list()
    for k1, k2 in zip(a, b):
        result.append(gen_time_diap(k1, k2))
    return result


def gen_timetable(start, end, num):
    a = list(np.linspace(start, end, num))
    b = a.copy()
    b.pop(0)
    b.append(0)
    result = list()
    for k1, k2 in zip(a, b):
        result.append(gen_time_diap(k1, k2))
    return result


w = ["понедельник", "вторник", "среда", "четверг",
     "пятница", "суббота", "воскресенье"]


def gen_weekday(start, end):
    ww = w+w
    return ww[start:end]


def mapout(file):
    res = list()
    with open(file) as f:
        data = f.readlines()
    for line, num in zip(data, range(len(data))):
        if '###' in line:
            string = list(filter(None, line.replace('\n', '').split(' ')))
            res.append(
                {'day': string[1], 'line': num, 'date': date(string[2])})
    return res


def day_to_num(day):
    return dict(zip(w, range(14)))[day]


def num_to_day(num):
    return dict(zip(range(14), w))[num]


parser = argparse.ArgumentParser()

parser.add_argument("-a", "--append", help="append to the end of file",
                    action="store_true")
parser.add_argument("-w", "--write", help="rewrite file",
                    action="store_true")

parser.add_argument('--start', action='store', type=int)
parser.add_argument('--from_to', action='store', type=int, nargs=2)
parser.add_argument('--to', action='store', type=int)
parser.add_argument('--slots', action="store", type=int)
parser.add_argument('--days', action="store", type=int)
parser.add_argument('--custom_slots', action="store", type=int, nargs='+')
parser.add_argument('--tasks', action="store", type=int)

parser.add_argument('--taskspec', action="store", nargs='+')

args = parser.parse_args()

if args.append:
    mode = "a"
    act_mode = "n"
elif args.write:
    mode = "w"
    act_mode = "d"
else:
    mode = "a"
    act_mode = "n"

with open("Неделя.md") as f:
    if '# Неделя' in f.read() and mode != "w":
        flag = False
    else:
        flag = True

mapp = mapout("Неделя.md")

start_day = 0

if args.days == None:
    Dday = 1
else:
    Dday = args.days


if args.taskspec == None:
    task_flag = False
    if args.tasks == None:
        tasks_num = 1
    else:
        tasks_num = args.tasks
else:
    task_flag = True
    taskspec = args.taskspec

if(act_mode == "n"):
    start_day = day_to_num((mapp[-1])['day'])+1

end_day = start_day+Dday

if args.from_to == None:
    if args.start == None:
        start_time = 8
    else:
        start_time = args.start

    if args.to == None:
        end_time = 24
    else:
        end_time = args.to
else:
    start_time = args.from_to[0]
    end_time = args.from_to[1]


if args.custom_slots == None:
    if args.slots == None:
        time_slots = 3
    else:
        time_slots = args.slots
    time_table = gen_timetable(start_time, end_time, time_slots)
else:
    time_table = gen_timetable_from_list(args.custom_slots)

last_date = (mapp[-1])['date']
dt_day = timedelta(days=1)
current_date = last_date + dt_day


with open("Неделя.md", mode) as f:
    if flag:
        f.write("# Неделя")
    for day in gen_weekday(start_day, end_day):
        f.write("\n\n ### " + str(day)+' ' +
                str(current_date.strftime(fmtstr)))
        current_date += dt_day
        for time, num in zip(time_table, range(1, len(time_table))):
            f.write("\n"+" "+str(time)+":")

            if task_flag:
                for k in taskspec:
                    f.write("\n\t - [ ] "+str(k))
            else:
                for k in range(tasks_num):
                    f.write("\n - [ ]  ")

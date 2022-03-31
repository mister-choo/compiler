def gen_timetable(start, end, nums):
  interval = end - start;
  t = start
  dt = interval // nums
  result = list([])
  while True:
    if(t>end):
      result.append(str(t-dt).format(0)+":00-"+str(end)+":00")
      break
    result.append(str(t).format(0)+":00-"+str(t+dt).format(0)+":00")
    t+=dt
    
  return result

with open("Неделя.md", "w") as f:
  f.write("\n# Неделя")
  for day in ["понедельник","вторник","среда","четверг","пятница","суббота","воскресенье"]:
    f.write("\n ### "+str(day))
    for time, num in zip(gen_timetable(8, 24, 3), range(1, 4)):
      f.write("\n"+str(num)+"."+" "+str(time)+":")
      for k in range(1):
        f.write("\n\t - [ ] дело " + str(k))
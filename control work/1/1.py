import pandas as pd

fio_list = [
  "Юрченко Глеб Сергеевич", 
  "Апальков Егор Владимирович",
  "Аристов Владислав Евгеньевич", # это я
  "Безбородов Евгений Сергеевич",
  "Белова Дарья Андреевна"
]

def count_vowels(name):
  return sum(1 for letter in name if letter.lower() in 'аеёиоуыэюя')

def get_resources(fio_list):
  alloc_vowels = [[], [], [], [], []]
  max_resources = [[], [], [], [], []]


  for i in range(len(fio_list)):
      for word in fio_list[i].split(' '):
         alloc_vowels[i].append(count_vowels(word))
         max_resources[i].append(len(word))
  
  return alloc_vowels, max_resources

alloc_vowels, max_resources = get_resources(fio_list)

N1 = sum(max_resources[0]) - sum(alloc_vowels[0]) - 5
N2 = sum(max_resources[1]) - sum(alloc_vowels[1]) - 5
N3 = sum(max_resources[2]) - sum(alloc_vowels[2]) - 5 

total_resources = [N1, N2, N3]

processes = ['Процесс A', 'Процесс B', 'Процесс C', 'Процесс D', 'Процесс E']

data = {
  'Выделенные R1': [alloc_vowels[i][0] for i in range(5)],
  'Выделенные R2': [alloc_vowels[i][1] for i in range(5)],
  'Выделенные R3': [alloc_vowels[i][2] for i in range(5)],
  'Необходимо R1': [max_resources[i][0] for i in range(5)],
  'Необходимо R2': [max_resources[i][1] for i in range(5)],
  'Необходимо R3': [max_resources[i][2] for i in range(5)],
  'Процесс': processes
}

df = pd.DataFrame(data)
df = df.set_index('Процесс')

print(df)
print("\nОбщее количество ресурсов: N1, N2, N3 =", total_resources)

def is_state_safe(allocations, max_demands, available_resources):
    work = available_resources.copy()
    finish = [False] * len(allocations)
    
    while False in finish:
        safety_sequence = []
        for i, (alloc, max_demand) in enumerate(zip(allocations, max_demands)):
            if finish[i]:
                continue
            if all(work[r] >= (max_demand[r] - alloc[r]) for r in range(len(work))):
                for r in range(len(work)):
                    work[r] += alloc[r]
                safety_sequence.append(i)
                finish[i] = True
                break
        else:
            return False, []
    return True, safety_sequence

allocations = [[alloc_vowels[i][j] for j in range(3)] for i in range(5)]
max_demands = [[max_resources[i][j] for j in range(3)] for i in range(5)]
available_resources = [N1, N2, N3]

safe, sequence = is_state_safe(allocations, max_demands, available_resources)
if safe:
    print("Состояние системы безопасно.")
    safe_sequence_names = [processes[i] for i in sequence]
    print("Безопасная последовательность:", safe_sequence_names)
else:
    print("Состояние системы небезопасно.")

